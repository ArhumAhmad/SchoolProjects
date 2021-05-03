#include <stdio.h>
#include <unistd.h>

int main(){
    execl("/bin/ls", "ls", "~/", (char *)NULL);
    perror("/bin/ls");

    return 0;
}