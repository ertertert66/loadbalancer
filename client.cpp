#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <thread>

using namespace std;

void task(int sockfd, sockaddr_in serverAddress, char* command) {
    connect(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    std::string cmd = std::string(command);
    send(sockfd, cmd.c_str(), cmd.length(), 0);
    close(sockfd);
}

int main(int argc, char* argv[]) {
    std::cout << "Total arguments received: " << argc << "\n";
    
    for (int i = 0; i < argc; ++i) {
        std::cout << "Argument [" << i << "]: " << argv[i] << "\n";
    }

    // Configure server's address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET; // ipv4
    serverAddress.sin_port = htons(8080); // port 8080
    serverAddress.sin_addr.s_addr = INADDR_ANY; // accepts connections on any address

    char* command = argv[1];

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    std::thread t(task, sock, serverAddress, command);

    t.join();

    return 0;
}