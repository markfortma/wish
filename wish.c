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
#include<unistd.h>

#define MAX_PATH_LENGTH 256
#define MAX_PATH_COUNT 16
/* whitespace tokens */
#define DELIMITERS " \t\r\n"

static
char paths[MAX_PATH_LENGTH][MAX_PATH_COUNT] = {
  "/bin"
};

static
int paths_count = 1;

int wish_chdir(char *path){
  if(access(path, F_OK)){
    return chdir(path);
  }
  return -1;
}

int wish_path(char *line){
  char *token = NULL;
  unsigned int count = 0;
  for(; (token = strsep(&line, DELIMITERS));){
    size_t length = strlen(token);
    strncpy(paths[count], token, length);
    // ensure NULL terminated
    paths[count][length] = '\0';
    count++;
  }
  paths_count = count;
  return count;
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
    if(strcmp("exit\n", line) == 0) break;

    token = strsep(&line, DELIMITERS);
    if(strcmp(token, "cd") == 0) {
      token = strsep(&line, DELIMITERS);
      wish_chdir(token);
    } else if(strcmp(token, "path") == 0){
      wish_path(line);
    }
  }
  /* release the line buffer */
  free(line);
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
