all:
	gcc server.c -o server
	gcc client.c -o client
	./server 65453 & 
	sleep 1
	./client 127.0.0.1:65453

	
