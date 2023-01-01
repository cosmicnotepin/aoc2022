#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <unordered_set>
#include <chrono>

typedef long unsigned int luint;

//constexpr int ftime = 26;
constexpr int ftime = 30;

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

std::vector<int> vfs;
std::vector<std::vector<int>> vdists;

std::vector<std::vector<std::unordered_map<short int, int>>> vcache;
std::vector<std::vector<int>> prune;


int dfs(int const & cr, int time, short int visited, int fsf)
{
    int max = time * vfs[cr];
    fsf += max;
    for (int i = time; i<=ftime; ++i)
        if (prune[i][cr] > fsf)
            return 0;
    prune[time][cr] = fsf;
    if (vcache[time][cr][visited] != 0)
        return vcache[time][cr][visited];
    for (long int n=0; n<(int)vdists[cr].size(); ++n) {
        if (n == cr || vdists[cr][n] == 0)
            continue;
        size_t bit = 1L<<n;
        if (visited & bit)
            continue;
        int rem_t = time - vdists[cr][n] - 1;
        if (rem_t <= 0)
            continue;
        max = std::max(max, dfs(n, rem_t, visited | bit, fsf) + time * vfs[cr]);
    }
    return vcache[time][cr][visited] = max;
}

luint run(std::string const filename)
{
    std::ifstream ifs {filename};

    std::regex pat 
    {R"(Valve (..) has flow rate=(\d+); tunnels? leads? to valves? (.*))"};
    std::smatch sm;
    std::regex pat2 {R"((\S\S),?)"};
    std::smatch sm2;

    std::unordered_map<std::string, std::vector<std::string>> neighss;
    std::unordered_map<std::string, 
        std::unordered_map<std::string, int>> dists;
    std::map<std::string, int> name2ind;
    std::unordered_map<std::string, int> fs;
    for (std::string line; std::getline(ifs, line);) {
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

    //compress graph
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

    //put data in global vectors for fast access
    int i = 0;
    for (auto ds : dists) {
        name2ind.insert({ds.first, i++});
    }

    vdists = std::vector<std::vector<int>>(
            name2ind.size(), std::vector<int>(name2ind.size(), {}));
    vfs = std::vector<int>(name2ind.size());

    for (auto ds : dists) {
        vfs[name2ind[ds.first]] = fs[ds.first];
        for (auto d : ds.second)
            vdists[name2ind[ds.first]][name2ind[d.first]] = d.second;
    }
    //data is now in global vectors

    vcache = std::vector(ftime+1, std::vector(name2ind.size(), std::unordered_map<short int, int>()));
    prune = std::vector<std::vector<int>>(ftime+1, std::vector<int>(name2ind.size(), -1));
    int max = 0; 
    int start_pos = name2ind["AA"];
    std::cout<<"p1: "<<dfs(start_pos,ftime,0, 0)<<'\n';
    //for (short unsigned int i = 0;
    //       i <= std::numeric_limits<short unsigned int>::max()/2 + 1; ++i) {
    //    max = std::max(max, dfs(start_pos,ftime,i, 0) + dfs(start_pos,ftime,~i, 0));
    //}
    return max;
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
