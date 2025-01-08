#ifndef BRIDGE_H
#define BRIDGE_H

#include<stdlib.h>
#include<stdin.h>
#include<string.h>
#include<sys/types.h> 
#include<sys/ipc.h> 
#include<sys/sem.h>
#include<sys/wait.h>
#include<unistd.h>


#define BUFFER 1024

int game();
int jQueue();
int playerRandomization();

#endif
