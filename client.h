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

#define BUFFER_SIZE 128

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
    std::unique_ptr<char[]> buffer;
    std::string name;
    struct sockaddr_in server_address;
    std::string hostname;
    std::tuple<char, int> read_command();
    void simulate_wait(int n);
    int sent_trans_count;

    double epoch_time_seconds();
    // log
    FILE *logfile;
    void log_client_info();
    void log_send(char c, int n);
    void log_recv(char c, int n);
    void log_sleep(int n);
    void log_summary();

};