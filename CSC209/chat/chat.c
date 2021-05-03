#include<stdio.h>
#include<stdlib.h>
#include<util.h>
#include<unistd.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<netdb.h>
#include<arpa/inet.h>

char *username;
int port;
char *hostname;
unsigned long ipaddr;
int sock;
char message[1024];
char null = '\0';

fd_set chat_fd;
int maxfd;

struct sockaddr_in server;

int main(int argc, char** argv){
    extern void client();
    extern void host();
    extern unsigned long hostlookup(char* host);

    username = malloc(sizeof(argv[1]) + sizeof(char));
    strcpy(username, argv[1]);

    int port = htons(atoi(argv[2]));

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("socket");
        exit(1);
    }


    server.sin_family = AF_INET;
    memset(&server.sin_zero, 0, 8);
    server.sin_port = port;

    if(argc != 3 && argc != 4){
        fprintf(stderr, "usage: %s [username] [port number] [hostname]", argv[0]);
        exit(1);
    }
    else if(argc == 3){
        host();
    }
    else{
        hostname = malloc(sizeof(argv[3]) + sizeof(char));
        strcpy(hostname, argv[3]);
        ipaddr = hostlookup(hostname);

        client();
    }


    return 0;
}

void client(){
    struct addrinfo *result;
    int addrinfo = getaddrinfo(hostname, NULL, NULL, &result);

    server.sin_addr = ((struct sockaddr_in *) result->ai_addr)->sin_addr;
    freeaddrinfo(result);

    int ret = connect(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in));
    if(ret < 0){
        perror("connect");
        exit(1);
    }
    else{
        printf("Connection succesful!\n");
    }

    FD_ZERO(&chat_fd);
    FD_SET(sock, &chat_fd);
    FD_SET(0, &chat_fd);

    maxfd = sock+1;

    int r;

    while(0 == 0){
        if(select(maxfd, &chat_fd, NULL, NULL, NULL) == -1){
            perror("select");
            exit(1);
        }

        if(FD_ISSET(sock, &chat_fd)){
            if((r = read(sock, message, 1024)) < 0){
                perror("read");
                exit(1);
            }else{
                strcat(message, &null);
                fprintf(stdout, "%s\n", message);
            }
        }

        if(FD_ISSET(0, &chat_fd)){
            fgets(message, sizeof(message) - 1, stdin);
            write(sock, message, sizeof(message));
        }
    }
}

void host(){
    int binding = bind(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
    if(binding < 0){
        perror("bind");
        close(sock);
        exit(1);
    }

    if(listen(sock, 1) < 0){
        perror("listen");
        exit(1);
    }

    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    unsigned int client_len = sizeof(struct sockaddr_in);

    printf("Waiting for connection.....\n");

    int ret = accept(sock, (struct sockaddr *)&client_addr, &client_len);
    if(ret < 0){
        perror("accept");
        close(sock);
        exit(1);
    }

    printf("Connection succesful!\n");

    FD_ZERO(&chat_fd);
    FD_SET(ret, &chat_fd);
    FD_SET(0, &chat_fd);

    maxfd = ret+1;

    int r;

    while(0 == 0){
        if(select(maxfd, &chat_fd, NULL, NULL, NULL) == -1){
            perror("select");
            exit(1);
        }

        if(FD_ISSET(ret, &chat_fd)){
            if((r = read(ret, message, 1024)) < 0){
                perror("read");
                exit(1);
            }else{
                strcat(message, &null);
                fprintf(stdout, "%s\n", message);
            }
        }

        if(FD_ISSET(0, &chat_fd)){
            fgets(message, sizeof(message) - 1, stdin);
            write(sock, message, sizeof(message));
        }
    }
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