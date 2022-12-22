#include <iostream>
#include <cmath>
#include <iomanip>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <regex>
#include <stack>
#include <queue>
#include <sstream>
#include <list>
#include <string>
#include <memory>
#include <set>
#include <functional>
#include <limits>
#include <algorithm>
#include <valarray>

typedef long unsigned int luint;
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


void pl(auto l)
{
    for (auto e : l)
        std::cout<<e.first<<":"<<e.second<<", ";
    std::cout<<'\n';
}

int get_insert_loc(int pos, int val, int lsize)
{
    int t_i = (pos + (val % lsize) + lsize) % lsize;
    return t_i;
}


long long int run(std::string const filename)
{
    std::ifstream ifs {filename};
    std::deque<std::pair<int, bool>> l;
    while (ifs.peek() != EOF)
    {
        std::string line;
        std::getline(ifs, line);
        l.push_back({std::stoi(line), false});
    }

    //pl(l);
    int moved = 0;
    int m_i = 0;
    while (moved < (int)l.size())
    {
        if(l[m_i].second)
        {
            ++m_i;
            continue;
        }
        int val = l[m_i].first;
        l.erase(l.begin() + m_i);
        int insert_i = get_insert_loc(m_i,val, (int)l.size());
        l.insert(l.begin() + insert_i, {{val, true}});
        if (insert_i <= m_i)
            ++m_i;
        ++moved;
        //pl(l);
    }

    int zero_i = 0;
    for(int i = 0; i<(int)l.size(); ++i)
        if(l[i].first == 0)
            zero_i = i;
    std::cout<<"zero_i : "<<zero_i <<'\n';

    return l[(zero_i+1000)%(int)l.size()].first + l[(zero_i+2000)%(int)l.size()].first + l[(zero_i+3000)%(int)l.size()].first;
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
}
