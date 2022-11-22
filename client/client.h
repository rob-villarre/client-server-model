#include <string>
#include <memory>

#include <iostream>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <tuple>

#define BUFFER_SIZE 100

class Client {

  public:
    Client(int port, std::string ip_address);
    ~Client();
    void send_data(int n);
    void run();

  private:
    int port;
    std::string ip_address;
    int socket_fd;
    pid_t pid;
    std::shared_ptr<char[]> buffer;
    struct sockaddr_in server_address;
    std::string hostname;
    std::tuple<char, int> read_command();

};