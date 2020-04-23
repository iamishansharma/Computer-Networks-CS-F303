/* 
	packet.h

	Computer Networks: Assignment - Sem 2 2019-20
	Ishan Sharma - 2016B2A70773P
*/

#ifndef PACKET_H
#define PACKET_H

#include <stdio.h>
#include <sys/socket.h> //for socket(), connect(), send(), recv() functions
#include <arpa/inet.h> // different address structures are declared here
#include <stdlib.h> // atoi() which convert string to integer
#include <string.h>
#include <unistd.h> // close() function

#define PACKET_SIZE 100 // Packet Size

struct packet
{
	int psize; // The size (number of bytes) of the payload

	int seq;   // The Seq. No. (in terms of byte) specifying the offset of 
			   // the first byte of the packet with respect to the input file.

	int lp;    // Whether the packet is last packet or not?

	int dora;  // DATA (1) or ACK (0) 

	int ch;    // Channel (0 or 1)

	char data[PACKET_SIZE]; // Main Data in the packet
};

typedef struct packet Packet;

void printPacket(Packet P)
{
	printf("\nPacket Size: %d\n", P.psize);
	printf("Sequence No: %d\n", P.seq);
	printf("Last Packet: %d\n", P.lp);
	printf("Data or Ack: %d\n", P.dora);
	printf("Channel: %d\n", P.ch);
	printf("Data: %s\n", P.data);
}

#endif