//import std;

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>

typedef unsigned int uint;

uint char2prio(char c)
{
    if ((int)c > 96)
        return (int)c - 96;
    else
        return (int)c - 64 + 26;
}

unsigned int run(std::string filename)
{
    std::ifstream ifs {filename};
    if(!ifs)
    {
        std::cerr<<"could not open "<<filename<<'\n';
        exit(1);
    }
    uint sum_of_prios = 0;
    for (std::string s; getline(ifs, s);)
    {
        std::vector<bool> shotmap(52, false);
        for(size_t i=0; i<s.size()/2; ++i)
            shotmap[char2prio(s[i])] = true;

        for(size_t i=s.size()/2; i<s.size(); ++i)
        {
            if (shotmap[char2prio(s[i])] == true)
            {
                sum_of_prios += char2prio(s[i]);
                break;
            }
        }
    }
    return sum_of_prios;
}

int main(int argc, char** argv)
{
    std::cout<<"input_t1 score: "<<run("input_t1")<<'\n';
    std::cout<<"input score: "<<run("input")<<'\n';
}
