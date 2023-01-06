#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <unordered_set>
#include <chrono>
#include <future>

typedef long unsigned int luint;
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

enum res_i {re_ore,re_cla,re_obs,re_geo,re_no};
enum rob_i {ro_ore,ro_cla,ro_obs,ro_geo,ro_no};

struct State 
{
    State(int time, int ore_r) : time(time)
    {rob[ro_ore]=ore_r;};
    short int time = 0;
    short int res[re_no] = {0,0,0,0};
    short int rob[ro_no] = {0,0,0,0};
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
        return hash;
    }
};

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

auto get_build_robot_ops(std::vector<short int> res_i, std::vector<short int> res_d, short int rob_i, short int max_rob[ro_no])
{
    return [=](State & s)
    {
        if (s.time <= 0)
            return false;

        if (s.rob[rob_i] >= max_rob[rob_i])
            return false;

        for (int i = 0; i<(int)res_i.size(); ++i) {
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

void dfs(State s, short int & global_max_geodes, std::unordered_set<State, State_hash> & state_set, std::vector<std::function<bool(State&)>> & ops)
{
    if (s.time == 0) {
        global_max_geodes = std::max(global_max_geodes, s.res[re_geo]);
        return;
    }
    if(state_set.contains(s)) {
        return;
    }
    state_set.insert(s);
    if (upper_bound(s) <= global_max_geodes)
        return;

    for (int i = 0; i<(int)ops.size(); ++i) {
        State sc = s;
        if (ops[i](sc)) {
            dfs(sc, global_max_geodes, state_set, ops);
        }
    }
}


luint run(std::string const filename)
{
    std::regex pat {R"( \d+\D)"};
    std::smatch sm;
    std::ifstream ifs {filename};
    int qual_level_acc = 1;

    int count = 0;
    std::vector<std::future<short int>> futs;
    for (std::string line; std::getline(ifs, line);) {
        if (count++ == 3)
            break;
        auto loopcode = [line, pat]() {
            std::unordered_set<State, State_hash> state_set;
            std::vector<std::function<bool(State&)>> ops;
            short int global_max_geodes = 0;
            short int max_rob[ro_no];

            auto wait = [&global_max_geodes](State & s) {
                if (upper_bound(s) <= global_max_geodes)
                    return false;
                if (s.time == 0)
                    return false;
                mine(s);
                return true;
            };

            auto vals = std::sregex_iterator(line.begin(), line.end(), pat);
            short int blah = std::stoi((*vals++)[0]);
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
            ops.clear();
            ops.push_back(get_build_robot_ops({re_ore}, {ore_r_o}, ro_ore, max_rob));
            ops.push_back(get_build_robot_ops({re_ore}, {clay_r_o}, ro_cla, max_rob));
            ops.push_back(get_build_robot_ops({re_ore,re_cla}, {obs_r_o, obs_r_c}, ro_obs, max_rob));
            ops.push_back(get_build_robot_ops({re_ore,re_obs}, {geodes_r_o, geodes_r_ob}, ro_geo, max_rob));
            ops.push_back(wait);
            state_set.clear();
            global_max_geodes = 0;
            dfs(State(32,1), global_max_geodes, state_set, ops);
            std::cout<<"global_max_geodes: "<<global_max_geodes<<'\n';
            return global_max_geodes;
        };
        futs.emplace_back(std::async(loopcode));
    }
    for (auto & fut : futs)
        qual_level_acc *= fut.get();
    return qual_level_acc;
}

int main(int argc, char** argv)
{
    auto start = std::chrono::steady_clock::now();
    //auto test_result = run("input_t1");
    //std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}
