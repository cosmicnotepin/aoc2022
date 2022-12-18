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
int last_gust = 0;
int last_rock = 0;

void print(std::vector<std::vector<char>> & map, int is, int js, int dist)
{
    std::cout<<"\n\n";
    std::cout<<"at i: \n";
    for (int i = is; i>=is-dist; --i)
    {
        for (auto column : map[i])
            std::cout<<column;
        std::cout<<'\n';
    }
    std::cout<<"\nat j: \n";

    for (int i = js; i>=js-dist; --i)
    {
        for (auto column : map[i])
            std::cout<<column;
        std::cout<<'\n';
    }
}

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

void print(std::vector<std::vector<char>> map, Rock rock, int x, int y)
{
    for (int yp=0; yp<(int)rock.size(); ++yp)
        for (int xp=0; xp<(int)rock[0].size(); ++xp)
            if (rock[yp][xp])
                map[yp + y][xp + x] = '@';

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
    Rock ret = rocks[last_rock];
    last_rock = (last_rock + 1) % rocks.size();
    return ret;
}

char get_next_gust()
{
    char ret = gusts[last_gust];
    last_gust = (last_gust + 1) % gusts.size();
    return ret;
}

int get_top_rock_y(std::vector<std::vector<char>> & map)
{
    for (int y=map.size() - 1; y>=0; --y)
        for (int x=1; x<(int)map[0].size()-1; ++x)
            if (map[y][x] != '.')
            {
                return y;
            }
    std::cerr<<"map has no bottom";
    return 0;
}

bool check(std::vector<std::vector<char>>&  map, Rock rock, int x, int y)
{
    for (int yp=0; yp<(int)rock.size(); ++yp)
        for (int xp=0; xp<(int)rock[0].size(); ++xp)
            if (rock[yp][xp] && map[yp + y][xp + x] != '.')
                return false;

    return true;
}

void drop_rock(std::vector<std::vector<char>>&  map, std::vector<std::valarray<int>> & state, std::vector<int> & heights)
{
    Rock rock = get_next_rock();
    int top_rock_y = get_top_rock_y(map);
    int rows_to_add = std::max(0, (int)top_rock_y + 4 + (int)rock.size() - (int)map.size());
    for (int i=0; i<rows_to_add; ++i)
        map.push_back({'|', '.', '.', '.', '.', '.', '.', '.', '|'});
    int x = 3;
    int y = top_rock_y + 4;

    bool moved = true;
    //print(map,rock,x,y);
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
        //print(map,rock,x,y);
        if (check(map, rock, x+x_change, y+y_change)) 
        {
            x += x_change;
            y += y_change;
            moved = true;
        }
        //print(map,rock,x,y);
    }
    state.push_back(std::valarray {x, last_gust, last_rock});
    heights.push_back(y+rock.size());
    for (int yp=0; yp<(int)rock.size(); ++yp)
        for (int xp=0; xp<(int)rock[0].size(); ++xp)
            if (rock[yp][xp])
                map[yp + y][xp + x] = '#';

}

luint run(std::string const filename)
{
    std::ifstream ifs {filename};
    std::getline(ifs, gusts);

    std::vector<std::vector<char>> map(1, std::vector<char> {'+', '-', '-', '-', '-', '-', '-', '-', '+'} );

    std::vector<std::valarray<int>> state;
    std::vector<int> heights;
    for (int i = 0; i< 200022; ++i)
    {
        drop_rock(map, state, heights);
        //print(map);
    }

    //for (int i = 0; i< (int)state.size()-1-20; ++i)
    int i = 1000;
    int period = 0;
    int last_period = 0;
    int pccounter = 0;
    int last_j = 0;
    int test_dist = 100;
        for (int j = i+1; j< (int)state.size()-test_dist; ++j)
        {
            for (int o = 0; o<test_dist; ++o)
            {
                if((state[i+o] != state[j+o]).max())
                    goto skip;
                //auto gah = (state[i+o] != state[j+o]);
                //for (int arg = 0; arg<(int)gah.size(); ++arg)
                //    std::cout<<gah[arg];
                //std::cout<<'\n';
                //std::cout<<state[i+o][0]<<state[j+o][0]<<'\n';
                //std::cout<<state[i+o][1]<<state[j+o][1]<<'\n';
                //std::cout<<state[i+o][2]<<state[j+o][2]<<'\n';
            }
            print(map, heights[i], heights[j], 20);
            period = j-last_j;
            last_j = j;
            if (last_period != period)
                ++pccounter;
            last_period = period;
            std::cout<<j-i<<'\n';
            ;
skip:       ;
        }
        std::cout<<"pccounter: "<<pccounter<<'\n';

    return get_top_rock_y(map);
}

int main(int argc, char** argv)
{
    //auto test_result = run("input_t1");
    //std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
}
