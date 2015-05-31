#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	MemoryValues(CountCellMemoryTBL),
	ASCIIValues(CountRowMemoryTBL)
{
	ui->setupUi(this);
    qDebug("BYTE: %u, WORD: %u, DWORD: %u, QWORD: %u", sizeof(BYTE), sizeof(WORD), sizeof(DWORD), sizeof(QWORD));

	/* таймера опросов (COM порты/память MCU, скорость/передачи данных) */
	connect(&tmrPollUSBDevices, SIGNAL(timeout()), SLOT(tmrPollUSBDevices_TimeOutEvent()));
	connect(&tmrPollDeviceMemory, SIGNAL(timeout()), SLOT(tmrPollDeviceMemory_TimeOutEvent()));
	connect(&tmrSpeedUpdate, SIGNAL(timeout()), SLOT(tmrSpeedUpdate_TimeOutEvent()));

	/* назначение обработчика прерываний от фида */
	connect(&feed, SIGNAL(FeedEvent(int)), SLOT(FeedEvent(int)));

	tmrPollUSBDevices.start(1000);// обновление списка USB устройств по таймеру

//	qDebug() << QStyleFactory::keys ();// available styles window on target platform

	ui->cmdOpenCloseFeed->setStyle(QStyleFactory::create("Fusion"));

	ui->txtAnswer->setStyle(QStyleFactory::create("Fusion"));
	ui->txtAnswer->setStyleSheet("background-color: #C0C0C0;");

/*** Инициализация таблицы памяти USBFeed устройства ***/
	ui->tblMemory->setColumnCount(CountColumnMemoryTBL);
	ui->tblMemory->setRowCount(CountRowMemoryTBL);
	ui->tblMemory->setHorizontalHeaderLabels(QString("0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F,ASCII").split(","));
    ui->tblMemory->horizontalHeader()->setStyleSheet("text-align: center; color: darkblue;  font-family: Sans Serif; font: 8.5px;");
    ui->tblMemory->verticalHeader()->setStyleSheet("text-align: center; color: darkblue;  font-family: Sans Serif; font: 8.5px;");
    ui->tblMemory->setStyleSheet("text-align: center; color: black; font-family: Sans Serif; font: 8.5px;"); //

	for(int c=0;c < (CountColumnMemoryTBL-1);c++)
	{
        ui->tblMemory->setColumnWidth(c, ui->tblMemory->width() / (CountColumnMemoryTBL + 6));
	}

	for(int c=0;c < CountRowMemoryTBL;c++)
	{
        ui->tblMemory->setRowHeight(c, (ui->tblMemory->height() / (CountRowMemoryTBL + 2)));
	}
	ui->tblMemory->horizontalHeader()->setSectionResizeMode((CountColumnMemoryTBL-1), QHeaderView::Stretch);

	/* связывание таблицы с массивами данных */
	for(int c=0, row=0,col=0;c < CountCellMemoryTBL;c++,col++)
	{
	ui->tblMemory->setItem(row,col,&MemoryValues[c]);

		if(col >= (CountColumnMemoryTBL-2))
		{
			ui->tblMemory->setItem(row,col+1,&ASCIIValues[row]);
			col=(-1);
			row++;
		}
	}
	updateVerticalHeaderLabelsTBL();
	ui->tblMemory->installEventFilter(this);// ловушка на скрол мыши над таблицей

    ui->barProgressTXfer->setVisible(false);
    setFixedSize(size());
/*** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***/
}

