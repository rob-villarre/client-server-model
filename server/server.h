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

#define BACKLOG 10
#define BUFFER_SIZE 100
#define MAX_NFDS 50
#define TIMEOUT 100000

class Server {

  public:
    Server(int port);
    ~Server();
    void run();
    

  private:
    int port;
    int listen_fd;
    int transaction_number;
    struct sockaddr_in server_address;
    std::shared_ptr<char[]> buffer; 
    std::vector<pollfd> fds;

    int accept_connection();
    int receive_data(int i);
    void simulate_transaction(int n, std::string client_machine_name, pid_t client_pid);
    
  
};


#endif