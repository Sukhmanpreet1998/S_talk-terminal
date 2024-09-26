#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "list.h"
#include <pthread.h>

 //int sockfd;
 //struct addrinfo hints, *res;
 //struct addrinfo recvhints, *recvres;

// initializing the sending reciveing threads
void send_recv_init(List* l1, List*l2, char* hostname, char*remoteport, char* myport);

// create a socket
int createsocket(char* hostname, char* remoteport, char* myport);

// bind the socket
int bindsocket();

// send message to the remote host
int sendMsg(char* msg);

// receive message from the remote host
int recvMsg(List* listmsgs);

// close socket
int closeSocket();

// thread function for sendMessage
void* sendMessage();  

// thread function for recieveMessage
void* recieveMessage();

//closing the thread
void close_send_recv();