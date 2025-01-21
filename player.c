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

int* playerNumP;
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

int cardValue(char* card){
  if(strlen(card) != 2){
    printf("Please input a valid card next time\n");
    return 0;
  }
  if(card[1] == '1'){
    printf("Dawg, this is an invalid card\n");
  }
  if(card[1] == 'A'){
    return 10000;
  }
  int value = 0;
  value += 10 * card[0];
  if(card[1] == 'J'){
    value += 11;
  }
  else if(card[1] == 'K'){
    value += 12;
  }
  else if(card[1] == 'Q'){
    value += 13;
  }
  else{
    value += card[1] - '0';
  }
  return value;
}

static void sighandler(int signo){
  if(signo == SIGINT){
    printf("\nYou quit the game!\n");
    *playerNumP-=1;
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

  // pointer here for what the previous card was
  int* prevCard;
  prevCard = shmp+3;

  FILE *file = fopen("gameProgress.txt", "a+");
  if(file == NULL){
    perror("fopen failed in player main\n");
    exit(1);
  }

  //allocating memory pointers here
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
  //int sem_id = semget(SEMKEY, 0, 0);
  if(sem_id == -1){
    perror("semget failed in player main\n");
    exit(1);
  }

  while(*playerNumP == MAX_PLAYERS){
    semDown(sem_id);

    if(*currPlayer == playerNum-1){ //because playerNumbers start indexing at 1

      printf("It's your turn!\n");

      // opening the file
      FILE *file = fopen("gameProgress.txt", "a+");
      if(file == NULL){
        perror("fopen failed in player main\n");
        exit(1);
      }

      int size = *prevCard;

      printf("value of size: %d\n", size);

      fseek(file, size-3, SEEK_SET);
      char lastCard[256];
      if(size > 0){
        fgets(lastCard, sizeof(lastCard), file);
        printf("previous player's card: %s\n", lastCard);
      }
      else{
        printf("You are the first person to place a card\n");
      }


      char newCard[1024];
      if(fgets(newCard, sizeof(newCard), stdin) != NULL){
        printf("Player string: %s\n", newCard);
      }
      else{
      printf("Error reading input\n");
      }

      fputs(newCard, file);
      *prevCard = ftell(file);

      fclose(file);

      *currPlayer = (*currPlayer + 1) % MAX_PLAYERS;

      semUp(sem_id);
    }
    else{
      printf("It is player %d's turn! Hopefully, they don't take more than 5 seconds to make their turn...\n", *currPlayer + 1);
      sleep(5);
    }
  }

  printf("Someone either left or there's too many players! Start everything over >:(\n");

  return 0;
}
