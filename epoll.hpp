#pragma once
#include "sock.hpp"
#include "http.hpp"
// static const uint16_t DEFAULT_PORT = 80;
// static const std::string REQUEST_METHOD = "GET";
// static const std::string HTTP_VERSION = "HTTP/1.1";
// static const std::string LF = "\n";
// static const std::string BLACKSPACE = " ";

// /*
//  * 实例化http类的同时会
//  * 1. 创建套接字
//  * 2. 三次握手
//  * 接口一： 构建http请求报文
//  * 接口二： 返回套接字
//  */

// // 封装了http请求报文、响应报文、构建方法、发送方法、接受方法
// class Epoll;
// class Http;

// class Http
// {
// public:
//     Http() {}
//     Http(std::string ip, std::string domain, Epoll *ep) : _ip(ip), _domain(domain), _r(ep)
//     {
//         // 初始化
//         _sock = Socket::CreateSocket();
//         Socket::Connect(_ip, _port, _sock);
//         Socket::SetNoBlock(_sock);
//         memset(&_peer, 0, sizeof(_peer));
//     }
//     ~Http() {}

//     int Sock() const
//     {
//         return _sock;
//     }

//     std::string BuildHttpRequest(std::string uri)
//     {
//         // 构建http请求报文
//         // 1. 构建http请求行
//         // GET uri http/1.1
//         _request_line += REQUEST_METHOD;
//         _request_line += BLACKSPACE;
//         _request_line += uri;
//         _request_line += BLACKSPACE;
//         _request_line += HTTP_VERSION;
//         _request_line += LF;
//         // 2. 构建http请求报头
//         // 空
//         // 3. 构建空行
//         _request_space = LF;
//         // 组建http请求报文
//         _request += _request_line;
//         _request += _request_space;
//         // 返回
//         return _request;
//     }

//     void SendHttpResquest()
//     {
//         _peer.sin_family = AF_INET;
//         _peer.sin_port = htons(_port);
//         _peer.sin_addr.s_addr = inet_addr(_ip.c_str());
//         int ret = send(_sock, _request.c_str(), _request.size(), 0);
//         if (ret < 0)
//         {
//             // TODO
//         }
//     }
//     void RecvHttpResquest(std::string *out)
//     {
//         while (true)
//         {
//             char buffer[1000000] = {0};
//             ssize_t s = recv(_sock, buffer, sizeof(buffer) - 1, 0);
//             if (s > 0)
//             {
//                 buffer[s] = '\0';
//                 (*out) += buffer;
//             }
//             else
//             {
//                 if (errno == EAGAIN || errno == EWOULDBLOCK)
//                 {
//                     std::cout << "EAGAIN" << std::endl;
//                     break;
//                 }
//                 else if (errno == EINTR)
//                 {
//                     continue;
//                 }
//                 else
//                 {
//                     std::cerr << "errrrrr" << std::endl;
//                 }
//             }
//         }
//         // std::cout << (*out) << std::endl;
//     }

// public:
//     std::string _response;
//     Epoll *_r;

// private:
//     std::string _ip;
//     std::string _domain;
//     uint16_t _port = DEFAULT_PORT;
//     int _sock;
//     sockaddr_in _peer;

//     std::string _request;
//     std::string _request_line;
//     std::string _request_header;
//     std::string _request_space;
// };

class Epoll
{
public:
    Epoll()
    {
        _epfd = epoll_create(128);
        if (_epfd < 0)
        {
            std::cerr << "epoll create error" << std::endl;
            exit(1);
        }
    }

    ~Epoll() {}

    void Add(const Http &http, uint32_t events)
    {
        int sock = http.Sock();
        struct epoll_event ev;
        ev.events = events;
        ev.data.fd = sock;
        int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, sock, &ev);
        if (ret < 0)
        {
            std::cerr << "epoll ADD error" << std::endl;
            exit(1);
        }
        else
        {
            _fd_http_map.insert({sock, http});
        }
    }

    void Modify(const Http &http, uint32_t events)
    {
        int sock = http.Sock();
        struct epoll_event ev;
        ev.events = events;
        ev.data.fd = sock;
        int ret = epoll_ctl(_epfd, EPOLL_CTL_MOD, sock, &ev);
        if (ret < 0)
        {
            std::cerr << "epoll MOD error" << std::endl;
            exit(1);
        }
    }

    void Delete(const Http &http)
    {
        int sock = http.Sock();
        int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, sock, nullptr);
        if (ret < 0)
        {
            std::cerr << "epoll DEL error" << std::endl;
            exit(1);
        }
        else
        {
            auto pos = _fd_http_map.find(sock);
            if (pos != _fd_http_map.end())
            {
                _fd_http_map.erase(pos);
            }
        }
    }

    void Dispatch(int num, int timeout)
    {
        struct epoll_event *event_arrays = new epoll_event[num];
        int n = epoll_wait(_epfd, event_arrays, num, timeout);
        for (int i = 0; i < num; i++)
        {

            int sock = event_arrays[i].data.fd;
            int events = event_arrays[i].events;
            if (_fd_http_map.find(sock) != _fd_http_map.end())
            {
                if (events & EPOLLIN)
                {
                    std::string &out = _fd_http_map[sock]._response;
                    _fd_http_map[sock].RecvHttpResquest(&out);
                    // std::cout << out << std::endl;
                    _fd_http_map[sock]._r->Delete(_fd_http_map[sock]);
                }
                if (events & EPOLLOUT)
                {
                    _fd_http_map[sock].BuildHttpRequest("/");
                    _fd_http_map[sock].SendHttpResquest();
                    _fd_http_map[sock]._r->Modify(_fd_http_map[sock], EPOLLIN);
                }
            }
        }
        delete[] event_arrays;
    }

private:
    int _epfd;
    std::unordered_map<int, Http> _fd_http_map;
};