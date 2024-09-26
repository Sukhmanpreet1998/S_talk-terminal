#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "list.h"
#include <pthread.h>

//initialising the input_output threads
void input_output_init(List* l1, List* l2);
void* inputMessage();                                                       
void* printMessage();
void close_input_output();
#endif