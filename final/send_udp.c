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
  // Definitions.
  int socket_fd;
  struct sockaddr_in  dest;
  struct hostent *hostptr;
  struct { char head; u_long body; char tail; } msgbuf;
  // opening the UDP socket.
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);
  bzero((char *) &dest, sizeof(dest)); /* They say you must do this */
  // getting the parametr from the command line.
  hostptr = gethostbyname(argv[1]);
  // Defining the paremeters for the send.
  dest.sin_family = (short) AF_INET;
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);
  dest.sin_port = htons((u_short)0x3333);
  // bulding the massage.
  msgbuf.head = '<';
  // the getpid function returns the process ID (PID) of the calling process, so it will return a number.
  msgbuf.body = htonl(getpid()); /* IMPORTANT! */
  msgbuf.tail = '>';
  // sending the massage using the socket.
  sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,
                  sizeof(dest));

  return 0;
}
