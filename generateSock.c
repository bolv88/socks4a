#include <strings.h>
#include <stdio.h>
#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <sys/socket.h>

struct sockaddr_in generateSockAddr(char *ip, int port)
{
	unsigned char addr[sizeof(struct in_addr)];
	int rno;
	struct in_addr sock_addr;
	struct sockaddr_in sock_addr_in;

	rno = inet_pton(AF_INET, ip, (void *)&sock_addr);
	if (rno == 0) {
		err_quit("invalid ip");
	} else if(rno < 0) {
		err_quit("inet_pton error");
	}

	sock_addr_in.sin_family = AF_INET;
	sock_addr_in.sin_port = htons(port);
	sock_addr_in.sin_addr = sock_addr;
	bzero(&(sock_addr_in.sin_zero),8); 

	return sock_addr_in;
}
