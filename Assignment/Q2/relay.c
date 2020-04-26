/* 

	relay.c

	Computer Networks: Assignment - Sem 2 2019-20
	Ishan Sharma - 2016B2A70773P

	Have to do the following: 

*/

#include "packet.h"

#define MAXPENDING 2
#define PORT 12345
#define BUFLEN 512
#define relayaddr "127.0.0.1"

void die(char *s)
{
	perror(s);
	exit(1);
}

int main(int argc, char *argv[])
{
	int port1 = 0;

	printf("Please enter the port number you want this relay on: ");
	scanf("%d",&port1);

	int serverport = 0;

	printf("Please enter the port number your server is running on: ");
	scanf("%d",&serverport);

	struct sockaddr_in si_me, client, server;
	int s, slen = sizeof(client), recv_len;
	char buf[BUFLEN];
	memset(buf,0,BUFLEN);

	if((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		die("socket");
	}
	 
	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(port1);
	si_me.sin_addr.s_addr = inet_addr(relayaddr);

	memset((char *) &server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(serverport);
	server.sin_addr.s_addr = inet_addr(relayaddr);

	if(bind(s, (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
	{
		die("bind");
	}

	char m1[BUFLEN];
	char m2[BUFLEN];

	memset(m1,0,BUFLEN);
	memset(m2,0,BUFLEN);

	printf("Waiting for client data...");
	while(1)
	{
		if((recv_len = recvfrom(s, m1, BUFLEN, 0, (struct sockaddr *) &client, (socklen_t *)&slen)) == -1)
		{
			die("recvfrom()");
		}

		if(sendto(s, m1, BUFLEN, 0, (struct sockaddr*) &server, slen) == -1)
		{
			die("sendto()");
		}

		if((recv_len = recvfrom(s, m2, BUFLEN, 0, (struct sockaddr *) &server, (socklen_t *)&slen)) == -1)
		{
			die("recvfrom()");
		}

		if(sendto(s, m2, BUFLEN, 0, (struct sockaddr*) &client, slen) == -1)
		{
			die("sendto()");
		}
	}
	close(s);
	return 0;
}