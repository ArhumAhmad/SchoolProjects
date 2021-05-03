#include <stdio.h>
#include "mysh.h"
#include "parse.h"
#include "execute.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


/* returns 0 if the string has a slash, 1 else */
int hasSLASH(char *str){
    int i = 0;
    while(str[i] != '\0'){
        if(str[i] == '/')
            return 0;
        i++;
    }
    return 1;
}

int pathIndex(char *command, char **path){
    struct stat buf;
    char *f;
    int i = 0;
    while(path[i] != NULL){;
        f = malloc(strlen(command) + strlen(path[i]) + 2);
        strcpy(f, path[i]);
        strcat(f, "/");
        strcat(f, command);

        if(stat(f, &buf) == 0){
            return i;
        }
        
        free(f);
        i++;
    }

    return -1;
}

void execute(struct parsed_line *p, char **path, int pathsize)
{
    /*
    if (p->inputfile)
        printf("redirecting input from %s\n", p->inputfile);
    if (p->outputfile)
        printf("redirecting output to %s\n", p->outputfile);
    if (p->argv)
        printf("executing %s...\n", p->argv[0]);
    */
   char *command;

   int OIn  = dup(STDIN_FILENO);
   int OOut = dup(STDOUT_FILENO);

    extern char **environ;

   if(p->argv){
       if(hasSLASH(p->argv[0]) == 0){
           command = malloc(strlen(p->argv[0]) + 1);
           strcpy(command, p->argv[0]);
           struct stat buf;
           if(stat(command, &buf) != 0){
               fprintf(stderr, "%s: command not found\n", p->argv[0]);
               exit(1);
           }
       }
       else{
           
           int i = pathIndex(p->argv[0], path);
           if(i == -1){
               fprintf(stderr, "%s: command not found\n", p->argv[0]);
               exit(1);
           }
           command = malloc(strlen(path[i]) + strlen(p->argv[0]) + 2);
           strcpy(command, path[i]);
           strcat(command, "/");
           strcat(command, p->argv[0]);
           //printf("%s\n", command);
       }
   }else
   {
       return;
   }
   

   int In  = STDIN_FILENO;
   int Out = STDOUT_FILENO;

   fflush(stdout);

   if(p->inputfile != NULL){
       In = open(p->inputfile, O_RDONLY | O_CREAT | O_TRUNC, 0666);
       if(In < 0){
           fprintf(stderr, "%s: could not open file\n", p->inputfile);
           exit(1);
       }
       if(dup2(In, STDIN_FILENO) < 0){
           perror("dup2");
           exit(1);
       }
       //close(In);
   }

   if(p->outputfile != NULL){
       Out = open(p->outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
       if(Out < 0){
           fprintf(stderr, "%s: could not open file\n", p->outputfile);
           exit(1);
       }
        if(dup2(Out, STDOUT_FILENO) < 0){
            perror("dup2");
            exit(1);
        }
        //close(Out);
   }

   int pid = fork();

   if(pid == 0){
        execve(command, p->argv, environ);
        perror("execve");
        exit(1);
   }
   else if(pid < 0){
        perror("fork");
        exit(1);
   }
   else{
        int status;
        if(wait(&status) < 0){
            perror("wait");
            exit(1);
        }

        laststatus = status;

        dup2(OIn, 0);
        dup2(OOut, 1);
        close(OIn);
        close(OOut);

   }

}
