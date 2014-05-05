#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define LISTEN_BACKLOG 50
//gcc -lsocket -lnsl build flags for sunos

void error(char *msg){ perror(msg); exit(0);}
int make_socket (uint16_t port);

int main(int argc, const char * argv[])
{
    printf("starting server...\n");
    
    
    struct sharknado
    {
        int a;//4
        char b;//1
        float c;

    };
    
    struct sharknado s;
 
  

//    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
//    if(!sockfd) error("socket");
//    
    struct sockaddr_in my_addr, peer_addr;
//    memset(&my_addr, 0, sizeof(struct sockaddr_in));
//    
//    if (bind(sockfd, (struct sockaddr *) &my_addr,
//             sizeof(struct sockaddr_in)) == -1)
//        error("bind");
    
    int sockfd=make_socket(8081);
    
    if (listen(sockfd, LISTEN_BACKLOG) == -1)
        error("listen");
    
    /* Now we can accept incoming connections one
     at a time using accept(2) */
    printf("starting listener...\n");
    for(;;)
    {
        socklen_t peer_addr_size;
        peer_addr_size = sizeof(struct sockaddr_in);
        int newsockfd = accept(sockfd, (struct sockaddr *) &peer_addr,
                     &peer_addr_size);
        if (newsockfd == -1)
            error("accept");
        
    
        char buffer[256];
        bzero(buffer,256);
        int n = read(newsockfd,&s,sizeof(s));
        if (n < 0) error("ERROR reading from socket");
        //printf("Here is the message: %s\n",buffer);
        
        //increment a
        s.a=s.a<<2;
        s.b=s.b+1;
        s.c=s.c+1;
        
        printf("new a=%d\n", s.a);
        printf("new b=%c\n", s.b);
        printf("new c=%f\n", s.c);
        
        char b [50];
        char * message=sprintf(b,"a=%d, b=%c, d=%f", s.a, s.b, s.c);
        
        n = write(newsockfd,b,50);
        if (n < 0) error("ERROR writing to socket");
    }
    
    return 0;
}

int
make_socket (uint16_t port)
{
    int sock;
    struct sockaddr_in name;
    
    /* Create the socket. */
    sock = socket (PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror ("socket");
        exit (EXIT_FAILURE);
    }
    
    /* Give the socket a name. */
    name.sin_family = AF_INET;
    name.sin_port = htons (port);
    name.sin_addr.s_addr = htonl (INADDR_ANY);
    if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
    {
        perror ("bind");
        exit (EXIT_FAILURE);
    }
    
    return sock;
}
