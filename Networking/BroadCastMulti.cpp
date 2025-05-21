#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <poll.h>
#include<vector>
#include<map>
#include<string>

using namespace std;

int main() {
    //We will start with creation of a socket
    int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd < 0) {
      cerr << "Failed to create socket" << endl;
      return 1;
    }
    cout << "Server File Descriptor: " << server_fd << endl;
    
    //Then we bind the socket to an Ip address
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address)); 
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(9999);
     
    if (bind(server_fd, (struct sockaddr*)&address,sizeof(address))< 0) {
      cerr << "Bind failed" << endl;
      close(server_fd);
      return 1;
    }
    //Next we listen
    
    if (listen(server_fd, 5) < 0) {
      cerr << "Listen failed" << endl;
      close(server_fd);
    return 1;
    }
    
    //lets set up poll
    vector<struct pollfd> poll_fds;
    struct pollfd server_pollfd;
    server_pollfd.fd=server_fd;
    server_pollfd.events=POLLIN;
    server_pollfd.revents=0;
    poll_fds.push_back(server_pollfd);
    
    vector<int> client_sockets;
    map<int,int> socket_id;
    int next_client=4;
    //now the connection while loop
    
    while(true){
      int poll_result=poll(poll_fds.data(),poll_fds.size(),-1);
      if(poll_result<0){
        cerr<<"Poll failed"<<endl;
        break;
      }
      if (poll_result==0){
        continue;
      }
      //Check if new connection by Bitwise AND on Server revents
      if(poll_fds[0].revents & POLLIN){
        cout<<"New Connection Detected"<<endl;
        //Accept new connections and add them in poll_fds
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) {
          cerr << "Accept failed" <<endl;
          continue;
        }
        //package the client fd and add to vector
    	struct pollfd client_pollfd;
    	client_pollfd.fd=client_fd;
    	client_pollfd.events=POLLIN;
    	client_pollfd.revents=0;
    	poll_fds.push_back(client_pollfd);
    	client_sockets.push_back(client_fd);
    	socket_id[client_fd]=next_client;
    	cout<<"Client connected: "<<client_fd<<endl;
    	next_client++;
    	
      }
      //Sending and receiving of messages
      for(size_t i=1;i<poll_fds.size();++i){
        if(poll_fds[i].revents & (POLLIN|POLLHUP)){
          int client_fd=poll_fds[i].fd;
          int cli=socket_id[poll_fds[i].fd];
          
          //check if socket is disconnected
          bool isdisconnected=(poll_fds[i].revents & POLLHUP);
          if(!isdisconnected){
            char buffer[1024];
            ssize_t bytes_received = recv(poll_fds[i].fd, buffer, sizeof(buffer), 0);
            if(bytes_received<0){
              cerr<<"Received Failed";
              continue;
            }
            if(bytes_received==0){
              continue;
            }
            buffer[bytes_received]='\0';
            string message="Client "+to_string(cli)+" :"+string(buffer);
          
            //Broadcast this created message
            for(int other_fd: client_sockets){
              if(other_fd!=poll_fds[i].fd){
                if(send(other_fd,message.c_str(),message.size(),0)<0){
                  cerr<<"Send Failed";  
                } 
              }
            }
          
            cout<<"Broadcasting -> "<<message<<endl;
          }
          if(isdisconnected){
            cout<<"Client Disconnected "<<cli<<endl;
            string message="Client "+to_string(cli)+" has left the chat";
            //Broadcast the left message
            for(int other_fd: client_sockets){
              if(other_fd!=poll_fds[i].fd){
                if(send(other_fd,message.c_str(),message.size(),0)<0){
                  cerr<<"Send Failed";  
                } 
              }
            }
            cout<<"Broadcasting -> "<<message<<endl;
            //after broadcast not we should close the fd and erase them
            close(client_fd);
            poll_fds.erase(poll_fds.begin()+i);
            client_sockets.erase(client_sockets.begin()+i);
            socket_id.erase(client_fd);
            
            //Adjust index
            --i;
          }
        } 
      }
    }
    
    
    close(server_fd);
    for(int i:client_sockets){
      close(i);
    }
    return 0;
}
