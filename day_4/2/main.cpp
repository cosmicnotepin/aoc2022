#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <regex>

typedef unsigned int uint;

uint char2prio(char c)
{
    if ((int)c > 96)
        return (int)c - 96;
    else
        return (int)c - 64 + 26;
}

uint run(std::string filename)
{
    std::ifstream ifs {filename};
    if(!ifs)
    {
        std::cerr<<"could not open "<<filename<<'\n';
        exit(1);
    }

    uint number_contains = 0;
    std::regex pat {R"((\d+)-(\d+),(\d+)-(\d+))"};
    std::smatch sm;

    while (ifs.peek() != EOF)
    {
        std::string line; 
        getline(ifs, line);
        std::regex_match(line, sm, pat);
        uint l1 = stoul(sm[1]);
        uint h1 = stoul(sm[2]);
        uint l2 = stoul(sm[3]);
        uint h2 = stoul(sm[4]);
        if (!(l1>h2 || l2>h1))
            ++number_contains;
    }
    return number_contains;
}

int main(int argc, char** argv)
{
    uint test_result = run("input_t1");
    std::cout<<"input_t1 score: "<<test_result<<'\n';
    uint result = run("input");
    std::cout<<"input score: "<<result<<'\n';
}
