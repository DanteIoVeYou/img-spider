#pragma once

#include "common.hpp"

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

  static std::string DNSConvertion(std::string domain)
  {
    struct hostent *host = gethostbyname(domain.c_str());
    if (host == nullptr)
    {
      std::cerr << "域名解析错误" << std::endl;
      exit(1);
    }
    else
    {
      std::string ip = inet_ntoa(*((struct in_addr *)host->h_addr_list[0]));
      return std::move(ip);
    }
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

  // void Loop()
  // {
  //   for (;;)
  //   {
  //     TcpModuleInit();
  //     printf("enter>");
  //     fflush(stdout);
  //     std::string msg;
  //     std::getline(std::cin, msg);
  //     if (send(_sock, msg.c_str(), msg.size(), 0) >= 0)
  //     {
  //       size_t size;
  //       char buffer[BUFFER_SIZE];
  //       if ((size = recv(_sock, buffer, sizeof(buffer) - 1, 0)) > 0)
  //       {
  //         buffer[size] = 0;
  //         std::cout << "========================================================" << std::endl;
  //         std::cout << buffer << std::endl;
  //         std::cout << "========================================================" << std::endl;
  //       }
  //       else
  //       {
  //         std::cerr << "recv failed" << std::endl;
  //       }
  //       close(_sock);
  //     }
  //     else
  //     {
  //       std::cerr << "send failed" << std::endl;
  //     }
  //   }
  // }
};
