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

#define PACKET_SIZE 100 // Packet Size
#define serAddr "127.0.0.1" // SET SERVER ADDRSS HERE

struct packet
{
	char data[PACKET_SIZE+1]; // Main Data
};

typedef struct packet Packet;

struct frame
{
	int psize;    // The size (number of bytes) of the payload

	int seq;      // The Seq. No. (in terms of byte) specifying the offset of 
			      // the first byte of the packet with respect to the input file.

	int lp;       // Whether the packet is last packet or not?

	int dora;     // DATA (1) or ACK (0) 

	int ch;       // Channel (0 or 1)

	Packet pack;  // Data Packer

	int totalframes; // Contains totalframes counter
};

typedef struct frame Frame;

void printFrame(Frame F)
{
	printf("\nPacket Size: %d\n", F.psize);
	printf("Sequence No: %d\n", F.seq);
	printf("Last Packet: %d\n", F.lp);
	printf("Data or Ack: %d\n", F.dora);
	printf("Channel: %d\n", F.ch);
	printf("Total Frames: %d\n",F.totalframes);
	printf("Data: %s\n", F.pack.data);
}
void printFrameFile(Frame F, FILE *f)
{
	fprintf(f,"%s", F.pack.data);
}

#endif