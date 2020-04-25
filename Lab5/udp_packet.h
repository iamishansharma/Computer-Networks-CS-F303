#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>   // close() function

#define BUFLEN 512  //Max length of buffer
   

struct packet1
{
	int sq_no;
};

typedef struct packet1 ACK_PKT;

struct packet2
{
	int sq_no;
	char data[BUFLEN];
};

typedef struct packet2 DATA_PKT;