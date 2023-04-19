#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <cstring>



int main(int argc, char* argv[]){
    if (argc != 3)
    {
        std::cerr << "Usage:" << argv[0] << "<server_addr> <file.txt>"<< std::endl;
        return 1;

    }

    std::ifstream input_file(argv[2]);
    std::string file_content((std::istreambuf_iterator<char>(input_file)), (std::istreambuf_iterator<char>()));
    input_file.close();
    
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");;
    server_addr.sin_port = htons(8080);
    server_addr.sin_family = AF_INET;
    
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        std::cerr << "[+]Error socket" << std::endl;
        return 1;
    }
    
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0)
    {
        std::cerr << "[+]Error wrong adrress" <<std::endl;
        close(socket_fd);
        return 1;
    }

    if (connect(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) <0)
    {
        std::cerr << "[+]Error connecting..." <<std::endl;
        close(socket_fd);
        return 1;
    }

    int bytes_sent = send(socket_fd, file_content.c_str(), file_content.length(), 0);
    if(bytes_sent < 0)
    {
        std::cerr << "[+]Error sending data to server" <<std::endl;
        close(socket_fd);
        return 1;

    }
    else
    {
        std::cerr << "[+]Data sent" << std::endl;
    }

    close(socket_fd);
    return 0;
}