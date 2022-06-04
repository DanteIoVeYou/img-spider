#pragma once

#include "sock.hpp"
static const uint16_t DEFAULT_PORT = 80;
static const std::string REQUEST_METHOD = "GET";
static const std::string HTTP_VERSION = "HTTP/1.1";
static const std::string LF = "\n";
static const std::string BLACKSPACE = " ";

/*
 * 实例化http类的同时会
 * 1. 创建套接字
 * 2. 三次握手
 * 接口一： 构建http请求报文
 * 接口二： 返回套接字
 */
class Http
{
public:
    Http(std::string ip) : _ip(ip)
    {
        // 初始化
        _sock = Socket::CreateSocket();
        Socket::Connect(_ip, _port, _sock);
        Socket::SetNoBlock(_sock);
        memset(&_peer, 0, sizeof(_peer));
    }
    ~Http() {}

    int Sock()
    {
        return _sock;
    }

    std::string BuildHttpRequest(std::string uri)
    {
        // 构建http请求报文
        // 1. 构建http请求行
        // GET uri http/1.1
        _request_line += REQUEST_METHOD;
        _request_line += BLACKSPACE;
        _request_line += uri;
        _request_line += BLACKSPACE;
        _request_line += HTTP_VERSION;
        _request_line += LF;
        // 2. 构建http请求报头
        // 空
        // 3. 构建空行
        _request_space = LF;
        // 组建http请求报文
        _request += _request_line;
        _request += _request_space;
        // 返回
        return _request;
    }

    void SendHttpResquest()
    {
        _peer.sin_family = AF_INET;
        _peer.sin_port = htons(_port);
        _peer.sin_addr.s_addr = inet_addr((Socket::DNSConvertion(_ip)).c_str());
        int ret = send(_sock, _request.c_str(), _request.size(), 0);
        if (ret < 0)
        {
            // TODO
        }
    }
    void RecvHttpResquest(std::string *out)
    {
        // int size = 0;
        // char buffer[1024] = {0};
        // while (size = recv(_sock, buffer, sizeof(buffer) - 1, 0) > 0)
        // {
        //     buffer[size] = 0;
        //     (*out) += buffer;
        //     buffer[0] = 0;
        // }
        // char buffer[120024] = {0};
        // ssize_t s = recv(_sock, buffer, sizeof(buffer) - 1, 0);
        // std::cout << "s: " << s << "=====" << buffer << std::endl;
        // char ch;
        // ssize_t s = 1;
        // while ((s = recv(_sock, &ch, 1, 0)))
        // {
        //     std::cout << "s:" << s << std::endl;
        //     (*out) += ch;
        //     std::cout << ch << std::endl;
        //     sleep(1);
        // }
        // std::cout << "=========####=========" << std::endl;
        while (true)
        {
            sleep(1);
            char buffer[1024] = {0};
            ssize_t s = recv(_sock, buffer, sizeof(buffer) - 1, 0);
            if (s > 0)
            {
                buffer[s] = '\0';
                (*out) += buffer;
                std::cout << (*out) << std::endl;
            }
            else
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {

                    std::cout << "EAGAIN" << std::endl;
                    break;
                }
                else if (errno == EINTR)
                {
                    continue;
                }
                else
                {
                    std::cerr << "errrrrr" << std::endl;
                }
            }
        }
        std::cout << (*out) << std::endl;
    }

private:
    std::string _ip;
    uint16_t _port = DEFAULT_PORT;
    int _sock;
    sockaddr_in _peer;

    std::string _request;
    std::string _request_line;
    std::string _request_header;
    std::string _request_space;
};