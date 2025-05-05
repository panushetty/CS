#include <iostream>
#include <cstring>        // For memset, strlen
#include <sys/socket.h>   // For socket functions
#include <netinet/in.h>   // For sockaddr_in
#include <arpa/inet.h>    // For inet_addr
#include <unistd.h>       // For close
#include <sys/select.h>   // For select, fd_set
#include <vector>         // For storing client sockets
#include<algorithm>

using namespace std;


int main(){
  //This snippet creates a socket for our multi server
  int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (server_fd < 0) {
    cerr << "Failed to create socket" << endl;
    return 1;
  }
  cout << "Server File Descriptor: " << server_fd << endl;

  //This snippet binds our server to an ipaddress and protocol
  struct sockaddr_in address;
  memset(&address, 0, sizeof(address)); // Clear structure
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  address.sin_port = htons(9999);

  if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    cerr << "Bind failed" << endl;
    close(server_fd);
    return 1;
  }

  //This snippet will make our server listen to the client

  if (listen(server_fd, 5) < 0) {
    cerr << "Listen failed" << endl;
    close(server_fd);
    return 1;
  }

  vector<int> client_sockets; // To store client socket descriptors
  fd_set read_fds;           // Set for select() to monitor readable sockets
  int max_fd = server_fd;    // Highest file descriptor for select()

  while(true){
    FD_ZERO(&read_fds);
    FD_SET(server_fd, &read_fds);
    for (int client_socket : client_sockets) {
      FD_SET(client_socket, &read_fds);
      max_fd=max(max_fd,client_socket);
    }

    //call select function


    int result = select(max_fd+1, &read_fds, NULL, NULL,NULL);
    if (result < 0) {
    cerr << "select failed" << endl;
    continue;
    }
    if (result>0 && FD_ISSET(server_fd, &read_fds)) {
      int client_fd = accept(server_fd, nullptr, nullptr);

      if (client_fd < 0) {
        cerr << "Accept failed" <<endl;
      }
      else{
    	cout<<"Client connected: "<<client_fd<<endl;
	client_sockets.push_back(client_fd);
	max_fd=max(max_fd,client_fd);
      }
    }


    char buffer[1024];
    if(result>0){
      for (int i=0;i<client_sockets.size();i++) {
	int client_socket=client_sockets[i];
	if (FD_ISSET(client_socket, &read_fds)){
	  memset(buffer, 0, sizeof(buffer));
	  ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
	  if (bytes_received < 0) {
	    cerr << "Receive failed" <<endl;
	    close(client_socket);
	    client_sockets.erase(client_sockets.begin() + i);
	    --i;
	  }
	  if (bytes_received==0){
	    cout<<"Client disconnected: "<<client_socket<<endl;
	    close(client_socket);
	    client_sockets.erase(client_sockets.begin() + i);
	    --i;
	  }
	  else{
	    buffer[0]='G';
	    buffer[1]='a';
	    buffer[2]='y';
	    ssize_t bytes_sent = send(client_socket, buffer, bytes_received, 0);
	    if (bytes_sent < 0) {
	      cerr << "Send failed" <<endl;
	      close(client_socket);
	      client_sockets.erase(client_sockets.begin() + i);
	      --i;
	    }
	  }
	}
    }
    }




  }




  return 0;
}
