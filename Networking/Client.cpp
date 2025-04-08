#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

int main() {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(9999);

    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connect failed" << std::endl;
        return 1;
    }
    
    const char* message = "Hello, Server!";
    ssize_t bytes_sent = send(sock_fd, message, strlen(message), 0);
    if (bytes_sent < 0) {
        std::cerr << "Send failed" << std::endl;
        return 1;
    }

    char buffer[1024] = {0};
    ssize_t bytes_received = recv(sock_fd, buffer, sizeof(buffer), 0);
    if (bytes_received < 0) {
        std::cerr << "Receive failed" << std::endl;
        return 1;
    }
    
    return 0;
}