MainWindow::~MainWindow()
{
	delete ui;
}
//-------------------------------------------------------------------
void MainWindow::tmrSpeedUpdate_TimeOutEvent()
{
DWORD TXB =(feed.getTXSpeed() << 2);        // * 4     = bytes per second
DWORD TXSpeed =(TXB << 3);                  // * 8     = bits per second
DWORD RXSpeed =(feed.getRXSpeed() << 5);    // * 4 * 8 = bits per second

	if(TXSpeed < 1024)
	{
		ui->lblUPSpeed->setText(QString().sprintf("Upload (bit/sec): %u", TXSpeed));
	}
	else	if(TXSpeed < 1048576)
			{
				ui->lblUPSpeed->setText(QString().sprintf("Upload (kbit/sec): %6.2f", TXSpeed / 1024.0));
			}
            else    if(TXSpeed < 1073741824)
                    {
                        ui->lblUPSpeed->setText(QString().sprintf("Upload (Mbit/sec): %6.2f", TXSpeed / 1048576.0));
                    }
                    else
                    {
                        ui->lblUPSpeed->setText(QString().sprintf("Upload (Gbit/sec): %6.2f", TXSpeed / 1073741824.0));
                    }

//....................................................................................
	if(RXSpeed < 1024)
	{
		ui->lblDOWNSpeed->setText(QString().sprintf("Download (bit/sec): %u", RXSpeed));
	}
	else	if(RXSpeed < 1048576)
			{
				ui->lblDOWNSpeed->setText(QString().sprintf("Download (kbit/sec): %6.2f", RXSpeed / 1024.0));
			}
            else    if(RXSpeed < 1073741824)
                    {
                        ui->lblDOWNSpeed->setText(QString().sprintf("Download (Mbit/sec): %6.2f", RXSpeed / 1048576.0));
                    }
                    else
                    {
                        ui->lblDOWNSpeed->setText(QString().sprintf("Download (Gbit/sec): %6.2f", RXSpeed / 1073741824.0));
                    }

    setProgressBar(TXB);
}
//--------------------------------------------------------------------------------------
void MainWindow::FeedEvent(int feedevent)
{
    if(feedevent & FEED_INT_EVT)
	{
		ui->txtAnswer->setTextColor(Qt::red);
		ui->txtAnswer->insertPlainText("Feed Interrupt Event\n");
		ui->txtAnswer->setTextColor(Qt::black);
	}

	if(feedevent & FEED_RX_EVT)
	{
	BYTE buff[512];
    DWORD rb;

		/* read answer */
		while(rb =feed.ReadFeed(buff, 512, 0)) // Asynchronic (read data IF present)
		{
			ui->txtAnswer->setTextColor(Qt::darkMagenta);
			ui->txtAnswer->insertPlainText("FeedOut:[" + QString("%1").arg(rb) + "]>> ");
			ui->txtAnswer->setTextColor(Qt::black);

			ui->txtAnswer->insertPlainText(QString::fromLocal8Bit((char*)buff) + "\n");
		}
	}

ui->txtAnswer->ensureCursorVisible();
}
//-------------------------------------------------------------------
void MainWindow::on_cmdOpenCloseFeed_clicked()
{
	if(ui->cmdOpenCloseFeed->isChecked())		//
	{
	ui->cmdOpenCloseFeed->setChecked(true);		//
	feed.setPortName(feed.findUSBFeedByName("USB Feed Port"));//

		if(feed.open(QIODevice::ReadWrite))//
		{
		tmrPollUSBDevices.stop();
		ui->txtPort->setText("0x0" + QString::number((DWORD)feed.handle(),16));
		ui->cmdOpenCloseFeed->setPalette(QColor(0x98FB98));

			feed.ResetFeedBufferOfDevice();
            feed.enableEvents(FEED_RX_EVT); //
                ui->chkFeedInt->clicked(ui->chkFeedInt->isChecked());
			feed.enableTransferometr(true);
			tmrSpeedUpdate.start(1000);

            ui->txtAnswer->setText("");
            ui->statusBar->showMessage("Feed opened");
		}
		else
		{
            ui->statusBar->showMessage("Error: " + feed.portName() + " - " + feed.errorString());
			ui->cmdOpenCloseFeed->setChecked(false); //
			ui->cmdOpenCloseFeed->setPalette(Qt::red);
		}
	}
	else										//
	{
		ui->chkPollMem->setChecked(false);
		ui->cmdOpenCloseFeed->setChecked(false);//
		feed.close();
		ui->cmdOpenCloseFeed->setPalette(QColor(0xFFE0DFE3));
		ui->txtPort->setText("0x0000");
		tmrPollUSBDevices.start(1000);
        ui->statusBar->showMessage("Feed closed");
	}
}
//...................................................................
void MainWindow::on_cmdSend_clicked()
{
if(!feed.isOpen())return;
DWORD dwAddrIFace;

memcpy(&dwAddrIFace,ui->txtAddressIFace->text().toLatin1(),4);

if(ui->txtAddressIFace->text().left(2) == "0x")
	dwAddrIFace =Hex2DWORD(ui->txtAddressIFace->text());

ui->txtAnswer->setTextColor(Qt::blue);
ui->txtAnswer->insertPlainText("FeedIn:["  + QString("%1").arg(ui->txtMessage->text().length()) + "]<< ");
ui->txtAnswer->setTextColor(Qt::black);
ui->txtAnswer->insertPlainText(ui->txtAddressIFace->text() + " : " + ui->txtMessage->text() + "\n");

	/* send message (address : message) */
    feed.WriteFeedToAddrEx(ui->chkRW->isChecked() ? (dwAddrIFace | 0x80000000) : (dwAddrIFace & 0x7FFFFFFF),
                           (BYTE*)ui->txtMessage->text().toLocal8Bit().data());
}
//...................................................................
void MainWindow::on_cmdRead_clicked()
{
fillTableFromFeed(Hex2DWORD(ui->txtAddressMemory->text()),0,CountCellMemoryTBL);
}
//...................................................................
bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
		if (object == ui->tblMemory && (event->type() == QEvent::Wheel))
		{
		QWheelEvent *wheelevent =static_cast<QWheelEvent* > (event);
		DWORD startaddr =Hex2DWORD(ui->txtAddressMemory->text());
		DWORD offset =(CountRowMemoryTBL-1)*(CountColumnMemoryTBL-1);

			if(wheelevent->delta() > 0)
			{
				if(startaddr > 0)
				{
					for(int c=(CountCellMemoryTBL-1);c >= (CountColumnMemoryTBL-1);c--)
						MemoryValues[c].setText(MemoryValues[c-(CountColumnMemoryTBL-1)].text());

					startaddr -=(CountColumnMemoryTBL-1);
					ui->txtAddressMemory->setText("0x" + DWORD2Hex(startaddr));
					fillTableFromFeed(startaddr, 0,(CountColumnMemoryTBL-1));
				}
			}
			else
			{
				for(int c=0;c < CountCellMemoryTBL-(CountColumnMemoryTBL-1);c++)
					MemoryValues[c].setText(MemoryValues[c+(CountColumnMemoryTBL-1)].text());

				startaddr +=(CountColumnMemoryTBL-1);
				ui->txtAddressMemory->setText("0x" + DWORD2Hex(startaddr));
				fillTableFromFeed(startaddr + offset, offset,(CountColumnMemoryTBL-1));
			}
		return true;
		}
