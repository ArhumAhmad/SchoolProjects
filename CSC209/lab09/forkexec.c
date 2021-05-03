#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void doit(int ind){
    exit((5*ind) + 4);
}

int main(){
    int sum = 0;
    int status;
    for(int i = 0; i < 5; i++){
        switch(fork()){
            case -1: 
                perror("fork");
                return -1;
            case 0:
                doit(i);
        }
    }

    for(int i = 0; i < 5; i++){
        wait(&status);
        sum += WEXITSTATUS(status);
    }

    printf("%d\n", sum);
    return 0;
}