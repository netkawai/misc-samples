#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>

int main(int argc, char*argv[])
{
  struct addrinfo hints; // hints and response
  struct sockaddr_in sa;
  struct addrinfo *results, *rp;

  struct sockaddr_in6 *ipv6;
  void *tmp;

  struct hostent *hp;

  hp = gethostbyname(argv[1]); // classic way only ipv4
  if(hp == NULL){
    fprintf(stderr,"Not found host default gethostbyname\n");
  }

#if 0
  // try IPv6 right now, the assumption is to use google public dns.
  hp6 = gethostbyname2(argv[1],AF_INET6);
  if(hp6 == NULL){
    fprintf(stderr,"Not found IPv6 host\n");
  }
  if(hp == NULL && hp6 == NULL){
    fprintf(stderr,"Not found host\n");
    exit(2);
  }
#endif


  // Fill out hints for getaddrinfo().
  memset (&hints, 0, sizeof (struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = hints.ai_flags | AI_CANONNAME;

  // Resolve target using getaddrinfo().
  if ((status = getaddrinfo (argv[1], NULL, &hints, &results)) != 0) {
    fprintf (stderr, "getaddrinfo() failed: %s\n", gai_strerror (status));
    exit (2);
  }

  // search ipv6
  for (rp = results; rp != NULL; rp = rp->ai_next) {
    if(rp->ai_family == AF_INET6){
      ipv6 = (struct sockaddr_in6 *) rp->ai_addr;
    }
  }
  if(ipv6 == NULL){
    fprintf(stderr,"Not found IPv6 address. Please check DNS64\n");
    exit(2);
  }


#if 0 // only for debug
  tmp = &(ipv6->sin6_addr);
  if (inet_ntop (AF_INET6, tmp, dst_ip, INET6_ADDRSTRLEN) == NULL) {
    status = errno;
    fprintf (stderr, "inet_ntop() failed.\nError message: %s", strerror (status));
    exit (EXIT_FAILURE);
  }
#endif

    int sfd = socket(PF_INET6, SOCK_STREAM, IPPROTO_TCP);

    if(connect(sfd,ipv6,sizeof(struct sockaddr_in6)) == -1){
      fprintf(stderr,"failed to connect the server");
      close(sfd);
      exit(2);
    }
    send_get_request(argv[1],sfd);

    close(sfd);

  return 0;
}


int send_get_request(char * url,int sfd) {
#define BUF_SIZE 1024

  char sendbuf[BUF_SIZE];
  char revbuf[BUF_SIZE];
  sprintf(sendbuf, "GET / HTTP/1.1\nHOST: %s\n\n", url);
  // send
  write(sfd, sendbuf, strlen(sendbuf));

  memset(&revbuf, 0, sizeof(revbuf));
  ret = recv(sockfd, revbuf, BUF_SIZE, 0);
  if(ret < 0){
    fprintf(stderr,"Received error\n");
    exit(2);
  }
  printf("%s",revbuf);
}
