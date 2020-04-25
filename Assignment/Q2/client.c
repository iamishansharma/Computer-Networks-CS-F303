/*
	client.c

	Computer Networks: Assignment - Sem 2 2019-20
	Ishan Sharma - 2016B2A70773P

	Have to do the following: 

*/

#include "packet.h"

#define RETRATIME 2 // (In Terms of Seconds) Retransmission Time
#define PORT 12345  // SET PORT NUMBER HERE
#define BUFLEN 512

int state = 0;

void die(char *s)
{
	perror(s);
	exit(1);
}

struct timeval timeout;

int main()
{
	timeout.tv_sec = RETRATIME; // sets the timeout time as 2 seconds
	timeout.tv_usec = 0;

	struct sockaddr_in si_other;
	int s, i, slen=sizeof(si_other);
	char buf[BUFLEN];
	char message[BUFLEN];
	memset(buf,0,BUFLEN);
	memset(message,0,BUFLEN);
 
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		die("socket");
	}
 
	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	si_other.sin_addr.s_addr = inet_addr("192.168.225.51");
	
	while(1)
	{
		printf("Enter your guess (Either NUmber (1-6) or F1 Driver Names): ");
		gets(message);
		 
		//send the message
		if (sendto(s,message,strlen(message),0,(struct sockaddr *) &si_other, slen) == -1)
		{
			die("sendto()");
		}

		//receive a reply and print it
		memset(buf,'\0', BUFLEN);
		//try to receive some data, this is a blocking call
		if (recvfrom(s,buf,BUFLEN,0,(struct sockaddr *)&si_other,(socklen_t *)&slen) == -1)
		{
			die("recvfrom()");
		}
		puts(buf);
	}
 
	close(s);
	return 0;

}