// send sip from file

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>   /* For <netinet/in.h> */
#include <netinet/in.h>  /* For struct sockaddr_in */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>       /* struct hostent */
#include <pwd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff /* should be in <netinet/in.h> */
#endif

static int size = 0;

static char* parse_file(const char *file) {
	char *buf;
	struct stat s;
	int fd = open(file, O_RDONLY);
	if (fd < 0) return NULL;
	fstat(fd, &s);
	buf = mmap(0, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
  size = s.st_size;
	if (buf == MAP_FAILED) return NULL;
	// if (munmap(buf, s.st_size) < 0) abort();
	return buf;
}

int main(int argc, char *argv[]) {
  int i, sock, bytes;
  char buf[80];
  struct sockaddr_in  srv_addr;  /* server's Internet socket address */
  struct hostent      *hp; /* from gethostbyname() */
  int port = 5060;

  if (argc != 3) {
    printf("Usage: %s <host[:port]> <file>\n", argv[0]);
    return 1;
  }

  char *host = argv[1];
  char *p = strchr(host, ':');

  if (p) {
    *p = '\0';
    p++;
    port = atoi(p);
  }

  if (port <= 0 || port > 65535) {
    port = 5060;
  }

  char *sip = parse_file(argv[2]);
  printf("%s\n", sip);

  bzero((char *) &srv_addr, sizeof(srv_addr));
  srv_addr.sin_family = AF_INET;
  srv_addr.sin_port = htons(port);
  if ((hp = gethostbyname(argv[1])) == NULL) {
    perror("host name error");
    return 1;
  }
  bcopy(hp->h_addr, (char *) &srv_addr.sin_addr, hp->h_length);
  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("opening udp socket");
    return 1;
  }

  printf("connecting to %s:%d\n", host, port);

  if (connect(sock, (struct sockaddr *) &srv_addr, sizeof(srv_addr))<0) {
    perror("connect error");
    return 1;
  }

  printf("sending %d bytes\n", size);

  if (send(sock, sip, size, 0) != size) perror("send");

  close(sock);
	if (munmap(sip, size) < 0) abort();
}

