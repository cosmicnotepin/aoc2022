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
    std::string line;
    getline(ifs, line);
    uint ml = 4;
    for (uint i=0; i<line.size()-ml; ++i) {
        for (uint j=0; j<ml; ++j)
            for (uint k=0; k<ml; ++k)
                if (j!=k && line[i+j] == line[i+k])
                    goto next_pos_in_line;
        std::cout<<"p1: "<< i + ml<<'\n'; 
        break;
next_pos_in_line:;
    }
    ml = 14;
    for (uint i=0; i<line.size()-ml; ++i) {
        for (uint j=0; j<ml; ++j)
            for (uint k=0; k<ml; ++k)
                if (j!=k && line[i+j] == line[i+k])
                    goto next_pos_in_line_p2;
        std::cout<<"p2: "<< i + ml<<'\n'; 
        break;
next_pos_in_line_p2:;
    }
    return 0;
}

int main(int argc, char** argv)
{
    run("input_t1");
    run("input");
}
