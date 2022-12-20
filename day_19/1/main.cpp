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
#include <cstdint>

typedef long unsigned int luint;
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

struct State 
{
    State(int time, int ore_r, int next_op) : time(time), ore_r(ore_r), next_op(next_op) {};
    short int time = 0;
    short int geodes = 0;
    short int geodes_r = 0;
    short int obs = 0;
    short int obs_r = 0;
    short int clay = 0;
    short int clay_r = 0;
    short int ore = 0;
    short int ore_r = 0;
    short int next_op = 0;
    auto operator<=>(const State & rhs) const = default;
    //{
    //    auto ti = time<=>rhs.time;
    //    if (ti != 0)
    //        return ti;
    //    auto op = next_op<=>rhs.next_op;
    //    if (op != 0)
    //        return op;
    //    auto geod = geodes<=>rhs.geodes;  
    //    if (geod != 0)
    //        return geod; 
    //    auto geodes = geodes_r<=>rhs.geodes_r;
    //    if (geodes != 0)
    //        return geodes;
    //    auto ob = obs<=>rhs.obs;
    //    if(ob != 0)
    //        return ob; 
    //    auto obs_ = obs_r<=>rhs.obs_r;
    //    if(obs_ != 0)
    //        return obs_; 
    //    auto cla = clay<=>rhs.clay;
    //    if(cla != 0)
    //       return cla;
    //    auto clay_ = clay_r<=>rhs.clay_r;
    //    if(clay_ != 0)
    //       return clay_;
    //    auto or_ = ore<=>rhs.ore;
    //    if(or_ != 0)
    //       return or_;
    //    return ore_r<=>rhs.ore_r;
    //};
};


std::ostream & operator<<(std::ostream & os, State s)
{
    return os<<s.time<<", ";
}

std::vector<std::function<bool(State&)>> ops;

int dfs(State s)
{
    static std::map<State, int> tab;
    static int min_time = 24 - 14;
    auto old = tab.find(s);
    if ( old != tab.end())
        return (*old).second;
    State old_state {0,0,0};
    if (s.time >= min_time)
        old_state = s;
    if (ops[s.next_op](s))
    {
        int max = 0;
        for (int i = 0; i<(int)ops.size(); ++i)
        {
            s.next_op = i;
            max = std::max(max, dfs(s));
        }
        if (s.time >= min_time)
            tab[old_state] = max;
        return max;
    }
    if (s.time >= min_time)
        tab[old_state] = s.geodes;
    return s.geodes;
}

void mine(State &s)
{
    s.ore += s.ore_r;
    s.clay += s.clay_r;
    s.obs += s.obs_r;
    s.geodes += s.geodes_r;
    --s.time;
}

luint run(std::string const filename)
{
    std::regex pat {R"( \d+\D)"};
    std::smatch sm;
    std::ifstream ifs {filename};
    while (ifs.peek() != EOF)
    {
        std::string line;
        std::getline(ifs, line);
        auto vals = std::sregex_iterator(line.begin(), line.end(), pat);
        int bpnr = std::stoi((*vals++)[0]);
        std::cout<<"bpnr : "<<bpnr <<'\n';
        int ore_r_o = std::stoi((*vals++)[0]);
        std::cout<<"ore_r_o : "<<ore_r_o <<'\n';
        int clay_r_o = std::stoi((*vals++)[0]);
        std::cout<<"clay_r_o : "<<clay_r_o <<'\n';
        int obs_r_o = std::stoi((*vals++)[0]);
        std::cout<<"obs_r_o : "<<obs_r_o <<'\n';
        int obs_r_c = std::stoi((*vals++)[0]);
        std::cout<<"obs_r_c : "<<obs_r_c <<'\n';
        int geodes_r_o = std::stoi((*vals++)[0]);
        std::cout<<"geodes_r_o : "<<geodes_r_o <<'\n';
        int geodes_r_ob = std::stoi((*vals++)[0]);
        std::cout<<"geodes_r_ob : "<<geodes_r_ob <<'\n';
        auto wait = [](State & s)
        {
            if (s.time > 0)
            {
                mine(s);
                return true;
            }
            return false;
        };
        auto ore_r = [ore_r_o](State & s)
        {
            if (s.time > 0 && s.ore >= ore_r_o)
            {
                mine(s);
                s.ore -= ore_r_o;
                ++s.ore_r;
                return true;
            }
            return false;
        };
        auto clay_r = [ore_r_o, clay_r_o](State & s)
        {
            if (s.time > 0 && s.ore >= clay_r_o)
            {
                mine(s);
                s.ore -= clay_r_o;
                ++s.clay_r;
                return true;
            }
            return false;
        };
        auto obs_r = [obs_r_c, obs_r_o](State & s)
        {
            if (s.time > 0 && s.ore >= obs_r_o && s.clay >= obs_r_c)
            {
                mine(s);
                s.ore -= obs_r_o;
                s.clay -= obs_r_c;
                ++s.obs_r;
                return true;
            }
            return false;
        };
        auto geodes_r = [geodes_r_o, geodes_r_ob](State & s)
        {
            if (s.time > 0 && s.ore >= geodes_r_o && s.obs >= geodes_r_ob)
            {
                mine(s);
                s.ore -= geodes_r_o;
                s.obs -= geodes_r_ob;
                ++s.geodes_r;
                return true;
            }
            return false;
        };
        ops.push_back(geodes_r);
        ops.push_back(obs_r);
        ops.push_back(clay_r);
        ops.push_back(ore_r);
        ops.push_back(wait);
        break;
    }

    int ret = dfs(State(24,1, ops.size()-1));

    return ret;
}

int main(int argc, char** argv)
{
    unsigned long long int ram = 32000000000;
    int depth = 1;
    unsigned long long int usage = sizeof(State);
    std::cout<<"usage : "<<usage <<'\n';
    while (usage < ram)
    {
        usage *=  5;
        ++depth;
        std::cout<<"usage : "<<usage <<'\n';
    }
    std::cout<<"depth: "<<depth-1<<'\n';
    int res = 0;
    for (int i = 1; i<=24; ++i)
        res += i;
    std::cout<<"res: "<<res<<'\n';
    std::cout<<"sizeof(int): "<<sizeof(int)<<'\n';
    std::cout<<"std::numeric_limits<int>::max(): "<<std::numeric_limits<int>::max()<<'\n';
    std::cout<<"sizeof(std::int8_t): "<<sizeof(std::uint8_t)<<'\n';
    std::cout<<"std::numeric_limits<std::int8_t>::max(): "<<static_cast<int>(std::numeric_limits<std::uint8_t>::max())<<'\n';
    std::cout<<"sizeof(unsigned char): "<<sizeof(unsigned char)<<'\n';
    std::cout<<"std::numeric_limits<unsigned char>::max(): "<<std::numeric_limits<unsigned char>::max()<<'\n';
    std::cout<<"sizeof(short int): "<<sizeof(short int)<<'\n';
    std::cout<<"std::numeric_limits<short int>::max(): "<<std::numeric_limits<short int>::max()<<'\n';
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    //auto result = run("input");
    //std::cout<<"input result: "<<result<<'\n';
}
