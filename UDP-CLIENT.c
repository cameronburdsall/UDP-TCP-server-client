// P2P Client
// Cameron Burdsall
// COEN 149L Th 2:15pm
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){

 //argv[1] is to be a file to be written/read

 //Declare socket file descriptor. All Unix I/O treams ae  referenced  by ecriptors
 int sockfd;
 FILE *fp;

 //Declare sending buffer of size 1k bytes
 char sbuf[1];

 //Declare server address
 struct sockaddr_in servAddr;
 socklen_t addrLen = sizeof(struct sockaddr);

 //Converts  domain  names  into  numerical IP ddresses va N
 struct hostent *host;
 host = (struct hostent *)gethostbyname("localhost"); //Local host runs the server. You may use "127.0.0.1": loopback IP address change to "localhost" for personal

 //Open a socket, if successful, returns a descriptor  associated with  an endpoint
 if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Failure to setup an endpoint socket\n");
    exit(1);
 }

 //Set the server address to send using socket addressing structure
 servAddr.sin_family = AF_INET;
 servAddr.sin_port = htons(5000);
 servAddr.sin_addr = *((struct in_addr *)host->h_addr);

 //Client to send messages to the server continuously using UDP socket
 int input;
 printf("Recieve from server 1\nSend file to server 2\n");
 scanf("%d", &input);
 sendto(sockfd, &input, 1, 0, (struct sockaddr *)&servAddr, sizeof(struct sockaddr));


 if (input == 1) {
   fp = fopen(argv[1], "w");
   if (fp == NULL)
   {
     printf ("Recieving File not opened...\nExiting...\n");
     exit(2);
   }
   printf("Awaiting data...\n");
   while(1) {
    recvfrom(sockfd, sbuf, 1, 0, (struct sockaddr *)&servAddr, &addrLen);
    printf("%s", sbuf);
    fwrite(&sbuf, 1, 1, fp);
    fflush(fp);
   }
   printf("File Recieved...\n");
 }
 
 if(input == 2) {
   fp = fopen(argv[1], "r");
   if (fp==NULL)
   {
     printf ("Sending File not opened...\nExiting...\n");
     exit(1);
   }
   printf("Sending File Opened...\n");
   while(fread(&sbuf, 1, 1, fp) > 0){ 
     printf("%s", sbuf);
     sendto(sockfd, sbuf, strlen(sbuf), 0, (struct sockaddr *)&servAddr, sizeof(struct sockaddr)); 
   }
   printf("File Sent...\n");
 }
 fclose(fp);
 return 0;
}
