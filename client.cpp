#include "client.h"
#include "tands.h"

#include <unistd.h>
#include <limits.h>
#include <chrono>

Client::Client(int port, std::string ip_address) {

  this->port = port;
  this->ip_address = ip_address;
  this->buffer = std::make_unique<char[]>(BUFFER_SIZE);

  // Client info
  this->pid = getpid();
  char hostname[HOST_NAME_MAX];
  gethostname(hostname, HOST_NAME_MAX);
  this->hostname = std::string(hostname);
  this->name = this->hostname + "." + std::to_string(this->pid);

  this->server_address.sin_family = AF_INET;
  this->server_address.sin_addr.s_addr = inet_addr(ip_address.c_str());
  this->server_address.sin_port = htons(this->port);

  // create socket
  if ((this->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket create");
    exit(EXIT_FAILURE);
  }

  // Connect
  if (connect(this->socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
      perror("socket connect");
      exit(EXIT_FAILURE);
  }

  this->logfile = fopen(this->name.c_str(), "w");
}

Client::~Client() {
  close(this->socket_fd);
  fclose(this->logfile);
}

void Client::send_data(int n) {

  // send request
  snprintf((char *) this->buffer.get(), BUFFER_SIZE, "%d %s\r\n", n, this->name.c_str());
  if (send(this->socket_fd, this->buffer.get(), strlen(this->buffer.get()), 0) < 0) {
      perror("socket send");
      exit(EXIT_FAILURE);
  }
  this->log_send('T', n);

  printf("Sent T%d to server\n", n);

  this->sent_trans_count++;

  // receive response
  ssize_t bytes = recv(this->socket_fd, this->buffer.get(), BUFFER_SIZE, 0);
  if (bytes < 0) {
    perror("recv");
    exit(EXIT_FAILURE);
  }
  if (bytes == 0) {
    fprintf(stderr, "The server closed the connection\n");
    exit(EXIT_FAILURE);
  }

  // data received
  char c;
  int tn;
  sscanf(this->buffer.get(), "%c %d", &c, &tn);
  printf("Recieved D%d from server\n", tn);
  this->log_recv(c, tn);
}

std::tuple<char, int> Client::read_command() {
  char command;
  int n;
  if (scanf(" %c%i", &command, &n) != EOF) {
    if (command != 'T' && command != 'S')
      return std::make_tuple('U', -1);
    return std::make_tuple(command, n);
  } else { // eof
    return std::make_tuple('E', -1);
  }
}

void Client::run() {
  
  this->log_client_info();
  for (;;) {
    std::tuple<char, int> command = this->read_command();
    char type = std::get<0>(command);
    int n = std::get<1>(command);
    
    // EOF / EXIT
    if (type == 'E')
      break;

    // Undefined
    if (type == 'U')
      continue;

    // Sleep
    if (type == 'S')
      this->simulate_wait(n);

    // Transaction
    if (type == 'T')
      this->send_data(n); 
  }

  this->log_summary();
  return;
}

void Client::simulate_wait(int n) {
  this->log_sleep(n);
  Sleep(n);
}

double Client::epoch_time_seconds() {
  return (double) std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / (double)1000;
}

void Client::log_client_info() {
  fprintf(this->logfile, "Using port %d\n", this->port);
  fprintf(this->logfile, "Using server address %s\n", this->ip_address.c_str());
  fprintf(this->logfile, "Host %s\n", this->name.c_str());
  fflush(this->logfile);
}

void Client::log_send(char c, int n) {
  fprintf(this->logfile, "%.2f: Send (%c %d)\n", this->epoch_time_seconds(), c, n);
  fflush(this->logfile);
}

void Client::log_recv(char c, int n) {
  fprintf(this->logfile, "%.2f: Recv (%c %d)\n", this->epoch_time_seconds(), c, n);
  fflush(this->logfile);
}

void Client::log_sleep(int n) {
  fprintf(this->logfile, "Sleep for %d units\n", n);
  fflush(this->logfile);
}

void Client::log_summary() {
  fprintf(this->logfile, "Sent %d transactions\n", this->sent_trans_count);
  fflush(this->logfile);
}