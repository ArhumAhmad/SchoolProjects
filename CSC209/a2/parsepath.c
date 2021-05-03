#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parsepath(char *path, char **result, int size){
    int plen = strlen(path) + 1;
    char *loc = malloc(sizeof(char) * plen);
    int locI = 0; //index of loc
    int resIndex = 0;

    for(int i = 0; i < plen; i++){
        if(path[i] != ':' && path[i] != '\0'){
            *(loc + locI) = path[i];
            locI++;
        }else{
            if(resIndex == size){ return -1; }

            *(loc + locI) = '\0';
            if(locI == 0){ *(loc) = '.'; *(loc+1) = '\0'; }

            result[resIndex] = malloc(locI+1);
            strcpy(result[resIndex], loc);
            resIndex++;

            free(loc);
            loc = malloc(sizeof(char) * plen);
            locI = 0;
        }
    }
    free(loc);

    return resIndex;
}