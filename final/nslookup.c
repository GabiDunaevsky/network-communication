#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) { // First of all we are sending in the commandline the the hostename.
  struct addrinfo* res;
  char* hostname;
  char* hostaddr;
  struct sockaddr_in* saddr;
  
  if (argc != 2) { // This function is to make sure we are sensing only 1 paremeter.
    perror("Usage: hostnamelookup <hostname>\n");
    exit(1);
  }

  hostname = argv[1];
  
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) {              // Into result we are puting the result of the getaddrinfo function(which is struct sockaddr type) it will help us
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);//  to get the IP address us a string.
    exit(1);
  }
  
  saddr = (struct sockaddr_in*)res->ai_addr;      // in this two lines we are getting the IP address.
  hostaddr = inet_ntoa(saddr->sin_addr);

  printf("Address for %s is %s\n", hostname, hostaddr);  // printing the ip address and the hostename of the ip address(the one that we put in the command line).
  exit(0);
}
