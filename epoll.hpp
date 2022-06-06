#pragma once
#include "sock.hpp"
#include "http.hpp"

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

    int Dispatch(int num, int timeout, std::vector<std::string> &storage)
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
                    std::string out = _fd_http_map[sock]._response;
                    _fd_http_map[sock].RecvHttpResquest(&out);
                    _fd_http_map[sock]._r->Delete(_fd_http_map[sock]);
                    storage.push_back(std::move(out));
                }
                if (events & EPOLLOUT)
                {
                    _fd_http_map[sock].BuildHttpRequest("/");
                    _fd_http_map[sock].SendHttpResquest();
                    _fd_http_map[sock]._r->Modify(_fd_http_map[sock], EPOLLIN | EPOLLET);
                }
            }
        }
        delete[] event_arrays;
        return n;
    }

private:
    int _epfd;
    std::unordered_map<int, Http> _fd_http_map;
};