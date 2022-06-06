#pragma once

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

private:
};