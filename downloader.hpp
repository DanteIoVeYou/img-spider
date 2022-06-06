#pragma once

#include "http.hpp"
#include "utils.hpp"
// #include "epoll.hpp"
class Epoll;
class Downloader
{
public:
    Downloader(const Url &url, Epoll *ep) : _url(url), _r(ep), _http(_url._ip, _url._domain, _r) {}

    void PreDownload()
    {
        _r->Add(_http, EPOLLOUT | EPOLLET); // 添加写事件
    }

    // int Download(int num, int timeout, std::vector<std::string> &storage)
    // {
    //     return _r->Dispatch(num, timeout, storage);
    // }
    ~Downloader() {}

private:
    Url _url;

public:
    Epoll *_r;
    Http _http;
};