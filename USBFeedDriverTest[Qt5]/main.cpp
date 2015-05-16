#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec> // подключение класса кодека текста

int main(int argc, char *argv[])
{
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8")); //изменения
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); //изменения
	//QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8")); //изменения
//--------------------------------------------------------------------------------------
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
