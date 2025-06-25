#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

int main(void)
{
    int server_fd, new_fd, port, max_fd;
    struct sockaddr_in server_address, client_address;
    socklen_t address_length = sizeof(client_address);

    char buffer[BUFFER_SIZE];
    int client_sockets[FD_SETSIZE];

    fd_set read_fds, all_fds;

    fprintf(stdout, "Enter port number" );
    if (scanf("%d", &port) != 1)
    {
        fprintf(stderr, "Incorrect input\n");
        exit(EXIT_FAILURE);
    }

    if (port < 1025 || port > 65535)
    {
        fprintf(stderr, "Number is incorrect!\n");
        exit(EXIT_FAILURE);
    }

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Failed to create a socket.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}