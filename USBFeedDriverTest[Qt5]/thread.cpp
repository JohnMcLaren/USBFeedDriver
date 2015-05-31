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
	if(threadroutine)
    {
		qDebug("Start thread [%X]", currentThreadId());
		threadroutine(threadargs);
    }
	qDebug("Thread [%X] ended.", currentThreadId());
}
//---------------------------------------------------------------------
