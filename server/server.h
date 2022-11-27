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

#define BACKLOG 10
#define BUFFER_SIZE 100
#define MAX_NFDS 50
#define TIMEOUT 5000

class Server {

  public:
    Server(int port);
    ~Server();
    void run();
    

  private:
    int port;
    int listen_fd;
    int transaction_number;
    std::unordered_map<std::string, int> transactions;
    struct sockaddr_in server_address;
    std::unique_ptr<char[]> buffer; 
    std::vector<pollfd> fds;

    int accept_connection();
    int receive_data(int i);
    void simulate_transaction(int n, std::string client_machine_name, pid_t client_pid);

    double epoch_time_seconds();
    double start_time;
    double end_time;

    // loggin
    FILE *logfile;
    void log_server_info();
    void log_recv_trans(int n, std::string client_name);
    void log_send_done(std::string client_name);
    void log_summary();
    
};


#endif