#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <unordered_set>
#include <chrono>

template <class T>
using Cache1 = std::vector<T>;
template <class T>
using Cache2 = std::vector<Cache1<T>>;
template <class T>
using Cache3 = std::vector<Cache2<T>>;
template <class T>
using Cache4 = std::vector<Cache3<T>>;
template <class T>
using Cache5 = std::vector<Cache4<T>>;
typedef std::unordered_map<short int, int> VisitedMap;
constexpr int ftime = 26;
//constexpr int ftime = 30;

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

std::vector<int> vfs;
std::vector<std::vector<int>> vdists;

Cache5<VisitedMap> vcache;
Cache5<int> prune;

int arr_t_chkd(int const & cr, int const & n, int const & time, short int const & visited, short int & nbit)
{
    if (n == cr || vdists[cr][n] == 0)
        return -1;
    nbit = 1L<<n;
    if (visited & nbit)
        return -1;
    return time - vdists[cr][n] - 1;
}


int dfs(int const & ce, int const & cm, int arr_te, int arr_tm, int time, short int visited, int fsf)
{
    int max = 0;
    for (int i = time; i<=ftime; ++i)
        if (prune[time][ce][cm][arr_te][arr_tm] > fsf)
            return 0;
    prune[time][ce][cm][arr_te][arr_tm] = fsf;
    prune[time][cm][ce][arr_tm][arr_te] = fsf;
    if (vcache[time][ce][cm][arr_te][arr_tm][visited] != 0)
        return vcache[time][ce][cm][arr_te][arr_tm][visited];
    if (arr_te == time && arr_tm == time) {
        max = time * (vfs[cm] + vfs[ce]);
        fsf += time * (vfs[cm] + vfs[ce]);
        bool stepped_down = false;
        for (long int nm=0; nm<(int)vdists[cm].size(); ++nm) {
            short int nmbit;
            int arr_tm = arr_t_chkd(cm,nm,time,visited,nmbit);
            if (arr_tm <= 0)
                continue;
            for (long int ne=0; ne<(int)vdists[ce].size(); ++ne) {
                short int nebit;
                int arr_te = arr_t_chkd(ce,ne,time,visited,nebit);
                if (arr_te <= 0)
                    continue;
                if (ce == nm || cm == ne || nm == ne)
                    continue;
                int final_arr_t = std::max(arr_te, arr_tm);
                stepped_down = true;
                max = std::max(max, dfs(ne, nm, arr_te, arr_tm, final_arr_t, visited | nebit | nmbit, fsf) + time * (vfs[cm] + vfs[ce]));
            }
        }
        if (stepped_down)
        {
            vcache[time][cm][ce][arr_tm][arr_te][visited] = max;
            return vcache[time][ce][cm][arr_te][arr_tm][visited] = max;
        } else {
            bool at_least_one_e = false;
            for (long int ne=0; ne<(int)vdists[ce].size(); ++ne) {
                short int nebit;
                if(arr_t_chkd(ce,ne,time,visited,nebit) <= 0)
                    continue;
                at_least_one_e = true;
                break;
            }
            bool at_least_one_m = false;
            for (long int nm=0; nm<(int)vdists[cm].size(); ++nm) {
                short int nmbit;
                if(arr_t_chkd(cm,nm,time,visited,nmbit) <= 0)
                    continue;
                at_least_one_m = true;
                break;
            }
            if (at_least_one_e && !at_least_one_m)
                arr_tm = 0;
            if (at_least_one_m && !at_least_one_e)
                arr_te = 0;
            if (!at_least_one_e && !at_least_one_m) {
                vcache[time][cm][ce][arr_tm][arr_te][visited] = max;
                return vcache[time][ce][cm][arr_te][arr_tm][visited] = max;
            }
        }
    }
    if (arr_tm == time) {
        max = time * vfs[cm];
        fsf += time * vfs[cm];
        bool stepped_down = false;
        for (long int nm=0; nm<(int)vdists[cm].size(); ++nm) {
            short int nmbit;
            int arr_tm = arr_t_chkd(cm,nm,time,visited,nmbit);
            if (arr_tm <= 0)
                continue;
            int final_arr_t = std::max(arr_te, arr_tm);
            stepped_down = true;
            max = std::max(max, dfs(ce, nm, arr_te, arr_tm, final_arr_t, visited | nmbit, fsf) + time * vfs[cm]);
        }
        if (!stepped_down) {
            max = std::max(max, dfs(ce, cm, arr_te, 0, arr_te, visited, fsf) + time * vfs[cm]);
        }
    }
    if (arr_te == time) {
        max = time * vfs[ce];
        fsf += time * vfs[ce];
        bool stepped_down = false;
        for (long int ne=0; ne<(int)vdists[ce].size(); ++ne) {
            short int nebit;
            int arr_te = arr_t_chkd(ce,ne,time,visited,nebit);
            if (arr_te <= 0)
                continue;
            int final_arr_t = std::max(arr_te, arr_tm);
            stepped_down = true;
            max = std::max(max, dfs(ne, cm, arr_te, arr_tm, final_arr_t, visited | nebit, fsf) + time * vfs[ce]);
        }
        if (!stepped_down) {
            max = std::max(max, dfs(ce, cm, 0, arr_tm, arr_tm, visited, fsf) + time * vfs[ce]);
        }
    }
    vcache[time][cm][ce][arr_tm][arr_te][visited] = max;
    return vcache[time][ce][cm][arr_te][arr_tm][visited] = max;
}

int run(std::string const filename)
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

    vcache = Cache5<VisitedMap>(ftime+1,
            Cache4<VisitedMap>(name2ind.size(),
            Cache3<VisitedMap>(name2ind.size(),
            Cache2<VisitedMap>(ftime+1,
            Cache1<VisitedMap>(ftime+1, VisitedMap())))));
    prune = Cache5<int>(ftime+1,
            Cache4<int>(name2ind.size(),
            Cache3<int>(name2ind.size(),
            Cache2<int>(ftime+1,
            Cache1<int>(ftime+1, 0)))));
    int start_pos = name2ind["AA"];

    return dfs(start_pos, start_pos, ftime, ftime, ftime, 0, 0);
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
