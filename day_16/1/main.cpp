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

luint dfs(std::string & target,
        std::map<std::string, bool> & vss,
        std::map<std::string, luint> & fs,
        std::map<std::string, std::vector<std::string>> & cs,
        luint time,
        luint res,
        luint current_fr,
        std::map< std::string, std::map< luint, luint>> & brsf,
        std::map< std::string, std::map< luint, luint>> & bfrsf)
{
    if (time < 2)
        return res;
    //if we have been here before with better flowrate 
    if (brsf[target][time] > res )//&& bfrsf[target][time] > current_fr) 
        return 0;
    //if (brsf[target][time] > res && bfrsf[target][time] > current_fr) 
    //    return 0;
    brsf[target][time] = res;
    bfrsf[target][time] = current_fr;

    luint max = 0;
    //first check without opening this valve
    for (std::string & c : cs[target])
    {
        max = std::max(max, dfs(c,vss,fs,cs,time-1,res, current_fr, brsf, bfrsf));
    }
    
    //then check with opening this valve
    if (vss[target] == false && fs[target] > 0)
    {
        vss[target] = true;
        current_fr += fs[target];
        time -= 1;
        res += time * fs[target];
        for (std::string & c : cs[target])
        {
            max = std::max(max, dfs(c,vss,fs,cs,time-1,res, current_fr, brsf, bfrsf));
        }
        vss[target] = false;
    }
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
    std::map< std::string, std::map< luint, luint>> bfrsf;
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
        bfrsf.insert({valvename, {}});
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
    std::string start_pos = "AA";
    luint res = dfs(start_pos,vss,fs,cs,30,0,0,brsf,bfrsf);
    return res;
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
}
