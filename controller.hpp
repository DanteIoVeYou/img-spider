#pragma once
#include "common.hpp"
#include "config.hpp"
#include "downloader.hpp"
#include "parser.hpp"

class Controller
{
public:
    Controller()
    {
    }
    ~Controller() {}
    // 读取配置文件

    // 输入URL种子
    // 下载器爬取页面
    // 保存页面+解析页面
    // 输入新的URL

    // 维护URL列表
    // 维护epoll模型

private:
    Config _config;         // 配置文件模块
    Downloader _downloader; // 下载模块
    Parser _parser;         // 页面处理模块

    std::unordered_set<Url> _url_set; // 已经抓取和待抓取的所有URL的集合
    std::queue<Url> _url_wait_queue;  // 未抓取的URL放入等待队列
};