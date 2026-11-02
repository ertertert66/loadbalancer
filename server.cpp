#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <mutex>

void task(int sockfd, int& output) {
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    int client_socket;
    char buffer[4096];

    while (true) {
        addr_size = sizeof(their_addr);
        
        client_socket = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

        ssize_t n = recv(client_socket, buffer, sizeof(buffer), 0);

        if (n > 0) {
            std::string command(buffer, n);
            if (command == "PING") {
                std::cout << "Match!\n";
            }
        }

        close(client_socket);
    }

}


int main(int argc, char* argv[]) {
    struct addrinfo hints, *res;
    int sockfd;
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // Either IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE; // Bind to the IP of the host the program is being run on (in this instance, locally)

    getaddrinfo(NULL, "8080", &hints, &res);
    if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
        return -1;
    }

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
        return -1;
    }

    if (listen(sockfd, 5) < 0) {
        return -1;
    }

    int numThreads = std::stoi(argv[1]);

    int output = 0;

    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; i++) {
        threads.emplace_back(task, sockfd, std::ref(output));
    }

    for (auto& t : threads) {
        t.join();
    }

    close(sockfd);

    return output;
}