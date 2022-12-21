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
#include <unordered_set>
#include <functional>
#include <limits>
#include <algorithm>
#include <valarray>
#include <cstdint>
#include <chrono>

typedef long unsigned int luint;
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

enum res_i {re_ore,re_cla,re_obs,re_geo,re_no};
enum rob_i {ro_ore,ro_cla,ro_obs,ro_geo,ro_no};

struct State 
{
    State(int time, int ore_r, int next_op) : time(time), next_op(next_op)
    {rob[ro_ore]=ore_r;};
    short int time = 0;
    short int res[re_no] = {0,0,0,0};
    short int rob[ro_no] = {0,0,0,0};
    short int next_op = 0;
    auto operator<=>(const State & rhs) const = default;
};

struct State_hash
{
    size_t operator()(const State & s) const
    {
        size_t hash = 17;
        hash = hash*31 + s.time;
        for (auto e : s.res)
            hash = hash*31 + e;
        for (auto e : s.rob)
            hash = hash*31 + e;
        hash = hash*31 + s.next_op;
        return hash;
    }
};

std::unordered_set<State, State_hash> state_set;
std::vector<std::function<bool(State&)>> ops;
short int global_max_geodes = 0;
short int max_rob[ro_no];

std::ostream & operator<<(std::ostream & os, State s)
{
    os<<"S time:"<<s.time<<", ";
    os<<"res:[";
    for (auto r : s.res)
        os<<r<<", ";
    os<<"], ";
    os<<"rob:[";
    for (auto r : s.rob)
        os<<r<<", ";
    os<<"], ";
    return os<<"next_op:"<<s.next_op<<"\n";
}


void mine(State &s)
{
    for (int i=0; i<ro_no; ++i)
        s.res[i] += s.rob[i];
    --s.time;
}

short int upper_bound(State & s)
{
    short int max_geodes = s.res[re_geo];
    short int curr_geo_robs = s.rob[ro_geo];
    for (int i = s.time; i>=0; --i)
        max_geodes += curr_geo_robs++;

    return max_geodes;
}

auto get_build_robot_ops(std::vector<short int> res_i, std::vector<short int> res_d, short int rob_i)
{
    return [=](State & s)
    {
        if (s.time <= 0)
            return false;
        if (s.rob[rob_i] >= max_rob[rob_i])
        {
            return false;
        }
        for (int i = 0; i<(int)res_i.size(); ++i)
        {
            if (s.res[res_i[i]] < res_d[i])
                return false;
        }

        for (int i = 0; i<(int)res_i.size(); ++i)
            s.res[res_i[i]] -= res_d[i];

        mine(s);
        ++s.rob[rob_i];
        return true;
    };
}
void dfs(State s)
{
    //static std::map<State, int> tab;
    if (s.time == 0)
    {
        global_max_geodes = std::max(global_max_geodes, s.res[re_geo]);
        return;
    }
    if(state_set.contains(s))
        return;
    state_set.insert(s);
    if (upper_bound(s) <= global_max_geodes)
        return;
    //static int min_time = 24 - 14;
    //auto old = tab.find(s);
    //if ( old != tab.end())
    //    return (*old).second;
    //State old_state {0,0,0};
    //if (s.time >= min_time)
    //    old_state = s;
    for (int i = 0; i<(int)ops.size(); ++i)
    {
        State sc = s;
        if (ops[s.next_op](sc))
        {
            {
                sc.next_op = i;
                dfs(sc);
            }
            //if (s.time >= min_time)
            //    tab[old_state] = max;
        }
    }
    //if (s.time >= min_time)
    //    tab[old_state] = s.geodes;
}


