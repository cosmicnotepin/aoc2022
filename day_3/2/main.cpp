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

uint run(std::string filename)
{
    std::ifstream ifs {filename};
    if(!ifs)
    {
        std::cerr<<"could not open "<<filename<<'\n';
        exit(1);
    }

    uint sum_of_prios = 0;
    while (ifs.peek() != EOF)
    {
        std::vector<uint> shotmap(53, 0);
        for (uint i=0; i<3; ++i)
        {
            std::string s; 
            getline(ifs, s);
            for (char c : s)
            {
                if (shotmap[char2prio(c)] == i)
                    shotmap[char2prio(c)] += 1;
            }
        }
        for (size_t i=0; i<shotmap.size(); ++i)
        {
            if (shotmap[i] == 3)
            {
                sum_of_prios += i;
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
