#include "server.h"
#include "tands.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <cstring>
#include <chrono>
#include <unistd.h>
#include <limits.h>

Server::Server(int port) {
  this->port = port;

  this->buffer = std::make_unique<char[]>(BUFFER_SIZE);
  // this->fds = std::vector<pollfd>();
  this->transaction_number = 0;

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

  // set up initial pollfd socket
  pollfd poll_fd;
  poll_fd.fd = this->listen_fd;
  poll_fd.events = POLLIN;
  this->fds.push_back(poll_fd);

  this->logfile = fopen("server.log", "w");

}

Server::~Server() {
  for (auto poll_fd : this->fds)
    close(poll_fd.fd);
  close(this->listen_fd);

  fclose(this->logfile);
}


void Server::run() {
  this->start_time = epoch_time_seconds();
  this->end_time = epoch_time_seconds();
  this->log_server_info();
  for (;;) {

    int result = poll(this->fds.data(), this->fds.size(), TIMEOUT);
    if (result < 0) {
      perror("poll");
      exit(EXIT_FAILURE);
    }
    if (result == 0) {
      fprintf(stderr, "poll timed out\n"); // TODO: set to 30 sec
      break;
    }

    for (int i = 0; i < this->fds.size(); i++) {

      if (fds[i].revents == 0)
        continue;
    
      if (fds[i].revents != POLLIN) {
        fprintf(stderr, "Unexpected returned event\n");
        exit(EXIT_FAILURE);
      }
      
      // Accept all incoming connections that are queued up
      // stop accepting when max number of fds is reached
      if (fds[i].fd == listen_fd)
          while (this->fds.size() <= MAX_NFDS && this->accept_connection() > 0);
      else
        receive_data(i);
    }
  };
  this->log_summary();
}

int Server::accept_connection() {
  int fd;
  if((fd = accept(this->listen_fd, NULL, NULL)) < 0) {
    if((errno == EAGAIN || errno == EWOULDBLOCK)) {
      return fd;
    }
    perror("accept");
    exit(EXIT_FAILURE);
  }
  printf("Client %d connected\n", fd);
  pollfd poll_fd;
  poll_fd.fd = fd;
  poll_fd.events = POLLIN;
  this->fds.push_back(poll_fd);
  return fd;
}

int Server::receive_data(int i) {

  ssize_t bytes = recv(this->fds[i].fd, this->buffer.get(), BUFFER_SIZE, 0);

  // error
  if (bytes < 0) {
    perror("recv");
    exit(EXIT_FAILURE);
    return 0;
  }

  // connection close by client
  if (bytes == 0) {
    fprintf(stdout, "Client %d disconnected\n", this->fds[i].fd);
    close(this->fds[i].fd);
    this->fds.erase(this->fds.begin()+i);
    return 0;
  }

  // data received
  int n;
  char client_name[BUFFER_SIZE] = {0};
  sscanf(this->buffer.get(), "%i %s", &n, client_name);  

  printf("Recieved T%d from %s\n", n, client_name);

  this->simulate_transaction(n, std::string(client_name));

  // response message
  snprintf((char *)this->buffer.get(), BUFFER_SIZE, "%c %d\r\n", 'D', this->transaction_number);
  if (send(this->fds[i].fd, this->buffer.get(), strlen(this->buffer.get()), 0) < 0) {
    fprintf(stderr, "Error: send failed\n");
    exit(EXIT_FAILURE);
  }

  printf("Sent D%d to %s\n", this->transaction_number, client_name);

  return 0;
}

void Server::simulate_transaction(int n, std::string client_name) {
  
  this->transaction_number++;

  this->log_recv_trans(n, client_name);

  Trans(n);

  this->log_send_done(client_name);
  this->end_time = epoch_time_seconds();

  this->transactions[client_name]++;

  return;
}

double Server::epoch_time_seconds() {
  return (double) std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / (double)1000;
}

void Server::log_server_info() {
  fprintf(this->logfile, "Using port %d\n", this->port);
  fflush(this->logfile);
}

void Server::log_recv_trans(int n, std::string client_name) {
  fprintf(this->logfile, "%.2f: # %d (T %d) from %s\n", this->epoch_time_seconds(), this->transaction_number, n, client_name.c_str());
  fflush(this->logfile);
}

void Server::log_send_done(std::string client_name) {
  fprintf(this->logfile, "%.2f: # %d (Done) from %s\n", this->epoch_time_seconds(), this->transaction_number, client_name.c_str());
  fflush(this->logfile);
}

void Server::log_summary() {
  fprintf(this->logfile, "\nSUMMARY\n");
  for (auto it : this->transactions)
    fprintf(this->logfile, "\t%d transactions from %s\n", it.second, it.first.c_str());
  double time = this->end_time - this->start_time;
  fprintf(this->logfile, "%.1f transactions/sec (%d/%.2f)\n", (float) this->transaction_number / time, this->transaction_number, time);
  fflush(this->logfile);
}
