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
luint run(std::string const filename)
{
    std::ifstream ifs {filename};
    if(!ifs)
    {
        std::cerr<<"could not open "<<filename<<'\n';
        exit(1);
    }

    int res = 0;
    luint clock = 0;
    int x = 1;
    std::set<luint> indices_of_interest;
    for (luint i=20; i<221; i+=40)
        indices_of_interest.insert(i);

    while (ifs.peek() != EOF)
    {
        std::string line;
        std::getline(ifs, line);
        luint clock_increase = 0;
        if (line[0] == 'n')
            clock_increase = 1;
        else 
            clock_increase = 2;

        for (luint i = 0; i<clock_increase; ++i)
        {
            ++clock;
            if (indices_of_interest.contains(clock))
            {
                int to_add = clock * x;
                std::cout<<"to_add : "<<to_add <<'\n';
                res += to_add;
            }
        }

        if (line[0] == 'a')
            x += std::stoi(line.substr(5));
    }
    return res;
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
}
