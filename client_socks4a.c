#include "apue.h"
#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <sys/socket.h>

#define BUFLEN 128
#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 128
#endif

extern int initserver(int, struct sockaddr *, socklen_t, int);
extern struct sockaddr_in generateSockAddr(char *ip, int port);
extern int connect_retry(int, const struct sockaddr *, socklen_t);

void serve(int sockfd)
{
  int clfd;
  FILE *fp;
  char buf[BUFLEN];
  int n;

  struct sockaddr_in addr_in;
  int sockfdS, err;
  addr_in = generateSockAddr("106.187.49.213", 21080);
  if ((sockfdS = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    err = errno;
    printf("socket error");
    exit(2);
  }
  if (connect_retry(sockfdS, (struct sockaddr*)&addr_in, sizeof(struct sockaddr))<0) {
     err = errno;
     fprintf(stderr, "can't connect to server:%s\n", strerror(err));
     exit(1);
  } else {
     printf("connected to server \n");
  }

  for (;;) {
    clfd = accept(sockfd, NULL, NULL);
    printf("accept  \n");
    if (clfd<0) {
      printf("accept error %s %d \n", strerror(errno), sockfd);
      exit(1);
    }
    if ((n = recv(sockfdS, buf, BUFLEN, 0))>0) {
      printf("receive from server \n");
      send(clfd, buf, strlen(buf),0);
    }
    //打开远程端口， 发送数据
    if ((n = recv(clfd, buf, BUFLEN, 0))>0) {
      printf("receive from brower \n");
      printf("1: %d \n", buf[0]);
      printf("2: %d \n", buf[1]);
      printf("len: %d \n", strlen(buf));
      send(sockfdS, buf, strlen(buf),0);
      printf("sended to server\n");

    }
    //从远程端口接收数据， 返回
    if ((n = recv(sockfdS, buf, BUFLEN, 0))>0) {
      printf("receive from server \n");
      send(clfd, buf, strlen(buf),0);
    }

    //关闭远程端口
  }
    close(clfd);

}
int main(int argc, char *argv[])
{
  struct addrinfo *ailist, *aip;
  struct addrinfo hint;
  struct sockaddr_in sinp;
  int sockfd, err, n;
  char *host;

  struct sockaddr *addr;
  char abuf[INET_ADDRSTRLEN];
  const char *addrname;
#ifdef _SC_HOST_NAME_MAX
  n = sysconf(_SC_HOST_NAME_MAX);
  if (n < 0)
#endif
    n = HOST_NAME_MAX;
  sinp = generateSockAddr("127.0.0.1", 21080);
  if ((sockfd = initserver(SOCK_STREAM, (struct sockaddr *) &sinp, sizeof(struct sockaddr), QLEN)) >= 0) {
    //sinp = (struct sockaddr_in *)aip->ai_addr;
    //addrname = inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN);

    printf("start serve %s\n", "sdf");
    serve(sockfd);
    exit(0);
  }else{
    printf("no serve\n");
  }

}
