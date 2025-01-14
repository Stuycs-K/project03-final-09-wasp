#ifndef BRIDGE_H
#define BRIDGE_H

#include<errno.h>
#include<fcntl.h>
#include<semaphore.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h> 
#include<sys/ipc.h> 
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<unistd.h>

#define NUM_PLAYERS 4
#define PIPE_NAME "/tmp/game_pipe"
#define SEMKEY 5842
#define SHAREDKEY "/game_pipe"
#define BUFFER 1024

/*int game();
int jQueue();
int playerRandomization();*/

#endif
