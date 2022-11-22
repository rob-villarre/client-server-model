#include "client.h"

#include <unistd.h>
#include <limits.h>

// https://www.ibm.com/docs/en/i/7.1?topic=clients-example-ipv4-ipv6-client
Client::Client(int port, std::string ip_address) {
  this->port = port;
  this->ip_address = ip_address;
  this->buffer = std::make_shared<char[]>(BUFFER_SIZE);

  this->pid = getpid();
  char hostname[HOST_NAME_MAX];
  gethostname(hostname, HOST_NAME_MAX);
  this->hostname = std::string(hostname);


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

  printf("Using port %d\n", this->port);
  printf("Using server address %s\n", this->ip_address.c_str());
  printf("Host %s.%d\n", this->hostname.c_str(), (int) this->pid);
}

Client::~Client() {
  close(this->socket_fd);
}

void Client::send_data(int n) {

  memset(this->buffer.get(), '\0', sizeof(this->buffer.get()));

  snprintf((char *) this->buffer.get(), sizeof(this->buffer.get()), "%d", n);
  // send request
  if (send(this->socket_fd, this->buffer.get(), strlen(this->buffer.get()), 0) < 0) {
      perror("socket send");
      exit(EXIT_FAILURE);
  }

  // receive response
  ssize_t bytes = recv(this->socket_fd, this->buffer.get(), sizeof(this->buffer.get()), 0);
  if (bytes < 0) {
    perror("recv");
    exit(EXIT_FAILURE);
  }
  if (bytes == 0) {
    fprintf(stderr, "The server closed the connection");
    exit(EXIT_FAILURE);
  }

  // data received
  fprintf(stdout, "received from server: %s\n", this->buffer.get());

}

std::tuple<char, int> Client::read_command() {
  char type;
  int n;
  if (scanf(" %c%i", &type, &n) != EOF) {
    std::cout << "r: " << type << std::endl;
    if (type != 'T' && type != 'S')
      return std::make_tuple('U', -1);
    return std::make_tuple(type, n);
  } else { // eof
    return std::make_tuple('E', -1);
  }
}

// hostname -i
void Client::run() {
  
  for (;;) {
    std::tuple<char, int> command = this->read_command();
    char type = std::get<0>(command);
    int n = std::get<1>(command);
    
    std::cout << type << " " << n << std::endl;
    // EOF / EXIT
    if (type == 'E')
      break;

    // Undefined
    if (type == 'U')
      continue;

    std::cout << "Sernding data\n";
    this->send_data(n);
    
  }
  
}