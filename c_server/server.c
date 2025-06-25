#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
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
        exit(EXIT_FAILURE); // TODO - change it with enum to have described errors
    }

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *) & server_address, sizeof(server_address)) < 0)
    {
        fprintf(stderr, "Failed to bind a socket.\n");
        close(server_fd);

        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0)
    {
        fprintf(stderr, "Failed to listen on socket.\n");
        close(server_fd);

        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < FD_SETSIZE; i++)
    {
        client_sockets[i] = -1;
    }

    FD_ZERO(&all_fds);
    FD_SET(server_fd, &all_fds);

    max_fd = server_fd;

    fprintf(stdout, "Server is working on port: %d\n", port);

    while (true)
    {
        read_fds = all_fds;

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0)
        {
            fprintf(stderr, "Failed to select on socket.\n");
            close(server_fd);

            break;
        }

        if (FD_ISSET(server_fd, &read_fds))
        {
            new_fd = accept(server_fd, (struct sockaddr *) & client_address, &address_length);
            if (new_fd < 0)
            {
                fprintf(stderr, "Failed to accept connection.\n");
                continue;
            }

            fprintf(stdout, "New connection from %s: %d", inet_ntoa(client_address.sin_addr),
                ntohs(client_address.sin_port));

            int i = 0;
            for (i = 0; i < FD_SETSIZE; i++)
            {
                if (client_sockets[i] == -1)
                {
                    client_sockets[i] = new_fd;
                    break;
                }
            }

            if (i == FD_SETSIZE)
            {
                fprintf(stderr, "Too many connections.\n");
                close(new_fd);

                continue;
            }

            FD_SET(new_fd, &all_fds);
            if (new_fd > max_fd)
            {
                max_fd = new_fd;
            }
        }

        for (int i = 0; i < FD_SETSIZE; i++)
        {
            int sock = client_sockets[i];
            if (sock < 0)
            {
                continue;
            }

            if (FD_ISSET(sock, &read_fds))
            {
                ssize_t bytes = recv(sock, buffer, BUFFER_SIZE - 1, 0);
                if (bytes <= 0)
                {
                    fprintf(stderr, "Client on socket %d disconnected!\n", sock);

                    close(sock);
                    FD_CLR(sock, &all_fds);

                    client_sockets[i] = 1;
                }
                else
                {
                    buffer[bytes] = '\0';

                    fprintf(stderr, "Message from client %d: %s.\n", sock, buffer);

                    for (int j = 0; j < FD_SETSIZE; j++)
                    {
                        int other = client_sockets[j];
                        if (other >= 0 && other != sock)
                        {
                            send(other, buffer, bytes, 0);
                        }
                    }
                }
            }
        }
    }

    close(server_fd);
    return 0;
}

























