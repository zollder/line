/*
 * ServerThread.h
 * Author: AUV Capstone
 */

#include "../sys/BaseThread.h"
#include "SocketServer.h"
#include <signal.h>
#include "../commons/Config.h"

#ifndef serverthread_h
#define serverthread_h


class ServerThread: public BaseThread
{

public:

	ServerThread(SocketServer* server, int threadId);
	~ServerThread();
	void* run();				// overrides BaseThread's run() method

	int stop();
	int kill();

private:
	SocketServer* socketServer;
};

#endif /* SRC_SERVERTHREAD_H_ */
