#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include <getopt.h>

void noopt(char x, char y){
    char i;
    while((i = getchar()) != EOF){
        if(i == x){
            putchar(y);
        }else{
            putchar(i);
        }
    }
}

void opt(char z){
    char i;
    while((i = getchar()) != EOF){
        if(i != z){
            putchar(i);
        }
    }
}

int main(int argc, char** argv){
    int hasoption = 0;

    int Opt;
    while((Opt = getopt(argc, argv, "d:")) != -1){
        switch(Opt)
        {
            case 'd':
                hasoption = 1;
                break;
            default:
                fprintf(stdout, "usage: %s [-d z] [x y]\n", argv[0]);
                exit(1);
        }
    }

    printf("%d\n", hasoption);

    if(hasoption){
        opt(argv[(int)optarg][0]);
    }else{
        noopt(argv[1][0], argv[2][0]);
    }

    return 0;
}