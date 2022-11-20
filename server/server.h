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
#define TIMEOUT 1000

class Server {

  public:
    Server(int port);
    ~Server();
    void run();

  private:
    int port;
    int listen_fd;
    struct sockaddr_in server_address;
    std::shared_ptr<char[]> buffer; 
    std::shared_ptr<std::vector<pollfd>> fd_vector;
    
  
};


#endif