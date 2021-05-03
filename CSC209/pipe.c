#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    int fd[2], f, result;

    pipe(fd);

    result = fork();

    if(result > 0){
        f = open("file1.txt", O_RDONLY);

        if(dup2(f, STDIN_FILENO) == -1){
            perror("dup2");
            exit(1);
        }

        if(dup2(fd[1], STDOUT_FILENO) == -1){
            perror("dup2");
            exit(1);
        }

        if(close(fd[0]) == -1){
            perror("close");
        }

        if(close(fd[1]) == -1){
            perror("close");
        }

        if(close(f) == -1){
            perror("close");
        }
        execl("/bin/cat", "cat", (char *) 0);
    }
    else if(result == 0){
        if(dup2(fd[0], STDIN_FILENO) == -1){
            perror("dup2");
            exit(1);
        }

        if(close(fd[1]) == -1){
            perror("close");
        }

        if(close(fd[0]) == -1){
            perror("close");
        }

        execl("/bin/cat", "cat", (char *) 0);
    }
    else{
        perror("fork");
        return -1;
    }
    return 0;
}