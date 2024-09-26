#include "mutex_head.h"


pthread_cond_t print_condition = PTHREAD_COND_INITIALIZER;
pthread_cond_t send_condition = PTHREAD_COND_INITIALIZER;

//MUTEXES TO CONTROL THE VARIABLES AND WRITING 
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t send_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t write_mutex = PTHREAD_MUTEX_INITIALIZER;    //use it for prepending and trimming input
pthread_mutex_t out_w_mutex = PTHREAD_MUTEX_INITIALIZER;

void printSignal()                                                  //signalling okay to print
{
    pthread_mutex_lock(&print_mutex);
    {
        pthread_cond_signal(&print_condition);
    }
    pthread_mutex_unlock(&print_mutex);
}

void sendSignal()                                                   //signalling okay to send
{
    pthread_mutex_lock(&send_mutex);
    {
        pthread_cond_signal(&send_condition);
    }
    pthread_mutex_unlock(&send_mutex);
}