#include <iostream>
#include <stdexcept>
#include "server.h"

int main(int argc, char *argv[]) {

  if (argc != 2) {
    std::cerr << "server only accepts one command-line argument\nserver port\n";
    return 0;
  }

  int port;
  try {
    port = std::stoi(argv[1]);
    if (port < 5000 || port > 64000)
      throw std::invalid_argument("Invalid port\nport must be in the range of 5,000 to 64,000\n");
  }
  catch(const std::exception& e) {
    std::cerr << e.what();
    return 0;
  }
  
  std::cout << port << "\n";

  std::shared_ptr<Server> server = std::make_shared<Server>(port);
  server.get()->run();

  return 0;
}