return QObject::eventFilter(object, event);
}
//-------------------------------------------------------------------
void MainWindow::fillTableFromFeed(const DWORD startaddress, const int startindex, ULONG length)
{
DWORD AddrOfReg,ValOfReg;
BYTE* pValOfRegByte =(BYTE*)(&ValOfReg);
AddrOfReg =startaddress;
if(startindex >= CountCellMemoryTBL)return;
if((startindex+length) > CountCellMemoryTBL)length =CountCellMemoryTBL - startindex;
length =((length >> 2) << 2); // выравнивание по границе DWORD - ((length / 4) * 4)

	for(int c=startindex;c < (startindex+length);c +=4, AddrOfReg +=4)
	{
		if(feed.SendFeedRequest(dwsMEM,AddrOfReg,4) && feed.ReadFeed(&ValOfReg,4))
		{
			MemoryValues[c].setText(Byte2Hex(*pValOfRegByte));
			MemoryValues[c+1].setText(Byte2Hex(*(pValOfRegByte+1)));
			MemoryValues[c+2].setText(Byte2Hex(*(pValOfRegByte+2)));
			MemoryValues[c+3].setText(Byte2Hex(*(pValOfRegByte+3)));
		}
		else
		{
			MemoryValues[c].setText("");
			MemoryValues[c+1].setText("");
			MemoryValues[c+2].setText("");
			MemoryValues[c+3].setText("");
		}
	}
updateVerticalHeaderLabelsTBL(Hex2DWORD(ui->txtAddressMemory->text()));
}
//-------------------------------------------------------------------
void MainWindow::updateVerticalHeaderLabelsTBL(const DWORD startaddress)
{
QStringList AddressColumn;
QByteArray ASCII;
AddressColumn.clear();

	for(int r=0;r < CountRowMemoryTBL;r++)
	{
	ASCII.clear();
	AddressColumn.append(DWORD2Hex(startaddress+(r*(CountColumnMemoryTBL-1))));
		for(int c=0;c < (CountColumnMemoryTBL-1);c++)
		{
			ushort ascii =QChar(ui->tblMemory->item(r,c)->text().toUShort(0,16)).toLatin1();
			if(ascii < 33)ascii ='.';
			ASCII +=ascii;
		}
	ASCIIValues[r].setText(QString::fromLocal8Bit(ASCII));
	}
ui->tblMemory->setVerticalHeaderLabels(AddressColumn);
}
//-------------------------------------------------------------------
void MainWindow::tmrPollUSBDevices_TimeOutEvent()
{
	ui->txtAnswer->setText(feed.getAvailablesPortsName());
}
//-------------------------------------------------------------------
void MainWindow::tmrPollDeviceMemory_TimeOutEvent()
{
DWORD startaddr =Hex2DWORD(ui->txtAddressMemory->text());
BYTE Buff[CountCellMemoryTBL];

	if(feed.SendFeedRequest(dwsMEM, startaddr, CountCellMemoryTBL) &&
	   feed.ReadFeed(Buff,CountCellMemoryTBL))
	{
		for(int c =0;c < CountCellMemoryTBL;c++)
			MemoryValues[c].setText(Byte2Hex(Buff[c]));

		updateVerticalHeaderLabelsTBL(startaddr);
	}
}
//-------------------------------------------------------------------
QString  MainWindow::Byte2Hex(BYTE bValue)
{
return QString("0%1").arg(bValue,0,16).right(2).toUpper();
}
//........................................................
QString  MainWindow::DWORD2Hex(DWORD dwValue)
{
return QString("0000000%1").arg(dwValue,0,16).right(8).toUpper();
}
//........................................................
DWORD MainWindow::Hex2DWORD(QString hexValue)
{
return hexValue.toUInt(0,16);
}
//-------------------------------------------------------------------
void MainWindow::setProgressBar(DWORD dwAddValue, DWORD dwMaxValue)
{
    if(dwMaxValue)
    {
        ui->barProgressTXfer->setVisible(true);
        ui->barProgressTXfer->setMaximum(dwMaxValue);
        ui->barProgressTXfer->setMinimum(0);
        ui->barProgressTXfer->setValue(0);
    }
        else
    if(ui->barProgressTXfer->isVisible())
    {
        if((ui->barProgressTXfer->value() >= ui->barProgressTXfer->maximum()) || ((!dwAddValue) && ui->barProgressTXfer->value()))
        {
            ui->barProgressTXfer->setVisible(false);
        }

        if(dwAddValue > ui->barProgressTXfer->maximum())
            dwAddValue =ui->barProgressTXfer->maximum();

        ui->barProgressTXfer->setValue(ui->barProgressTXfer->value() + dwAddValue);
    }
}
//-------------------------------------------------------------------
void MainWindow::on_chkPollMem_toggled(bool checked)
{
	if(checked)
		tmrPollDeviceMemory.start(1000);
	else
		tmrPollDeviceMemory.stop();
}

