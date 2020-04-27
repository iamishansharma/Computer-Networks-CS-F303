/*
	client.c

	Computer Networks: Assignment - Sem 2 2019-20
	Ishan Sharma - 2016B2A70773P

	Have to do the following: 

*/

#include "packet.h"

#define RETRATIME 2 // (In Terms of Seconds) Retransmission Time

int state = 0;

void die(char *s)
{
	perror(s);
	exit(1);
}

struct timeval timeout;

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("Command Line Arguments on as per format. Check readme.txt.\n");
		exit(1);
	}

	timeout.tv_sec = RETRATIME; // sets the timeout time as 2 seconds
	timeout.tv_usec = 0;

	int totalframes;

	char buffer[PACKET_SIZE+1];

	char filename[100];
	memset(filename,0,100);
	strcpy(filename,argv[2]); // Set file name here to be transmitted

	FILE *f = fopen(filename,"r");

	if(f == NULL)
	{
		printf("Error Opening The File %s",argv[2]);
		exit(0);
	}

	int sock[2];

	/* CREATE A TCP SOCKET*/
	
	sock[0] = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	sock[1] = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(sock[0] < 0) 
	{ 
		printf ("Error in opening a socket 0");
		exit (0);
	}
	//printf ("Client Socket 0 Created.\n");

	if(sock[1] < 0) 
	{ 
		printf ("Error in opening a socket 1");
		exit (0);
	}
	//printf ("Client Socket 1 Created.\n");
		
	/*CONSTRUCT SERVER ADDRESS STRUCTURE*/
	
	struct sockaddr_in serverAddr;
	memset(&serverAddr,0,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1])); //You can change port number here
	serverAddr.sin_addr.s_addr = inet_addr(serAddr); //Specify server's IP address here
	//printf("Address assigned.\n");
	
	/*ESTABLISH CONNECTION*/

	int conn[2];
	
	conn[0] = connect(sock[0], (struct sockaddr*) &serverAddr, sizeof(serverAddr));
	if (conn[0] < 0)
	{ 
		printf("Error while establishing connection\n");
		exit (0);
	}
	//printf ("Connection Established for Socket 0\n");

	conn[1] = connect(sock[1], (struct sockaddr*) &serverAddr, sizeof(serverAddr));
	if (conn[1] < 0)
	{ 
		printf("Error while establishing connection\n");
		exit (0);
	}
	//printf ("Connection Established for Socket 1\n");

	/************************************* Operations here START *************************************/

	int eofflag = 0;

	int i;   				// PACKET_SIZE Iterator
	char ch,ch2; 				// Read Character from input.txt file
	int randch = 0;

	int offset = 0;

	totalframes = 0;

	while(!eofflag)
	{
		for(i = 0; i < PACKET_SIZE && ch2 != EOF; i++)
		{
			ch2 = fgetc(f);
		}
		if(ch2 == EOF)
			eofflag = 1;

		totalframes++;
	}

	eofflag = 0;

	FILE *f1 = fopen(filename,"r");
	Frame frames[totalframes];
	int fra = 0;
	srand(time(0));

	while(!eofflag)
	{
		memset(buffer,0,PACKET_SIZE+1);

		for(i = 0; i < PACKET_SIZE && ch != EOF; i++)
		{
			ch = fgetc(f1);
			if(ch != EOF)
				buffer[i]=ch;
		}
		if(ch == EOF)
			eofflag = 1;

		randch = (rand()%(1-0+1)) + 0;

		Frame newFrame;
		newFrame.psize = i;
		offset += i;
		newFrame.seq = offset;
		if(eofflag)
			newFrame.lp = 1;
		else
			newFrame.lp = 0;
		newFrame.dora = 1;
		newFrame.ch = randch;
		strcpy(newFrame.pack.data, buffer);
		newFrame.totalframes = totalframes;

		frames[fra++] = newFrame;
	}

	int j = 0; // Frames Iterator

	eofflag = 0;

	int jeof = 0;
	int j1eof = 0;

	printf("\n***** Client Trace *****\n\n");

	while(!eofflag)
	{
		Frame currentFrame = frames[j];
		Frame nextFrame;

		if(frames[j].lp == 1)
			jeof = 1;

		if(!jeof)
			nextFrame = frames[j+1];

		if(frames[j+1].lp == 1)
			j1eof = 1;

		Frame recvACK;
		Frame nextrecvACK;

		switch(state)
		{
			case 0:
					if(setsockopt(sock[currentFrame.ch], SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0)
            			perror("setsockopt(SO_RCVTIMEO) failed");

					if(send(sock[currentFrame.ch], &frames[j], sizeof(Frame), 0) != sizeof(Frame))
					{
						printf("Error while sending the frame");
						exit(0);
					}

					printf("SENT PCK: Seq No: %d of size %d bytes from channel %d\n", currentFrame.seq, currentFrame.psize, currentFrame.ch);

					state = 1;
					break;

			case 1:
					//printf("\n************** Current Frame -> ************\n");

					//printFrame(currentFrame);

					if(recv(sock[currentFrame.ch], &recvACK, sizeof(Frame), 0) == 0)
					{
						printf("\n\nDidn't get ACK for Seq 0\n\n");
						state = 0;
					}
					else
					{
						//printf("\n************** Received Frame Seq 0 -> ************\n");

						//printFrame(recvACK);

						if(recvACK.seq == currentFrame.seq)
						{
							//printf("\n\nReceived ACK for Seq. no. %d\n\n", recvACK.seq);
							state = 2;
						}
						else
						{
							state = 0;
							break;
						}

						if(jeof)
						{
							state = 0;
							eofflag = 1;
						}
						else
							state = 2;

						printf("RCVD ACK: for PKT with Seq. No. %d from channel %d\n", recvACK.seq+1, recvACK.ch);
					}

					break;

			case 2:
					if(setsockopt(sock[nextFrame.ch], SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0)
            			perror("setsockopt(SO_RCVTIMEO) failed");

					if(send(sock[nextFrame.ch], &frames[j+1], sizeof(Frame), 0) != sizeof(Frame))
					{
						printf("Error while sending the frame");
						exit(0);
					}

					printf("SENT PCK: Seq No: %d of size %d bytes from channel %d\n", nextFrame.seq, nextFrame.psize, nextFrame.ch);

					state = 3;
					break;

			case 3:
					//printf("\n************** Next Frame -> ************\n");

					//printFrame(nextFrame);

					if(recv(sock[nextFrame.ch], &nextrecvACK, sizeof(Frame), 0) < 0)
					{
						//printf("\n\nDidn't get ACK for Seq 1\n\n");
						state = 2;
						break;
					}
					else
					{
						if(nextrecvACK.seq == nextFrame.seq)
						{
							//printf("\n************** Received Frame Seq 1 -> ************\n");

							//printFrame(nextrecvACK);

							//printf("\n\nReceived ACK for Seq. no. %d\n\n", nextrecvACK.seq);
							state = 0;
						}
						else
						{
							state = 2;
							break;
						}

						if(j1eof)
							eofflag = 1;

						printf("RCVD ACK: for PKT with Seq. No. %d from channel %d\n", nextrecvACK.seq+1, nextrecvACK.ch);

						j = j + 2;
					}

					break;
		}
	}

	printf("\nTransmission completed from input.txt (from client) to output.txt (server.txt)\n\n");

	/************************************* Operations here END *************************************/

	fclose(f1);
	close(sock[0]);
	close(sock[1]);

	return 0;
}