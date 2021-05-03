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


int benode(int fromparent, int toparent, int key, char *valuearg)
{
    struct message msg;
    struct message nmsg;

    char buf[STRINGSIZE + 1];

    pipe(toright);
    pipe(fromright);

    int rd = 0;

    int result;

    while (1) {
        if (fromparent < 0) {
            if (fgets(buf, sizeof buf, stdin) == NULL) {
                //shutdown
            }
            parse(buf, &msg);
            if(rd == 0){ rd = 1; }

            else{
                if(atoi(buf) <= key){
                    if(lpid == 0){
                        pipe(toleft);
                        pipe(fromleft);

                        
                    }
                }
            }
        } else {
            //... read ...
            read(fromparent, buf, sizeof buf);
            
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