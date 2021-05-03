#include<stdio.h>
#include<stdlib.h>
#include<util.h>
#include<unistd.h>
#include<string.h>

int main(){
    char input[100];
    fgets(input, 99, stdin);

    int r = fork();

    if(r == 0){
        execlp(input, input, "sh -c", NULL);
        perror("Yo what the fuck");
        exit(1);
    }

    int status;
    wait(&status);

    printf("%d\n", status);


    exit(0);
}