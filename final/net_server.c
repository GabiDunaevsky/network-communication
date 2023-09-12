#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>     
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#define PORT 5060
#define SIM_LENGTH 10
// ANSWERS IN THE PDF FILE.

int main(void)
{ 
  //Definitions.
  int sock; 
  int connect_sock; 
  struct sockaddr_in serv_name; 
  size_t len; 
  int count;
  // opening the socket.
  sock = socket(AF_INET, SOCK_STREAM, 0);  
  bzero(&serv_name, sizeof(serv_name));
  // difining the parameters for the server.
  serv_name.sin_family = AF_INET;                                   
  serv_name.sin_port = htons(PORT); 
  // making the bind and listening.
  bind(sock, (struct sockaddr *)&serv_name, sizeof(serv_name));
  listen(sock, 1); 
  len = sizeof(serv_name);
  // accepting the things that are going through the socket.
  connect_sock = accept(sock, (struct sockaddr *)&serv_name, &len);
  // a for loop that prints all the numbers that the server sent, (he is writing to the send_udp file).
  for (count = 1; count <= SIM_LENGTH; count++)
    { write(connect_sock, &count, 4);
      printf("Server has written %d to socket.\n", count);
    }
  // closing the socket.
  close(connect_sock); 
  close(sock);

}
