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

typedef long unsigned int luint;

std::map<std::string, size_t> name2index;
std::map<size_t, std::string> index2name;
std::vector<luint> vfs;
std::vector<bool> vvss;
std::vector<std::vector<std::vector<luint>>> vbrsf;
std::vector<std::vector<luint>> vcs;
luint full_time = 30;

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

luint dfs(luint target, luint time, luint res)
{
    if (time == 0)
        return res;

    for (size_t i = full_time; i >= time; --i)
    {
        if (vbrsf[i][target][0] != 0 && vbrsf[i][target][0] >= res) 
            return 0;
    }

    vbrsf[time][target][0] = res;

    luint max = 0;

    if (!vvss[target] && vfs[target] > 0)
    {
        vvss[target] = true;
        luint tres = res + (time-1) * vfs[target];
        max = std::max(max, dfs(target,time-1,tres));
        vvss[target] = false;
    }

    for (luint & c : vcs[target])
        max = std::max(max, dfs(c,time-1,res));

    return max;
}

luint run(std::string const filename)
{
    std::ifstream ifs {filename};
    if(!ifs)
    {
        std::cerr<<"could not open "<<filename<<'\n';
        exit(1);
    }

    std::regex pat {R"(Valve (..) has flow rate=(\d+); tunnels? leads? to valves? (.*))"};
    std::smatch sm;
    std::regex pat2 {R"((\S\S),?)"};
    std::smatch sm2;
    std::map<std::string, std::vector<std::string>> cs;
    std::map<std::string, luint> fs;
    std::map< std::string, std::map< luint, luint>> brsf;
    std::map<std::string, bool> vss;

    while (ifs.peek() != EOF)
    {
        std::vector<std::vector<luint>> rocks;
        std::string line;
        std::getline(ifs, line);
        std::regex_match(line, sm, pat);
        std::string valvename = sm[1];
        vss.insert({valvename, false});
        brsf.insert({valvename, {}});
        luint flowrate = stoul(sm[2]);
        fs.insert({valvename, flowrate});
        std::string connections = sm[3];
        std::vector<std::string> conns;
        for (auto conn = std::sregex_iterator(connections.begin(), connections.end(), pat2);
                conn != std::sregex_iterator(); ++conn) {
            conns.push_back((*conn)[1]);
        }
        cs.insert({valvename, conns});
    }


    name2index.clear();
    index2name.clear();
    vfs.clear();
    vvss.clear();
    vbrsf.clear();
    vcs.clear();

    size_t i = 0;
    for (auto val : vss)
    {
        name2index.insert({val.first, i});
        index2name.insert({i++, val.first});
        vcs.push_back({});
        vfs.push_back({});
        vvss.push_back(val.second);
    }

    for (auto val : fs)
    {
        vfs[name2index[val.first]] = val.second;
    }

    for (size_t j = 0; j<full_time+1; ++j)
    {
        std::vector<std::vector<luint>> time;
        for (size_t i = 0; i<name2index.size(); ++i)
            time.push_back(std::vector<luint>(name2index.size()));
        vbrsf.push_back(time);
    }

    for (auto val : cs)
    {
        for (auto conn : val.second)
            vcs[name2index[val.first]].push_back(name2index[conn]);
    }

    luint start_pos = name2index["AA"];
    luint res = dfs(start_pos,full_time,0);
    return res;
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
}
