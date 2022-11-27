#ifndef SERVER_H
#define SERVER_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <poll.h>
#include <memory>
#include <vector>
#include <unordered_map>

#define BACKLOG 16
#define BUFFER_SIZE 128
#define MAX_NFDS 32
#define TIMEOUT 5000

class Server {

  public:
    Server(int port);
    ~Server();
    void run();
    
  private:
    int port;
    struct sockaddr_in server_address;
    int listen_fd;
    int transaction_number;
    std::unordered_map<std::string, int> transactions;
    std::unique_ptr<char[]> buffer; 
    std::vector<pollfd> fds;

    int accept_connection();
    int receive_data(int i);
    void simulate_transaction(int n, std::string client_machine_name);

    double epoch_time_seconds();
    double start_time;
    double end_time;

    // loggin members and methods
    FILE *logfile;
    void log_server_info();
    void log_recv_trans(int n, std::string client_name);
    void log_send_done(std::string client_name);
    void log_summary();
};


#endif