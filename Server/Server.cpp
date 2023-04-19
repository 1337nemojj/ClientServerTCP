#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <cstring>

int server_socket_fd;

void signal_handler(int signal){
    if (signal == SIGTERM || signal == SIGHUP)
    {
        std::cout << "Terminal signal. Shutdown server"<< std::endl;
        close(server_socket_fd);
        exit(0);
    }
}

int main(int argc, char* argv[]){
    signal(SIGTERM, signal_handler);
    signal(SIGHUP, signal_handler);

    sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);
    server_addr.sin_family =AF_INET;


    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    
    bind(server_socket_fd, (sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_socket_fd, SOMAXCONN);
    
    while(1)
    {
        sockaddr_in client_addr;
        socklen_t client_add_len = sizeof(client_addr);

        int client_socket_fd = accept(server_socket_fd, (sockaddr*)&client_addr, &client_add_len);

        if (client_socket_fd < 0)
        {
            std::cout << "error incoming connection" <<std::endl;
            close(client_socket_fd);
            continue;
        }

        char buffer[1024];
        int bytes_recieved = recv(client_socket_fd, buffer, sizeof(buffer), 0);
        if (bytes_recieved <=0)
        {
            std::cerr << "error data from client" <<std::endl;
            close(client_socket_fd);
            continue;
        }
        //data

        std::ofstream outfile("recieved.txt");
        outfile.write(buffer, bytes_recieved);
        outfile.close();

        close(client_socket_fd);
    }
    return 0;
}