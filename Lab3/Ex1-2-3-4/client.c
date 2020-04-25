#include <stdio.h>
#include <sys/socket.h> //for socket(), connect(), send(), recv() functions
#include <arpa/inet.h> // different address structures are declared here
#include <stdlib.h> // atoi() which convert string to integer
#include <string.h>
#include <unistd.h> // close() function
#define BUFSIZE 32
int main()
{
	/* CREATE A TCP SOCKET*/
	
		int sock0 = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		int sock1 = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

		if(sock0 < 0) 
		{ 
			printf ("Error in opening a socket 0");
			exit (0);
		}

		printf ("Client Socket 0 Created.\n");

		if(sock1 < 0) 
		{ 
			printf ("Error in opening a socket 1");
			exit (0);
		}

		printf ("Client Socket 1 Created.\n");
		
	/*CONSTRUCT SERVER ADDRESS STRUCTURE*/
	
		struct sockaddr_in serverAddr;
		memset(&serverAddr,0,sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(12345); //You can change port number here
		serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Specify server's IP address here
		printf("Address assigned.\n");
	
	/*ESTABLISH CONNECTION*/
	
		int c0 = connect(sock0, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
		if (c0 < 0)
		{ 
			printf("Error while establishing connection\n");
			exit (0);
		}
		printf ("Connection Established for Socket 0\n");

		int c1 = connect(sock1, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
		if (c1 < 0)
		{ 
			printf("Error while establishing connection\n");
			exit (0);
		}
		printf ("Connection Established for Socket 1\n");
		
	/*SEND DATA for 0*/
	
		printf ("ENTER MESSAGE FOR SERVER through socket 0 with max 32 characters\n");
		char msg0[BUFSIZE];
		gets(msg0);
		int bytesSent0 = send(sock0,msg0,strlen(msg0),0);
		if (bytesSent0!=strlen(msg0))
		{ 
			printf("Error while sending the message with socket 0");
			exit(0);
		}
		printf ("Data Sent by Socket 0\n");

	/*RECEIVE BYTES for 0*/

		char recvBuffer0[BUFSIZE];
		int bytesRecvd0 = recv(sock0, recvBuffer0, BUFSIZE-1, 0); 
		if(bytesRecvd0 < 0)
		{ 
			printf ("Error while receiving data from server through socket 0"); 
			exit(0);
		}
		recvBuffer0[bytesRecvd0] = '\0';
		printf ("%s\n",recvBuffer0);

	/*SEND DATA for 1*/

		printf ("ENTER MESSAGE FOR SERVER through socket 1 with max 32 characters\n");
		char msg1[BUFSIZE];
		gets(msg1);
		int bytesSent1 = send(sock1,msg1,strlen(msg1),0);
		if (bytesSent1!=strlen(msg1))
		{
			printf("Error while sending the message with socket 1");
			exit(0);
		}
		printf ("Data Sent by Socket 1\n");

	/*RECEIVE BYTES for 1*/

		char recvBuffer1[BUFSIZE];
		int bytesRecvd1 = recv(sock1, recvBuffer1, BUFSIZE-1, 0); 
		if(bytesRecvd1 < 0)
		{ 
			printf ("Error while receiving data from server through socket 0"); 
			exit(0);
		}
		recvBuffer1[bytesRecvd1] = '\0';
		printf ("%s\n",recvBuffer1);
			
	/* CLOSING SOCKET */	
		
		close(sock0);
		close(sock1);
}