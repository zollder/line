/*
 * ServerThread.cpp
 *
 *      Author: AUV Capsone
 */


#include "ServerThread.h"

//-----------------------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------------------
ServerThread::ServerThread(SocketServer* server, int threadId)
{
	setThreadId(threadId);
	socketServer = server;
}
//-----------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------
ServerThread::~ServerThread()
{
	if( stop() != 0 )
	{
		syslog(LOG_NOTICE,"[KPI::CLIENT THREAD] failed stop");
		kill();
	}

}

//-----------------------------------------------------------------------------------------
// Overrides BaseThread's run() method
//-----------------------------------------------------------------------------------------
void* ServerThread::run()
{
	syslog(LOG_NOTICE,"[KPI::THREAD] START");

	socketServer->start();
	socketServer->run();

	syslog(LOG_NOTICE,"[KPI::THREAD] END");

	return NULL;
}

int ServerThread::stop()
{
	syslog(LOG_NOTICE,"[KPI::THREAD] STOP");
	return pthread_cancel(getThreadId());
}

int ServerThread::kill()
{
	syslog(LOG_NOTICE,"[KPI::THREAD] KILL");
	return pthread_kill(getThreadId() , SIGQUIT);
}


