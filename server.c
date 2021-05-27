#include "server.h"

pthread_mutex_t mutex;
pthread_mutex_t mutex_file;

void mutex_lock()
{
	pthread_mutex_lock(&mutex);
	pthread_mutex_lock(&mutex_file);
}

void mutex_unlock()
{
	pthread_mutex_unlock(&mutex_file);
	pthread_mutex_unlock(&mutex);
}

void* ClientStart(void* param)
{
	// function accepts only 1 parameter

	SOCKET client = (SOCKET)param;
	char recieve[1024], transmit[1024];
	int ret; // number of bytes read

	sprintf_s(transmit, "Login: ");

	// send the message to the client
	ret = send(client, transmit, sizeof(transmit), 0);

	if (ret == SOCKET_ERROR)
	{
		mutex_lock();
		printf("Error sending data\n");
		mutex_unlock();
		return (void*)2;
	}

	// the client sends some message, put it to the buffer "recieve"
	ret = recv(client, recieve, 1024, 0);

	if (!ret || ret == SOCKET_ERROR)
	{
		mutex_lock();
		printf("Error getting data\n");
		fprintf(stdout, "test");
		mutex_unlock();
		return (void*)1;
	}
	recieve[ret] = '\0';

	mutex_lock();
	fprintf(stdout, "test");
	printf("%s\n", recieve);
	mutex_unlock();
	sprintf_s(transmit, "%s %s %s\n", "Your data", recieve, "recieved");

	// send the message to the client
	ret = send(client, transmit, sizeof(transmit), 0);

	if (ret == SOCKET_ERROR)
	{
		mutex_lock();
		printf("Error sending data\n");
		mutex_unlock();
		return (void*)2;
	}

	return (void*)0;
}

int CreateServer()
{
	// Create 2 sockets: 
	// a socket for a server and 
	// a socket for a client, which is connected with the server
	// The socket is an application entry point.
	// It should know an IP-address, a port and a protocol
	SOCKET server, client;
	sockaddr_in localaddr, clientaddr;
	int size;

	// create a socket for the server
	server = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

	// if a socket for the server was NOT created, return 1
	if (server == INVALID_SOCKET)
	{
		printf("Error create server\n");
		return 1;
	}
	localaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // set server address
	localaddr.sin_family = AF_INET; // set a family a Internet protocols for the server
	localaddr.sin_port = htons(5510); // set a port number for the server
	// port number is for example, must be more than 1024

	// bind server with the structure localaddr
	// if bind was NOT successful, return 2 
	if (bind(server, (sockaddr*)&localaddr, sizeof(localaddr)) == SOCKET_ERROR)
	{
		printf("Can't start server\n");
		return 2;
	}
	else
	{
		printf("Server is started\n");
	}
 
	listen(server, 50); // 50 clients can be in a queue
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutex_file, NULL);

	// the server works always, before closing the server, it should save the history
	while (1) 
	{
		size = sizeof(clientaddr);

		// "catch" a client, write the info about the client 
		// to the structure clientaddr
		client = accept(server, (sockaddr*)&clientaddr, &size);

		// if a client was NOT "caught" and go to the next iteration
		if (client == INVALID_SOCKET)
		{
			printf("Error accept client\n");
			continue;
		}
		else
		{
			printf("Client is accepted\n");
		}
		pthread_t mythread;
		// here the function ClientStart begins working,
		// client is an argument of the function
		int status = pthread_create(&mythread, NULL, ClientStart, (void*)client);
		pthread_detach(mythread);
	}
	pthread_mutex_destroy(&mutex_file);
	pthread_mutex_destroy(&mutex);
	printf("Server is stopped\n");
	closesocket(server);
	return 0;
}

