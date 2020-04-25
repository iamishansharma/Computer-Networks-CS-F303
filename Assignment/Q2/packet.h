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

#define PACKET_SIZE 100 // Packet Size

struct packet
{
	char data[PACKET_SIZE]; // Main Data
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

	Packet pack;  // Data Packer
};

typedef struct frame Frame;

void printFrame(Frame F)
{
	printf("\nPacket Size: %d\n", F.psize);
	printf("Sequence No: %d\n", F.seq);
	printf("Last Packet: %d\n", F.lp);
	printf("Data or Ack: %d\n", F.dora);
	printf("Relay: %d\n", F.relay);
	printf("Data: %s\n", F.pack.data);
}

#endif