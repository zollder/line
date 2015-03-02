/*
 * SocketServer.cpp
 *
 *      Author: AUV Capstone
 */
#include "SocketServer.h"

//-----------------------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------------------
SocketServer::SocketServer(DataService* service)
{
	init(PORT, CONNECTIONS);
	dataService = service;
}

SocketServer::SocketServer(int port, int connections, DataService* service)
{
	init(port, connections);
	dataService = service;
}

//-----------------------------------------------------------------------------------------
// initialization of variables
//-----------------------------------------------------------------------------------------
void SocketServer::init(int port, int connections)
{
	portNumber = port;
	maxUser = connections;

	// Initialize System Log
	logger = new Logger("Socket Server [KPI]");

	//Initializing Socket data
	connfd = -1;
	sockfd = -1;

	//Initialize socket structures
	memset(&server_addr, '0', sizeof(server_addr));
	memset(&client_addr, '0', sizeof(client_addr));
}

//-----------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------
SocketServer::~SocketServer()
{
	stop();
	delete logger;
}

//-----------------------------------------------------------------------------------------
// Class Execution
//-----------------------------------------------------------------------------------------
void SocketServer::start()
{
	if (!dataService)
	{
		logger->error("[ERROR] missing data holder.");
		exit(EXIT_FAILURE);
	}

	//create socket inside the kernel and return socket descriptor
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		logger->error("[ERROR] failed to open socket.");
		exit(EXIT_FAILURE);
	}

	//initialize server structure
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(portNumber);

	client_len = sizeof(client_addr);

	//bind host address
	if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr) ) < 0)
	{
		int delay = 8;
		while (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr) ) < 0 && --delay)
		{
			//Max Linux delay is 60 seconds by default.
			//creates TIME_WAIT for making sure all socket connection had chance to send data back or from server.
			sleep(10);
		}

		if (delay == 0)
		{
			logger->error("[ERROR] failed to bind socket.");
			exit(EXIT_FAILURE);
		}
	}

	logger->info("[INFO] socket server successfully initialized.");
}

void SocketServer::stop()
{
	// Closing Active Session
	if (connfd < 0)
		logger->notice("[NOTICE] session already closed.");
	else
	{
		if (shutdown(connfd, SHUT_RDWR) < 0)
			logger->error("[NOTICE] invalid session descriptor.");
		else
			logger->info("[INFO] socket session closed.");

		connfd = -1;
	}

	// Closing Server Socket
	if (sockfd < 0)
		logger->notice("[NOTICE] socket server already closed");
	else
	{
		if (close(sockfd) < 0)
			logger->error("[NOTICE] failed to close socket server.");
		else
			logger->info("[INFO] socket server closed.");

		sockfd = -1;
	}
}

void SocketServer::run()
{
	// Start Listening for clients
	listen(sockfd, maxUser);
	logger->info("[INFO] listening ...");

	while (sockfd > 1)
	{
		//wait for clients
		connfd = accept(sockfd, (struct sockaddr *) &client_addr, (socklen_t *) &client_len);

		if (connfd < 1)
		{
			logger->error("[ERROR] failed to accept connection.");
			close( connfd);
		}
		else
		{
			int result = send(connfd, dataService->getData(), dataService->getSize(), 0);
			if (result < 0)
				logger->error("[ERROR] buffer dispatch error.");

			close( connfd);
		}

	}

	logger->info( "[INFO] End Run");
}
