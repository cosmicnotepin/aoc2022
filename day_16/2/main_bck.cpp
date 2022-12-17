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

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

luint dfs(luint & target,
        luint & etarget,
        std::vector<bool> & vss,
        std::vector<luint> & fs,
        std::vector<std::vector<luint>> & cs,
        luint time,
        luint res,
        std::vector<std::vector<std::vector<luint>>> & brsf)
{
    //std::cout<<"time : "<<time <<'\n';
    if (time < 2)
        return res;
    //if we have been here before with better flowrate 
    //std::cout<<"target+etarget: "<<target+etarget<<'\n';
    //std::cout<<"time: "<<time<<'\n';
    //std::cout<<"target: "<<target<<'\n';
    //std::cout<<"etarget: "<<etarget<<'\n';
    if (brsf[time][target][etarget] > res) 
    {
        //std::cout<<"done better\n";
        return 0;
    }
    brsf[time][target][etarget] = res;
    brsf[time][etarget][target] = res;

    luint max = 0;


    //have check "both open their valves
    if ( etarget != target && vss[etarget] == false && vss[target] == false && fs[target] > 0 && fs[etarget] > 0)
    {
        vss[target] = true;
        vss[etarget] = true;
        luint tres = res + (time-1) * fs[target] + (time-1) * fs[etarget];

        max = std::max(max, dfs(target, etarget,vss,fs,cs,time-1,tres, brsf));
        vss[target] = false;
        vss[etarget] = false;
    }

    //have to check "open for me, elephant moves"
    if (vss[target] == false && fs[target] > 0)
    {
        vss[target] = true;
        luint tres = res + (time-1) * fs[target];
        for (luint & ec : cs[etarget])
        {
            max = std::max(max, dfs(target,ec,vss,fs,cs,time-1,tres, brsf));
        }
        vss[target] = false;
    }
    //have to check "open for elephant, me moves"
    if (vss[etarget] == false && fs[etarget] > 0)
    {
        vss[etarget] = true;
        luint tres = res + (time-1) * fs[etarget];
        for (luint & c : cs[target])
        {
            max = std::max(max, dfs(c,etarget,vss,fs,cs,time-1,tres, brsf));
        }
        vss[etarget] = false;
    }

    //have to check "no valves are opened, both move"
    for (luint & c : cs[target])
    {
        for (luint & ec : cs[etarget])
        {
            if ( !(c == etarget && ec == target))
                max = std::max(max, dfs(c,ec,vss,fs,cs,time-1,res, brsf));
        }
    }

    ////if both valves were open to begin with or both flowrates 0 : return now
    //if ((vss[target] == true && vss[etarget] == true) || (fs[target] == 0 && fs[etarget] == 0))
    //    return max;

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
        //std::cout<<"sm[1]: "<<sm[0]<<'\n';
        std::string valvename = sm[1];
        ////std::cout<<"valvename : "<<valvename <<'\n';
        vss.insert({valvename, false});
        brsf.insert({valvename, {}});
        luint flowrate = stoul(sm[2]);
        //std::cout<<"flowrate : "<<flowrate <<'\n';
        fs.insert({valvename, flowrate});
        std::string connections = sm[3];
        //std::cout<<"connections : "<<connections <<'\n';
        std::vector<std::string> conns;
        for (auto conn = std::sregex_iterator(connections.begin(), connections.end(), pat2);
                conn != std::sregex_iterator(); ++conn) {
            //std::cout<<"    conn would be: "<<(*conn)[0]<<"\n";
            conns.push_back((*conn)[1]);
        }
        cs.insert({valvename, conns});
    }
    std::map<std::string, size_t> name2index;
    std::vector<luint> vfs;
    std::vector<bool> vvss;
    std::vector<std::vector<std::vector<luint>>> vbrsf;
    std::vector<std::vector<luint>> vcs;
    size_t i = 0;
    for (auto val : vss)
    {
        name2index.insert({val.first, i++});
        vcs.push_back({});
        vfs.push_back({});
        vvss.push_back(val.second);
    }

    for (auto val : fs)
    {
        vfs[name2index[val.first]] = val.second;
    }

    for (size_t j = 0; j<27; ++j)
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
    luint estart_pos = name2index["AA"];
    luint res = dfs(start_pos, estart_pos,vvss,vfs,vcs,26,0,vbrsf);
    return res;
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
}
