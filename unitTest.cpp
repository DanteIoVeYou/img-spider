
#include "http.hpp"
int main(int argc, char *argv[])
{
  std::string ip = Socket::DNSConvertion("www.baidu.com");
  std::string msg;
  Http http(ip);

  http.BuildHttpRequest("/");
  http.SendHttpResquest();
  http.RecvHttpResquest(&msg);

  std::cout << msg << std::endl;
  return 0;
}
