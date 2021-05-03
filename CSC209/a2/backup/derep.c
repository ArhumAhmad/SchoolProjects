#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void replace(char *str){
        int size = strlen(str);
        char *nstr = malloc(size * 2);
        char *index = nstr;
        int counter = 0;
        char message[100];

        if(size==0 || size == 1){
                printf("%s\n", str);
                free(nstr);
                return;
        }

        *index = *str;
        index += sizeof(char);
        for(int i = 1; i < size; i++){
                if(*(str+i) == *(str+i-1)){
                        if(counter == 0){ counter += 2; }
                        else{ counter++; }
                }else{
                        counter = 0;
                }

                *index = *(str+i);
                index += sizeof(char);

                if(counter == 10){
                        counter = 0;
                        int k = i+1;
                        while(*(str+k) == *(str+i)){
                                counter++;
                                k++;
                        }

                        sprintf(message,"[%d more %d bytes]", counter, (int) *(str+i));
                        if(counter > 0){ sprintf(index, "[%d more %d bytes]", counter, (int) *(str+i)); }
                        counter = 0;
                        i = k-1;
                        index += strlen(message);
                        //message[0] = '\0';
                }
        }

        printf("%s", nstr);
        
        free(nstr);
}

void process(FILE *fp)
{
    int c;
    int size = 2;
    char *string = malloc(size);
    int index = 0;
    while ((c = getc(fp)) != EOF) {
        *(string + index) = (char)c;
        size++;
        string = (char *) realloc(string, size);
        index++;

        if(c == '\n'){
                *(string + index) = '\0';
                replace(string);
                free(string);

                size = 2;
                string = malloc(size);
                index = 0;
        }
    }

    //*(string + index) = '\0';

    //replace(string);
    free(string);
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
                    perror(argv[i]);
                    return(1);
                }
                process(fp);
                fclose(fp);
            }
        }
    }
    return(0);
}
