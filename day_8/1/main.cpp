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

    std::cout<<"forest.size(): "<<forest.size()<<'\n';
    std::cout<<"forest.size(): "<<forest[0].size()<<'\n';
    for (size_t i=0; i<forest.size(); ++i)
    {
        int max = -1;
        for (size_t j=0; j<forest[0].size(); ++j)
        {
            if (forest[i][j].first > max)
            {
                forest[i][j].second = true;
                max = forest[i][j].first;
            }
        }
        max = -1;
        for (size_t j=forest[0].size(); j>0; --j)
        {
            if (forest[i][j-1].first > max)
            {
                forest[i][j-1].second = true;
                max = forest[i][j-1].first;
            }
        }
    }

    for (size_t i=0; i<forest[0].size(); ++i)
    {
        int max = -1;
        for (size_t j=0; j<forest.size(); ++j)
        {
            if (forest[j][i].first > max)
            {
                forest[j][i].second = true;
                max = forest[j][i].first;
            }
        }
        max = -1;
        for (size_t j=forest.size(); j>0; --j)
        {
            if (forest[j-1][i].first > max)
            {
                forest[j-1][i].second = true;
                max = forest[j-1][i].first;
            }
        }
    }

    luint sum = 0;
    for (size_t i=0; i<forest.size(); ++i)
    {
        for (size_t j=0; j<forest[0].size(); ++j)
        {
            if (forest[i][j].second)
                ++sum;
        }
    }
    return sum;
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
}
