#include "controller.hpp"
#include "utils.hpp"
#include "parser.hpp"
int main(int argc, char *argv[])
{
  // std::string ip = Socket::DNSConvertion("www.baidu.com");
  // std::string msg;
  // Http http(ip);

  // http.BuildHttpRequest("/");
  // http.SendHttpResquest();
  // http.RecvHttpResquest(&msg);

  // std::cout << msg << std::endl;

  // Url seed("www.baidu.com");
  Url seed("www.sogou.com");
  Controller con(seed);
  con.Start();

  // std::string domain;
  // std::string path;
  // std::string url = "http://www.baidu.com/a/b/c/index/html";
  // std::string url = "http://www.baidu.com/index.html";
  // Parser::ParseDomainAndPath(url, &domain, &path);
  // std::cout << "###"
  //           << "domain: " << domain << "###" << std::endl;
  // std::cout << "###"
  //           << "path: " << path << "###" << std::endl;
  return 0;
}
