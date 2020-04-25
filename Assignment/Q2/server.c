/* 

	server.c

	Computer Networks: Assignment - Sem 2 2019-20
	Ishan Sharma - 2016B2A70773P

	Have to do the following: 

*/

#include "packet.h"

#define MAXPENDING 2
#define PORT 12345
#define BUFLEN 512

void die(char *s)
{
	perror(s);
	exit(1);
}

int main()
{
	struct sockaddr_in si_me, si_other;
	int s, i, slen = sizeof(si_other) , recv_len;
	char buf[BUFLEN];
	memset(buf,0,BUFLEN);

	char guess[16][20] = 
	{
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"Lewis Hamilton",
		"Sebastian Vettel",
		"Max Versteppan",
		"Carlos Sainz",
		"Daniel Riccardo",
		"Sergio Perez",
		"Pierre Gasly",
		"Kimi Raikkonen",
		"Romain Grosjean",
		"George Russell"
	};

	int randguess = 0;

	char sendback[30];
	
	//create a UDP socket
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		die("socket");
	}
	 
	// zero out the structure
	memset((char *) &si_me, 0, sizeof(si_me));
	 
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	 
	//bind socket to port
	if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
	{
		die("bind");
	}
	 
	//keep listening for data
	while(1)
	{
		randguess = (rand()%(15 - 0 + 1)) + 0;

		printf("Waiting for data...");

		fflush(stdout);
		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, (socklen_t *)&slen)) == -1)
		{
			die("recvfrom()");
		}
		 
		//print details of the client/peer and the data received
		printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));

		printf("Guessed by Client: %s | Randomly Generated by Server: %s\n" , buf, guess[randguess]);

		memset(sendback,0,15);

		if(strcmp(buf,guess[randguess]) == 0)
		{
			strcpy(sendback,"Correct Guess, Bravo!");
		}
		else
		{
			strcpy(sendback,"Wrong Guess, Sorry!");
		}
		 
		//now reply the client with the same data
		if (sendto(s, sendback, sizeof(sendback), 0, (struct sockaddr*) &si_other, slen) == -1)
		{
			die("sendto()");
		}
	}
	close(s);
	return 0;
}