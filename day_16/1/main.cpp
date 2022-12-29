#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <unordered_set>
#include <chrono>

typedef long unsigned int luint;

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

std::unordered_map<std::string, int> fs;
std::unordered_map<std::string, std::vector<std::string>> neighss;
std::unordered_map<std::string, std::unordered_map<std::string, int>> dists;


int dfs(std::string const & cr, std::unordered_map<std::string, bool> vss,
        int time, int fsf, std::unordered_set<std::string> visited)
{
    if (time <= 0)
        return 0;
    vss[cr] = true;
    fsf += time * fs[cr];
    int max = fsf;
    for (auto n : dists[cr]) {
        if (!visited.contains(n.first)) {
            std::unordered_set<std::string> vc(visited); 
            vc.insert(n.first);
            max = std::max(max, dfs(n.first, vss, time - n.second - 1, fsf, vc));
        }
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
    std::unordered_map<std::string, bool> vss;

    fs.clear();
    neighss.clear();
    dists.clear();
    while (ifs.peek() != EOF) {
        std::vector<std::vector<luint>> rocks;
        std::string line;
        std::getline(ifs, line);
        std::regex_match(line, sm, pat);
        std::string valvename = sm[1];
        fs.insert({valvename, stoi(sm[2])});

        std::string neigh_string = sm[3];
        std::vector<std::string> neighs;
        for (auto n = std::sregex_iterator(neigh_string.begin(), neigh_string.end(), pat2);
                n != std::sregex_iterator(); ++n) {
            neighs.push_back((*n)[1]);
        }
        neighss.insert({valvename, neighs});
    }

    //for (auto fr : fs)
    //    std::cout<<fr.first<<":"<<fr.second<<"\n";

    for (auto v : fs) {
        if (v.first != "AA" && v.second == 0)
            continue;
        std::deque<std::pair<std::string, int>> todo;
        std::unordered_set<std::string> visited;
        todo.push_back({v.first, 0});
        while (todo.size() > 0) {
            auto cr = todo.front();
            todo.pop_front();
            if (fs[cr.first] > 0 && v.first != cr.first)
                dists[v.first][cr.first] = cr.second;
            for (auto n: neighss[cr.first]) {
                if (!visited.contains(n)) {
                    visited.insert(n);
                    todo.push_back({n, cr.second + 1});
                }
            }
        }
    }

    //for (auto ds : dists) {
    //    std::cout<<ds.first<<": [";
    //    for (auto d : ds.second)
    //        std::cout<<d.first<<":"<<d.second<<", ";
    //    std::cout<<"]\n";
    //}

    std::string start_pos = "AA";
    std::unordered_set<std::string> visited;
    int res = dfs(start_pos,vss,30,0,visited);
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
