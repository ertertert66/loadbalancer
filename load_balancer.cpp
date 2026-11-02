#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

int main() {
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

    if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
        return -1;
    }

    if (listen(sockfd, 5) < 0) {
        return -1;
    }

    close(sockfd);

    return 0;
}