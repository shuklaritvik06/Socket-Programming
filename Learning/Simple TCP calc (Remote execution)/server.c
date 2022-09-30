#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void socket_error(const char *msg, int n)
{
    if (n < 0)
    {
        error(msg);
    }
}

int compute(int num1, int num2, int operation)
{
    switch (operation)
    {
    case 1:
        return num1 + num2;
        break;
    case 2:
        return num1 - num2;
        break;
    case 3:
        return num1 * num2;
        break;
    case 4:
        return num1 / num2;
        break;
    default:
        return 0;
        break;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    int sockfd, newConnection, portno, n;
    char buffer[255];
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressLength;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *)&serverAddress, sizeof(serverAddress));
    portno = atoi(argv[1]);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serverAddress,
             sizeof(serverAddress)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);
    clientAddressLength = sizeof(clientAddress);
    newConnection = accept(sockfd,
                           (struct sockaddr *)&clientAddress,
                           &clientAddressLength);
    if (newConnection < 0)
        error("ERROR on accept");
    int num1, num2, ans, operation;
    n = write(newConnection, "Welcome to the server\n", sizeof("Welcome to the server\n"));
    socket_error("ERROR writing to socket", n);
    n = write(newConnection, "Enter the first number\n", sizeof("Enter the first number\n"));
    socket_error("ERROR writing to socket", n);
    n = read(newConnection, &num1, sizeof(int));
    socket_error("ERROR reading from socket", n);
    n = write(newConnection, "Enter the second number\n", sizeof("Enter the second number\n"));
    socket_error("ERROR writing to socket", n);
    n = read(newConnection, &num2, sizeof(int));
    socket_error("ERROR reading from socket", n);
    n = write(newConnection, "Enter the operation\n1. Addition\n2. Subtraction\n3. Multiply\n4. Divide\n", sizeof("Enter the operation\n1. Addition\n2. Subtraction\n3. Multiply\n4. Divide\n"));
    socket_error("ERROR writing to socket", n);
    n = read(newConnection, &operation, sizeof(int));
    socket_error("ERROR reading from socket", n);
    ans = compute(num1, num2, operation);
    n = write(newConnection, &ans, sizeof(int));
    socket_error("ERROR writing to socket", n);
    close(newConnection);
    close(sockfd);
    return 0;
}