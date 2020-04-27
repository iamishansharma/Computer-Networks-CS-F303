/* 

	server.c

	Computer Networks: Assignment - Sem 2 2019-20
	Ishan Sharma - 2016B2A70773P

	Have to do the following: 

*/

#include "packet.h"

#define MAXPENDING 2

char cctime[20];

void die(char *s)
{
	perror(s);
	exit(1);
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Command Line Arguments on as per format. Check readme.txt.\n");
		exit(1);
	}
	
	char filename[50];
	memset(filename,0,50);
	strcpy(filename,argv[1]);	// SET output file name here

	FILE *f = fopen(filename,"w");

	if(f == NULL)
	{
		printf("Error opening file %s",filename);
		exit(1);
	}

	int port1 = 0;

	printf("Port Number for Server:  ");
	scanf("%d",&port1);

	int relayport1 = 0;

	printf("Port Number for Relay 1: ");
	scanf("%d",&relayport1);

	int relayport2 = 0;

	printf("Port Number for Relay 2: ");
	scanf("%d",&relayport2);

	struct sockaddr_in si_me, si_other;
	int s, i, slen = sizeof(si_other) , recv_len;
	
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		die("socket");
	}
	 
	memset((char *) &si_me, 0, sizeof(si_me));
	 
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(port1);
	si_me.sin_addr.s_addr = inet_addr(server_addr);

	if(bind(s, (struct sockaddr*)&si_me, sizeof(si_me)) == -1)
	{
		die("bind");
	}

	printf("Server running .....\n");
	
	Frame buffer[WINDOW_SIZE];

	printf("\n-----------------------------------------------------------------\n");
	printf("NODENAME EVENT     TIMESTAMP      PKTTYPE SEQ   SOURCE      DEST\n");
	printf("-----------------------------------------------------------------\n");

	Frame recvframe;
	Frame sendACK;

	int eofflag = 0;

	while(!eofflag)
	{
		getCurrentTime();
		if(recvfrom(s, &recvframe, sizeof(Frame), 0, (struct sockaddr *) &si_other, (socklen_t *)&slen) == -1)
		{
			die("recvfrom()");
		}
		recvframe.eve = 1;
		printLog(recvframe.dest, recvframe);

		printFrameFile(recvframe,f);

		sendACK = recvframe;
		sendACK.dora = 0;
		sendACK.src = 3;
		sendACK.dest = recvframe.src;

		getCurrentTime();
		if(sendto(s, &sendACK, sizeof(Frame), 0, (struct sockaddr*) &si_other, slen) == -1)
		{
			die("sendto()");
		}
		sendACK.eve = 0;
		printLog(recvframe.dest, sendACK);

		if(recvframe.lp)
			eofflag = 1;
	}
	printf("-----------------------------------------------------------------\n");
	close(s);
	return 0;
}