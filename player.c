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
#define SHMKEY 5839
#define MAX_PLAYERS 4

int playerNum;

void semDown(int sem_id){
  struct sembuf sb;
  sb.sem_num = playerNum;
  sb.sem_op = -1;
  sb.sem_flg = 0;
  semop(sem_id, &sb, 1);
}

void semUp(int sem_id){
  struct sembuf sb;
  sb.sem_num = (playerNum+1) % MAX_PLAYERS;
  sb.sem_op = 1;
  sb.sem_flg = 0;
  semop(sem_id, &sb, 1);
}

static void sighandler(int signo){
  if(signo == SIGINT){
    printf("\nYou quit the game!\n");
    exit(0);
  }
}

int main(){
  signal(SIGINT, sighandler);

  int shm_id = shmget(SHMKEY, 1024, 0666);
  if(shm_id < 0){
    perror("shmget failed in player main\n");
    exit(1);
  }
  int* shmp = (int*)shmat(shm_id, NULL, 0);
  if(shmp == (int*)-1){
    perror("shmat failed in player main\n");
    exit(1);
  }

  //allocating memory pointers here
  int* playerNumP;
  int* prevNum;
  int* currPlayer;
  playerNumP = shmp;
  prevNum = shmp+1;
  currPlayer = shmp+2; // put the current player in shared memory; this will go in a loop somewhere.
  
  // player's number is put into shared memory. Also creates another variable that doesn't change/is unique to this person.
  *playerNumP+=1;
  playerNum = *playerNumP;

  // telling player how many more players are required.
  while(*playerNumP < MAX_PLAYERS || *playerNumP > MAX_PLAYERS){
    printf("You need %d more players!\n", MAX_PLAYERS - *playerNumP);
    sleep(5);
  }

  printf("Your player number is: %d\n", playerNum);

  int sem_id = semget(SEMKEY, 1, 0666);
  if(sem_id == -1){
    perror("semget failed in player main\n");
    exit(1);
  }

while(1){
    semDown(sem_id);

    if(*currPlayer == playerNum-1){ //because playerNumbers start indexing at 1

      printf("It's your turn!\n");

      if(*prevNum != 0){
        printf("previous player's line: %d\n", *prevNum);
      }

      char buffer[1024];
      if(fgets(buffer, sizeof(buffer), stdin) != NULL){
        printf("Player string: %s\n", buffer);
      }
      else{
      printf("Error reading input\n");
      }

      int value = atoi(buffer);
      printf("testing to make sure it convers to integer: %d\n", value);

      *prevNum = value;

      *currPlayer = (*currPlayer + 1) % MAX_PLAYERS;

      semUp(sem_id);
    }
  }

  return 0;
}
