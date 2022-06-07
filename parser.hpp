#pragma once
#include "utils.hpp"
class Parser
{
public:
    Parser() {}
    ~Parser() {}
    static bool GetTitle(const std::string &page, std::string &filename)
    {
        auto begin = page.find("<title>");
        if (begin != page.npos)
        {
            // 找到了<title>标签
            begin = begin + strlen("<title>");
        }
        else
        {
            return false;
        }
        auto end = page.find("</title>");
        if (end == page.npos)
        {
            return false;
        }
        filename = "pages/";
        filename += page.substr(begin, end - begin);
        return true;
    }

    static bool GetALable(std::vector<std::string> &pages, std::unordered_map<std::string, Url> &url_map, std::queue<Url> &url_wait_queue)
    {
        for (auto &page : pages)
        {
            size_t begin = 0;
            while (true)
            {
                std::cout << "get a <a> lable" << std::endl;
                sleep(1);
                begin = page.find("<a href=\"", begin);
                if (begin != page.npos)
                {
                    // std::cout << "<a href=" << std::endl;
                    // std::cout << "begin: " << begin << std::endl;

                    begin += strlen("<a href=\"");
                    size_t end = page.find("\"", begin);
                    if (end != page.npos)
                    {
                        std::cout << "begin: " << begin << std::endl;
                        std::cout << "end: " << end << std::endl;
                        size_t len = end - begin;
                        std::cout << "len: " << len << std::endl;

                        std::string str = page.substr(begin, len);
                        std::cout << "str: " << str << std::endl;
                        begin = end; // 对于起始寻找a标签的位置进行迭代
                        std::string domain;
                        std::string path;
                        ParseDomainAndPath(str, &domain, &path);
                        Url url(domain, path);
                        std::cout << "domain: " << url._domain << std::endl;
                        if (url_map.find(domain + path) == url_map.end())
                        {
                            url_map.insert({domain + path, url});
                            url_wait_queue.push(url);
                        }
                        else
                        {
                            std::cout << "not insert map" << std::endl;
                            continue;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    std::cerr << "<a> not found..." << std::endl;
                    break;
                }
            }
        }
        return true;
    }

    static bool StartFromHtml(std::string &page)
    {
        auto pos = page.find("<!DOCTYPE html>");
        if (pos != page.npos)
        {
            page.erase(0, pos);
        }
        else
        {
            return false;
        }
        return true;
    }

    static bool ParseDomainAndPath(const std::string &url, std::string *domain, std::string *path)
    {
        // auto begin = url.find("://"); // 请求的页面a标签没有http:
        auto begin = url.find("//");
        if (begin != url.npos)
        {
            // begin += strlen("://");
            begin += strlen("//");
            auto end = url.find("/", begin);
            if (end != url.npos)
            {
                size_t len = end - begin;
                (*domain) = url.substr(begin, len);
                (*path) = url.substr(end);
            }
            else
            {
                // 访问的是网站根目录
                (*domain) = url.substr(begin);
                (*path) = "/";
            }
            std::cout << "domain0: " << domain << std::endl;
        }
        else
        {
            std::cerr << ":// not found" << std::endl;
            return false;
        }
        return true;
    }
};