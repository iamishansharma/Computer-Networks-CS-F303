compile:
	gcc -o client client_broken_ftp.c
	gcc -o server server_broken_ftp.c

clean:
	rm client
	rm server