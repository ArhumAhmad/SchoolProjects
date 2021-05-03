#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
    int pid, status;
    extern void docommand();

    printf("Executing 'ls -c | tr e f'\n");

    fflush(stdout);
    switch((pid = fork())){
        case -1: 
            perror("fork");
            break;
        case 0:
            docommand();
            break;
        default: 
            pid = wait(&status);
            printf("child exit status was %d\n", WEXITSTATUS(status));
    }

    return 0;
}

void docommand(){
    int pipefd[2];

    if(pipe(pipefd)){
        perror("pipe");
        exit(127);
    }

    switch(fork()){
        case -1: 
            perror("fork");
            exit(127);
        case 0: 
            //child
            //do redirections
            close(pipefd[0]);
            dup2(pipefd[1], 1);
            close(pipefd[1]);
            execl("/bin/ls", "ls", "-C", (char *)NULL);
            perror("/bin/ls");
            exit(127);
        default:
            close(pipefd[1]);
            dup2(pipefd[0], 0);
            close((pipefd[0]));
            execl("/usr/bin/tr", "tr", "e", "f", (char *)NULL);
            perror("/usr/bin/tr");
            exit(127);
    }
}