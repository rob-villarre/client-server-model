#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char *argv[]) {

  if (argc != 3) {
    std::cerr << "cleint only accepts two command-line argument\nclient port ip-address\n";
    return 0;
  }

  int port;
  std::string ip_address;
  try {
    port = std::stoi(argv[1]);
    if (port < 5000 || port > 64000)
      throw std::invalid_argument("Invalid port\nport must be in the range of 5,000 to 64,000\n");
    
    ip_address = argv[2];
  }
  catch(const std::exception& e) {
    std::cerr << e.what();
    return 0;
  }
  
  std::cout << port << " " << ip_address << "\n";


  return 0;
}