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
    return 0;
}
