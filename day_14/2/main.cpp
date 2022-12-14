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

bool insert_sand(auto & map, auto & sand, auto & count)
{
    static std::vector<std::vector<int>> drops = {{0,1}, {-1,1}, {1,1}};
    if (map[sand[1]][sand[0]] != '.')
        return false;
    std::vector<luint> sand_pos = sand;
    bool moved = true;
    while (moved)
    {
        moved = false;
        for (auto drop : drops)
        {
            int tar_y = sand_pos[1]+drop[1];
            int tar_x = sand_pos[0]+drop[0];
            if (tar_x < 0)
            {
                //dropping off the map, inrease map size
                for (auto & row : map)
                {
                    row.insert(row.begin(), '.');
                }
                map[map.size()-1][0] = '#';
                ++sand_pos[0];
                ++sand[0];
                moved = true;
                break;
            }
            if (tar_x > (int)map[0].size() - 1)
            {
                //dropping off the map, inrease map size
                for (auto & row : map)
                {
                    row.push_back('.');
                }
                map[map.size()-1][map[0].size()-1] = '#';
                moved = true;
                break;
            }

            if (map[tar_y][tar_x] == '.') 
            {
                sand_pos[1] = tar_y;
                sand_pos[0] = tar_x;
                moved = true;
                break;
            }
        }
    }
    map[sand_pos[1]][sand_pos[0]] = 'o';
    ++count;
    return true;
}

void print_map(const auto & map)
{
    for (auto row : map)
    {
        for (auto column : row)
        {
            std::cout<<column;
        }
        std::cout<<'\n';
    }
}

void insert_rocks(auto & map, std::vector<std::vector<std::vector<luint>>> const & rockss)
{
    for (std::vector<std::vector<luint>> const & rocks : rockss)
    {
        for (size_t i=0; i<rocks.size() - 1; ++i)
        {
            std::vector<luint> start = rocks[i];
            std::vector<luint> end = rocks[i+1];
            int x_dist = end[0]-start[0];
            int y_dist = end[1]-start[1];
            int x_step = x_dist == 0 ? 1 : sgn(x_dist);
            int y_step = y_dist == 0 ? 1 : sgn(y_dist);
            for (int y=start[1]; y!=(int)end[1]+y_step; y+=y_step)
            {
                for (int x=start[0]; x!=(int)end[0]+x_step; x+=x_step)
                {
                    map[y][x] = '#';
                }
            }
        }
    }
}

luint run(std::string const filename)
{
    std::ifstream ifs {filename};
    if(!ifs)
    {
        std::cerr<<"could not open "<<filename<<'\n';
        exit(1);
    }

    std::regex pat {R"((\d+),(\d+)(?=( -> )?))"};
    std::smatch sm;
    std::vector<std::vector<std::vector<luint>>> rockss;
    std::vector<luint> sand {500,0};
    luint max_x = sand[0];
    luint max_y = sand[1];
    luint min_x = sand[0];
    luint min_y = sand[1];
    while (ifs.peek() != EOF)
    {
        std::vector<std::vector<luint>> rocks;
        std::string line;
        std::getline(ifs, line);
        std::regex_match(line, sm, pat);
        for (auto points = std::sregex_iterator(line.begin(), line.end(), pat);
                points != std::sregex_iterator(); ++points) {
            std::cout<<"point would be: ("<<(*points)[1]<<", "<<(*points)[2]<<")\n";
            luint x = std::stoul((*points)[1]);
            luint y = std::stoul((*points)[2]);
            max_x = std::max(max_x, x);
            max_y = std::max(max_y, y);
            min_x = std::min(min_x, x);
            min_y = std::min(min_y, y);
            rocks.push_back(std::vector<luint> {x, y});
        } 
        rockss.push_back(rocks);
    }

    sand[0] -= min_x;
    sand[1] -= min_y;
    for (auto & rocks : rockss)
        for (auto & rock : rocks)
        {
            rock[0] -= min_x;
            rock[1] -= min_y;
            std::cout<<"new rock: "<<rock[0]<< ", "<<rock[1]<<'\n';
        }

    std::vector<std::vector<char>> map(max_y-min_y + 2, std::vector<char>(max_x-min_x + 1, '.'));
    map.push_back(std::vector<char>(max_x-min_x + 1, '#'));
    print_map(map);
    insert_rocks(map, rockss);
    print_map(map);
    luint rested_sand = 0;
    while (insert_sand(map, sand, rested_sand))
    {
        //print_map(map);
    }
        print_map(map);
    return rested_sand;
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
}
