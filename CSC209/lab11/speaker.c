#include <stdio.h>
#include "wordlist.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>


int main(){
    int fd;
    if((fd = open("/dev/urandom", O_RDONLY)) == -1){
        perror("open");
        exit(1);
    }

    int R;
    unsigned char r;

    for(int i = 0; i < 10; i++){
        if(read(fd, &r, 1) != 1){
            perror("read");
            exit(1);
        }
        R = r % wordlistsize;
        printf("%s\n", wordlist[R]);
        sleep(1);
    }

    return 0;
}