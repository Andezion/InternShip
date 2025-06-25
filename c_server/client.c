#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>

#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void buffer(void)
{
    int c;
    while ((c = getchar()) != '\0' && c != '\n');
}

int main(int argc, char *argv[])
{
    int number_of_port = 0;

    struct sockaddr_in server_address;

    char ip_address[15];
    char message_from_client[1024];
    char message_to_client[1024];

    const int socket_number = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_number == -1)
    {
        fprintf(stderr, "Error opening socket\n");
    }

    fprintf(stdout, "Socket has been created! Socket number: %d\n", socket_number);

    fprintf(stdout, "Enter the IP address of the client: ");
    fgets(ip_address, 15, stdin);

    buffer();

    server_address.sin_addr.s_addr = inet_addr(ip_address);
    server_address.sin_family = AF_INET;

    fprintf(stdout, "Enter port number: ");
    if (scanf("%d", &number_of_port) != 1) // TODO - change to strtol
    {
        fprintf(stderr, "Incorrect port input!\n");
    }

    server_address.sin_port = htons(number_of_port);
    fprintf(stdout, "We are on port: %d\n", number_of_port);

    if (connect(socket_number, (struct sockaddr *) & server_address, sizeof(server_address)) < 0)
    {
        fprintf(stderr, "Error in connecting!\n");
        return 1;
    }

    fprintf(stdout, "Connected to server!\n");

    while (1)
    {
        buffer();

        fprintf(stdout, "Enter message to send: ");
        fgets(message_from_client, sizeof(message_from_client), stdin);

        const int send_bytes = (int) send(socket_number, message_from_client, strlen(message_from_client), 0);
        if (send_bytes < 0)
        {
            fprintf(stderr, "Error while sending the message!\n");
            return 2; // TODO - should there be return or rather just break???
        }

        fprintf(stdout, "We send %d bytes.\n", send_bytes);

        buffer();

        const int received = (int) recv(socket_number, message_to_client, sizeof(message_to_client) - 1, 0);
        if (received < 0)
        {
            fprintf(stderr, "Error while receiving message!\n");
            break;
        }
        message_to_client[received] = '\0';

        fprintf("Server reply is: %s and it has %d bytes!\n", message_to_client, received);

        if (strcmp(message_from_client, "exit\n") == 0)
        {
            break;
        }
    }

    close(socket_number);
    return 0; // TODO - change it to enum
}
