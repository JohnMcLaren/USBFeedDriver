#include "thread.h"
//---------------------------------------------------------------------
CThread::CThread(THREAD_ROUTINE startaddress, void *args)
	: threadroutine(startaddress), threadargs(args)
{
	start();
}
//---------------------------------------------------------------------
CThread::~CThread()
{
	quit();
	wait();
}
//---------------------------------------------------------------------
void CThread::run()
{
/*** Without this 'QObject' we have problem for "Release Build" ***/
QObject* pObj = new QObject;
delete pObj;
/*** "QObject::killTimers: timers cannot be stopped from another thread" ***/

	if(threadroutine)
		threadroutine(threadargs);
}
//---------------------------------------------------------------------
