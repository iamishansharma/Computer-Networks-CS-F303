## Instruction to compile

	Run 'make clean' to delete any previously generated executables.
	Run 'make' to compile and generate executibles

## Instructions to run

	< > -> command line arguments

	1. ./server <server port number> <output file name>
	2. ./client <server port number> <input file name>

	All IP addresses have been defined as macros in 'packet.h'
	and set at submission as '127.0.0.1'.
	
	Please run 1 and 2 in order.

	Eg. 

	./server 12345 output.txt
	./client 12345 input.txt


## Some Details about the code

	The code may take some time to execute as timeout value might be large please be patient, 
	the code will surely terminate on its own.

	Stop and Wait protocol has been implemented by modifying rdt 3.0.

	Macros: 

		Retransmission Time macro is defined in client.c
		Packet Size macro is defined in packet.h
		Packet Drop Rate macro is defined in server.c