luint run(std::string const filename, int max_bps)
{
    std::regex pat {R"( \d+\D)"};
    std::smatch sm;
    std::ifstream ifs {filename};
    auto wait = [](State & s)
    {
        if (upper_bound(s) <= global_max_geodes)
            return false;
        if (s.time == 0)
            return false;
        mine(s);
        return true;
    };
    int qual_level_acc = 1;
    int bps_done = 0;
    while (ifs.peek() != EOF && bps_done < max_bps)
    {
        ++bps_done;
        std::string line;
        std::getline(ifs, line);
        auto vals = std::sregex_iterator(line.begin(), line.end(), pat);
        short int bpnr = std::stoi((*vals++)[0]);
        short int ore_r_o = std::stoi((*vals++)[0]);
        short int clay_r_o = std::stoi((*vals++)[0]);
        short int obs_r_o = std::stoi((*vals++)[0]);
        short int obs_r_c = std::stoi((*vals++)[0]);
        short int geodes_r_o = std::stoi((*vals++)[0]);
        short int geodes_r_ob = std::stoi((*vals++)[0]);

        max_rob[ro_ore] = std::max({ore_r_o, clay_r_o, obs_r_o, geodes_r_o});
        max_rob[ro_cla] = obs_r_c;
        max_rob[ro_obs] = geodes_r_ob;
        max_rob[ro_geo] = std::numeric_limits<short int>::max();
        std::cout<<"bpnr : "<<bpnr <<'\n';
        std::cout<<"ore_r_o : "<<ore_r_o <<'\n';
        std::cout<<"clay_r_o : "<<clay_r_o <<'\n';
        std::cout<<"obs_r_o : "<<obs_r_o <<'\n';
        std::cout<<"obs_r_c : "<<obs_r_c <<'\n';
        std::cout<<"geodes_r_o : "<<geodes_r_o <<'\n';
        std::cout<<"geodes_r_ob : "<<geodes_r_ob <<'\n';
        ops.clear();
        ops.push_back(get_build_robot_ops({re_ore}, {ore_r_o}, ro_ore));
        ops.push_back(get_build_robot_ops({re_ore}, {clay_r_o}, ro_cla));
        ops.push_back(get_build_robot_ops({re_ore,re_cla}, {obs_r_o, obs_r_c}, ro_obs));
        ops.push_back(get_build_robot_ops({re_ore,re_obs}, {geodes_r_o, geodes_r_ob}, ro_geo));
        ops.push_back(wait);
        global_max_geodes = 0;
        state_set.clear();
        auto start = std::chrono::steady_clock::now();
        dfs(State(32,1, ops.size()-1));
        qual_level_acc *= global_max_geodes;
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    }


    return qual_level_acc;
}

int main(int argc, char** argv)
{
    // State s1(24,1, ops.size()-1);
    // State s2(24,2, ops.size()-1);
    // State s3(s2);
    // s2.rob[0] = 3;
    //unsigned long long int ram = 32000000000;
    //int depth = 1;
    //unsigned long long int usage = sizeof(State);
    //std::cout<<"usage : "<<usage <<'\n';
    //while (usage < ram)
    //{
    //    usage *=  5;
    //    ++depth;
    //    std::cout<<"usage : "<<usage <<'\n';
    //}
    //std::cout<<"depth: "<<depth-1<<'\n';
    //int res = 0;
    //for (int i = 1; i<=24; ++i)
    //    res += i;
    //std::cout<<"res: "<<res<<'\n';
    //std::cout<<"sizeof(int): "<<sizeof(int)<<'\n';
    //std::cout<<"std::numeric_limits<int>::max(): "<<std::numeric_limits<int>::max()<<'\n';
    //std::cout<<"sizeof(std::int8_t): "<<sizeof(std::uint8_t)<<'\n';
    //std::cout<<"std::numeric_limits<std::int8_t>::max(): "<<static_cast<int>(std::numeric_limits<std::uint8_t>::max())<<'\n';
    //std::cout<<"sizeof(unsigned char): "<<sizeof(unsigned char)<<'\n';
    //std::cout<<"std::numeric_limits<unsigned char>::max(): "<<std::numeric_limits<unsigned char>::max()<<'\n';
    //std::cout<<"sizeof(short int): "<<sizeof(short int)<<'\n';
    //std::cout<<"std::numeric_limits<short int>::max(): "<<std::numeric_limits<short int>::max()<<'\n';
    //auto test_result = run("input_t1", 2);
    //std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input", 3);
    std::cout<<"input result: "<<result<<'\n';
}
