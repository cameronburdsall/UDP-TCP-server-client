//TCP Server
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define N 5

pthread_t threads[N];

//char sbuf[1024];

void *fileTransferThread (void *argument);


struct thread_args
{
   struct sockaddr_in clienAddr;
   int connfd;
};

int main(){
 //Declare socket file descriptor. All  Unix  I/O  streams  are  referenced  by  descriptors
 int sockfd, connfd, rb, sin_size;

 //Declare receiving and sending buffers of size 1k bytes
 char rbuf[1024];
 char sbuf[1024];

 //Declare sentf("Connection Established with client: IP %s and Port %d\n", inet_ntoa(clienAddr.sin_addr), ntohs(clienAddr.sin_port));
 while ((rb = read(connfd, rbuf, 1024))>0){
		rbuf[rb] = '\0';
		printf("Client sent: %s\n", rbuf);
	    write(connfd, "Acknowledge", 20);
    } = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Failure to setup an endpoint socket");
    exit(1);
 }

 //Setup the server address to bind using socket addressing structure
 servAddr.sin_family = AF_INET;
 servAddr.sin_port = htons(5000);
 servAddr.sin_addr.s_addr = INADDR_ANY;

 //Set address/port of server endpoint for socket socket descriptor
 if ((bind(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) < 0){
    perror("Failure to bind server address to the endpoint socket");
    exit(1);
 }

 printf("Server waiting for client at port 5000\n");

 // Server listening to the socket endpoint, and can queue 5 client requests
 //listen(sockfd, 5);
 listen (sockfd, N); //change to queue N clients, for N threads
 sin_size = sizeof(struct sockaddr_in);
int thread_count = 0;
while (1){
 //Server accepting to establish a connection with a connecting client, if successful, returns a connection descriptor  
 if ((connfd = accept(sockfd, (struct sockaddr *)&clienAddr, (socklen_t *)&sin_size)) < 0){
	perror("Fai1024)!=0lure to accept connection to the client");
    exit(1);
 }
 if (connfd >= 0 && thread_count < N)
 {
    //struct thread_args args;
    //args.connfd = connfd;
    //args.clienAddr = clienAddr;
    //pthread_create(&threads[thread_count++], NULL, fileTransferThread, (void *)&args);
    printf("Client Connected, Creating new thread...\n");
    pthread_create(&threads[thread_count++], NULL, fileTransferThread, (void *)connfd);
    printf("Thread created, Total Threads: %d\n", thread_count);
    int i;
    //join all active threads
    for (i = 0; i < thread_count; i++)
    {
       pthread_join (threads[i], NULL);
    }
 }
 else if (thread_count >= N)
 {
    printf ("Maximum number of threads reached... Closing Connection\n");
    strcpy(sbuf, "ERROR NO SERVER THREAD AVAILABLE");
    write (connfd, sbuf, strlen (sbuf));
    close(connfd);
    break;
 }

 //Connection established, server begins to read and write to the connecting client
 /*
 printf("Connection Established with client: IP %s and Port %d\n", inet_ntoa(clienAddr.sin_addr), ntohs(clienAddr.sin_port));
 while ((rb = read(connfd, rbuf, 1024))>0){
		rbuf[rb] = '\0';
		printf("Client sent: %s\n", rbuf);
	    write(connfd, "Acknowledge", 20);
    }
    */
    close(connfd);
}

close(sockfd);
return 0;
}

void *fileTransferThread (void *argument)
{
   //struct thread_args *arg = (struct thread_args *) argument;
   int file = open ("src.dat", O_RDONLY);
   //int connfd = (int) arg.connfd;
   int connfd = (int)argument;
   //struct sockaddr_in clienAddr = arg.clienAddr;
   char sbuf[1024];
   while (read(file, sbuf, 1024)!=0)
   {
      write (connfd, sbuf, strlen(sbuf));
   }
   close(file);
}
