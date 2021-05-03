#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define STRINGSIZE 10  /* including the terminating \0 */

int verbose = 0;

struct message {
    int key;
    char value[STRINGSIZE];
};

extern int benode(int fromparent, int toparent, int key, char *value);
extern void parse(char *s, struct message *msgp);
extern void formatbuf(char *buf, int key, char *value);
extern void mystrncpy(char *s, char *t);


int toleft[2], fromleft[2], toright[2], fromright[2];
int lpid = 0;
int rpid = 0;
struct message msg;

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


int benode(int fromparent, int toparent, int key, char *valuearg)
{
    char buf[STRINGSIZE + 1];

    while (1) {
        if (fromparent < 0) {
            if (fgets(buf, sizeof buf, stdin) == NULL) {
                //shutdown
            }
            parse(buf, &msg);

            if(atoi(buf) <= key){

                if(lpid == 0){

                    if(pipe(toleft) == -1){
                        perror("pipe");
                        exit(1);
                    }

                    if(pipe(fromleft) == -1){
                        perror("pipe");
                        exit(1);
                    }

                    lpid = fork();

                    if(lpid == 0){
                        /*Child */
                        close(fromleft[0]);
                        close(toleft[1]);

                        read(toleft[1], buf, sizeof buf);
                        parse(buf, &msg);
                        benode(toleft[1], fromleft[0], msg.key, msg.value);

                    }else if(lpid < 0){
                        perror("fork");
                        exit(1);
                    }
                    else{
                        /*Parent */
                        close(fromleft[1]);
                        close(toleft[0]);

                        write(toleft[0], buf, sizeof buf);

                    }

                }
                else{
                    printf("%d\n", (int) write(toleft[0], buf, sizeof buf));
                }

            }else{

                if(rpid == 0){

                    if(pipe(toright) == -1){
                        perror("pipe");
                        exit(1);
                    }

                    if(pipe(fromright) == -1){
                        perror("pipe");
                        exit(1);
                    }

                    rpid = fork();

                    if(rpid == 0){
                        /*Child */
                        close(fromright[0]);
                        close(toright[1]);

                        read(toright[1], buf, sizeof buf);
                        parse(buf, &msg);
                        benode(toright[1], fromright[0], msg.key, msg.value);

                    }else if(lpid < 0){
                        perror("fork");
                        exit(1);
                    }
                    else{
                        /*Parent */
                        close(fromright[1]);
                        close(toright[0]);

                        write(toright[0], buf, sizeof buf);

                    }

                }
                else{
                    write(toright[0], buf, sizeof buf);
                }

            }
            
        } else {
            //... read ...
            //printf("%s\n", msg.value);
            return 0;
            if(read(fromparent, buf, sizeof buf) > 0){
                printf("%s\n", buf);
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