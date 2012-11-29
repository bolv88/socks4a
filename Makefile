#gcc server_udp_thread.c initserver.c generateSock.c -lpthread
all:
	gcc client_socks4a.c initserver.c generateSock.c connect_retry.c -lpthread -o client.ext
	gcc client_old.c initserver.c generateSock.c connect_retry.c -o client_old.ext
