#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>


int main(int argc, char **argv)
{

    if (argc != 3 || strlen(argv[2]) != 1) {
        fprintf(stderr, "usage: %s character\n", argv[0]);
        return(1);
    }

        pid_t pid = atoi(argv[1]);
        //printf("%d\n",pid);

        int c = argv[2][0];
        //printf("c is: %d\n",c);

    for (int i = 0; i < 8; i++) {

        if (c & 128){
            //printf("1\n");
                //send signal SIGUSR2
                //printf("Sending signal SIGUSR2\n");
                if ((kill(pid,SIGUSR2)) != 0){
                        perror("kill");

                }

                usleep(100000);
        }
        else{
           // printf("0\n");
                //SEND SIGNAL SIGUSR1
                //printf("Sending signal SIGUSR1\n");
                if( (kill(pid,SIGUSR1)) != 0){
                        perror("kill");
                }

                usleep(100000);
        }
        c <<= 1;
        }



    return(0);

}