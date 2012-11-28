#include "apue.h"
#include "err.h"
#include <errno.h>
#include <sys/socket.h>

int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen)
{
	int fd,ret;
	int err = 0;

  int on = 1;

	if ((fd = socket(addr->sa_family, type, 0))<0) {
		printf("socket err: %s (fd: %d)\n", strerror(errno),fd);
		return(-1);
	}
  ret = setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
	if (bind(fd, addr, alen) <0 ) {
		err = errno;
		printf("bind err: %s (fd: %d)\n", strerror(errno),fd);
		goto errout;
	}
	if (type == SOCK_STREAM || type == SOCK_SEQPACKET) {
		if (listen(fd, qlen) < 0) {
			err = errno;
			printf("listen err: %s\n", strerror(errno));
			goto errout;
		}
	}
	return(fd);
errout:
	close(fd);
	errno = err;
	return(-1);
}
