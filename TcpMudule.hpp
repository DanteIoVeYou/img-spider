#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const static size_t BUFFER_SIZE = 128*1024;

class TcpModule {
  
public:
  TcpModule(uint16_t port, std::string ip): _port(port), _sock(-1), _ip(ip) {}

  ~TcpModule() {}

  void TcpModuleStart() {
    Loop();
  }

private:
  bool TcpModuleCreateSocket() {
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if(_sock >= 0) {
      return true;
    }
    std::cerr << "create socket failed" << std::endl;
    return false;
  } 

  bool TcpModuleConnect() {
    sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(_port); 
    server.sin_addr.s_addr = inet_addr(_ip.c_str());
    if(connect(_sock, (sockaddr*)&server, sizeof(server)) == 0) {
      return true;
    }
    std::cerr << "connect fail" << std::endl;
    return false;
  }

  bool TcpModuleInit() {
    if(TcpModuleCreateSocket() && TcpModuleConnect()) {
      return true;
    }
    std::cerr << "init failed" << std::endl;
    return false;
  }

  void Loop() {
    for(;;) {
      TcpModuleInit();
      printf("enter>");
      fflush(stdout);
      std::string msg;
      std::getline(std::cin, msg);
      if(send(_sock, msg.c_str(), msg.size(), 0) >= 0) {
        size_t size;
        char buffer[BUFFER_SIZE];
        if((size = recv(_sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
          buffer[size] = 0;
          std::cout << "========================================================" << std::endl;
          std::cout << buffer << std::endl;
          std::cout << "========================================================" << std::endl;
        }
        else {
          std::cerr << "recv failed" << std::endl;
        }
        close(_sock);
      }
      else {
        std::cerr << "send failed" << std::endl;
      }
    }
  }
  
private:
  uint16_t _port = 80;
  int _sock;
  std::string _ip;
};
