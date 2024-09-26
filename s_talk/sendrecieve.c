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
#include "sendrecieve.h"
#include "mutex_head.h"
#define SUCCESS 0
#define FAIL 1
#define MAXBUFLEN 1000

int sockfd;
struct addrinfo hints, *res;
struct addrinfo recvhints, *recvres;
int addrstatus;
int sendstatus;
struct sockaddr_storage remoteaddr;
int numbytes, flagger;
char buf[MAXBUFLEN];
socklen_t addrlen;

static List* input_list;
static List* output_list;
static pthread_t send_thread;
static pthread_t recieve_thread;


void send_recv_init(List* l1, List* l2, char* hostname, char*remoteport, char* myport)
{
    input_list  =  l1;
    output_list =  l2;
    createsocket(hostname, remoteport, myport);
    bindsocket();
    pthread_create(&send_thread, NULL, sendMessage, NULL);
    pthread_create(&recieve_thread, NULL, recieveMessage, NULL);
}
// create a socket
int createsocket(char* hostname, char* remoteport, char* myport) 
{
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((addrstatus = getaddrinfo(hostname, remoteport, &hints, &res)) == -1) {
        printf("createsocket: error in getaddrinfo\n");
        return FAIL;
    }
    if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
        printf("createsocket: error creating a socket\n");
        return FAIL;
    }
    if (res == NULL) {
        printf("createsocket: addrinfo in res is null\n");
        return FAIL;
    }


    memset(&recvhints, 0, sizeof recvhints);
    recvhints.ai_family = AF_UNSPEC;
    recvhints.ai_socktype = SOCK_DGRAM;
    recvhints.ai_flags = AI_PASSIVE;

    if ((addrstatus = getaddrinfo(NULL, myport, &recvhints, &recvres)) == -1) {
        printf("createsocket: error in getaddrinfo\n");
        return FAIL;
    }
    if (recvres == NULL) {
        printf("createsocket: addrinfo in res is null\n");
        return FAIL;
    }

    return SUCCESS;
}

// bind the socket
int bindsocket()
{

    if (bind(sockfd, recvres->ai_addr, recvres->ai_addrlen) == -1) {
        printf("bindsocket: error in binding the socket\n");
        return FAIL;
    }

    return SUCCESS;
}

// send message to the remote host
int sendMsg(char* msg) 
{
    // logic to send message

    if (res == NULL) {
        printf("sendMsg: Addrinfo in res is NULL\n");
        return FAIL;
    }
    if ((numbytes = sendto(sockfd, msg, strlen(msg), 0, res->ai_addr, res->ai_addrlen)) == -1 ) {
        printf("sendMsg: error sending the message\n");
        return FAIL;
    }

    //printf("%d bytes sent\n", numbytes);
    return SUCCESS;
}

// receive message from the remote host
int recvMsg(List* listmsgs)
{
    // logic to receive message
    if (recvres == NULL) {
        printf("recvMsg: Addrinfo in res is NULL\n");
        return FAIL;
    }

    addrlen = sizeof remoteaddr;
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0, (struct sockaddr *)&remoteaddr, &addrlen)) == -1){
        printf("error recieving");
        return FAIL;
    }


    // char *message;
    buf[numbytes] = '\0';

    // printf("the value of message is %s", message);
    flagger = List_prepend(output_list, (void*)buf);
    if(flagger == -1)
    {
        return FAIL;
    }   
    return SUCCESS;
}

// close socket
int closeSocket(){
    freeaddrinfo(res);
    freeaddrinfo(recvres);
    return close(sockfd);
}


void* sendMessage()   
{
    int count;
    char* message;
    while(1)
    {
        // waiting for the signal from the input message from inputoutput
        pthread_mutex_lock(&send_mutex);
        {
            pthread_cond_wait(&send_condition, &send_mutex);
        }
        pthread_mutex_unlock(&send_mutex);

        // if writing is happening making sure, it waits until the writing is done
        pthread_mutex_lock(&write_mutex);
        {
            count = List_count(input_list);
            if (count == 0)
            {
                continue;
            }
            message =List_trim(input_list);
        }
        pthread_mutex_unlock(&write_mutex);    
        if(strcmp(message, "!\n") == 1)
        {
            pthread_cancel(&send_thread);
            pthread_cancel(&recieve_thread);
        }
        sendMsg(message);
    }
    return NULL;
}

void* recieveMessage()                                          
{
    int flag;
    int count;
    char* message;
    while(1)
    {
        pthread_mutex_lock(&out_w_mutex);
        {
            flag = recvMsg(output_list);
        }
        pthread_mutex_unlock(&out_w_mutex);            
        if (flag == 1)
        {
            continue;
        }
        pthread_mutex_lock(&out_w_mutex);
        {
            count = List_count(output_list);
           
        }
        pthread_mutex_unlock(&out_w_mutex);
        // if (message[0] == '!') {
                // printSignal();
                // pthread_cancel(&recieve_thread);                                                                                   //try terminating the thread over here  
                // break;
        // }
        // if(strcmp(message, "!\n") == 1)
        //     {
        //         printSignal();
        //         pthread_cancel(&recieve_thread);                                                                                   //try terminating the thread over here  
        //         break;
        //     }
        printSignal();
    }
    return NULL;
}

void close_send_recv()
{
    pthread_join(send_thread, NULL);
    pthread_join(recieve_thread, NULL);
    closeSocket();
}