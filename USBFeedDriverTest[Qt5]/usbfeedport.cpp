#include "usbfeedport.h"
//--------------------------------------------------------
CUSBFeedPort::CUSBFeedPort(QObject *parent) :
	QSerialPort(parent)
{

}
//--------------------------------------------------------
void CUSBFeedPort::FeedPortEventHandler(DWORD feedevent)
{
if(!feedevent)return;

	if(EventsEnabled)
		emit FeedEvent(feedevent);
}
//........................................................
/* ловушка на Received сигнал */
void CUSBFeedPort::FeedPortEventHandler()
{

	if(XferMetrEnabled)
	{
		rx_count +=bytesAvailable();
	}

	FeedPortEventHandler(FEED_RX_EVT & MaskEvents);
}
/* ловушка на Written/Transmitted сигнал */
void CUSBFeedPort::FeedPortEventHandler(qint64 byteswritten)
{

	if(XferMetrEnabled)
	{
		tx_count +=byteswritten;
	}

	FeedPortEventHandler(FEED_TX_EVT & MaskEvents);
}
//--------------------------------------------------------
void CUSBFeedPort::enableTransferometr(bool bEnableXferMetr)
{
	if(bEnableXferMetr)
	{
		tx_count =0;
		rx_count =0;
		tx_speed =0;
		rx_speed =0;

		connect(this, SIGNAL(bytesWritten(qint64)), SLOT(FeedPortEventHandler(qint64)));
		connect(this, SIGNAL(readyRead()), SLOT(FeedPortEventHandler()));

		connect(&tmrXferMetr, SIGNAL(timeout()), SLOT(tmrXferMetr_TimeOutEvent()));
		tmrXferMetr.start(250);
		XferMetrEnabled =true;
	}
	else
	{
		XferMetrEnabled =false;
		tmrXferMetr.stop();
		if(!EventsEnabled)
		{
			disconnect(this, SIGNAL(readyRead()));
			disconnect(this, SIGNAL(bytesWritten(qint64)));
		}
		disconnect(&tmrXferMetr, SIGNAL(timeout()));
	}
}
//--------------------------------------------------------
ULONG CUSBFeedPort::getTXSpeed()
{
ULONG res =tx_speed;

	tx_speed =0;

return res;
}
//........................................................
ULONG CUSBFeedPort::getRXSpeed()
{
ULONG res =rx_speed;

	rx_speed =0;

return res;
}
//--------------------------------------------------------
void CUSBFeedPort::tmrXferMetr_TimeOutEvent()
{
	if(tx_count)
	{
		tx_speed =tx_count;
		tx_count =0;
	}

	if(rx_count)
	{
		rx_speed =rx_count;
		rx_count =0;
	}
}
//--------------------------------------------------------
void CUSBFeedPort::enableEvents(DWORD mask)
{
DWORD changingEvents =(MaskEvents ^ mask);

	if(MaskEvents =mask)
		EventsEnabled = true;
	else
		EventsEnabled = false;
//..............................
	if(changingEvents & 0x03FF) // change pinouts events mask
	{
		if(pThreadPoll)delete(pThreadPoll); // завершить поток если запущен

		if(MaskEvents & 0x03FF) // запустить поток опроса
		{
			pThreadPoll = new CThread((THREAD_ROUTINE)&pollpinoutsroutine, this);
		}
		else
		{
			pThreadPoll =NULL;
		}
	}
//.................................
	if(changingEvents & FEED_RX_EVT)
	{
		if(MaskEvents & FEED_RX_EVT)
			connect(this, SIGNAL(readyRead()), SLOT(FeedPortEventHandler()));
		else
			disconnect(this, SIGNAL(readyRead()));
	}
//.................................
	if(changingEvents & FEED_TX_EVT)
	{
		if(MaskEvents & FEED_TX_EVT)
			connect(this, SIGNAL(bytesWritten(qint64)), SLOT(FeedPortEventHandler(qint64)));
		else
			disconnect(this, SIGNAL(bytesWritten(qint64)));
	}
}
//--------------------------------------------------------
void	CUSBFeedPort::enableEvents()
{
	EventsEnabled =true;
}
//......................................
void	CUSBFeedPort::disableEvents()
{
	EventsEnabled =false;
}
//-------------------------------------------------------- выполняется в отдельном потоке
void CUSBFeedPort::pollpinoutsroutine(void *args)
{
DWORD pinoutsstate =0;
if(!args)return;
CUSBFeedPort *port =(CUSBFeedPort*)args;

	while(port->isOpen() && (port->MaskEvents & 0x03FF))
	{
		/* если изменение состояния пина И он задан по маске */
		if(0x03FF & ((port->pinoutSignals() ^ pinoutsstate) & port->MaskEvents))
		{
			port->FeedPortEventHandler(pinoutsstate =port->pinoutSignals());
		}
	}
}
//--------------------------------------------------------
QString	CUSBFeedPort::findUSBFeedByName(const QString &feedname)
{
/* получаем список СТРУКТУР <QSerialPortInfo> */
QList<QSerialPortInfo> serialPortInfoList =QSerialPortInfo::availablePorts();

/* проходим по этому списку получаем имена портов */
	foreach(const QSerialPortInfo &serialPortInfo, serialPortInfoList)
	{
		if(serialPortInfo.description() == feedname)
			return serialPortInfo.portName();
	}

return ("");
}
//--------------------------------------------------------
QString	CUSBFeedPort::getAvailablesPortsName()
{
QString portsnames ="";

/* получаем список СТРУКТУР <QSerialPortInfo> */
QList<QSerialPortInfo> serialPortInfoList =QSerialPortInfo::availablePorts();

/* проходим по этому списку получаем имена портов */
	foreach (const QSerialPortInfo &serialPortInfo, serialPortInfoList)
	{
		portsnames +=serialPortInfo.portName() + ":" + serialPortInfo.description() + "\n";
	}

return (portsnames);
}
/*********************************************************************
* БезАдресное чтение фида
* по умолчанию синхронная (блокирующая) по таймауту
*********************************************************************/
qint64 CUSBFeedPort::ReadFeed(void *pBuff, ULONG lBuff, ULONG lTimeOut)
{
if(!isOpen()||!pBuff)return 0;
memset(pBuff,0,lBuff);

	if(lTimeOut)
	{
		disableEvents();
		bool wait =waitForReadyRead(lTimeOut);	// ждём прихода первых данных
		enableEvents();
		if(!wait)return 0;	// сработал таймаут
	}

return read((char*)pBuff, lBuff);
}
/*********************************************************************
* БезАдресная запись фида
* асинхронная
*********************************************************************/
ULONG CUSBFeedPort::WriteFeed(void *pData, ULONG lData, void *pDataHeader, BYTE lDataHeader)
{
if(!isOpen() || !pData)return 0;

BYTE	FeedFrame[MAX_FEED_FRAME_SIZE];
BYTE*	pWD;
ULONG	wtData=0;
BYTE	wb;
bool	LastFrame;
if(!lData)lData	=strlen((char*)pData);

	do
	{
	pWD	=(FeedFrame + 1);			// pointer to header data of frame
	wb =MAX_FEED_FRAME_DATA_SIZE;	// MAX_FEED_FRAME_SIZE - (frame_hdr_size + frame_crc_size)

		/* если последняя порция данных равна фрейму, нужен будет пустой фрейм для целевого интерфейса */
		if(wb == lData)
			LastFrame =true;
		else
			LastFrame =false;

		/* заполняем хидер адресного протокола фида */
		if(pDataHeader && lDataHeader)
		{
			memcpy(pWD, pDataHeader, lDataHeader);
			pWD +=lDataHeader;		// pointer to data of frame
			wb -=lDataHeader;
		}
		/* определяем доступную длину данных в фрейме */
		if(wb > lData)wb =lData;

		/* копируем данные во фрейм */
		memcpy(pWD, pData, wb);
		pWD +=wb;	// pointer to crc frame

		/* пишем в заголовок фрейма его длину */
		*FeedFrame =(pWD - FeedFrame) + 1; // hdr_frame + (hdr_data) + data + crc
		/* считаем и записываем crc фрейма */
		*pWD =GetCRC8((BYTE*)FeedFrame, (pWD - FeedFrame), 0);
		pWD++;	// pointer to end frame

	wtData +=write((char*)FeedFrame, (pWD - FeedFrame));
	lData -=wb;
	pData +=wb;
	}while(lData || LastFrame);

return wtData;
}
//*******************************************************************************/
/************ Чтение фида с 1-байтового адреса интерфейса устройства ************/
/********************************************************************************/
BYTE CUSBFeedPort::ReadFeedByteFromAddr(BYTE bAddr)
{
if(!isOpen())return 0;
BYTE bValue =0;

	if(WriteFeed(&(bAddr &=0x7F), sizeof(BYTE)))
	{
		return ReadFeed(&bValue, 1);
	}

return 0;
}
/********************************************************************************/
/************ Запись фида по 1-байтовому адресу интерфейса устройства ***********/
/********************************************************************************/
BYTE CUSBFeedPort::WriteFeedByteToAddr(BYTE bAddr, BYTE bData)
{
if(!isOpen())return 0;
FEED_BYTE_ADDR_REQUEST	bfeedreq;

	bfeedreq.bAddress	=(bAddr|0x80);	// 7-бит маркер записи по адресу
	bfeedreq.bData		=bData;

return WriteFeed(&bfeedreq, sizeof(FEED_BYTE_ADDR_REQUEST));
}
/********************************************************************************/
/************ Чтение фида по 4-байтовому адресу интерфейса устройства ***********/
/********************************************************************************/
ULONG CUSBFeedPort::ReadFeedFromAddr(DWORD dwAddr, BYTE *pBuff, ULONG lRead)
{
if(!isOpen() || !pBuff || !lRead)return 0;
HDR_FEED_ADDR_REQUEST	feedreq;

	feedreq.dwAddress	=(dwAddr & 0x7FFFFFFF);	// 31-бит маркер чтения по адресу
	feedreq.dwRequest	=lRead;

	WriteFeed(&feedreq, sizeof(HDR_FEED_ADDR_REQUEST));

return ReadFeed((char*)pBuff, lRead);
}
/********************************************************************************/
/************ Запись фида по 4-байтовому адресу интерфейса устройства ***********/
/********************************************************************************/
ULONG CUSBFeedPort::WriteFeedToAddr(DWORD dwAddr, BYTE *pData, ULONG lData)
{
if(!isOpen() || !pData)return 0;

	dwAddr |=0x80000000; // 31-бит маркер записи по адресу

return WriteFeed(pData, lData, &dwAddr, sizeof(DWORD));
}
/********************************************************************************/
/************ Отправка данных на 4-байтовый адрес интерфейса устройства *********/
/************ маркер чтения/записи в поле адреса НЕ ИЗМЕНЯЕТСЯ          *********/
/********************************************************************************/
ULONG CUSBFeedPort::WriteFeedToAddrEx(DWORD dwAddr, BYTE *pData, ULONG lData)
{
if(!isOpen()||!pData)return 0;

return WriteFeed(pData, lData, &dwAddr, sizeof(DWORD));
}
/*********************************************************************/
/************ Запрос фиксированной длины к интерфейсу устройства *****/
/*********************************************************************/
ULONG CUSBFeedPort::SendFeedRequest(DWORD dwAddress, DWORD dwRequest, DWORD dwValue)
{
if(!isOpen())return 0;
HDR_FEED_ADDR_REQUEST	feedreq;

	feedreq.dwAddress	=dwAddress;
	feedreq.dwRequest	=dwRequest;

return WriteFeed(&dwValue, sizeof(DWORD), &feedreq, sizeof(HDR_FEED_ADDR_REQUEST));
}
/*********************************************************************/
/************ Запрос переменной длины к интерфейсу устройства ********/
/*********************************************************************/
ULONG CUSBFeedPort::SendFeedRequestEx(DWORD dwAddress, DWORD dwRequest, BYTE *pData, ULONG lData)
{
if(!isOpen()||!pData)return 0;
HDR_FEED_ADDR_REQUEST	feedreq;

	feedreq.dwAddress	=dwAddress;
	feedreq.dwRequest	=dwRequest;

return WriteFeed(pData, lData, &feedreq, sizeof(HDR_FEED_ADDR_REQUEST));
}
/*********************************************************************/
/************ Сбос фид-буфера приема устройства **********************/
/*********************************************************************/
void CUSBFeedPort::ResetFeedBufferOfDevice()
{
if(!isOpen())return;
BYTE ResetData[255];

	reset();
	clear(Output); /* если 'AllDirections' QSerialPort зависает! */

	memset(ResetData, 0x00, 255);
	write((char*)ResetData, 255);

	readAll();
}
/*********************************************************************/

