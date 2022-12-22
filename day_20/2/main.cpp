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
#include <unordered_set>
#include <functional>
#include <limits>
#include <algorithm>
#include <valarray>
#include <chrono>

typedef long unsigned int luint;
typedef long long int llint;
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


void pl(auto l)
{
    for (auto e : l)
        std::cout<<e.first<<":"<<e.second<<", ";
    std::cout<<'\n';
}

llint get_insert_loc(llint pos, llint val, llint lsize)
{
    llint t_i = (pos + (val % lsize) + lsize) % lsize;
    return t_i;
}


long long int run(std::string const filename)
{
    llint dk =  811589153;
    //llint dk = 1;
    int repeats = 10;
    //int repeats = 1;
    std::ifstream ifs {filename};
    //value,index in orig
    std::list<std::pair<llint,int>> l;
    std::vector<std::list<std::pair<llint,int>>::iterator> orig;
    int zero_i = 0;
    while (ifs.peek() != EOF)
    {
        std::string line;
        std::getline(ifs, line);
        llint val = std::stoll(line) * dk;
        std::pair<llint,int> el {val, orig.size()};
        orig.push_back(l.insert(l.end(),el));
        if (val == 0)
            zero_i = orig.size()-1;
    }

    //pl(l);
    for (int bleh=0; bleh<repeats; ++bleh)
    {
        for (auto & it : orig)
        {
            int m_i = std::distance(l.begin(), it);
            auto val = *it;
            l.erase(it);
            llint insert_i = get_insert_loc(m_i,val.first, l.size());
            auto iit = l.begin();
            for(int i = 0; i<insert_i; ++i)
                ++iit;
            it = l.insert(iit, val);
        }
        //pl(l);
    }

    int zero_i_l = std::distance(l.begin(), orig[zero_i]);
    std::cout<<"zero_i : "<<zero_i_l <<'\n';

    llint res = 0;
    for (int i = 1; i< 4; ++i)
    {
        llint thou = 1000*i;
        llint ind = (zero_i_l + thou) % (llint)l.size();
        auto it = l.begin();
        for (int i=0; i<ind; ++i)
            ++it;
        res += (*it).first;
    }


    return res;
}

int main(int argc, char** argv)
{
    auto start = std::chrono::steady_clock::now();
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}
