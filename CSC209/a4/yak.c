#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include <netdb.h>
#include "peer.h"
#include "parsemessage.h"
#include "util.h"

#define DEFPORT 3000  /* (default both for connecting and for listening) */

unsigned long ipaddr = 0;  /* 0 means not known yet */
int myport = DEFPORT;
int relaymax = 10;
int verbose = 0;
struct peer *p, *nextp;
int soc;

int c;
fd_set fdlist;
int maxfd;


int main(int argc, char **argv)
{
    FD_ZERO(&fdlist);

    extern void doconnect(unsigned long ipaddr, int port);
    extern void makeserver(int port);
    extern unsigned long hostlookup(char *host);

    while ((c = getopt(argc, argv, "p:c:v")) != EOF) {
        switch (c) {
        case 'p':
            if ((myport = atoi(optarg)) == 0) {
                fprintf(stderr, "%s: port number must be a positive integer\n",
                        argv[0]);
                return(1);
            }
            break;
        case 'c':
            relaymax = atoi(optarg);
            break;
        case 'v':
            verbose = 1;
            break;
        default:
            fprintf(stderr, "usage: %s [-p port] [-c relaymax] [-v] [host [port]]\n", argv[0]);
            return(1);
        }
    }

    if (optind < argc) {
        optind++;
        doconnect(hostlookup(argv[optind - 1]),
                (optind < argc) ? atoi(argv[optind]) : DEFPORT);
    }else{
        makeserver(myport);
    }

    //...
}

void makeserver(int port){
    printf("got here\n");
    soc = socket(AF_INET, SOCK_STREAM, 0);
    if (soc < 0){
        perror("socket");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(addr.sin_zero), 0, 8);

    if(bind(soc, (struct sockaddr *)&addr, sizeof(struct  sockaddr_in)) < 0){
        perror("bind");
        close(soc);
        exit(1);
    }

    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    unsigned int client_len = sizeof(struct sockaddr_in);

    if(listen(soc, relaymax) < 0){
        perror("listen");
        exit(1);
    }

    int r = accept(soc, (struct sockaddr *)&client_addr, &client_len);
    if (r < 0){
        perror("accept");
        exit(1);
    }
    printf("%d\n", r);
}

//..
void doconnect(unsigned long ipaddr, int port){
    soc = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    memset(&server.sin_zero, 0, 8);
    server.sin_port = htons(port);
    server.sin_addr.s_addr = ipaddr;

    int r = connect(soc, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
    if(r < 0){
        perror("connect");
        exit(1);
    }
    printf("%d\n", r);

    p->ipaddr = ipaddr;
    p->port   = port;
    p->fd     = soc;
}


unsigned long hostlookup(char *host)
{
    struct hostent *hp;
    struct in_addr a;

    if ((hp = gethostbyname(host)) == NULL) {
        fprintf(stderr, "%s: no such host\n", host);
        exit(1);
    }
    if (hp->h_addr_list[0] == NULL || hp->h_addrtype != AF_INET) {
        fprintf(stderr, "%s: not an internet protocol host name\n", host);
        exit(1);
    }
    memcpy(&a, hp->h_addr_list[0], hp->h_length);
    return(ntohl(a.s_addr));
}


int analyze_banner(char *s, struct peer *p)
{
    unsigned long a, b, c, d, newipaddr;
    int numfields;
    int newport;

    numfields = sscanf(s, "%lu.%lu.%lu.%lu %d", &a, &b, &c, &d, &newport);
    if (numfields < 4) {
        fprintf(stderr, "'%s' does not begin with an IP address\n", s);
        return(-1);
    }

    newipaddr = (a << 24) | (b << 16) | (c << 8) | d;
    if (ipaddr == 0) {
        ipaddr = newipaddr;
        printf("I've learned that my IP address is %s\n",
                format_ipaddr(ipaddr));
    } else if (ipaddr != newipaddr) {
        fprintf(stderr,
"fatal error: I thought my IP address was %s, but newcomer says it's %s\n",
                format_ipaddr(ipaddr), s);
        exit(1);
    }

    if (numfields > 4) {
        if (p->port == 0) {
            struct peer *q = find_peer(p->ipaddr, newport);
            if (q == NULL) {
                p->port = newport;
                printf(
"I've learned that the peer on fd %d's port number is %d\n",
                        p->fd, p->port);
            } else {
                printf(
"fd %d's port number is %d, so it's a duplicate of fd %d, so I'm dropping it.\n",
                        p->fd, newport, q->fd);
                delete_peer(p);
            }
        } else if (p->port != newport) {
            printf(
"I'm a bit concerned because I thought the peer on fd %d's port number was %d, but it says it's %d\n",
                    p->fd, p->port, newport);
        }
    }

    return(0);
}
