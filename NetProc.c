#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <pthread.h>

#include <sys/socket.h>

#include <netinet/in.h>

//#include <fcntl.h>
//#include <dirent.h>

#include <errno.h>


//Tree containing max number of threads
////Add a thread, delete a thread, sarch for a thread! of username type int
typedef struct ThreadList_{
//
int User;
//
pthread_t* Thread;
//
struct ThreadList_*Next;
//
} ThreadList;
//

//Always have a pointer to last occurrence and the most recent occurrence.
//pointer is like linked list to NULL from top down
//top is originally NULL
//Int operation to perform

typedef struct NetQueue_{
//The number of nodes(sockets) needed for the operation done( if operation is large, 4K(2^12) bytes, EX: 8K read needs Size 2 for 2 connection multiplex ( two threads), THIS MEANS THE OPERATIONS ARE DONE OVER BOTH THREADS, half and half Approx. 4K each thread. 
int Size;
//Sum of all sizes should never be over 10!
//THEN WILL HAVE AT MAX 10 PORTS/sockets!
int type;
//What type of operation needs to be done? Open, read, write?.
//0 for open, 1 for read, 2 for write
//What type of operation do you require?
int Port;
//Port number the client socket is connected to for server.
void* Instruction;
//Points to the arguments of either open, read, or write.
//What instruction this pointer points to, must be interpreted according to type of operation.
struct NetQueue_* Down;
//To the next client.
}NetQueue;


NetQueue* Top = NULL;

NetQueue* Bottom = NULL;

//Originally bottom is located at the top.

void Push(NetQueue** Q,int type, void* instruction);

NetQueue* Pop(NetQueue** Q);

int Execute(int Operation);

//int LISTEN_BACKLOG 

int main( int argc, char** argv){
Bottom=Top;

//struct sockaddr addr;

int port = 8002;

int sock = socket(AF_INET,SOCK_STREAM,0);

if(sock<0){
printf("SOCKET ERROR:%s",strerror(errno));

return errno;

}

struct sockaddr_in serv_addr;

serv_addr.sin_family=AF_INET;

serv_addr.sin_port=htons(port);

serv_addr.sin_addr.s_addr = INADDR_ANY;

printf("\nSERVER FAMILY: %d, SERVER PORT(Converted to network byte order): %d, SERVER IP: %d\n",serv_addr.sin_family,serv_addr.sin_port,serv_addr.sin_addr.s_addr);

if(bind(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){

printf("SOCKET BINDING ERROR:%s",strerror(errno));

return errno;

}

printf("\nBIND OF PORT:%d SUCCESSFUL!\n",port);


//return 0;

int Listen=1;
socklen_t clilen;
int newsock;
struct sockaddr_in cli_addr;

char buffer[256];
int n=0;
//Listen for the same port numbers, use socket which is now binded to port.
while(Listen>=0){
printf("LISTENING\n");

//Only listen until reach 10 socket connections!
Listen = listen(sock,10);

printf("Status: %s\n",strerror(Listen));
if(Listen>=0){
//Found a connection??

//clilen = sizeof(cli_addr);
//cli_addr_size = sizeof(serv_addr);
clilen=sizeof(cli_addr);

printf("\nWAITING TO ACCEPT A SOCKET\n");
newsock = accept(sock,(struct sockaddr*) &cli_addr,&clilen);

if(newsock<0){
printf("\nACCEPT ERROR:%s\n",strerror(errno));
return errno;
}
//Found a socket connection!
do{
printf("\nFOUND A SOCKET CONNECTION USING PORT: %d! INSTRUCTIONS\n",port);
bzero(buffer,256);
n=read(newsock,buffer,255);


if(n<0){

printf("ERROR: %s",strerror(errno));

exit(0);
}

printf("\nMessage from client:\n%s, LENGTH: %d\n",buffer,strlen(buffer));



}while(strcmp(buffer,"QUIT")!=0&&strlen(buffer)>1);

}
}
if(errno!=0){

printf("LISTEN ERROR:%s",strerror(errno));

return errno;

}

printf("END LISTENING");
//printf("%s",strerror(errno));
printf("???");


//int binder = bind(sock,

//netserverinit("adapter.cs.rutgers.edu","exclusive");

//errno=-1;

//printf("%s",strerror(errno));

//perror(strerror(errno));

return 0;
}
//Push to the bottom the type of operation and the pointer to the operation's instructions.
void Push(NetQueue**Q,int type, void* instruct){
//Pointer should be the last
if(Top==NULL||*Q==NULL){
Top = malloc(sizeof(NetQueue));
Top->type = type;
Top->Instruction = instruct;
Bottom=Top->Down=NULL;
}
else{
//Push to top, so push every other pointer down.
NetQueue* Q2 = malloc(sizeof(NetQueue));
Q2->type=type;
Q2->Instruction = instruct;
Q2->Down=Top;
//New Top.
(*Q)=Q2;
}
return;
}

NetQueue* Pop(NetQueue**Q){
//Execute, then delete.
//
//
//
//
//
//
while((*Q)!=NULL){
if((*Q)->Down==NULL){
free((*Q)->Instruction);
free(*Q);
(*Q)=NULL;
}
Bottom=(*Q);
Q=&((*Q)->Down);
}
return *Q;
}

int Execute(int op){



return 0;
}
