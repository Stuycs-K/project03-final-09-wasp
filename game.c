#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/stat.h>
//#include <semaphore.h> //consider using this one instead, since it's 

#define SEMKEY 5489

void semDown(int sem_id){
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_op = -1;
  sb.sem_flg = 0;
  semop(sem_id, &sb, 1);
}

void semUp(int sem_id){
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_op = 1;
  sb.sem_flg = 0;
  semop(sem_id, &sb, 1);
}

int main(){
  int sem_id = semget(SEMKEY, 1, IPC_CREAT | 0666);
  if(sem_id == -1){
    perror("semget failed in game main\n");
    exit(1);
  }
  if(semctl(sem_id, 0, SETVAL, 1) == -1){
    perror("semctl failed in game main\n");
  }
  // assuming that if I change the 0 here, the order will change as well? Which might we what we want.
  // also, something like GETPID would be nice to use as well. 
  return 0;
}

