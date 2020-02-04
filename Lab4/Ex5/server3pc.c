#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAXPENDING 5
#define BUFFERSIZE 32
int main ()
{
	/*CREATE A TCP SOCKET*/
	
		int serverSocket = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (serverSocket < 0) 
		{ 
			printf ("Error while server socketcreation"); 
			exit (0); 
		}
		printf ("Server Socket Created\n");
		
	/*CONSTRUCT LOCAL ADDRESS STRUCTURE*/
	
		struct sockaddr_in serverAddress, clientAddress;
		memset (&serverAddress, 0, sizeof(serverAddress));
		/*memset() is used to fill a block of memory with a particular value*/
		
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(12345);
		serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
		printf ("Server address assigned\n");
		
	/* ESABLISHING CONNECION */	
		
		int temp = bind(serverSocket,(struct sockaddr*)&serverAddress,sizeof(serverAddress));
		if (temp < 0)
		{ 
			printf ("Error while binding\n");
			exit (0);
		}
		printf ("Binding successful\n");
		
	/* LISTENING FOR CONNECIONS */		
		
		int temp1 = listen(serverSocket, MAXPENDING);
		if (temp1 < 0)
		{ printf ("Error in listen");
		exit (0);
		}
		printf ("Now Listening\n");
		
	/* HANDLING CLIENT SOCKET */	
		
		char msg[BUFFERSIZE];
		int clientLength = sizeof(clientAddress);
		int clientSocket = accept(serverSocket,(struct sockaddr*)&clientAddress, &clientLength);
		
		if (clientLength < 0) 
		{
			printf ("Error in client socket"); 
			exit(0);
		}
		
		printf("Handling Client %s\n", inet_ntoa(clientAddress.sin_addr));
		
	/* RECEIVING CLIENT MESSAGE */	
		
		int temp2 = recv(clientSocket, msg, BUFFERSIZE, 0);
		if (temp2 < 0)
		{ 
			printf ("problem in temp 2");
			exit (0);
		}
		printf("%s\n", msg);
		
	/* SENDING MESSAGE TO CLIENT */	
		
		printf("ENTER MESSAGE FOR CLIENT\n");
		gets(msg);
		int bytesSent = send (clientSocket,msg,strlen(msg),0);
		if (bytesSent != strlen(msg))
		{ 
			printf ("Error while sending message to client");
			exit(0);
		}
		
	/* CLOSING SOCKETS */
	
		close (serverSocket);
		close (clientSocket);
}
