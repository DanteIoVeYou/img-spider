#pragma once

#include "header.hpp"

const static size_t BUFFER_SIZE = 128 * 1024;

class Socket
{

public:
  Socket() {}
  ~Socket() {}
  static void SetNoBlock(int sock)
  {
    int fl = fcntl(sock, F_GETFL);
    fcntl(sock, F_SETFL, fl | O_NONBLOCK);
  }

  static int CreateSocket()
  {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock > 0)
    {
      return sock;
    }
    else
    {
      // TODO
      std::cerr << "create socket failed" << std::endl;
      return -1;
    }
  }

  static bool Connect(std::string ip, uint16_t port, int sock)
  {
    sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    if (connect(sock, (sockaddr *)&server, sizeof(server)) == 0)
    {
      return true;
    }
    else
    {
      std::cerr << "connect fail" << std::endl;
      return false;
    }
  }
};


