#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

static void die(const char *s) { perror(s); exit(1); }

int main(int argc, char **argv){
    if (argc != 2){
        fprintf(stderr, "usage: %s <server-port>\n", argv[0]);
        exit(1);
    }

    unsigned short port = atoi(argv[1]);

    // Create a listening socket (also called server socket) 
    int servsock;

    if ((servsock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        die("socket failed"); 

    // Construct local address strucutre

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port); 

    //Bind to the local address

    if (bind(servsock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        die("bind failed");

    //Start listening for incoming connections
    if (listen(servsock, 5)<0)
        die("listen failed");

    int clntsock;
    socklen_t clntlen;
    struct sockaddr_in clntaddr;

    while(1){
        // Accept an incoming connection

        fprintf(stderr, "waiting for client ... ");

        clntlen = sizeof(clntaddr); // initialize the in-out parameter

        if ((clntsock = accept(servsock,
                        (struct sockaddr *) &clntaddr, &clntlen)) < 0)
            die("accept failed");

        // accept() returned a connected socket (also called client socket)
        // and filled in the client's address into clntaddr

        fprintf(stderr, "client ip: %s\n", inet_ntoa(clntaddr.sin_addr));
        return 0; 
    }

    
}

