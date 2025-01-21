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
#define SHMKEY 5839
#define MAX_PLAYERS 4

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

int main(int argc, char** argv){
  int shm_id = shmget(SHMKEY, 1024, IPC_CREAT | 0666);
  if(shm_id < 0){
    perror("shmget failing in game main\n");
    exit(1);
  }
  int* shmp = (int*)shmat(shm_id, NULL, 0);
  if(shmp == (int*)-1){
    perror("shmat failing in game main\n");
    exit(1);
  }
  *shmp = 0;
  *(shmp+1) = 0;
  *(shmp+2) = 0;
  *(shmp+3) = 0;

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

  int fd = open("gameProgress.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd < 0){
    perror("File failed to generate in game main\n");
    exit(1);
  }

  close(fd);
  
  if(argc > 1){
    if(shmdt(shmp) == -1){
      perror("shmdt failed in game main\n");
      exit(1);
    }

    if(shmctl(shm_id, IPC_RMID, NULL) == -1){
      perror("shmctl failed in game main\n");
      exit(1);
    }
  }
  return 0;
}

