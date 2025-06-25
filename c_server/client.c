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

    return 0;
}
