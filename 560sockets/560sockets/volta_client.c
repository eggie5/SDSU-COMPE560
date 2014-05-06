#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void _error(char *msg){perror(msg);exit(0);}
float ReverseFloat( const float inFloat );
int32_t swap_int32( int32_t val );

int main(int argc, char *argv[])
{
    struct sharknado
    {
        int a;//4
        char b;//1
        float c;
        
    };
    
    struct sharknado s={0x0f000000,'a',ReverseFloat(2.0f)};
    
    
    
    
    int sockfd, portno, n;
    
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    char buffer[256];
    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        _error("ERROR opening socket");
    server = gethostbyname(argv[1]);
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
        _error("ERROR connecting");
    
    //    printf("Please enter the message: ");
    //    bzero(buffer,256);
    //    fgets(buffer,255,stdin);
    
    //The only difference between send() and write() is the presence of flags. With zero flags parameter, send() is equivalent to write()
    n = write(sockfd,(char *)&s, sizeof(s));
    if (n < 0)
        _error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0)
        _error("ERROR reading from socket");
    printf("%s\n",buffer);
    return 0;
}
float ReverseFloat( const float inFloat )
{
    float retVal;
    char *floatToConvert = ( char* ) & inFloat;
    char *returnFloat = ( char* ) & retVal;
    
    // swap the bytes into a temporary buffer
    returnFloat[0] = floatToConvert[3];
    returnFloat[1] = floatToConvert[2];
    returnFloat[2] = floatToConvert[1];
    returnFloat[3] = floatToConvert[0];
    
    return retVal;
}
//! Byte swap int
int32_t swap_int32( int32_t val )
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | ((val >> 16) & 0xFFFF);
}