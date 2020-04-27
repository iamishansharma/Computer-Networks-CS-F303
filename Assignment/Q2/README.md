## Instruction to compile

	Run 'make clean' to delete any previously generated executables.
	Run 'make' to compile and generate executibles

## Instructions to run

	< > -> command line arguments

	1. ./server	<output file name>

		Enter server port, relay 1 port number and relay 2 port number

	2. ./relay

		Enter port number for 'this' relay and server

	3. ./relay

		Enter port number for 'this' relay and server

	4. ./client <input file name>

		Enter port number for relay 1 and relay 2

	All IP addresses have been defined as macros in 'packet.h'
	and set at submission as '127.0.0.1'. 

	Please run 1 to 4 in order.

	Eg. 

		1. Run 		./server output.txt 			in a new terminal

			Port Number for Server:  1101
			Port Number for Relay 1: 1234
			Port Number for Relay 2: 4321

		2. Run 		./relay 						in a new terminal

			Port Number for this Relay: 1234
			Port Number for Server:  1101

		3. Run 		./relay 						in a new terminal

			Port Number for this Relay: 4321
			Port Number for Server:  1101

		4. Run 		./client input.txt 			in a new terminal

			Port Number for Relay 1: 1234
			Port Number for Relay 2: 4321

		Now every terminals runs by itself.

		One of the relays may not terminate, 
		please terminate that after every other 
		terminals has finished execution

## Some Details about the code

	I couldn't finish the implementation of Selective Repeat Protocol.
	
	Hence the partial code with transfer of client to server of input file over UDP through relays.

	Macros: 

		Retransmission Time macro is defined in client.c
		Packet Size macro is defined in packet.h
		Window Size macro is defined in packet.h
		Delay Time macro is deinfed in relay.c
		Packet Drop Rate macro is defined in relay.c