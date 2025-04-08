#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

int main() {
    int server_fd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    cout<<"Server File Descripter :"<<server_fd<<endl;
    if (server_fd < 0) {
        cerr << "Failed to create socket" << endl;
        return 0;
    }
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(9999);
    
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return 0;
    }
    
    if (listen(server_fd, 5) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return 1;
    }
    
    int client_fd = accept(server_fd, nullptr, nullptr);
    if (client_fd < 0) {
        std::cerr << "Accept failed" << std::endl;
        return 1;
    }
    cout<<"Client File Descripter :"<<client_fd<<endl;
    
    char buffer[1024] = {0};
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
    if (bytes_received < 0) {
        std::cerr << "Receive failed" << std::endl;
        return 1;
    }
    
    ssize_t bytes_sent = send(client_fd, buffer, bytes_received, 0);
    if (bytes_sent < 0) {
        std::cerr << "Send failed" << std::endl;
        return 1;
    }
    
    close(client_fd);
    close(server_fd);
    
    return 0;
}