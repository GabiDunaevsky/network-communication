
#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>    
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define SIM_LENGTH 10

#define IP_ADDRESS "127.0.0.1"
// ANSWERS IN THE PDF FILE.

int main(int argc, char* argv[])
{ 
  // Definitions.
  char urlName[200];
  int port = 80;
  int sock; 
  struct sockaddr_in cli_name; 
  struct addrinfo* res;
  char* urlAddr = IP_ADDRESS;
  char* arg;
  struct sockaddr_in* saddr;
  char path[2056];
  path[0] = '\0';
  int is_path = 1;
  // checking if arguments that sending in the commandline are diffarent then 2.
  if (argc != 2) {
    fprintf(stderr, "Usage: arglookup <arg>. Too many args.\n");
    exit(1);
  }
  // getting the url and parse it according to his structure.
  // in this parsing i was consulting with a friend, and we found the source:
  // https://www.faqcode4u.com/faq/16257/best-ways-of-parsing-a-url-using-c. 
  else{
    // getting the url
    arg = argv[1];
    // moving 7 steps to pass the http start.
    int length = strlen(arg);
    memmove(arg, arg + 7, length - 7 + 1);
    // searching the "":" sign to check if there is port. 
    char* findPort = strchr(arg, ':');
    if (findPort != NULL){
      sscanf(arg, "%99[^:]:%99d/%99[^\n]", urlName, &port, path);
    }
    else{
      // searching for "/" sign to check for the path.
      char* findPath = strchr(arg, '/');
      if (findPath != NULL){
        sscanf(arg, "%99[^/]/%99[^\n]", urlName, path);
      }
      else{
        sscanf(arg, "%99[^/]", urlName);
        is_path = 0;
      }
    }
    // Into result we are puting the result of the getaddrinfo function(which is struct sockaddr type) it will help us
    //  to get the IP address us a string.
    if (0 != getaddrinfo(urlName, NULL, NULL, &res)) {
      fprintf(stderr, "Error in resolving arg %s", urlName);
      exit(1);
    }
    else{
      saddr = (struct sockaddr_in*)res->ai_addr;
      urlAddr = inet_ntoa(saddr->sin_addr);
    }
  }
  // opening a TCP socket.
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) 
    { perror ("Error opening channel");
      close(sock);
      exit(1);
    }
  // Defining the paremeters for the server.
  struct sockaddr_in server;
  server.sin_addr.s_addr = inet_addr(urlAddr);
  server.sin_family = AF_INET;
  server.sin_port = htons(port);

  printf("check:\n%s\n%d", urlAddr, inet_addr(urlAddr));
  // making the connect.
  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
  { 
    perror ("Error establishing communications");
    close(sock);
    exit(1);
  }
  
  char msg[600];
  char *header = (char*)malloc(1024 * sizeof(char));
  // preparing the get massege.
  if (is_path == 0){
    sprintf(header, "GET / HTTP/1.0\r\nHost: %s\r\n\r\n", urlName);
  }
  else{
    sprintf(header, "GET /%s HTTP/1.0\r\nHost: %s\r\n\r\n", path, urlName);
  }
  
  printf("%s\n\n", header);
  // sending the get massege.
  if (send(sock,header,strlen(header),0) < 0){
    printf("http request failed.");
    exit(1);
  }
// reciving the packets from the server and printing.
  int numberOfBytes = recv(sock,msg,sizeof(msg)-1,0);
  while (numberOfBytes != 0)
  {
      msg[numberOfBytes] = '\0';
      printf("%s",msg);
      numberOfBytes = recv(sock,msg,sizeof(msg)-1,0); 
  }
  printf("\n\n");
  //closing the socket.
  close(sock); 
  exit(0);
}