void MainWindow::on_txtAddressMemory_textEdited(const QString &arg1)
{
	ui->chkPollMem->setChecked(false);
}
//-------------------------------------------------------------------

void MainWindow::on_cmdFile_clicked()
{
QString fileName = QFileDialog::getOpenFileName(this,
												tr("Send file.."),
												0,
												tr("All Files (*.*)"));
	if (!fileName.isEmpty())
	{
	QFile file(fileName);

		if (file.open(QFile::ReadOnly))
		{
			ui->txtAnswer->insertPlainText(QString().sprintf("Send file: %ls - size: %u Bytes\n",
															 QFileInfo(file).fileName().data(),
															 file.size()));
		QByteArray buff = file.readAll();
		DWORD dwAddrIFace;

            memcpy(&dwAddrIFace,ui->txtAddressIFace->text().toLatin1(), 4);

			if(!feed.WriteFeedToAddr(dwAddrIFace, (BYTE*)buff.data(), buff.size()))
			{
				ui->txtAnswer->insertPlainText("Error send file!\n");
			}
            else
                setProgressBar(0, file.size());

		file.close();
		}
	}
}

void MainWindow::on_chkFeedInt_clicked(bool checked)
{
   feed.enableEvents((checked) ? (feed.MaskEvents | FEED_INT_EVT) : (feed.MaskEvents & (~FEED_INT_EVT)));
}
