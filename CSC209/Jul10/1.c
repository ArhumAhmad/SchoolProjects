#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(){
    int pid, status;
    char *buf = malloc(100);
    fgets(buf, sizeof buf, stdin);

    fflush(stdout);
    switch((pid = fork())){
        case -1:
            perror("fork");
            break;
        case 0: 
            execl("/bin/sh", "sh", "-c", buf, (char *)NULL);
            break;
        default:
            pid = wait(&status);
            break;
    }

    printf("%d\n", pid);

    return 0;
}