#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>

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
  int sem_id = semget(SEMKEY, 1, 0666);
  if(sem_id == -1){
    perror("semget failed in player main\n");
    exit(1);
  }
  
  semDown(sem_id);

  char buffer[1024];
  if(fgets(buffer, sizeof(buffer), stdin) != NULL){
    printf("Player string: %s\n", buffer);
  }
  else{
    printf("Error reading input\n");
  }

  semUp(sem_id);

  return 0;
}
