#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void replace(char *str, int isSTDIN){
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

        if(isSTDIN == 1){ printf("%s\n", nstr); }
        else{ printf("%s", nstr); }
        
        free(nstr);
}

int getSTD(char **string){
        int size = 2;
        int index = 0;
        char *ch = malloc(sizeof(char));

        //for some reason, these 2 lines solved a lot of problems
        free(ch);
        ch = malloc(sizeof(char));

        while((*ch = getchar()) != '\n'){
                if(*ch == EOF){ 
                        free(ch);
                        return 1; 
                }

                *(*string + index) = *ch;
                size++;
                *string = (char *) realloc(*string, size);
                index ++;
        }
        *(*string + index) = '\0';
        free(ch);

        return 0;
}

void doSTD(){
        char *string = malloc(2);
        while(getSTD(&string) == 0){
                replace(string, 1);
                free(string);
                string = malloc(2);
        }
        free(string);
}


int main(int argc, char **argv){
        if(argc == 1){
                doSTD();
        }else{
                FILE *f;
                char *content;
                int size;
                int comp;

                for(int i = 1; i < argc; i++){
                        comp = strcmp(argv[i], "-");
                        if(comp == 0){ 
                                printf("%d\n", comp);
                                doSTD(); 
                                continue; 
                        }

                        //Open file
                        f = fopen(argv[i], "r");
                        if(f == NULL){
                                printf("usage: derep file...\n");
                                return 1;
                        }

                        //Get size by seeking to the end and getting position
                        fseek(f, 0, SEEK_END);
                        size = ftell(f);
                        rewind(f);

                        //reads file into buffer
                        content = malloc(size+1);
                        
                        //for some reason, these 2 lines solved a lot of issues
                        free(content);
                        content = malloc(size+1);

                        fread(content, sizeof(char), size, f);
                        *(content+size) = '\0';

                        replace(content, 0);

                        free(content);
                        fclose(f);
                }
        }

        return 0;
}