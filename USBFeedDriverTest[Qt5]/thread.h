#ifndef THREAD_H
#define THREAD_H

#include <QtWidgets>
//-----------------------------------------------------------------------------
typedef	void (*THREAD_ROUTINE)(void *args);
//----------------------------------------------------------------------------
class CThread : public QThread
{
	Q_OBJECT

public:
	CThread(THREAD_ROUTINE startaddress, void *args =NULL);
	~CThread();

private:
	THREAD_ROUTINE	threadroutine;
	void			*threadargs;

protected:
	void run();
};
//............................................................................

//----------------------------------------------------------------------------
#endif // THREAD_H
