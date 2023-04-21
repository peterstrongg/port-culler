#include <iostream>
#include <fstream>
#include <arpa/inet.h>      // sockaddr_in, socket, bind
#include <pthread.h>
#include <unistd.h>         // close

#define PORT 5000

void* cull_ip(void*);
void log(std::string);

struct t_struct {
    int client_fd;
    sockaddr_in client_addr;
    socklen_t client_addr_size;
};

int main(int argc, char *argv[]) {
    int socket_fd, port = PORT;

    if(argc == 2) {
        port = atoi(argv[1]);
    } else if (argc > 2) {
        printf("Usage: %s <PORT>", argv[0]);
        return -1;
    }

    struct sockaddr_in address = {AF_INET, htons(port), INADDR_ANY};

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    bind(socket_fd, (struct sockaddr*)&address, sizeof(struct sockaddr_in));
    listen(socket_fd, 1);

    struct t_struct *t;
    t->client_addr_size = sizeof(t->client_addr);

    for(;;) {
        t->client_fd = accept(socket_fd, (sockaddr*)&(t->client_addr), &(t->client_addr_size));

        pthread_t thread;
        pthread_create(&thread, NULL, cull_ip, (void *)t);
    }

    return 0;
}

void* cull_ip(void* t) {
    struct t_struct *t_data = (struct t_struct *)t;

    t_data->client_addr_size = sizeof(t_data->client_addr);
    getpeername(t_data->client_fd, (sockaddr*)&(t_data->client_addr), &(t_data->client_addr_size));
    
    std::string ip = inet_ntoa(t_data->client_addr.sin_addr);

    std::string ban_cmd = "iptables -A INPUT -s " + ip + " -j DROP";
    system(ban_cmd.c_str());

    log(ip);

    close(t_data->client_fd);
    pthread_exit(NULL);
}

void log(std::string ip) {
    std::ofstream file;
    file.open("log.txt", std::ios_base::app);
    file << ip << std::endl;
    file.close();
}
