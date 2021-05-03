#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void process(FILE *fp)
{
    int c;
    int counter = 1;
    int prev;
    int started = 0;
    while ((c = getc(fp)) != EOF) {
        if(started == 0){
            started = 1;
            prev = c;
            putchar(c);
        }else if(c == prev && counter < 10){
            counter++;
            putchar(c);
        }else if(c == prev && counter >= 10){
            counter++;
        }else if(c != prev && counter > 10){
            printf("[%d more %d bytes]", counter-10, prev);
            prev = c;
            counter = 1;
            putchar(c);
        }else{
            prev = c;
            counter = 1;
            putchar(c);
        }
    }
    fseek(fp, 0, SEEK_SET);
}


int main(int argc, char **argv)
{
    int i;
    FILE *fp;
    extern void process(FILE *fp);

    if (argc == 1) {
        process(stdin);
    } else {
        for (i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-") == 0) {
                process(stdin);
            } else {
                if ((fp = fopen(argv[i], "r")) == NULL) {
                    perror("Error: ");
                    return(1);
                }
                process(fp);
                fclose(fp);
            }
        }
    }
    return(0);
}
