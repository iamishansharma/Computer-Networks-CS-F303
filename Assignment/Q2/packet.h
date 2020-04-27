/* 
	packet.h

	Computer Networks: Assignment - Sem 2 2019-20
	Ishan Sharma - 2016B2A70773P
*/

#ifndef PACKET_H
#define PACKET_H

#include <stdio.h>
#include <sys/socket.h> //for socket(), connect(), send(), recv() functions
#include <arpa/inet.h>  // different address structures are declared here
#include <stdlib.h> 	// atoi() which convert string to integer
#include <string.h>
#include <unistd.h> 	// close() function 
#include <sys/types.h>
#include <netinet/in.h>  
#include <sys/time.h> 	//FD_SET, FD_ISSET, FD_ZERO macros
#include <time.h>

#define client_addr "127.0.0.1" // client IP address
#define relay_addr "127.0.0.1"	// relay IP address
#define	server_addr "127.0.0.1" // server IP address

#define PACKET_SIZE 100 // Packet Size
#define WINDOW_SIZE 4 	// SET WINDOW SIZE HERE

extern char cctime[20];

struct packet
{
	char data[PACKET_SIZE+1]; // Main Data
};

typedef struct packet Packet;

struct frame
{
	int psize;    // The size (number of bytes) of the payload

	int lp;       // Whether the packet is last packet or not?

	int seq;      // The Seq. No. (in terms of byte) specifying the offset of 
			      // the first byte of the packet with respect to the input file.
	
	int dora;     // DATA (1) or ACK (0) 

	int relay;    // Relay (0 for even wala channel and 1 for odd wala channel)

	int totalframes; // Has Totak Frames

	int no;		  // Packet Number starts with 0

	int isACK; 	  // Packet was succesfully delivered and ACKed

	Packet pack;  // Data Packer

	int src;	  // Source Number corresponding to char srdest

	int dest;	  // Destination Number corresponding to char srdest

	int eve;	  // Event Number corresponding to char event
};

typedef struct frame Frame;

void printFrame(Frame F)
{
	printf("\nPacket Size: %d\n", F.psize);
	printf("Sequence No: %d\n", F.seq);
	printf("Last Packet: %d\n", F.lp);
	printf("Data or Ack: %d\n", F.dora);
	printf("Relay: %d\n", F.relay);
	printf("Total Frames: %d\n",F.totalframes);
	printf("Packet Number: %d\n",F.no);
	printf("is ACKed: %d\n",F.isACK);
	printf("Data: %s\n", F.pack.data);
}
void printFrameFile(Frame F, FILE *f)
{
	fprintf(f,"%s", F.pack.data);
}

char event[5][5] = {"S","R","D","TO","RE"};
char srcdest[5][10] = {"Client","Relay1","Relay2","Server"};

void *getCurrentTime()
{
	memset(cctime,0,20);
	time_t current = time(NULL);;
	struct tm *ptr = localtime(&current);;
	struct timeval tv;

	gettimeofday(&tv,NULL);

	strftime(cctime, 20, "%H:%M:%S", ptr);

	char ms[8];

	sprintf(ms, ".%06ld", tv.tv_usec);
	strcat(cctime,ms);

	return cctime;
}

void printLog(int cn, Frame frame)
{
	char dora[5];
	memset(dora,0,5);

	if(frame.dora)
		strcpy(dora,"DATA");
	else
		strcpy(dora,"ACK");

	printf("%-10s %-5s %-17s %-6s %-5d %-10s %-10s\n", srcdest[cn], event[frame.eve], cctime, dora, frame.seq, srcdest[frame.src], srcdest[frame.dest]);
}

#endif