#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>

// ANSWERS IN THE PDF FILE.

// This functiom prints the IP and the port (the socket).
void printsin(struct sockaddr_in *s, char *str1, char *str2) {
  printf("%s\n", str1);
  printf("%s: ", str2);
  // -- port: sin->sin_port (host integer type) 
  // -- IP: sin->sin_addr (IP in dotted-decimal notation)
  char buffer[INET_ADDRSTRLEN];
  inet_ntop( AF_INET, &s->sin_addr, buffer, sizeof( buffer ));// This function putting the address as a char array.
  printf("ip= %s,", buffer);
  printf(" port= %d", s->sin_port);
  printf("\n");
}


int main(int argc, char *argv[])
{
  // Definitions.
  int socket_fd, cc, fsize;
  struct sockaddr_in  s_in, from;
  struct { char head; u_long  body; char tail;} msg;
 // opening the UDP socket.
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);

  bzero((char *) &s_in, sizeof(s_in));  /* They say you must do this    */
  // Defining the paremeters for the reciver.
  s_in.sin_family = (short)AF_INET;
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */
  s_in.sin_port = htons((u_short)0x3333);

  printsin( &s_in, "RECV_UDP", "Local socket is:"); 
  fflush(stdout);
  // starting listening.
  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));
  // An infinate for loop that recives the massages an print them( the loop always listening).
  for(;;) {
    fsize = sizeof(from);
    cc = recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize);
    printsin( &from, "recv_udp: ", "Packet from:");
    printf("Got data ::%c%ld%c\n",msg.head,(long) ntohl(msg.body),msg.tail); 
    fflush(stdout);
  }
  
  return 0;
}

