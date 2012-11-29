#include "apue.h"
#include <pthread.h>
#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <sys/socket.h>
#include <poll.h>

#define BUFLEN 128
#define BUF_SIZE 128
#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 128
#endif

extern int initserver(int, struct sockaddr *, socklen_t, int);
extern struct sockaddr_in generateSockAddr(char *ip, int port);
extern int connect_retry(int, const struct sockaddr *, socklen_t);

void * serve(void * sockfd)
{
  int clfd;
  FILE *fp;
  char buffer[BUFLEN];
  int n;
  int nr;
  int nw;

  struct sockaddr_in addr_in;
  int sockfdS, err;
  addr_in = generateSockAddr("106.187.49.213", 21080);


  struct  pollfd pfds[2];




  for(;;) {

    printf("[[[[[[[[[[[[[[[[\n");
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
    printf("===before accept one\n");
    clfd = accept(*(int *)sockfd, NULL, NULL);
    printf("===accept one\n");
    //输入
    pfds[0].fd=clfd;
    pfds[0].events=POLLIN;
    pfds[0].revents=0;
    //输出
    pfds[1].fd=sockfdS;
    pfds[1].events=POLLIN;
    pfds[1].revents=0;

    for (;;) {
      //clfd = accept(sockfd, NULL, NULL);
      printf("before poll ==\n");
      int pr=poll(pfds,2,60000);
      if(pr == 0) continue;
      if(pr <= -1) {
          continue;
      }
      printf("=====\n");
      if(pfds[0].revents != 0) {
          if((nr=read(pfds[0].fd,buffer,BUF_SIZE)) <= 0) {
              printf( "sock4aWroker:read_from_client: ");
              close(pfds[0].fd);
              close(pfds[1].fd);
              return ;
          }
          printf("1: %d\n",(short int)(buffer[0]));
          printf("2: %d\n",(short int)(buffer[1]));
          printf("len: %d \n", strlen(buffer));
          //simpleEncode(buffer,nr);
          if((nw=send(pfds[1].fd,buffer,nr,0)) != nr) {
              printf( "sock4aWorker:write_to_server: ");
              close(pfds[0].fd);
              close(pfds[1].fd);
              return ;
          }
      }
      if(pfds[1].revents != 0) {
          if((nr=read(pfds[1].fd,buffer,BUF_SIZE)) <= 0) {
              printf( "sock4aWorker:read_from_server: ");
              close(pfds[0].fd);
              close(pfds[1].fd);
              return ;
          }
          //simpleDecode(buffer,nr);
          printf("res 1: %d\n",(short int)(buffer[0]));
          printf("res 2: %d\n",(short int)(buffer[1]));
          if((nw=send(pfds[0].fd,buffer,nr, 0)) != nr) {
              printf( "sock4aWorker:write_to_client: ");
              close(pfds[0].fd);
              close(pfds[1].fd);
              return ;
          }
      }
      pfds[0].revents=0;
      pfds[1].revents=0;
      /*
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
      //
      */
    }
  }
  close(clfd);
  return;
}
pthread_t ntid;
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
  sinp = generateSockAddr("0.0.0.0", 21080);
  if ((sockfd = initserver(SOCK_STREAM, (struct sockaddr *) &sinp, sizeof(struct sockaddr), QLEN)) >= 0) {
    //sinp = (struct sockaddr_in *)aip->ai_addr;
    //addrname = inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN);

    printf("start serve %s\n", "sdf");
    //serve(sockfd);

    int j=0;
    for (j=0; j<20;++j) {
      err = pthread_create(&ntid, NULL, serve, &sockfd);
    }
    sleep(5000000);
    exit(0);
  } else {
    printf("no serve\n");
  }

}
