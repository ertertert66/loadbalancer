#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <thread>

using namespace std;

//int counter = 0;
//std::mutex cout_mutex;

void task(int sockfd, sockaddr_in serverAddress, char* command) {
    connect(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    //cout_mutex.lock();
    std::string msgStr = "Hello from concurrent client " + std::to_string(command);
    counter = counter + 1;
    //cout_mutex.unlock();
    send(sockfd, msgStr.c_str(), msgStr.length(), 0);
    close(sockfd);
}

int main(int argc, char* argv[]) {
    std::cout << "Total arguments received: " << argc << "\n";
    
    for (int i = 0; i < argc; ++i) {
        std::cout << "Argument [" << i << "]: " << argv[i] << "\n";
    }

    //int numClients = std::stoi(argv[1]);

    // Configure server's address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET; // ipv4
    serverAddress.sin_port = htons(8080); // port 8080
    serverAddress.sin_addr.s_addr = INADDR_ANY; // accepts connections on any address

    //std::vector<std::thread> threads;

    char* command = argv[1];

    /*for (int i = 0; i < numClients; ++i) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        threads.emplace_back(task, sock, serverAddress);
    }*/

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    std::thread t(task, sock, serverAddress);

    t.join();

    return 0;
}