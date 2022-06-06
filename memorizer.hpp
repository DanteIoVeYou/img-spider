#pragma once
#include "header.hpp"
#include "parser.hpp"
class Memorizer
{
public:
    Memorizer() {}
    ~Memorizer() {}
    static void Save(std::vector<std::string> &pages)
    {
        for (auto &page : pages)
        {
            std::string filename;
            bool is_find_title = Parser::GetTitle(page, filename);
            if (is_find_title == true)
            {
                filename += ".html";
                int fd = open(filename.c_str(), O_WRONLY | O_CREAT, 0644);
                if (fd > 0)
                {
                    Parser::StartFromHtml(page);
                    int cnt = write(fd, page.c_str(), page.size());
                    if (cnt > 0)
                    {
                        std::cout << "write success" << std::endl;
                    }
                }
                else
                {
                    // TODO
                }
                close(fd);
            }
        }
    }

private:
};