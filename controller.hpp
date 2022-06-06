#pragma once
#include "utils.hpp"
#include "epoll.hpp"
#include "http.hpp"
#include "downloader.hpp"
#include "memorizer.hpp"
// #include "config.hpp"
// #include "downloader.hpp"
// #include "parser.hpp"

class Controller
{
public:
    Controller(Url seed) : _seed(seed)
    {
        // 输入URL种子
        _url_map.insert({_seed._domain + _seed._path, _seed});
        _url_wait_queue.push(_seed);
    }
    ~Controller() {}

    void Start()
    {
        // while (!_url_wait_queue.empty())
        // {
        //     Url url = _url_wait_queue.front();
        //     _url_wait_queue.pop();
        //     std::string ip = url._ip;
        //     std::string domain = url._domain;
        //     std::string path = url._path;
        //     Http http(ip, domain, &_epoll);
        //     _epoll.Add(http, EPOLLOUT | EPOLLET); // 构建http请求
        //     _epoll.Dispatch(10, 1000, _page_storage);
        //     _epoll.Dispatch(10, 1000, _page_storage);
        //     std::cout << _page_storage.front() << std::endl;
        // }

        // while (true)
        // {
        //     if (!_url_wait_queue.empty())
        //     {
        //         Url url = _url_wait_queue.front();
        //         _url_wait_queue.pop();
        //         std::string ip = url._ip;
        //         std::string domain = url._domain;
        //         std::string path = url._path;
        //         Http http(ip, domain, &_epoll);
        //         _epoll.Add(http, EPOLLOUT | EPOLLET); // 添加写事件
        //     }
        //     else
        //     {
        //         int n = _epoll.Dispatch(10, 3000, _page_storage);
        //         if (n == 0)
        //         {
        //             std::cout << "all ev resolved.." << std::endl;
        //             break;
        //         }
        //         else
        //         {
        //             // for (auto &e : _page_storage)
        //             // {
        //             //     std::cout << e << std::endl;
        //             // }
        //             std::cout << "n: " << n << std::endl;
        //         }
        //     }
        // }

        while (true)
        {
            if (!_url_wait_queue.empty())
            {
                // 取出等待队列中的一个待抓取的URL
                Url url = _url_wait_queue.front();
                _url_wait_queue.pop();
                // 调度一个下载器进行下载
                Downloader downloader(url, &_epoll);
                downloader.PreDownload();
            }
            else
            {
                // int n = downloader.Download(10, 2000, _page_storage);
                int n = _epoll.Dispatch(10, 100, _page_storage);
                if (n == 0)
                {
                    std::cout << "all ev resolved.." << std::endl;
                    break;
                }
                else
                {
                    // for (auto &e : _page_storage)
                    // {
                    //     std::cout << e << std::endl;
                    // }
                    // 对于页面进行持久化处理
                    Memorizer::Save(_page_storage);
                    _page_storage.clear();
                    // 进行页面处理，提取更多的URL

                    std::cout << "n: " << n << std::endl;
                }
            }
        }
    }
    // 读取配置文件

    // 输入URL种子
    // 下载器爬取页面
    // 保存页面+解析页面
    // 输入新的URL

    // 维护URL列表
    // 维护epoll模型

private:
    Epoll _epoll;
    // Config _config;         // 配置文件模块
    // std::queue<Downloader> _downloaders; // 下载模块
    // Parser _parser;         // 页面处理模块
    Url _seed;
    std::unordered_map<std::string, Url> _url_map; // 已经抓取和待抓取的所有URL的集合
    std::queue<Url> _url_wait_queue;               // 未抓取的URL放入等待队列
    std::vector<std::string> _page_storage;        // 保存已经获取的响应的页面的数组
};