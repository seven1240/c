/* talk.c */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff /* should be in <netinet/in.h> */
#endif

int main(int argc, char *argv[]) {
   char buf[80]; 
   unsigned long int inaddr;
   int sock, serv_host_port;
   struct sockaddr_in serv_addr;
   char *serv_host_addr;
   struct hostent *hp;

   if (argc != 3) {
      fprintf(stderr, "%s - client to try TCP connection to server", argv[0]);
      fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
      fprintf(stderr, "\t<host>\t- Internet name of server host\n");
      fprintf(stderr, "\t<port>\t- port\n");
      exit(1);
   }
 
   serv_host_addr = argv[1];
   serv_host_port = atoi(argv[2]);
   
   printf("Talk activated.\n\n");
   printf("Trying to connect to server %s at port %d...\n", 
	  serv_host_addr, serv_host_port);

   /*
    * First try to convert the host name as a dotted-decimal number.
    * Only if that fails do we call gethostbyname().
    */
   bzero((void *) &serv_addr, sizeof(serv_addr));
   printf("Looking up %s...\n", serv_host_addr);
   if ((hp = gethostbyname(serv_host_addr)) == NULL) {
     perror("host name error");
     exit(1);
   }
   bcopy(hp->h_addr, (char *) &serv_addr.sin_addr, hp->h_length);

   printf("Found it.  Setting port connection to %d...\n", serv_host_port);
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(serv_host_port);
   /* serv_addr.sin_addr.s_addr = inet_addr(serv_host_addr); */

   /* open a TCP socket (an Internet stream socket). */
   puts("Done. Opening socket...");
   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
     perror("opening socket");
     exit(1);
   }

   /* socket open, so connect to the server */
   puts("Open. Trying connection to server...");
   if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
     perror("can't connect");
     exit(1);
   }

   printf("Connection established! Type in messages to send to server.\n\n");

   
   /* read from stdin, sending to server, print responses, until quit */
   while (gets(buf)) {
     printf("sending: %s\n", buf);
     write(sock, buf, strlen(buf));
   }

    
   close(sock);

   exit(0);
}
   


   
