#include "server.h"

// main for server
int main()
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(1, 1), &wsd) == 0)
	{
		printf("Connected to socket lib\n");
	}
	else
	{
		return 1;
	}
	return CreateServer();
}

// main for client
/*int main()
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(1, 1), &wsd) != 0)
	{
		printf("Can't connect to socket lib");
		return 1;
	}
	int i = 0;
	srand(time(0));
	rand();
	int number = rand();
	while (i < 1000)
	{
		SendData2Server(++i, number);
		Sleep(rand() % 10);
	}
	printf("Session is closed\n");
	Sleep(1000);
	return 0;
}*/