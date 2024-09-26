#ifndef _MUTEX_HEAD_H
#define _MUTEX_HEAD_H
#include <pthread.h>

extern pthread_cond_t print_condition;
extern pthread_cond_t send_condition;

//MUTEXES TO CONTROL THE VARIABLES AND WRITING 
extern pthread_mutex_t print_mutex;
extern pthread_mutex_t send_mutex;
extern pthread_mutex_t write_mutex;    //use it for prepending and trimming input
extern pthread_mutex_t out_w_mutex;    //use for output_list
extern void printSignal();
extern void sendSignal();

#endif 