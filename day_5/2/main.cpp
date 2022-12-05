#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <regex>
#include <stack>
#include <sstream>
#include <list>

typedef unsigned int uint;

struct Move
{
        uint count;
        uint from;
        uint to;
};

void execute(std::list<Move> const & moves, std::vector<std::stack<char>>& stacks)
{
    for (Move m : moves)
    {
        std::stack<char> crane;
        for (uint i=0; i<m.count; ++i)
        {
            crane.push(stacks[m.from].top());
            stacks[m.from].pop();
        }
        for (uint i=0; i<m.count; ++i)
        {
            stacks[m.to].push(crane.top());
            crane.pop();
        }
    }
}

void visualize(std::vector<std::stack<char> > const v)
{
    for (auto s:v)
    {
        std::string line;
        while (s.size() > 0)
        {
            line += s.top();
            s.pop();
        }
        std::reverse(line.begin(), line.end());
        std::cout<<line<<'\n';
    }
}


std::string run(std::string const filename)
{
    std::ifstream ifs {filename};
    if(!ifs)
    {
        std::cerr<<"could not open "<<filename<<'\n';
        exit(1);
    }

    std::stack<std::string> crates;
    uint number_of_stacks = 0;
    while (ifs.peek() != EOF)
    {
        std::string line; 
        getline(ifs, line);
        if (line[1] == '1')
        {
            std::istringstream iss(line);
            std::string item;
            while (std::getline(iss>>std::ws, item, ' '))
                ;
            number_of_stacks = stoul(item);
            break;
        }
        crates.push(line);
    }

    std::vector<std::stack<char>> stacks(number_of_stacks);
    while (crates.size() > 0)
    {
        auto line = crates.top();
        crates.pop();
        for (uint i=0; i<number_of_stacks; ++i)
        {
            char crate = line[i*4 + 1];
            if (crate != ' ')
                stacks[i].push(crate);
        }

    }
    visualize(stacks);


    std::regex moves_re {R"(move (\d+) from (\d+) to (\d+))"};
    std::smatch sm;

    std::list<Move> moves;
    std::string line_trash;
    getline(ifs, line_trash); //throw away empty line
    while (ifs.peek() != EOF)
    {
        std::string line;  
        getline(ifs, line);
        std::regex_match(line, sm, moves_re);
        moves.push_back(Move {.count = (uint)stoul(sm[1]), 
                              .from = (uint)stoul(sm[2]) - 1, 
                              .to = (uint)stoul(sm[3]) - 1});
    }
    execute(moves, stacks);
    visualize(stacks);
    std::string ret;
    for (auto s:stacks)
    {
        if(s.size() > 0)
            ret += s.top();
    }
    return ret;
}

int main(int argc, char** argv)
{
    //std::string blah = "1 2 3 4 5 6 ";
    //std::regex pat {R"((\d )*)"};
    //std::smatch sm;
    //std::regex_match(blah, sm, pat);
    //for (auto blah : sm)
    //    std::cout<<blah<<'\n';

    std::string test_result = run("input_t1");
    std::cout<<"input_t1 score: "<<test_result<<'\n';
    std::string result = run("input");
    std::cout<<"input score: "<<result<<'\n';
}
