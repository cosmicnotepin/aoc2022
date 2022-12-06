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


uint run(std::string const filename)
{
    std::ifstream ifs {filename};
    if(!ifs)
    {
        std::cerr<<"could not open "<<filename<<'\n';
        exit(1);
    }

    while (ifs.peek() != EOF)
    {
        std::string line; 
        getline(ifs, line);
        for (uint i=0; i<line.size()-14; ++i)
        {
            for (uint j=0; j<14; ++j)
            {
                for (uint k=0; k<14; ++k)
                {
                    if (j!=k && line[i+j] == line[i+k])
                        goto next_pos_in_line;
                }
            }
            return i + 13 + 1; //index + 3: index of last element of 4-Block starting at i, +1 because of 1-based counting
            next_pos_in_line:;
        }
    }
    return 0;
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
}
