#include<stdio.h>
#include<stdlib.h>
#include<util.h>
#include<unistd.h>

int main(){
    execl("/bin/ls", "ls", "/Users/arhumahmad");


    perror("Yo what the fuck");
    exit(1);
}