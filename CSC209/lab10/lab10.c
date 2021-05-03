#include <stdio.h>

int main(int argc, char **argv)
{
    FILE *fp;
    char c;

    if (argc != 2){
        perror("usage: lab10 file");
        return 1;
    }

    fp = fopen(argv[1], "r");
    if(fp == NULL){
        fprintf(stderr, "fopen %s\n", argv[1]);
        return 1;
    }

    while ((c = getc(fp)) != EOF){
        putchar(c);
    }

    if(fclose(fp) != 0){
        fprintf(stderr, "fclose %s\n", argv[1]);
        return 1;
    }

    return(0);
}