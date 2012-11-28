#gcc server_udp_thread.c initserver.c generateSock.c -lpthread
all:
	gcc client_socks4a.c initserver.c generateSock.c connect_retry.c -o client.ext
