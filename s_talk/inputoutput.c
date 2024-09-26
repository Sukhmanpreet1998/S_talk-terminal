#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "inputoutput.h"
#include "mutex_head.h"
#include <unistd.h>
#define MAXBUFLEN 1000


static List* input_list;
static List* output_list;
static pthread_t input_thread;
static pthread_t print_thread;

void input_output_init(List* l1, List* l2)
{
    input_list  =  l1;
    output_list =  l2;
    pthread_create(&input_thread, NULL, inputMessage, NULL);
    pthread_create(&print_thread, NULL, printMessage, NULL);
}



void close_input_output()
{
    pthread_join(input_thread, NULL);
    pthread_join(print_thread, NULL);
}
void* inputMessage()                                                                            //if the user enters the ! , we have to signal the sender as well and then terminate the threads of recieve and print as we do not want to recieve anything to print
{
    char reader[MAXBUFLEN];
    int size;
    int insertion;                                       
    while(1)
    {
        printf("YOU  :   ");
        if (fgets(reader, MAXBUFLEN, stdin) == NULL) 
        {
            perror("reading error in input message\n");
            exit(-1);
        }                                                                                                             //setting the last char to \0 instead of \n for transferring purposes
        pthread_mutex_lock(&write_mutex);
        {
            List_prepend(input_list,(void *) reader);                                        //writing to the list and signalling everyone that this list is being written from over here
        }
        pthread_mutex_unlock(&write_mutex);
     
        if(reader[0]=='!')                                                              //ending the function
        {
            printf("End of chat \n");
            sendSignal();                                                                                   
            pthread_cancel(&input_thread);
            pthread_cancel(&print_thread);
            break;          
        }
        sendSignal();
    }
    return NULL;
}

void* printMessage()                                                                
{
    char* writer;
    int count ;
    while(1)
    {
        pthread_mutex_lock(&print_mutex);                                                                //waiting for the signal to from recieve function                                  
        {
            pthread_cond_wait(&print_condition, &print_mutex);
        }
        pthread_mutex_unlock(&print_mutex);
         
        writer = List_last(output_list);
        printf("\nFriend:  %s\n", writer);
        if(writer[0]=='!')                                                              //ending the function
        {
            printf("End of chat \n");                                                                                   
            pthread_cancel(&print_thread);
            pthread_cancel(&input_thread);
            break;          
        }
    }
    return NULL;

}