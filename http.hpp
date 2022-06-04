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
        _request_space += LF;
        // 组建http请求报文
        _request += _request_header;
        _request += _request_space;
        // 返回
        return std::move(_request);
    }

    void SendHttpResquest()
    {
        int ret = send(_sock, _request.c_str(), _request.size(), 0);
        if (ret < 0)
        {
            // TODO
        }
    }
    void RecvHttpResquest()
    {
        
    }

private:
    std::string _ip;
    uint16_t _port = DEFAULT_PORT;
    int _sock;
    std::string _request;
    std::string _request_line;
    std::string _request_header;
    std::string _request_space;
};