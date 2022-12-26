#include <iostream>
#include <map>
#include <fstream>
#include <regex>

typedef unsigned int uint;

void visualize(std::map<int, std::deque<char>> const stacks)
{
    for (auto stack : stacks) {
        for (auto crate : stack.second)
            std::cout<<crate;
        std::cout<<'\n';
    }
}

void run(std::string const filename) {
    std::ifstream ifs {filename};
    std::map<int, std::deque<char>> stacks;

    for (std::string line; getline(ifs, line);) {
        if (line[1] == '1')
            break;
        for (size_t i = 0; 1+4*i<line.size(); ++i)
            if(line[1+4*i] != ' ')
                stacks[i+1].insert(stacks[i+1].begin(),line[1+4*i]);
    }

    auto p2_stacks(stacks);

    std::regex moves_re {R"(move (\d+) from (\d+) to (\d+))"};
    std::smatch sm;
    std::string line_trash;
    getline(ifs, line_trash); //throw away empty line
    while (ifs.peek() != EOF)
    {
        std::string line;  
        getline(ifs, line);
        std::regex_match(line, sm, moves_re);
        int count = stoi(sm[1]);
        int from = stoi(sm[2]);
        int to = stoi(sm[3]);
        auto ff = p2_stacks[from].begin() + (p2_stacks[from].size() - count);
        auto ft = p2_stacks[from].end();
        p2_stacks[to].insert(p2_stacks[to].end(), ff, ft);
        p2_stacks[from].erase(ff, ft);
        while (count--)
        {
            stacks[to].push_back(stacks[from].back());
            stacks[from].pop_back();
        }
    }
    std::cout<<"p1: ";
    for (auto s : stacks)
        std::cout<<s.second.back();
    std::cout<<"\np2: ";
    for (auto s : p2_stacks)
        std::cout<<s.second.back();
    std::cout<<'\n';
}

int main(int argc, char** argv)
{
    run("input_t1");
    run("input");
}
