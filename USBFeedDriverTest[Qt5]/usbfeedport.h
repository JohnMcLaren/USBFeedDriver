#ifndef USBFEEDPORT_H
#define USBFEEDPORT_H

#include <QtWidgets>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "thread.h"
#include "dwsfeed.h"
//-----------------------------------------------------------------------------------------
#define MAX_FEED_FRAME_SIZE			(255)
#define MAX_FEED_FRAME_DATA_SIZE	(MAX_FEED_FRAME_SIZE - 2)
//-----------------------------------------------------------------------------------------
class CUSBFeedPort : public QSerialPort
{
	Q_OBJECT
public:
	explicit	CUSBFeedPort(QObject *parent = 0);
	QString		findUSBFeedByName(const QString &feedname);
	QString		getAvailablesPortsName();

	qint64		ReadFeed(void *pBuff, ULONG lBuff=512, ULONG lTimeOut=1000);
	ULONG		WriteFeed(void *pData, ULONG lData=0, void *pDataHeader =NULL, BYTE lDataHeader =0);
	BYTE		ReadFeedByteFromAddr(BYTE bAddr);
	BYTE		WriteFeedByteToAddr(BYTE bAddr, BYTE bData);
	ULONG		ReadFeedFromAddr(DWORD dwAddr,BYTE* pBuff, ULONG lRead=512);
	ULONG		WriteFeedToAddr(DWORD dwAddr,BYTE* pData, ULONG lData=0);
	ULONG		WriteFeedToAddrEx(DWORD dwAddr,BYTE* pData, ULONG lData=0);
	ULONG		SendFeedRequest(DWORD dwAddress, DWORD dwRequest, DWORD dwValue);
	ULONG		SendFeedRequestEx(DWORD dwAddress, DWORD dwRequest, BYTE* pData, ULONG lData=0);
	void		ResetFeedBufferOfDevice();
	void		enableEvents(DWORD mask);
	void		enableEvents();
	void		disableEvents();
	void		enableTransferometr(bool bEnableXferMetr);
	ULONG		getTXSpeed();
	ULONG		getRXSpeed();
	void		close()
				{
					enableEvents(0);
				QSerialPort::close();
				}
private:
	DWORD			MaskEvents		=0;
	bool			EventsEnabled	=false;
	__inline BYTE	GetCRC8(BYTE* pData, ULONG lData,BYTE bCRC8=0)
					{
						while(lData--)bCRC8+=(*pData++);
					return(~bCRC8);
					}
	CThread			*pThreadPoll	=NULL;
	bool			XferMetrEnabled =false;
	ULONG			tx_count;
	ULONG			rx_count;
	ULONG			tx_speed;
	ULONG			rx_speed;
	QTimer			tmrXferMetr;
	void			pollpinoutsroutine(void *args);
signals:
	void	FeedEvent(int feedevent); // DWORD в Qt почему-то нестандартный тип. поэтому int)
public slots:
private slots:
	void	FeedPortEventHandler();					// hook Received signal
	void	FeedPortEventHandler(DWORD feedevent);	// main hook signals
	void	FeedPortEventHandler(qint64 byteswritten); // hook Written/Transmitted signal
	void	tmrXferMetr_TimeOutEvent();
};
//------------------------------------------------------------------------------------------
#pragma pack(1)
//************** Неизменяемые данные пакета Адрес =4 байта + Запрос =4 байта **************/
// Variable length data Request/Reading packet
typedef struct _HDR_FEED_ADDR_REQUEST_
{
	DWORD	dwAddress;	/* Адрес назначения в устройстве */
	DWORD	dwRequest;	/* Запрос (либо длина данных) к интерфейсу по указанному адресу в устройстве */

}HDR_FEED_ADDR_REQUEST;
//***************************************** Адрес =1 байт ********************************/
// 1 byte Writing packet
typedef	struct _HDR_FEED_BYTE_ADDR_REQUEST_
{
	BYTE	bAddress;
	BYTE	bData;

}FEED_BYTE_ADDR_REQUEST;

#pragma pack();
/************************************************************************************************
	COM pinout							Qt const							MS const
1. DCD - Data Carrier Detect (in)	- DataCarrierDetectSignal	=0x08	MS_RLSD_ON ((DWORD)0x0080)
2. RXD - Receive Data (in)			- ReceivedDataSignal		=0x02
3. TXD - Transmit Data (out)		- TransmittedDataSignal		=0x01
4. DTR - Data Terminal Ready (out)	- DataTerminalReadySignal	=0x04
5. GND - Signal Ground (-)
6. DSR - Data Set Ready (in)		- DataSetReadySignal		=0x10	MS_DSR_ON ((DWORD)0x0020)
7. RTS - Request to Send (out)		- RequestToSendSignal		=0x40
8. CTS - Clear to Send (in)			- ClearToSendSignal			=0x80	MS_CTS_ON ((DWORD)0x0010)
9. RI - Ring Indicator (in)			- RingIndicatorSignal		=0x20	MS_RING_ON ((DWORD)0x0040)
*************************************************************************************************/
#define FEED_DSR_EVT	((DWORD)QSerialPort::DataSetReadySignal)		// 0x10
#define FEED_CTS_EVT	((DWORD)QSerialPort::ClearToSendSignal)			// 0x80
#define FEED_RI_EVT		((DWORD)QSerialPort::RingIndicatorSignal)		// 0x20
#define FEED_DCD_EVT	((DWORD)QSerialPort::DataCarrierDetectSignal)	// 0x08
#define FEED_RX_EVT		((DWORD)0x400)
#define FEED_TX_EVT		((DWORD)0x800)
#define FEED_INT_EVT	FEED_DSR_EVT
#endif // USBFEEDPORT_H
