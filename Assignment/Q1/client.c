/* 
	client.c

	Computer Networks: Assignment - Sem 2 2019-20
	Ishan Sharma - 2016B2A70773P

	Have to do the following: 

*/

#include "packet.h"

#define RETRATIME 2 // (In Terms of Seconds) Retransmission Time

int main()
{
	char buffer[100];

	FILE *f = fopen("input.txt","r");

	if(f == NULL)
	{
		printf("Error opening the file input.txt");
		exit(0);
	}

	int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(sock < 0) 
	{ 
		printf ("Error in opening a client socket");
		exit (0);
	}

	struct sockaddr_in serverAddr;
	memset(&serverAddr,0,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(12345); //You can change port number here
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Specify server's IP address here

	int conn = connect(sock,(struct sockaddr*) &serverAddr, sizeof(serverAddr));

	if(conn < 0)
	{ 
		printf("Error while establishing connection\n");
		exit (0);
	}

	Packet nP;
	nP.psize = 69;
	nP.seq = 0;
	nP.lp = 0;
	nP.dora = 1;
	nP.ch = 0;
	strcpy(nP.data,"Hello This Is For Testing");

	int bytesSent = send(sock, &nP, sizeof(Packet), 0);

	if(bytesSent != sizeof(Packet))
	{
		printf("Error While Sending The Packet");
		exit(0);
	}

	close(sock);

	return 0;
}