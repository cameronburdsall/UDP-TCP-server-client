// P2P Server
// Cameron Burdsall
// COEN 149L Th 2:15pm
// Waits for input from Client, then either sends or recives a file
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){

  //argv[1] is to be a file to be written/read

 //Declare socket file descriptor. All Unix I/O treams ae  referenced  by ecriptors
 int sockfd;

 //Declare receiving buffer of size 1k bytes
 char rbuf[1];

 //Declare server address to which to bind for receiving messages and client address to fill in sending address
 struct sockaddr_in servAddr, clienAddr;
 socklen_t addrLen = sizeof(struct sockaddr);

 //Open a UDP socket, if successful, returns a descriptor  associated with  an endpoint
 if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Failure to setup an endpoint socket");
    exit(1);
 }

 //Setup the server address to bind using socket addressing structure
 servAddr.sin_family = AF_INET;
 servAddr.sin_port = htons(5000); //Port 5000 is assigned
 servAddr.sin_addr.s_addr = INADDR_ANY; //Local IP address of any interface is assigned (generally one interface IP address)

 //Set address/port of server endpoint for socket socket descriptor
 if ((bind(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) < 0){
	perror("Failure to bind server address to the endpoint socket");
    exit(1);
 }

FILE *fp;
char msg;
printf("Waiting for client\n");
recvfrom(sockfd, &msg, 1, 0, (struct sockaddr *)&clienAddr, &addrLen);
int input = (int)msg;
if(input == 1) {
  fp = fopen(argv[1], "r");
  if (fp==NULL)
  {
     printf ("Sending File not opened...\nExiting...\n");
     exit(1);
  }
  printf("Sending File Ready...\n");
  while(fread(&rbuf, 1, 1, fp) > 0) {
    printf("%s", rbuf);
    sendto(sockfd, rbuf, strlen(rbuf), 0, (struct sockaddr *)&clienAddr, sizeof(struct sockaddr));
  }
}

if(input == 2) {
  fp = fopen(argv[1], "w");
  if (fp==NULL)
  {
     printf ("Recieving File not opened...\nExiting...\n");
     exit(2);
  }

  printf("Recieving File Opened...\n");
  printf("Awaiting data...\n");
  while (1){
    recvfrom(sockfd, rbuf, 1, 0, (struct sockaddr *)&clienAddr, &addrLen);
    printf("%s", rbuf);
    fwrite(&rbuf, sizeof(rbuf), 1, fp); 
    fflush(fp);
   }

 }
 fclose(fp);
 return 0;
}
