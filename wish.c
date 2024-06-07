/*
 * author    Matthew Markfort <matthew.markfort@my.metrostate.edu>
 * class     ICS-462 Operating Systems (Summer 2024), Metropolitan State University
 * date      28 May 2024
 * due       22 Jul 2024
 *
 * "wisconsin shell" (wish) is a rudimentary Linux shell for learning purposes
 *
 */

// built-in commands: exit, cd, and path
// - path overwrites PATH
// default PATH=/bin
// redirection to file also includes stderr
// allow background processes (&)

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<unistd.h>

#define MAX_PATH_LENGTH 256
#define MAX_PATH_COUNT 16
/* whitespace tokens */
#define DELIMITERS " \t\r\n"

static
char paths[MAX_PATH_COUNT][MAX_PATH_LENGTH] = {
  "/bin"
};

static
int paths_count = 1;

int wish_path(char *line){
  char *token = NULL;
  unsigned int count = 0;
  while((token = strtok(NULL, DELIMITERS))){
    size_t length = strlen(token);
    strncpy(paths[count], token, length);
    // ensure NULL terminated
    paths[count][length] = '\0';
    count++;
  }
  paths_count = count;
  return count;
}

void wish_resolve(char *command, char *resolved){
  char path[256];
  memset(path, 0, sizeof(path));
  for(size_t index = 0; index < paths_count; index++){
    strcat(path, paths[index]);
    strcat(path, "/");
    strcat(path, command);
    if(access(path, X_OK)){
      // if the executable exists, then exit iteration
      strncpy(resolved, path, strlen(path));
    }
    // reset the buffer
    memset(path, 0, sizeof(path));
  }
}

void wish_mkargs(char *args){
  char *token = NULL;
  size_t length = 0;
  while((token = strtok(NULL, DELIMITERS))){
    strcat(args, token);
    strcat(args, " ");
  }
  // remove blank from end
  length = strlen(args);
  args[length - 1] = '\0';
}

int wish_launch(char *bin, char *args){
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(bin, &args) == -1) {
      perror("wish");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("wish");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

void wish_execute(FILE *in){
  char *line = NULL;
  size_t linelen = 0;
  ssize_t nbytes = 0;
  while(!feof(in)){
    char *token;
    if(in == stdin)
      printf("wish> ");
    nbytes = getline(&line, &linelen, in);

    // exit while if no bytes read
    if(0 == nbytes) break;
    // skip on comment lines
    if('#' == line[0]) continue;
    // exit loop on exit
    if(strcmp(line, "exit\n") == 0) break;

    token = strtok(line, DELIMITERS);
    if(strcmp(token, "cd") == 0) {
      // handle cd (change directory) command
      token = strtok(NULL, DELIMITERS);
      chdir(token);
    } else if(strcmp(token, "path") == 0){
      // handle path command
      wish_path(line);
    } else {
      // handle exterior commands
      char bin[256], args[256];
      memset(bin, 0, sizeof(bin));
      memset(args, 0, sizeof(args));
      wish_resolve(token, bin);
      wish_mkargs(args);
      if(strlen(bin) > 0 && strlen(args) > 0){
	wish_launch(bin, args);
      }
    }
  }
  /* release the line buffer */
  if(line) free(line);
}

int main(int argc, char *argv[]){
  if(2 == argc){
    FILE *in = fopen(argv[1], "r");
    wish_execute(in);
    fclose(in);
  } else if(1 == argc){
    wish_execute(stdin);
  } else {
    fprintf(stderr, "Usage: %s [script.sh]\n", argv[0]);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
