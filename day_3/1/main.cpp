//import std;

#include <iostream>
#include <unordered_set>
#include <set>
#include <vector>
#include <fstream>
#include <algorithm>

typedef unsigned int uint;

uint char2prio(char c)
{
    if ((int)c >= 'a')
        return (int)c - 'a' + 1;
    else
        return (int)c - 'A' + 26 + 1;
}

unsigned int run(std::string filename)
{
    std::ifstream ifs {filename};
    uint sum_of_prios = 0;
    for (std::string s; getline(ifs, s);)
    {
        int x = s.size()/2;
        std::set<char> s1(s.begin(), s.begin()+x);
        std::set<char> s2(s.begin()+x, s.end());
        std::vector<char> res;
        //only works as expected on _sorted_ sets!!
        std::set_intersection(s1.begin(), s1.end(), 
                s2.begin(), s2.end(), std::back_inserter(res));
        sum_of_prios += char2prio(*(res.begin()));
    }
    return sum_of_prios;
}

int main(int argc, char** argv)
{
    std::cout<<"input_t1 score: "<<run("input_t1")<<'\n';
    std::cout<<"input score: "<<run("input")<<'\n';
}
