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
#include <time.h>

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

void shuffleStrings(char* arr[], int n){
  srand(time(NULL));

  for(int i = n-1; i > 0; i--){
    int j = rand() % (i+1);

    char* temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
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
  *(shmp+4) = 0;
  *(shmp+5) = 0;
  *(shmp+6) = 0;
  *(shmp+7) = 0;
  *(shmp+8) = 1;


  int sem_id = semget(SEMKEY, 1, IPC_CREAT | 0666);
  if(sem_id == -1){
    perror("semget failed in game main\n");
    exit(1);
  }
  if(semctl(sem_id, 0, SETVAL, 1) == -1){
    perror("semctl failed in game main\n");
  }

  int fd = open("gameProgress.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd < 0){
    perror("File failed to generate in game main\n");
    exit(1);
  }
  close(fd);


  char* arr[] = {"C2\n", "C3\n", "C4\n", "C5\n", "C6\n", "C7\n", "C8\n", "C9\n", "C10\n", "CJ\n", "CQ\n", "CK\n", "CA\n", 
    "D2\n", "D3\n", "D4\n", "D5\n", "D6\n", "D7\n", "D8\n", "D9\n", "D10\n", "DJ\n", "DQ\n", "DK\n", "DA\n",
    "H2\n", "H3\n", "H4\n", "H5\n", "H6\n", "H7\n", "H8\n", "H9\n", "H10\n", "HJ\n", "HQ\n", "HK\n", "HA\n",
    "S2\n", "S3\n", "S4\n", "S5\n", "S6\n", "S7\n", "S8\n", "S9\n", "S10\n", "SJ\n", "SQ\n", "SK\n", "SA\n"};
  int n = sizeof(arr)/sizeof(arr[0]);
  shuffleStrings(arr, n);

  int fd2 = open("shuffledArray.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd2 < 0){
    perror("File 2 failed to generate in game main\n");
    exit(1);
  }
  for(int i = 0; i < n; i++){
    if(write(fd2, arr[i], strlen(arr[i])) == -1){
      perror("Error writing to fd2\n");
      close(fd2);
      exit(1);
    }
  }
  close(fd2);
  
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

