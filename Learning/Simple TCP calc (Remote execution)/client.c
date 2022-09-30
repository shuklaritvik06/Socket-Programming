#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void socket_error(const char *msg, int n)
{
    if (n < 0)
    {
        error(msg);
    }
}

int main(int argc, char *argv[])
{
    int n1,n2,operation,ans;
    if (argc < 3)
    {
        fprintf(stderr, "Please provide all required arguments\n1.IP Address\n2.Port Number\nIt should be of the form ./filename IP Port");
        exit(0);
    }
    int sockfd, portno, n;
    char buffer[255];
    struct sockaddr_in serv_addr;
    struct hostent *server;
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Error opening socket");
    }
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Error, no such host");
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Error connecting");
    }
    int i = strncmp("close", buffer, strlen("close"));
    if (i == 0)
    {
        close(sockfd);
    };
    n = read(sockfd, buffer, 255);
    socket_error("Error reading from socket", n);
    printf("%s", buffer);
    bzero(buffer, 255);
    n = read(sockfd, buffer, 255);
    socket_error("Error reading from socket", n);
    printf("%s", buffer);
    scanf("%d", &n1);
    n = write(sockfd, &n1, sizeof(int));
    socket_error("Error writing to socket", n);
    bzero(buffer, 255);
    n = read(sockfd, buffer, 255);
    socket_error("Error reading from socket", n);
    printf("%s", buffer);
    scanf("%d", &n2);
    n = write(sockfd, &n2, sizeof(int));
    socket_error("Error writing to socket", n);
    bzero(buffer, 255);
    n = read(sockfd, buffer, 255);
    socket_error("Error reading from socket", n);
    printf("%s", buffer);
    scanf("%d", &operation);
    n = write(sockfd, &operation, sizeof(int));
    socket_error("Error writing to socket", n);
    bzero(buffer, 255);
    n = read(sockfd, &ans, 255);
    socket_error("Error reading from socket", n);
    printf("\nAnswer is %d", ans);
    close(sockfd);
    return 0;
}
