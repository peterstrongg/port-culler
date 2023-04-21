// TEST FILE FOR FOR DAEMON

#include <iostream>
#include <arpa/inet.h>      // sockaddr_in, socket, bind

#define PORT 5000

int main(int argc, char *argv[]) {
    int socket_fd;
    struct sockaddr_in address = {AF_INET, htons(PORT), INADDR_ANY};
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    inet_pton(AF_INET, "172.19.174.51", &address.sin_addr);
    connect(socket_fd, (struct sockaddr*)&address, sizeof(address));
    return 0;
}