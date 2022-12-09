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

void follow(std::vector<int>& head, std::vector<int>& tail)
{
    int xdiff = head[0] - tail[0];
    int ydiff = head[1] - tail[1];

    if (std::abs(xdiff) == 2 && std::abs(ydiff) == 2)
    {
        tail[0] += xdiff/2;
        tail[1] += ydiff/2;
    }
    else if (std::abs(xdiff) == 2)
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

void move(char dir, std::vector<int>& head)
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
}

luint run(std::string const filename)
{
    std::ifstream ifs {filename};
    if(!ifs)
    {
        std::cerr<<"could not open "<<filename<<'\n';
        exit(1);
    }
    std::vector<std::vector<int>> rope;
    size_t rope_size = 10;
    rope.reserve(rope_size);
    for (size_t i = 0; i<rope_size; ++i)
        rope.push_back(std::vector<int> {0, 0});

    std::set<std::vector<int>> tail_positions;
    tail_positions.insert(std::vector<int>(rope[rope.size()-1]));

    while (ifs.peek() != EOF)
    {
        std::string line;
        std::getline(ifs, line);
        for (luint i=0; i<std::stoul(line.substr(2,2)); ++i)
        {
            move(line[0], rope[0]);
            for (size_t i=0; i<rope.size()-1; ++i)
                follow(rope[i], rope[i+1]);
            tail_positions.insert(std::vector<int>(rope[rope.size()-1]));
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
