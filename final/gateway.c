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

#include <time.h>
#include <stdlib.h>
// ANSWERS IN THE PDF FILE.

// This functiom prints the IP and the port (the socket).
void printsin(struct sockaddr_in *s, char *str1, char *str2) {
  printf("%s\n", str1);
  printf("%s: ", str2);
  // -- port: sin->sin_port (host integer type) 
  // -- IP: sin->sin_addr (IP in dotted-decimal notation)
  char buffer[INET_ADDRSTRLEN];
  inet_ntop( AF_INET, &s->sin_addr, buffer, sizeof( buffer )); // This function putting the address as a char array.
  printf("ip= %s,", buffer);
  printf(" port= %d", s->sin_port);
  printf("\n");
}


int main(int argc, char *argv[])
{
  // Definitions.
  int socket_fd, cc, fsize,socket_fd_next;
  struct sockaddr_in  s_in, from, dest;
  struct hostent *hostptr;
  struct { char head; u_long  body; char tail;} msg;
// opening the socket between the source and the getaway.
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);
// opening the socket between the getaway and the sink.
  socket_fd_next = socket (AF_INET, SOCK_DGRAM, 0);

  bzero((char *) &s_in, sizeof(s_in));
  bzero((char *) &dest, sizeof(dest));
  // getting the first parameter from the command line.
  hostptr = gethostbyname(argv[1]);
  // getting the second parameter from the command line.
  unsigned long portNum =strtoumax(argv[2],NULL,0);
  // incrementing the port by one as requested.
  int nextPortNum = portNum + 1;
  // difining the parameters between the getaway and the source.
  s_in.sin_family = (short)AF_INET;
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);
  s_in.sin_port = portNum;
  // difining the parameters between the getaway and the sink.
  dest.sin_family = (short) AF_INET;
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);
//   dest.sin_port = htons((u_short)0x3333);
  dest.sin_port = nextPortNum;

  printsin( &s_in, "RECV_UDP", "Local socket is:"); 
  fflush(stdout);
  // making the socket listening to the massages that are sending.
  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));
  // anitiallizing the random as requiers in the task.
  srandom(time(NULL));
  // for loop that recving and printing the masseges it is getting and send it to the sink after using the random
  // function that "loosing 50 pracents of the packets."
  for(;;) {
    fsize = sizeof(from);
    cc = recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize);
    printsin( &from, "recv_udp: ", "Packet from:");
    printf("Got data ::%c%ld%c\n",msg.head,(long) msg.body,msg.tail); 
    fflush(stdout);
    float randomal = ((float)random())/((float)RAND_MAX);
    if(randomal > 0.5){
        sendto(socket_fd_next,&msg,sizeof(msg),0,(struct sockaddr *)&dest,
                  sizeof(dest));
    }
  }
  
  return 0;
}