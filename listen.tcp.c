/* listen.c */

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
   char message[80];
   int bytes;
   int sock, serv_host_port, clilen, newsock;
   struct sockaddr_in cli_addr, serv_addr;

   if (argc != 2) {
      fprintf(stderr, "%s - server to accept TCP connections", argv[0]);
      fprintf(stderr, "usage: %s <port>\n", argv[0]);
      fprintf(stderr, "\t<port>\t- port to listen on\n");
      exit(1);
   } 

   serv_host_port = atoi(argv[1]);

   printf("Listen activating.\n");

   printf("Trying to create socket at port %d...\n", serv_host_port);   

   /* Create socket from which to read */
   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("can't open stream socket");
      exit(1);
   }
   
   /* bind our local address so client can connect to us */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port = htons(serv_host_port);
   
   if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("can't bind to local address");
      exit(1);
   }

   printf("Socket created! Accepting connections.\n\n");

   listen(sock, 5);

   clilen = sizeof(cli_addr);
   newsock = accept(sock, (struct sockaddr *) &cli_addr, &clilen);
   if (newsock < 0) {
     perror("accepting connection");
     exit(1);
   }
   
   while ((bytes = read(newsock, message, 1024)) > 0) {
     message[bytes] = '\0';
     printf("recv: %s\n", message);
   }
}


