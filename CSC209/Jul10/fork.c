#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    fork();
    fork();
    printf("%d\n", getpid());

    return 0;
}