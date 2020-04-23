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
	
		struct sockaddr_in serverAddress, clientAddress1,  clientAddress2,  clientAddress3;

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
		{ 
			printf ("Error in listen");
			exit (0);
		}

		printf ("Now Listening\n");
		
	/* HANDLING CLIENT SOCKET */	
		
		char msg1[BUFFERSIZE];
		char msg2[BUFFERSIZE];
		char msg3[BUFFERSIZE];

		int clientLength1 = sizeof(clientAddress1);
		int clientLength2 = sizeof(clientAddress2);
		int clientLength3 = sizeof(clientAddress3);

		int clientSocket1 = accept(serverSocket,(struct sockaddr*)&clientAddress1, &clientLength1);
		int clientSocket2 = accept(serverSocket,(struct sockaddr*)&clientAddress2, &clientLength2);
		int clientSocket3 = accept(serverSocket,(struct sockaddr*)&clientAddress3, &clientLength3);
		
		if (clientSocket1 < 0) 
		{
			printf ("Error in client socket 1"); 
			exit(0);
		}
		if (clientSocket2 < 0) 
		{
			printf ("Error in client socket 1"); 
			exit(0);
		}
		if (clientSocket3 < 0) 
		{
			printf ("Error in client socket 1"); 
			exit(0);
		}
		
		printf("Handling Client 1: %s\n", inet_ntoa(clientAddress1.sin_addr));
		printf("Handling Client 2: %s\n", inet_ntoa(clientAddress2.sin_addr));
		printf("Handling Client 3: %s\n", inet_ntoa(clientAddress3.sin_addr));
		
	/* RECEIVING CLIENT 1 MESSAGE */	
		
		int temp21 = recv(clientSocket1, msg1, BUFFERSIZE, 0);

		if (temp21 < 0)
		{ 
			printf ("problem in temp 21");
			exit (0);
		}

		printf("%s\n", msg1);

	/* RECEIVING CLIENT 2 MESSAGE */	
		
		int temp22 = recv(clientSocket2, msg2, BUFFERSIZE, 0);

		if (temp22 < 0)
		{ 
			printf ("problem in temp 22");
			exit (0);
		}

		printf("%s\n", msg2);

	/* RECEIVING CLIENT 3 MESSAGE */	
		
		int temp23 = recv(clientSocket3, msg3, BUFFERSIZE, 0);

		if (temp23 < 0)
		{ 
			printf ("problem in temp 23");
			exit (0);
		}

		printf("%s\n", msg3);
		
	/* SENDING MESSAGE TO CLIENT 1 */	
		
		printf("ENTER MESSAGE FOR CLIENT 1\n");

		gets(msg1);

		int bytesSent1 = send (clientSocket1,msg1,strlen(msg1),0);

		if (bytesSent1 != strlen(msg1))
		{ 
			printf ("Error while sending message to client 1");
			exit(0);
		}
		
	/* SENDING MESSAGE TO CLIENT 2 */	
		
		printf("ENTER MESSAGE FOR CLIENT 2\n");

		gets(msg2);

		int bytesSent2 = send (clientSocket2,msg2,strlen(msg2),0);

		if (bytesSent2 != strlen(msg2))
		{ 
			printf ("Error while sending message to client 2");
			exit(0);
		}
		
	/* SENDING MESSAGE TO CLIENT 3 */	
		
		printf("ENTER MESSAGE FOR CLIENT 3\n");

		gets(msg3);

		int bytesSent3 = send (clientSocket3,msg3,strlen(msg3),0);

		if (bytesSent3 != strlen(msg3))
		{ 
			printf ("Error while sending message to client 3");
			exit(0);
		}	
		
	/* CLOSING SOCKETS */
	
		close(serverSocket);
		close(clientSocket1);
		close(clientSocket2);
		close(clientSocket3);
}
