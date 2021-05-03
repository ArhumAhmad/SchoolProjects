#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define STRINGSIZE 10  /* including the terminating \0 */

int verbose = 0;

int toleft[2], fromleft[2], toright[2], fromright[2];
int lpid = 0;
int rpid = 0;

struct message {
    int key;
    char value[STRINGSIZE];
};

extern int benode(int fromparent, int toparent, int key, char *value);
extern void parse(char *s, struct message *msgp);
extern void formatbuf(char *buf, int key, char *value);
extern void mystrncpy(char *s, char *t);


int main(int argc, char **argv)
{
    int c, status = 0;


    while ((c = getopt(argc, argv, "v")) != EOF) {
        switch (c) {
        case 'v':
            verbose = 1;
            break;
        default:
            status = 1;
        }
    }
    if (status || optind + 2 != argc) {
        fprintf(stderr, "usage: %s [-v] key value\n", argv[0]);
        return(1);
    }
    benode(-1, -1, atoi(argv[optind]), argv[optind+1]);
    return(0);
}

void makeChild(int from[2], int to[2], int *cpid, struct message *msg, char *buf){
    /* 
    if(pipe(to) == -1){
        perror("pipe");
        exit(1);
    }

    if(pipe(from) == -1){
        perror("pipe");
        exit(1);
    } */

    *cpid = fork();

    if(*cpid == 0){
        /*Child */
        close(from[0]);
        close(to[1]);

        read(to[1], buf, sizeof buf);
        parse(buf, msg);
        lpid = 0;
        rpid = 0;
        //printf("%s\n", msg->value);
        benode(to[1], from[0], msg->key, msg->value);
    }else if(*cpid < 0){
        perror("fork");
        exit(1);
    }
    else{
        /*Parent */
        close(from[1]);
        close(to[0]);

        write(to[1], buf, sizeof buf);
    }
}

void send(char *buf, struct message *msg, int key){
    int bufsize;
    int bread;
    if((bufsize = atoi(buf)) <= key){

        if(lpid == 0){
            makeChild(fromleft, toleft, &lpid, msg, buf);
        }
        else{
            //write()
            bread = write(toleft[1], buf, sizeof buf);
            printf("left %d\n", bread);
        }
    }

    else if(bufsize > key){

        if(rpid == 0){
            makeChild(fromright, toright, &rpid, msg, buf);
        }
        else{
            bread = write(toright[1], buf, sizeof buf);
            printf("right %d\n", bread);
        }

    }

    printf("%d\t%d\n", lpid, rpid);
}

int benode(int fromparent, int toparent, int key, char *valuearg)
{
    char buf[STRINGSIZE + 1];
    struct message msg;

    pipe(fromleft);
    pipe(toleft);

    pipe(fromright);
    pipe(toright);

    while (1) {
        if (fromparent < 0) {
            if (fgets(buf, sizeof buf, stdin) == NULL) {
                //shutdown
            }
            parse(buf, &msg);

            send(buf, &msg, key);

            
        } else {
            //... read ...
            //return 0;
            if(read(fromparent, buf, sizeof buf) > 0){
                send(buf, &msg, key);
                printf("hi");
            }
            
        }




    }
}


/* Turn user input into a struct message. */
void parse(char *s, struct message *msgp)
{
    char *p;

    msgp->key = atoi(s);

    if ((p = strchr(s, ' ')) == NULL) {
        msgp->value[0] = '\0';
    } else {
        while (*p && isspace(*p))
            p++;
        mystrncpy(msgp->value, p);
        if ((p = strchr(msgp->value, '\n')))
            *p = '\0';
    }
}


void mystrncpy(char *s, char *t)
{
    strncpy(s, t, STRINGSIZE);
    s[STRINGSIZE - 1] = '\0';
}