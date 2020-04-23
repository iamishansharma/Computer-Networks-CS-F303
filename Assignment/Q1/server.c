/* 
	server.c

	Computer Networks: Assignment - Sem 2 2019-20
	Ishan Sharma - 2016B2A70773P

	Have to do the following: 

*/

#include "packet.h"

#define MAXPENDING 2
#define PDR 10 		// Packet Drop Rate

int main()
{

	int serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(serverSocket < 0) 
	{ 
		printf ("Error while server socket creation"); 
		exit(0); 
	}

	struct sockaddr_in serverAddress, clientAddress;
	memset (&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(12345);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	int binder = bind(serverSocket,(struct sockaddr*)&serverAddress,sizeof(serverAddress));

	if (binder < 0)
	{ 
		printf ("Error while binding\n");
		exit(0);
	}

	int listener = listen(serverSocket, MAXPENDING);

	if (listener < 0)
	{ 
		printf ("Error in listen");
		exit (0);
	}

	int clientLength = sizeof(clientAddress);
	int clientSocket = accept(serverSocket,(struct sockaddr*)&clientAddress, &clientLength);
	
	if (clientLength < 0) 
	{
		printf ("Error in client socket"); 
		exit(0);
	}

	Packet P;

	int reciever = recv(clientSocket, &P, sizeof(P), 0);

	if (reciever < 0)
	{ 
		printf ("problem in temp 2");
		exit (0);
	}

	printPacket(P);

	close (serverSocket);	
	close (clientSocket);

	return 0;
}