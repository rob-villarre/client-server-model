#include "server.h"
#include <stdio.h>
#include <stdlib.h>


// https://www.ibm.com/docs/en/i/7.1?topic=designs-example-nonblocking-io-select
// https://www.ibm.com/docs/en/i/7.1?topic=designs-using-poll-instead-select
Server::Server(int port) {
  this->port = port;

  this->buffer = std::make_shared<char[]>(BUFFER_SIZE);
  this->fd_vector = std::make_shared<std::vector<pollfd>>();

  // create socket
  if ((this->listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket create");
    exit(EXIT_FAILURE);
  }

  // allow socket descriptor to be reusable
  int on = 1;
  if ((setsockopt(this->listen_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on))) < 0) {
    perror("socket reusable opt");
    exit(EXIT_FAILURE);
  }

  // set socket to non-blocking
  if ((ioctl(this->listen_fd, FIONBIO, (char *) &on)) < 0) {
    perror("socket non-blocking");
    exit(EXIT_FAILURE);
  }

  // bind socket
  this->server_address.sin_family = AF_INET;
  this->server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  this->server_address.sin_port = htons(this->port);

  if (bind(this->listen_fd, (struct sockaddr *)&this->server_address, sizeof(this->server_address)) < 0) {
    perror("socket bind");
    exit(EXIT_FAILURE);
  }
  
  // listen socket
  if (listen(this->listen_fd, BACKLOG) < 0) {
    perror("socket listen");
    exit(EXIT_FAILURE);
  }

}

Server::~Server() {}

void Server::run() {
  auto fds = this->fd_vector.get()->data();
  int nfds = this->fd_vector.get()->size();

  int result = poll(fds, nfds, TIMEOUT);
  if (result < 0) {
    perror("poll");
    exit(EXIT_FAILURE);
  }
  if (result == 0) {
    fprintf(stderr, "poll timed out\n");
    exit(EXIT_FAILURE);
  }
  

}