/* 

	server.c

	Computer Networks: Assignment - Sem 2 2019-20
	Ishan Sharma - 2016B2A70773P

	Have to do the following: 

*/

#include "packet.h"

#define MAXPENDING 2
#define BUFLEN 512

void die(char *s)
{
	perror(s);
	exit(1);
}

int main(int argc, char *argv[])
{
	int port1 = 0;

	printf("Please enter the port number you want the server on: ");
	scanf("%d",&port1);

	int relayport1 = 0;

	printf("Please enter the port number you will put the relay 1 on: ");
	scanf("%d",&relayport1);

	int relayport2 = 0;

	printf("Please enter the port number you will put the relay 2 on: ");
	scanf("%d",&relayport2);

	struct sockaddr_in si_me, si_other;
	int s, i, slen = sizeof(si_other) , recv_len;

	char aaya1[BUFLEN];
	memset(aaya1,0,BUFLEN);

	char gaya1[BUFLEN];
	memset(gaya1,0,BUFLEN);
	
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		die("socket");
	}
	 
	memset((char *) &si_me, 0, sizeof(si_me));
	 
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(port1);
	si_me.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(bind(s, (struct sockaddr*)&si_me, sizeof(si_me)) == -1)
	{
		die("bind");
	}
	 
	while(1)
	{
		if((recv_len = recvfrom(s, aaya1, BUFLEN, 0, (struct sockaddr *) &si_other, (socklen_t *)&slen)) == -1)
		{
			die("recvfrom()");
		}

		if(si_other.sin_port == htons(relayport1))
			strcat(aaya1," Yes relay 1 se aaya.");
		else
			strcat(aaya1," Yes relay 2 se aaya.");

		if(sendto(s, aaya1, BUFLEN, 0, (struct sockaddr*) &si_other, slen) == -1)
		{
			die("sendto()");
		}
	}

	close(s);
	return 0;
}