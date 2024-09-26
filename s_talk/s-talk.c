#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sendrecieve.h"
#include "inputoutput.h"

char* myport;
char* hostname;
char* otherport;

int main(int argc, char *argv[])                                                         
{
    List* i_list = List_create();
    List* o_list = List_create();
    if (argc != 4) 
    {
         printf("usage: s_talk yourport remotehostname remoteport\n");
         exit(0);
    }
    
    myport = argv[1];
    hostname = argv[2];
    otherport = argv[3];
    input_output_init(i_list, o_list);
    send_recv_init(i_list, o_list, hostname, otherport, myport);
    close_input_output();
    close_send_recv();
    List_free(i_list, free);
    List_free(o_list, free);
    return 0;
}