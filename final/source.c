#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>

// ANSWERS IN THE PDF FILE.

int main(int argc, char *argv[])
{
  //Defanitions.
  int socket_fd;
  struct sockaddr_in  dest;
  struct hostent *hostptr;
  struct { char head; u_long body; char tail; } msgbuf;
  // opening the socket.
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);
  bzero((char *) &dest, sizeof(dest));
  // getting the first parameter from the command line.
  hostptr = gethostbyname(argv[1]);
  // getting a second parameter from the command line.
  unsigned long portNum =strtoumax(argv[2],NULL,0);
  // defining the send parameters.
  dest.sin_family = (short) AF_INET;
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);
//   dest.sin_port = htons((u_short)0x3333); we can use this but in this case we are using the port from the command line as requested.
  dest.sin_port = portNum;
// bulding the massege , the body is an integer starting from zero and incrementing by one.
  msgbuf.head = '<';
  msgbuf.body = 0;
  msgbuf.tail = '>';
// an infinate  for loop that sending the massages that we difined above.
  for(;;){
    sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,
                  sizeof(dest));
    msgbuf.body++;
    sleep(1);
  }
  return 0;
}