#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

#define LISTEN_BACKLOG 50
//gcc -lsocket -lnsl build flags for sunos

void error(char *msg){ perror(msg); exit(0);}
int make_socket (uint16_t port);
void client_mode();

int main(int argc, const char * argv[])
{
    if (argc < 4) {
        fprintf(stderr,"usage %s host_port remote_hostname remote_port\n", argv[0]);
        exit(0);
    }
    
    int server_portno = atoi(argv[1]);
    
    const char * name=argv[2];
    

    printf("starting server...\n");
    
    
    struct sharknado
    {
        int a;//4
        char b;//1
        float c;

    };
    
    struct sharknado s;
    
    
    //setup client
    int csockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    portno = atoi(argv[3]);
    csockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (csockfd < 0)
        error("ERROR opening socket (remote)");
    server = gethostbyname(argv[2]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    
    serv_addr.sin_port = htons(portno);
    
    //end setup client
    
  

//    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
//    if(!sockfd) error("socket");
//    
    struct sockaddr_in my_addr, peer_addr;
//    memset(&my_addr, 0, sizeof(struct sockaddr_in));
//    
//    if (bind(sockfd, (struct sockaddr *) &my_addr,
//             sizeof(struct sockaddr_in)) == -1)
//        error("bind");
    
    int sockfd=make_socket(server_portno);
    
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
        //blocking read
        int n = read(newsockfd,&s,sizeof(s));
        if (n < 0) error("ERROR reading from socket");
        //printf("Here is the message: %s\n",buffer);
        
        printf("before:\ta=%d, b=%c, c=%f\n", s.a, s.b, s.c);
        
        //increment a
        s.a=s.a<<1;
        s.b=s.b+1;
        s.c=s.c+1;
        
        printf("after:\ta=%d, b=%c, c=%f\n", s.a, s.b, s.c);
        
        char b [50];
        char * message=sprintf(b,"a=%d, b=%c, d=%f", s.a, s.b, s.c);
        
        n = write(newsockfd,b,50);
        if (n < 0) error("ERROR writing to socket");
        
        //connect & write to 3rd party server
        if (connect(csockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
            error("ERROR connecting (remote server)");

        n = write(csockfd,(char *)&s, sizeof(s));
        if (n < 0)
            error("ERROR writing to csocket");
        
        printf("done");
    }
    
    return 0;
}

void client_mode(char * servers, int port)
{
    struct sharknado
    {
        int a;//4
        char b;//1
        float c;
        
    };
    
    struct sharknado s;
    int sockfd, portno, n;
    
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    char buffer[256];
    portno = port;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(servers);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    
    //    printf("Please enter the message: ");
    //    bzero(buffer,256);
    //    fgets(buffer,255,stdin);
    
    //The only difference between send() and write() is the presence of flags. With zero flags parameter, send() is equivalent to write()
    n = write(sockfd,(char *)&s, sizeof(s));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0)
        error("ERROR reading from socket");
    printf("%s\n",buffer);
    return;

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
