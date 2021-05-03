#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

void work(int n, int limit, int *counter, int s, char* path){
    DIR *dr;
    DIR *sub;
    struct dirent *dp;

    struct stat file;
    int mtime;
    int ctime;

    dr = opendir(path);

    if(dr == NULL){
        perror("Not a directory");
        exit(1);
    }

    while ((dp = readdir(dr)) != NULL){
        sub = opendir(dp->d_name);

        //if these conditions are true, we exit the program since we've already hit the limit
        if(n == 2 && *counter == limit){ exit(2); }

        //Null implies file, not null implies folder
        if(sub == NULL){
            if(stat(dp->d_name, &file)){
                perror("Invalid argument (file)");
                exit(1);
            }
            mtime = file.st_mtime;
            ctime = file.st_ctime;

            if(mtime != ctime){
                if((s == 1 && mtime < ctime) || (s == 0)){
                    printf("%s\n", dp->d_name);
                    if(n == 2 && *counter == limit){ counter++; }
                }
            }
        }else if ((strcmp(dp->d_name, ".") != 0)){
            work(n, limit, counter, s, dp->d_name);
        }
    }

    closedir(dr);
}

int main(int argc, char *argv[]){
    if(argc == 1){
        perror("usage: ./funnytimes [-s] [-n limit] dir ...\n");
        return 1;
    }

    int opt;
    int n = 0;
    int limit;
    int s = 0;
    while((opt = getopt(argc, argv, "sn:")) != -1){
        switch (opt){
        case 's':
            s = 1;
            printf("s\n");
            break;
        case 'n':
            n = 1;
            limit = atoi(optarg);
            printf("%d\n", limit);
            break;
        default:
            perror("usage: ./funnytimes [-s] [-n limit] dir ...\n");
            return 1;
            break;
        }
    }

    int *counter = malloc(sizeof(int));
    *counter = 0;
    char *path = argv[argc-1];
    printf("%s\n", path);

    work(n, limit, counter, s, path);

    return n;
}