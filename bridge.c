#include "bridge.h"

int game(){
  return 0;
}

int jQueue(){
  return 0;
}

int playerRandomization(){
  return 0;
}


#define SEMKEY 637691
#define SHAREDKEY 653237

/*union semun {
  int val;                  //used for SETVAL
  struct semid_ds *buf;     //used for IPC_STAT and IPC_SET
  unsigned short  *array;   //used for SETALL
  struct seminfo  *__buf;
};*/

/*struct sembuf {
  short sem_op;
  short sem_num;
  short sem_flag;
};*/ // I don't need this since it's defined on mac already globally, but maybe put this back for linux machines or something... I'm no expert

// make sure the error messages are more serious

int main(int argc, char* argv[]){
  if(strcmp(argv[1], "create") == 0){
    createStory();
  }
  else if(strcmp(argv[1], "remove") == 0){
    deleteStory();
  }
  else if(strcmp(argv[1], "view") == 0){
    viewStory();
  }
  else{
    perror("Something is wrong with the arguments mannnn");
    exit(1);
  }
  return 0;
}

void createStory(){
  int shm_id = shmget(SHAREDKEY, sizeof(int), IPC_CREAT | 0666);
  if(shm_id < 0){
    perror("something is WRONG with shmget");
    exit(1);
  }

  int *shmp = (int*)shmat(shm_id, NULL, 0);
  if(shmp == (int*)-1){
    perror("now something is wrong with shmat? we need to calm down here");
    exit(1);
  }
  *shmp = 0;

  int sem_id = semget(SEMKEY, 1, IPC_CREAT | 0666);
  if(sem_id < 0){
    perror("alright fam, if semget fails now, that's juust like sad fr fr");
    exit(1);
  }
  semctl(sem_id, 0, SETVAL, 1);

  int fd = open("story.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd < 0){
    perror("you are never seeing this story bud");
    exit(1);
  }

  close(fd);

}

void deleteStory(){
  int shm_id = shmget(SHAREDKEY, sizeof(int), 0666);
  if(shm_id < 0){
    perror("yep shmget failure that's shmtastic");
    exit(1);
  }

  int* shmp = (int*)shmat(shm_id, NULL, 0);
  if(shmp == (int*)-1){
    perror("shmp failure just like me fr fr");
    exit(1);
  }

  shmctl(shm_id, IPC_RMID, NULL);
  int sem_id = semget(SEMKEY, 1, 0666);
  if(sem_id < 0){
    perror("semget failure now great blud thinks he's going to cornell with this one");
    exit(1);
  }
  semctl(sem_id, 0, IPC_RMID);

  viewStory();

  remove("story.txt");
  
}

void viewStory(){
  FILE *file = fopen("story.txt", "r");
  if(file == NULL){
    perror("fopen failed, sad");
    exit(1);
  }

  char line[256];
  while(fgets(line, sizeof(line), file)){
    printf("%s", line);
  }
  fclose(file);

}


#define FIFO_NAME "tempfifo"

void alterMessage(int childId, char *input, char *output) {
  if(childId == 0){
    int i;
    for (i = 0; input[i] != '\0'; i++) {
      if ('A' <= input[i] && input[i] <= 'Z')
        output[i] = ((input[i] - 'A' + 13) % 26) + 'A';
      else if ('a' <= input[i] && input[i] <= 'z')
        output[i] = ((input[i] - 'a' + 13) % 26) + 'a';
      else
        output[i] = input[i];
    }
    output[i] = '\0';
  }
  else if(childId == 1){
    int len = strlen(input);
    for(int i = 0; i < len; i++){
      output[i] = input[len-i-1];
    }
    output[len] = '\0';
  }
  else if(childId == 2){
    int len = strlen(input);
    for(int i = 0; i < len; i++){
      output[i] = toupper(input[i]);
    }
    output[len] = '\0';
  }
  else if(childId == 3){
    int len = strlen(input);
    for(int i = 0; i < len; i++){
      output[i] = tolower(input[i]);
    }
    output[len] = '\0';
  }
  else{
    perror("something is wrong with this childId thing");
    exit(1);
  }
}

int random_int(int min, int max) {
  int dev = open("/dev/urandom", O_RDONLY);
  if (dev < 0) {
      perror("Failed to open /dev/urandom");
      exit(1);
  }

  unsigned int random;
  read(dev, &random, sizeof(random));
  close(dev);

  return min+(random%(max-min+1));
}

void handle_sigint(int sig) {
    unlink(FIFO_NAME);
    exit(0);
}

int main() {
    int pipes[4][2];
    pid_t children[4];
    char input[256];
    char output[256];
    int childNum, fifo_fd;

    signal(SIGINT, handle_sigint);

    childNum = random_int(2, 4);
    printf("Spawning \e[35m%d\e[0m worker processes.\n", childNum);

    if (mkfifo(FIFO_NAME, 0666) == -1) {
        perror("so the pipe doesn't wanna open");
        exit(1);
    }

    for (int i = 0; i < childNum; i++) {
      if (pipe(pipes[i]) == -1) {
        perror("so the pipe doesn't wanna be MADE");
        exit(1);
      }

      children[i] = fork();
      if (children[i] == 0) {
          close(pipes[i][1]);

          fifo_fd = open(FIFO_NAME, O_WRONLY);
          if (fifo_fd == -1) {
            perror("Child failed to open FIFO");
            exit(1);
          }

          while (1) {
            ssize_t bytes = read(pipes[i][0], input, 256);
            if (bytes > 0) {
              input[bytes] = '\0';

              int sleep_time = random_int(0, 4000000);
              printf("(\e[36m%d\e[0m) sleeping for \e[35m%.6f\e[0m seconds\n", getpid(), sleep_time/1000000.0);
              usleep(sleep_time);

              alterMessage(i, input, output);

              char result[256];
              snprintf(result, 256, "pid(\e[36m%d\e[0m): %s\n", getpid(), output);
              write(fifo_fd, result, strlen(result) + 1);
            }
          }

          close(pipes[i][0]);
          close(fifo_fd);
          exit(0);
      }
      close(pipes[i][0]);
    }

    fifo_fd = open(FIFO_NAME, O_RDONLY);
    if (fifo_fd == -1) {
        perror("Parent failed to open FIFO");
        exit(1);
    }

    while (1) {
      printf("User Control Program (\e[36m%d\e[0m), enter a line: ", getpid());
      if (fgets(input, 256, stdin) == NULL) {
        break;
      }

      input[strcspn(input, "\n")] = '\0';

      for (int i = 0; i < childNum; i++) {
        write(pipes[i][1], input, strlen(input) + 1);
      }

      for (int i = 0; i < childNum; i++) {
        read(fifo_fd, output, 256);
        printf("result %d: %s", i, output);
      }
    }

    close(fifo_fd);
    remove(FIFO_NAME);

    for (int i = 0; i < childNum; i++) {
      close(pipes[i][1]);
    }

    return 0;
}


