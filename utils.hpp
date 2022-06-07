#pragma once
#include "header.hpp"

class Utils
{
public:
    static inline std::string DNSConvertion(std::string domain)
    {
        struct hostent *host = gethostbyname(domain.c_str());
        std::cout << "###" << domain << "###" << std::endl;
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
};

class Url
{
public:
    Url(std::string domain, std::string path = "/") : _domain(domain), _depth(1)
    {
        _ip = Utils::DNSConvertion(_domain);
    }
    ~Url() {}

public:
    std::string _domain;
    std::string _ip;
    std::string _path;
    int _depth;
};