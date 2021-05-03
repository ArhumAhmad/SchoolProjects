#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

void doit(int ind){
    exit((5*ind) + 4);
}

void*addfive(void *arg){
    int j, sum = 0;
    int from = *(int *)arg;
    for(j = 0; j < 5; j++){
        sum += from + j;
    }
    *(int *)arg = sum;
    return NULL;
}

int main(){
    int sum = 0;
    int data[5];
    pthread_t tid[5];
    //int status;
    for(int i = 0; i < 5; i++){
        data[i] = i*5;
        pthread_create(&tid[i], NULL, addfive, &data[i]);
    }

    for(int i = sum = 0; i < 5; i++){
        pthread_join(tid[i], NULL);
        sum += data[i];
    }

    printf("%d\n", sum);
    return 0;
}