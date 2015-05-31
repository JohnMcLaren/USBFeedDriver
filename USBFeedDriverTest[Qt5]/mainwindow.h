#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QTimer>
#include "usbfeedport.h"

#define	CountColumnMemoryTBL	(17)
#define	CountRowMemoryTBL		(17)
#define	CountCellMemoryTBL		((CountColumnMemoryTBL-1)*CountRowMemoryTBL)

#define STR(str)	QTextCodec::codecForName("Windows-1251")->toUnicode(str)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
    Ui::MainWindow  *ui;
	CUSBFeedPort	feed;
	QTimer			tmrPollUSBDevices;
	QTimer			tmrPollDeviceMemory;
	QTimer			tmrSpeedUpdate;
	QVector<QTableWidgetItem> MemoryValues;
	QVector<QTableWidgetItem> ASCIIValues;
	QString			Byte2Hex(BYTE bValue);
	QString			DWORD2Hex(DWORD dwValue);
	DWORD			Hex2DWORD(QString hexValue);
	QGridLayout		*mainLayout;
	void			fillTableFromFeed(const DWORD startaddress, const int startindex, ULONG length);
	void			updateVerticalHeaderLabelsTBL(const DWORD startaddress=0);
    void            setProgressBar(DWORD dwAddValue, DWORD dwMaxValue =0);

protected:
	bool			eventFilter(QObject *object, QEvent *event);

private slots:
	void	tmrPollUSBDevices_TimeOutEvent();
	void	tmrPollDeviceMemory_TimeOutEvent();
	void	tmrSpeedUpdate_TimeOutEvent();
	void	on_cmdOpenCloseFeed_clicked();
	void	on_cmdSend_clicked();
	void	on_cmdRead_clicked();
	void	on_chkPollMem_toggled(bool checked);
	void	on_txtAddressMemory_textEdited(const QString &arg1);
	void	FeedEvent(int feedevent);
	void on_cmdFile_clicked();
    void on_chkFeedInt_clicked(bool checked);
};
#endif // MAINWINDOW_H
