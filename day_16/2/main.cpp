#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <regex>
#include <sstream>
#include <string>
#include <set>
#include <limits>

typedef long unsigned int luint;

std::unordered_map<std::string, int> name2index;
std::vector<int> fs;
std::vector<bool> vvss;
std::vector<std::vector<std::vector<int>>> vbrsf;
std::vector<std::vector<int>> vcs;
int full_time = 26;

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

int dfs(int target, int etarget, int time, int res)
{
    if (time == 0)
        return res;

    for (int i = full_time; i >= time; --i)
    {
        if (vbrsf[i][target][etarget] != 0 && vbrsf[i][target][etarget] >= res) {
            std::cout<<"pruned";
            return 0;
        }
    }

    vbrsf[time][target][etarget] = res;
    vbrsf[time][etarget][target] = res;

    int max = 0;

    if ( target != etarget && !vvss[target] && fs[target] > 0 && !vvss[etarget] && fs[etarget] > 0)
    {
        vvss[target] = true;
        vvss[etarget] = true;
        int tres = res + (time-1) * (fs[target] + fs[etarget]);
        max = std::max(max, dfs(target,etarget,time-1,tres));
        vvss[target] = false;
        vvss[etarget] = false;
    }

    if (!vvss[target] && fs[target] > 0 && !(!vvss[etarget] && fs[etarget] > 0))
    {
        vvss[target] = true;
        int tres = res + (time-1) * fs[target];
        for (int & ec : vcs[etarget])
            max = std::max(max, dfs(target,ec,time-1,tres));
        vvss[target] = false;
    }

    if (!(!vvss[target] && fs[target] > 0) && !vvss[etarget] && fs[etarget] > 0)
    {
        vvss[etarget] = true;
        int tres = res + (time-1) * (fs[etarget]);
        for (int & c : vcs[target])
            max = std::max(max, dfs(c,etarget,time-1,tres));
        vvss[etarget] = false;
    }

    //both move without activating valves
    for (int & c : vcs[target]) {
        for (int & ec : vcs[etarget]) {
            if (c != etarget && ec != target)
                max = std::max(max, dfs(c,ec,time-1,res));
        }
    }

    return max;
}

int run(std::string const filename)
{
    std::ifstream ifs {filename};
    std::regex pat {R"(Valve (..) has flow rate=(\d+); tunnels? leads? to valves? (.*))"};
    std::smatch sm;
    std::regex pat2 {R"((\S\S),?)"};
    std::smatch sm2;
    std::vector<std::vector<std::string>> cs;

    for (std::string line; std::getline(ifs, line);) {
        std::regex_match(line, sm, pat);
        std::string valvename = sm[1];
        vvss.push_back(false);
        name2index.insert({valvename, name2index.size()});
        fs.push_back(stoi(sm[2]));
        std::string connections = sm[3];
        std::vector<std::string> conns;
        for (auto conn = std::sregex_iterator(connections.begin(), connections.end(), pat2);
                conn != std::sregex_iterator(); ++conn) {
            conns.push_back((*conn)[1]);
        }
        cs.push_back(conns);
    }

    for (size_t i = 0; i<fs.size(); ++i)
    {
        vcs.push_back({});
        for (auto c : cs[i])
            vcs[i].push_back(name2index[c]);
    }


    for (int j = 0; j<full_time+1; ++j)
    {
        std::vector<std::vector<int>> time;
        for (size_t i = 0; i<name2index.size(); ++i)
            time.push_back(std::vector<int>(name2index.size()));
        vbrsf.push_back(time);
    }


    int start_pos = name2index["AA"];
    int res = dfs(start_pos,start_pos,full_time,0);
    return res;
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    fs.clear();
    name2index.clear();
    vvss.clear();
    vcs.clear();
    vbrsf.clear();
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
}
