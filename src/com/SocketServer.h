#include <stdio.h>
#include <stdlib.h>		//EXIT FUNCTION
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>		//memset, strlen
#include <unistd.h>		//close

/** Dependencies */
#include "../commons/Config.h"
#include "../data/DataService.h"
#include "../commons/Logger.h"

#ifndef socketserver_h
#define socketserver_h

/**
 * Socket server implementation
 * Used to send corresponding data back to the client through the socket connection.
 **/
//-----------------------------------------------------------------------------------------------------
class SocketServer 
{
	public:

		SocketServer(DataService *);		//Default Server Config
		SocketServer(int port, int connections, DataService *);

		~SocketServer();

		void run();							//start listening to port
		void start();						//initializes the server
		void stop();						//close Socket Server Process
		void init(int, int);				//Initialize variables

	private:

		int sockfd = 0;						//server socket descriptor
		int connfd = 0;						//client connection descriptor
		int portNumber = 0;					//listening port number
		int maxUser = 0;					//maximum number of users connected
		int client_len = 0;					//length of the client IP

		struct sockaddr_in server_addr;	//server address
		struct sockaddr_in client_addr;	//client address

		Logger* logger;
		DataService* dataService;
};

#endif
