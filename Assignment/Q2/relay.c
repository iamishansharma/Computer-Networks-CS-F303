/* 

	relay.c

	Computer Networks: Assignment - Sem 2 2019-20
	Ishan Sharma - 2016B2A70773P

	Have to do the following: 

*/

#include "packet.h"

#define MAXPENDING 2
#define relay_addr "127.0.0.1"
#define DELAY_TIME 2 	// IN MSEC
#define PDR 10			// Packet Drop Rate

char cctime[20];

void die(char *s)
{
	perror(s);
	exit(1);
}

int main(int argc, char *argv[])
{
	int port1 = 0;

	printf("Port Number for this Relay: ");
	scanf("%d",&port1);

	int serverport = 0;

	printf("Port Number for Server:  ");
	scanf("%d",&serverport);

	struct sockaddr_in si_me, client, server;
	int s, slen = sizeof(client), recv_len;

	if((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		die("socket");
	}
	 
	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(port1);
	si_me.sin_addr.s_addr = inet_addr(relay_addr);

	memset((char *) &server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(serverport);
	server.sin_addr.s_addr = inet_addr(server_addr);

	if(bind(s, (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
	{
		die("bind");
	}

	printf("Relay running .....\n");

	printf("\n-----------------------------------------------------------------\n");
	printf("NODENAME EVENT     TIMESTAMP      PKTTYPE SEQ   SOURCE      DEST\n");
	printf("-----------------------------------------------------------------\n");
	srand(time(0));

	int randdelay = 0;
	int randdrop = 0;

	Frame recvframe;
	Frame serverframe;
	Frame recvserver;
	Frame sendACK;

	int eofflag = 0;
	int skipcount = 0;
	int firsttotfra = 0;
	int totalframes = 0;

	while(!eofflag)
	{
		getCurrentTime();
		if(recvfrom(s, &recvframe, sizeof(Frame), 0, (struct sockaddr *) &client, (socklen_t *)&slen) == -1)
		{
			die("recvfrom()");
		}
		recvframe.eve = 1;
		printLog(recvframe.dest, recvframe);

		if(firsttotfra == 0)
		{
			totalframes = recvframe.totalframes;
			firsttotfra++;
			skipcount = ((PDR/2) * totalframes) / 100;
		}

		randdelay = (rand()%(DELAY_TIME-0+1)) + 0;

		randdrop = (rand()%(1-0+1)) + 0;

		if(randdrop == 1 && skipcount != 0)
		{
			skipcount--;
			//goto droppacket;
		}

		usleep(randdelay*1000); 	// Sleep for 0 to DELAY_TIME

		serverframe = recvframe;
		serverframe.eve = 0;
		serverframe.src = recvframe.dest;
		serverframe.dest = 3;

		getCurrentTime();
		if(sendto(s, &serverframe, sizeof(Frame), 0, (struct sockaddr*) &server, slen) == -1)
		{
			die("sendto()");
		}
		printLog(recvframe.dest, serverframe);

		getCurrentTime();
		if(recvfrom(s, &recvserver, sizeof(Frame), 0, (struct sockaddr *) &server, (socklen_t *)&slen) == -1)
		{
			die("recvfrom()");
		}
		recvserver.eve = 1;
		printLog(recvframe.dest, recvserver);

		sendACK = recvserver;
		sendACK.dora = 0;
		sendACK.src = recvframe.dest;
		sendACK.dest = recvserver.src;

		getCurrentTime();
		if(sendto(s, &sendACK, sizeof(Frame), 0, (struct sockaddr*) &client, slen) == -1)
		{
			die("sendto()");
		}
		printLog(recvframe.dest, sendACK);

		droppacket: ;

		if(recvframe.lp)
			eofflag = 1;
	}

	printf("-----------------------------------------------------------------\n");
	close(s);
	return 0;
}