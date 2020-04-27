/*
	client.c

	Computer Networks: Assignment - Sem 2 2019-20
	Ishan Sharma - 2016B2A70773P

	Have to do the following: 

*/

#include "packet.h"

#define RETRATIME 2 // (In Terms of Seconds) Retransmission Timeå

char cctime[20];

int state = 0;

void die(char *s)
{
	perror(s);
	exit(1);
}

struct timeval timeout;

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Command Line Arguments on as per format. Check readme.txt.\n");
		exit(1);
	}

	int totalframes = 0;

	char filename[100];
	memset(filename,0,100);
	strcpy(filename,argv[1]); // Set file name here to be transmitted

	FILE *f = fopen(filename,"r"); // For calcualting total frames
	FILE *f1 = fopen(filename,"r"); // For making all frames

	if(f == NULL)
	{
		printf("Error opening file %s",filename);
		exit(1);
	}

	int port1 = 0;

	printf("Port Number for Relay 1: ");
	scanf("%d",&port1);

	int port2 = 0;

	printf("Port Number for Relay 2: ");
	scanf("%d",&port2);

	timeout.tv_sec = RETRATIME; // sets the timeout time as 2 seconds
	timeout.tv_usec = 0;

	struct sockaddr_in relay1, relay2;

	int s1, slen = sizeof(relay1);
 
	if((s1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		die("socket");
	}

	relay1.sin_family = AF_INET;
	relay1.sin_port = htons(port1);
	relay1.sin_addr.s_addr = inet_addr(relay_addr);

	relay2.sin_family = AF_INET;
	relay2.sin_port = htons(port2);
	relay2.sin_addr.s_addr = inet_addr(relay_addr);

	printf("Client running .....\n");

	char buffer[PACKET_SIZE+1];
	memset(buffer,0,PACKET_SIZE+1);

	int eofflag = 0;

	int i;   				// PACKET_SIZE Iterator
	char ch,ch2; 			// Read Character from input.txt file

	int offset = 0;

	totalframes = 0;

	while(!eofflag)
	{
		for(i = 0; i < PACKET_SIZE && ch2 != EOF; i++)
		{
			ch2 = fgetc(f);
		}
		if(ch2 == EOF)
			eofflag = 1;

		totalframes++;
	}

	eofflag = 0;

	Frame frames[totalframes];

	int fra = 0;

	while(!eofflag)
	{
		memset(buffer,0,PACKET_SIZE+1);

		for(i = 0; i < PACKET_SIZE && ch != EOF; i++)
		{
			ch = fgetc(f1);
			if(ch != EOF)
				buffer[i]=ch;
		}
		if(ch == EOF)
			eofflag = 1;

		Frame newFrame;
		newFrame.psize = i;
		offset += i;
		newFrame.seq = offset;

		if(eofflag)
			newFrame.lp = 1;
		else
			newFrame.lp = 0;
		
		newFrame.dora = 1;
		
		if(fra % 2 == 0)
			newFrame.relay = 2;
		else
			newFrame.relay = 1;

		strcpy(newFrame.pack.data, buffer);
		newFrame.totalframes = totalframes;
		newFrame.no = fra;
		newFrame.isACK = 0;
		newFrame.src = -1;
		newFrame.dest = -1;
		newFrame.eve = -1;

		frames[fra++] = newFrame;
	}

	// END Getting All Frames Populated
	
	eofflag = 0;

	int k = 0;	// Window iterator

	int base = 0;

	int end = 0;

	// Sliding Window Protocol START
	printf("\n-----------------------------------------------------------------\n");
	printf("NODENAME EVENT     TIMESTAMP      PKTTYPE SEQ   SOURCE      DEST\n");
	printf("-----------------------------------------------------------------\n");

	Frame recvACK;

	for(k=0; k<totalframes; k++)
	{
		if(k % 2 == 0)
		{
			frames[k].src = 0;
			frames[k].dest = 2;
			frames[k].eve = 0;

			getCurrentTime();
			if(sendto(s1, &frames[k], sizeof(Frame), 0, (struct sockaddr*) &relay1, slen) == -1)
			{
				die("sendto()");
			}
			printLog(0, frames[k]);

			getCurrentTime();
			if(recvfrom(s1, &recvACK, sizeof(Frame), 0, (struct sockaddr *) &relay1, (socklen_t *)&slen) == -1)
			{
				die("recvfrom()");
			}
			recvACK.eve = 1;
			printLog(0, recvACK);
		}
		else
		{
			frames[k].src = 0;
			frames[k].dest = 1;
			frames[k].eve = 0;
			getCurrentTime();

			if(sendto(s1, &frames[k], sizeof(Frame), 0, (struct sockaddr*) &relay2, slen) == -1)
			{
				die("sendto()");
			}
			printLog(0, frames[k]);

			getCurrentTime();
			if(recvfrom(s1, &recvACK, sizeof(Frame), 0, (struct sockaddr *) &relay2, (socklen_t *)&slen) == -1)
			{
				die("recvfrom()");
			}
			recvACK.eve = 1;
			printLog(0, recvACK);
		}
	}
	printf("-----------------------------------------------------------------\n");
	// Sliding Window Protocol END
 
	close(s1);
	return 0;
}