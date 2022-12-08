#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <regex>
#include <stack>
#include <sstream>
#include <list>
#include <string>
#include <memory>

typedef long unsigned int luint;

luint run(std::string const filename)
{
    std::ifstream ifs {filename};
    if(!ifs)
    {
        std::cerr<<"could not open "<<filename<<'\n';
        exit(1);
    }

    std::vector<std::vector<std::pair<int, bool>>> forest;
    while (ifs.peek() != EOF)
    {
        std::string line;
        std::getline(ifs, line);
        std::vector<std::pair<int, bool>> row;
        for (char c : line)
        {
            std::pair<luint, bool> tree(std::stoi(std::string(1, c)), false);
            row.push_back(tree);
        }
        forest.push_back(row);
    }

    luint ss = 0;
    for (size_t i=0; i<forest.size(); ++i)
    {
        for (size_t j=0; j<forest[0].size(); ++j)
        {
            luint down = 0;
            for (size_t ii=i+1; ii<forest.size(); ++ii)
            {
                ++down;
                if (forest[i][j].first <= forest[ii][j].first)
                    break;
            }
            luint up = 0;
            for (size_t ii=i; ii>0; --ii)
            {
                ++up;
                if (forest[i][j].first <= forest[ii-1][j].first)
                    break;
            }
            luint right = 0;
            for (size_t jj=j+1; jj<forest[0].size(); ++jj)
            {
                ++right;
                if (forest[i][j].first <= forest[i][jj].first)
                    break;
            }
            luint left = 0;
            for (size_t jj=j; jj>0; --jj)
            {
                ++left;
                if (forest[i][j].first <= forest[i][jj-1].first)
                    break;
            }
            ss = std::max(ss, right*left*up*down);
        }
    }

    return ss;
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
}
