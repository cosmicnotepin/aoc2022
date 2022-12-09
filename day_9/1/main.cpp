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
#include <set>

typedef long unsigned int luint;

void move(char dir, std::vector<int>& head, std::vector<int>& tail)
{
    switch (dir)
    {
        case 'U':
            ++head[1];
            break;
        case 'D':
            --head[1];
            break;
        case 'R':
            ++head[0];
            break;
        case 'L':
            --head[0];
            break;
    }

    int xdiff = head[0] - tail[0];
    int ydiff = head[1] - tail[1];

    if (std::abs(xdiff) == 2)
    {
        tail[0] += xdiff/2;
        tail[1] += ydiff;
    }
    else if (std::abs(ydiff) == 2)
    {
        tail[1] += ydiff/2;
        tail[0] += xdiff;
    }
}

luint run(std::string const filename)
{
    std::ifstream ifs {filename};
    if(!ifs)
    {
        std::cerr<<"could not open "<<filename<<'\n';
        exit(1);
    }
    std::vector<int> head = {0, 0};
    std::vector<int> tail = {0, 0};
    std::set<std::vector<int>> tail_positions;
    tail_positions.insert(std::vector<int>(tail));
    while (ifs.peek() != EOF)
    {
        std::string line;
        std::getline(ifs, line);
        for (luint i=0; i<std::stoul(line.substr(2,2)); ++i)
        {
            move(line[0], head, tail);
            tail_positions.insert(std::vector<int>(tail));
        }
    }
    return tail_positions.size();
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
}
