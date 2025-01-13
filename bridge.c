#include "bridge.h"

// We will store a semaphore in shared memory so that
// it can be accessed by parent and children processes.
/*typedef struct {
    sem_t start_sem;   // Semaphore to signal that game can start
} shared_data_t;

int main(void)
{
    int i;
    int pipes[NUM_PLAYERS][2]; // Each child will write to parent through its own pipe
    pid_t pids[NUM_PLAYERS];

    // Create shared memory for semaphore
    shared_data_t *shared_data = mmap(NULL, sizeof(shared_data_t),
                                      PROT_READ | PROT_WRITE,
                                      MAP_SHARED | MAP_ANONYMOUS,
                                      -1, 0);
    if (shared_data == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Initialize the semaphore in shared memory
    // pshared=1 means this semaphore is shared between processes
    if (sem_init(&(shared_data->start_sem), 1, 0) != 0) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    // Create pipes for each player
    for (i = 0; i < NUM_PLAYERS; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Fork child processes (players)
    for (i = 0; i < NUM_PLAYERS; i++) {
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pids[i] == 0) {
            // Child process (player i)

            // Close the read end; child only writes to the pipe
            close(pipes[i][0]);

            // Simulate "joining" the game by sending a message to the parent
            // The content of the message is not very important; just sending a byte.
            char msg = 'R';  // "Ready"
            if (write(pipes[i][1], &msg, 1) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
            close(pipes[i][1]);

            // Now wait until the parent signals that all 4 players have joined
            sem_wait(&(shared_data->start_sem));

            // Once unblocked, all players have joined
            printf("Player %d: Starting the game!\n", i);
            fflush(stdout);

            // Child is done, exit
            exit(EXIT_SUCCESS);
        }
        else {
            // Parent continues the loop creating more children
        }
    }

    // Parent process:
    // Close all the write-ends in the parent, since parent only reads from these pipes
    for (i = 0; i < NUM_PLAYERS; i++) {
        close(pipes[i][1]);
    }

    int playersJoined = 0;

    // Read a "ready" byte from each child to confirm they have joined
    // as they come in. Each read corresponds to one player joined.
    for (i = 0; i < NUM_PLAYERS; i++) {
        char msg;
        if (read(pipes[i][0], &msg, 1) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        close(pipes[i][0]);  // Close this pipe after reading

        playersJoined++;
        int playersLeft = NUM_PLAYERS - playersJoined;
        printf("[Parent] Player %d joined. Players left to join: %d\n", i, playersLeft);
        fflush(stdout);
    }

    // All 4 players have joined at this point
    // Let each child proceed by posting (incrementing) the semaphore 4 times
    for (i = 0; i < NUM_PLAYERS; i++) {
        sem_post(&(shared_data->start_sem));
    }

    // Wait for all children to finish
    for (i = 0; i < NUM_PLAYERS; i++) {
        wait(NULL);
    }

    // Cleanup
    sem_destroy(&(shared_data->start_sem));
    munmap(shared_data, sizeof(shared_data_t));

    printf("[Parent] All players have started the game. Exiting.\n");
    return 0;
}*/

int main(){
}

int game(){
  return 0;
}

int jQueue(){
  return 0;
}

int playerRandomization(){
  return 0;
}

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

/*int main(int argc, char* argv[]){
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
}*/


