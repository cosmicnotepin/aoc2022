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
#include <chrono>

typedef long unsigned int luint;
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

void pm(std::vector<std::vector<char>> const & elf_map)
{
    std::cout<<'\n';
    for(auto row : elf_map)
    {
        for(auto col : row)
            std::cout<<col;
        std::cout<<'\n';
    }
}
        
void propose(std::vector<std::vector<char>> const & elf_map, std::vector<std::valarray<int>> const & elves, std::vector<std::vector<int>> & prps_map, std::vector<std::valarray<int>> & prps)
{
    static std::vector<std::vector<std::valarray<int>>> dir2poss {
        {{-1,-1},{0,-1},{1,-1}},
        {{-1,1},{0,1},{1,1}},
        {{-1,-1},{-1,0},{-1,1}},
        {{1,-1},{1,0},{1,1}},
    };
    static int first_dir = 0;
    int doners= 0;
    for (int i=0; i<(int)elves.size(); ++i)
    {
        auto e = elves[i];
        bool done = true;
        for (int y : {-1,0,1})
            for (int x : {-1,0,1})
                if(!(x==0 && y==0) && elf_map[e[1]+y][e[0]+x] == '#')
                {
                    done = false;
                    goto done;
                }
done:
        if(done)
        {
            ++doners;
            prps.push_back(e); //propose to not move
            prps_map[e[1]][e[0]] += 1;
            continue;
        }

        bool free = true;
        for(int i=0; i<4; ++i)
        {
            free = true;
            for (auto dir : dir2poss[(first_dir + i)%4])
            {
                auto p = e+dir;
                if (elf_map[p[1]][p[0]] == '#')
                {
                    free = false;
                    break;
                }
            }
            if (free)
            {
                std::valarray ps = e+dir2poss[(first_dir + i)%4][1];
                prps.push_back(ps);
                prps_map[ps[1]][ps[0]] += 1;
                break;
            }
        }
        if (!free)
            prps.push_back(e); //propose to not move
    }
    first_dir = (first_dir + 1) % 4;
}

bool move(std::vector<std::vector<char>> & elf_map, std::vector<std::valarray<int>> & elves, std::vector<std::vector<int>> const & prps_map, std::vector<std::valarray<int>> & prps)
{
    int min_x = 2;
    int max_x = elf_map[0].size() - 3;
    int min_y = 2;
    int max_y = elf_map.size() - 3;
    bool someone_moved = false;
    for (int i=0; i<(int)elves.size(); ++i)
    {
        auto & p = prps[i];
        auto e = elves[i];
        if ((e != p).min() == 1 || prps_map[p[1]][p[0]] > 1)
            p = e;
        min_x = std::min(min_x, p[0]);
        max_x = std::max(max_x, p[0]);
        min_y = std::min(min_y, p[1]);
        max_y = std::max(max_y, p[1]);
    }

    int x_off = (min_x == 0) ? 1 : ((min_x == 2) ? -1 : 0);
    int y_off = (min_y == 0) ? 1 : ((min_y == 2) ? -1 : 0);
    for (int i=0; i<(int)elves.size(); ++i)
    {
        auto & e = elves[i];
        auto & p = prps[i];
        if((e == p).min() == 1)
        {
            e[0] += x_off;
            e[1] += y_off;
            continue;
        }
        elf_map[e[1]][e[0]] = '.';
        elf_map[p[1]][p[0]] = '#';
        e[0] = p[0] + x_off;
        e[1] = p[1] + y_off;
        someone_moved = true;
    }

    //begin order matters
    int old_x_size = (int)elf_map[0].size();
    int old_y_size = (int)elf_map.size();
    if (max_x == old_x_size - 1)
        for (auto & row : elf_map)
            row.push_back('.');

    if (max_x == old_x_size - 3)
        for (auto & row : elf_map)
            row.pop_back();

    if (x_off == 1)
        for (auto & row : elf_map)
            row.insert(row.begin(), '.');

    if (x_off == -1)
        for (auto & row : elf_map)
            row.erase(row.begin());

    if (max_y == old_y_size -1)
        elf_map.push_back(std::vector<char>(elf_map[0].size(), '.'));

    if (max_y == old_y_size -3)
        elf_map.pop_back();

    if (y_off == 1)
        elf_map.insert(elf_map.begin(), 
                std::vector<char>(elf_map[0].size(), '.'));

    if (y_off == -1)
        elf_map.erase(elf_map.begin());
    //end: order matters

    return someone_moved;
}

luint run(std::string const filename)
{
    std::ifstream ifs {filename};
    std::vector<std::valarray<int>> elves;
    std::vector<std::vector<char>> elf_map;
    int max_x=0;
    int max_y=0;
    while (ifs.peek() != EOF)
    {
        std::string line;
        std::getline(ifs, line);
        std::vector<char> row {'.'};
        for (int x=0; x<(int)line.size(); ++x)
        {
            row.push_back(line[x]);
            if(line[x] == '#')
            {
                elves.push_back(std::valarray<int> {x+1,max_y + 1});
            }
        }
        row.push_back('.');
        elf_map.push_back(row);
        ++max_y;
        max_x = line.size();
    }

    max_x += 2;
    max_y += 2;
    elf_map.insert(elf_map.begin(), std::vector<char>(max_x, '.'));
    elf_map.push_back(std::vector<char>(max_x, '.'));

    int moves = 0;
    bool someone_moved = true;
    while(someone_moved)
    {
        std::vector<std::valarray<int>> prps;
        std::vector<std::vector<int>> prps_map(
                elf_map.size(), std::vector<int>(elf_map[0].size(), 0));
        propose(elf_map, elves, prps_map, prps);
        someone_moved = move(elf_map, elves, prps_map, prps);
        pm(elf_map);
        ++moves;
        if (moves == 10)
            break;
    }
        //pm(elf_map);

    return (elf_map.size() - 2) * (elf_map[0].size() - 2) - elves.size();
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
    //4004 too high
}
