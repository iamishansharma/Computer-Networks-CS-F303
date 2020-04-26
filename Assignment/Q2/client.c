/*
	client.c

	Computer Networks: Assignment - Sem 2 2019-20
	Ishan Sharma - 2016B2A70773P

	Have to do the following: 

*/

#include "packet.h"

#define RETRATIME 2 // (In Terms of Seconds) Retransmission Timeå
#define BUFLEN 512

int state = 0;

void die(char *s)
{
	perror(s);
	exit(1);
}

struct timeval timeout;

int main(int argc, char *argv[])
{
	int port1 = 0;

	printf("Please enter port number for relay channel 1: ");
	scanf("%d",&port1);

	int port2 = 0;

	printf("Please enter port number for relay channel 2: ");
	scanf("%d",&port2);

	timeout.tv_sec = RETRATIME; // sets the timeout time as 2 seconds
	timeout.tv_usec = 0;

	struct sockaddr_in relay1, relay2;

	int s1, slen = sizeof(relay1);

	char gaya1[BUFLEN];
	char aaya1[BUFLEN];
	char gaya2[BUFLEN];
	char aaya2[BUFLEN];

	memset(gaya1,0,BUFLEN);
	memset(aaya1,0,BUFLEN);
	memset(gaya2,0,BUFLEN);
	memset(aaya2,0,BUFLEN);
 
	if((s1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		die("socket");
	}

	relay1.sin_family = AF_INET;
	relay1.sin_port = htons(port1);
	relay1.sin_addr.s_addr = inet_addr("127.0.0.1");

	relay2.sin_family = AF_INET;
	relay2.sin_port = htons(port2);
	relay2.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	while(1)
	{
		fflush(stdout);
		fflush(stdin);

		printf("\nEnter Data for Relay 1: \n");
		gets(gaya1);

		if(sendto(s1, gaya1, BUFLEN, 0, (struct sockaddr*) &relay1, slen) == -1)
		{
			die("sendto()");
		}

		if(recvfrom(s1, aaya1, BUFLEN, 0, (struct sockaddr *) &relay1, (socklen_t *)&slen) == -1)
		{
			die("recvfrom()");
		}

		printf("%s\n",aaya1);

		fflush(stdout);
		fflush(stdin);

		printf("\nEnter Data for Relay 2: \n");
		gets(gaya2);

		if(sendto(s1, gaya2, BUFLEN, 0, (struct sockaddr*) &relay2, slen) == -1)
		{
			die("sendto()");
		}

		if(recvfrom(s1, aaya2, BUFLEN, 0, (struct sockaddr *) &relay2, (socklen_t *)&slen) == -1)
		{
			die("recvfrom()");
		}

		printf("%s\n",aaya2);
	}
 
	close(s1);
	return 0;
}