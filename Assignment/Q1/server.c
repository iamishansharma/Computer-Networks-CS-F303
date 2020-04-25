/* 
	server.c

	Computer Networks: Assignment - Sem 2 2019-20
	Ishan Sharma - 2016B2A70773P

	Have to do the following: 

*/

#include "packet.h"

#define MAXPENDING 2
#define PDR 10 		// Packet Drop Rate
#define PORT 12345

void die(char *s)
{
	perror(s);
	exit(1);
}

int main(int argc, char *argv[])
{
	FILE *f = fopen("output.txt","w");
	if(f == NULL)
	{
		printf("Error Opening The File output.txt");
		exit(0);
	}

	int masterSocket, client_socket[2], max_sd, sd, activity, new_socket, valread;
	
	fd_set readfds;

	int totalframes;

	masterSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(masterSocket < 0) 
	{
		printf("Error While Server Socket Creation"); 
		exit(0); 
	}

	for(int i = 0; i < 2; i++)
		client_socket[i] = 0;

	int opt = 1;

	if(setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)   
	{   
		perror("Set Master Socket Field");   
		exit(0);   
	}

	struct sockaddr_in serverAddress, clientAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(atoi(argv[2]));
	serverAddress.sin_addr.s_addr = inet_addr(argv[1]);

	int binder = bind(masterSocket,(struct sockaddr*)&serverAddress, sizeof(serverAddress));

	if(binder < 0)
	{ 
		printf ("Error While Binding\n");
		exit(0);
	}

	int listener = listen(masterSocket, MAXPENDING);

	if(listener < 0)
	{ 
		printf ("Error In Listen");
		exit(0);
	}

	printf("Waiting for clients ...\n");

	int eofflag = 0;
	int firsttotfra = 0;
	int skip;
	int skipcount = 0;

	srand(time(0));

	while(!eofflag)
	{
		FD_ZERO(&readfds);
		FD_SET(masterSocket, &readfds);   
		max_sd = masterSocket;
		for(int i = 0; i < 2; i++)   
		{   
			sd = client_socket[i]; 

			if(sd > 0)   
				FD_SET(sd, &readfds);   

			if(sd > max_sd)   
				max_sd = sd;   
		}
		activity = select(max_sd+1, &readfds, NULL, NULL, NULL);
		if(activity < 0)   
		{   
			printf("Client Error");   
		}
		if(FD_ISSET(masterSocket, &readfds))
		{
			int addrlen = sizeof(serverAddress);
			new_socket = accept(masterSocket, (struct sockaddr *)&serverAddress, (socklen_t *)&addrlen);
			if(new_socket < 0)
			{   
				perror("Error in Accept");   
				exit(0);   
			}
			for (int i = 0; i < 2; i++)   
			{ 
				if(client_socket[i] == 0)   
				{
					client_socket[i] = new_socket;   
					break;
				}
			}
		}

		Frame newFrame;

		if(FD_ISSET(client_socket[0], &readfds))
		{
			if((valread = recv(client_socket[0], &newFrame, sizeof(Frame), 0)) == 0)   
			{
				getpeername(sd, (struct sockaddr*)&serverAddress, (socklen_t*)sizeof(serverAddress));
				close(sd);
				client_socket[0] = 0;   
			}
			else
			{
				if(firsttotfra == 0)
				{
					totalframes = newFrame.totalframes;
					firsttotfra++;
					skipcount = (PDR * totalframes) / 100;
				}

				if(newFrame.lp == 1)
					eofflag = 1;

				skip = (rand()%(1-0+1)) + 0;

				if(skip == 1 && skipcount != 0)
				{
					skipcount--;
					goto droppacket0;
				}

				//printFrame(newFrame);
				printFrameFile(newFrame,f);

				Frame newACK;
				newACK.psize = newFrame.psize;
				newACK.seq = newFrame.seq;
				newACK.lp = newFrame.lp;
				newACK.dora = 0;
				newACK.ch = newFrame.ch;
				newACK.totalframes = newFrame.totalframes;
				strcpy(newACK.pack.data,"ACK Recieved");

				int sendACK = send(client_socket[newACK.ch], &newACK, sizeof(Frame),0);
				if (sendACK != sizeof(Frame))
				{ 
					printf ("Error while sending message to client");
					exit(0);
				}

				droppacket0: ;
			}
		}

		if(FD_ISSET(client_socket[1], &readfds))
		{
			if((valread = recv(client_socket[1], &newFrame, sizeof(Frame), 0)) == 0)   
			{
				getpeername(sd, (struct sockaddr*)&serverAddress, (socklen_t*)sizeof(serverAddress));
				close(sd);
				client_socket[1] = 0;   
			}
			else
			{
				if(firsttotfra == 0)
				{
					totalframes = newFrame.totalframes;
					firsttotfra++;
					skipcount = (PDR * totalframes) / 100;
				}

				if(newFrame.lp == 1)
					eofflag = 1;

				skip = (rand()%(1-0+1)) + 0;

				if(skip == 1 && skipcount != 0)
				{
					skipcount--;
					goto droppacket1;
				}

				//printFrame(newFrame);
				printFrameFile(newFrame,f);

				Frame newACK;
				newACK.psize = newFrame.psize;
				newACK.seq = newFrame.seq;
				newACK.lp = newFrame.lp;
				newACK.dora = 0;
				newACK.ch = newFrame.ch;
				newACK.totalframes = newFrame.totalframes;
				strcpy(newACK.pack.data,"ACK Recieved");

				int sendACK = send(client_socket[newACK.ch], &newACK, sizeof(Frame),0);
				if (sendACK != sizeof(Frame))
				{ 
					printf ("Error while sending message to client");
					exit(0);
				}

				droppacket1: ;
			}
		}
	}

	printf("\nTransmission recieved successfully from input.txt (from client) to output.txt (server.txt)\n\n");

	fclose(f);

	return 0;
}