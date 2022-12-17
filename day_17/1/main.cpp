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

typedef long unsigned int luint;
typedef std::vector<std::vector<bool>> Rock;

std::string gusts;

void print(std::vector<std::vector<char>> & map)
{
    std::cout<<"\n\n";
    for (int i = map.size()-1; i>=0; --i)
    {
        for (auto column : map[i])
            std::cout<<column;
        std::cout<<'\n';
    }
}


template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

Rock get_next_rock()
{
    static std::vector<Rock> rocks = {
        {{true,true,true,true}},
        {{false,true,false},
         {true,true,true},
         {false,true,false}},
        {{true,true,true},
         {false,false,true},
         {false,false,true}},
        {{true},
         {true},
         {true},
         {true}},
        {{true,true},
        {true,true}}
    };
    static size_t last_rock = 0;
    Rock ret = rocks[last_rock];
    last_rock = (last_rock + 1) % rocks.size();
    return ret;
}

char get_next_gust()
{
    static size_t last_gust = 0;

    char ret = gusts[last_gust];
    last_gust = (last_gust + 1) % gusts.size();
    return ret;
}

int get_top_rock_y(std::vector<std::vector<char>> & map)
{
    for (int y=map.size() - 1; y>=0; --y)
        for (int x=1; x<(int)map[0].size()-2; ++x)
            if (map[y][x] != '.')
                return y;
    std::cerr<<"map has no bottom";
    return 0;
}

bool check(std::vector<std::vector<char>>&  map, Rock rock, size_t x, size_t y)
{
    for (size_t yp=0; yp<rock.size(); ++yp)
        for (size_t xp=0; xp<rock[0].size(); ++xp)
            if (rock[yp][xp] == true && map[yp + y][xp + x] != '.')
                return false;

    return true;
}

void drop_rock(std::vector<std::vector<char>>&  map)
{
    Rock rock = get_next_rock();
    int top_rock_y = get_top_rock_y(map);
    int rows_to_add = std::max(0, (int)top_rock_y + 4 + (int)rock.size() - (int)map.size());
    for (int i=0; i<rows_to_add; ++i)
        map.push_back({'|', '.', '.', '.', '.', '.', '.', '.', '|'});
    int x = 3;
    int y = top_rock_y + 4;

    bool moved = true;
    while (moved)
    {
        moved = false;
        char gust = get_next_gust();
        if (gust != '<' and gust != '>')
        {
            std::cerr<<"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            abort();
        }
        int x_change = gust == '<' ? -1 : 1;
        int y_change = 0;
        if (check(map, rock, x+x_change, y+y_change)) 
        {
            x += x_change;
            y += y_change;
        }
        x_change = 0;
        y_change = -1;
        if (check(map, rock, x+x_change, y+y_change)) 
        {
            x += x_change;
            y += y_change;
            moved = true;
        }
    }
    for (size_t yp=0; yp<rock.size(); ++yp)
        for (size_t xp=0; xp<rock[0].size(); ++xp)
            if (rock[yp][xp] == true)
                map[yp + y][xp + x] = '#';

}

luint run(std::string const filename)
{
    std::ifstream ifs {filename};
    std::getline(ifs, gusts);

    std::vector<std::vector<char>> map(1, std::vector<char> {'+', '-', '-', '-', '-', '-', '-', '-', '+'} );

    for (int i = 0; i< 2022; ++i)
    {
        drop_rock(map);
        print(map);
    }

    return get_top_rock_y(map);
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    //auto result = run("input");
    //std::cout<<"input result: "<<result<<'\n';
}
