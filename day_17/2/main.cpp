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
    for (int i = is; i<is+dist; ++i)
    {
        for (auto column : map[i])
            std::cout<<column;
        std::cout<<'\n';
    }
    std::cout<<"\nat j: \n";

    for (int i = js; i<js+dist; ++i)
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

long long int run(std::string const filename)
{
    std::ifstream ifs {filename};
    std::getline(ifs, gusts);

    std::vector<std::vector<char>> map(1, std::vector<char> {'+', '-', '-', '-', '-', '-', '-', '-', '+'} );

    std::vector<std::valarray<int>> state;
    std::vector<int> heights;
    for (int i = 0; i< 20022; ++i)
    {
        drop_rock(map, state, heights);
        //print(map);
    }

    int first_i = -1;
    int test_dist = 100;
    std::vector<int> rep_indices;
    for (int i = 0; i< (int)state.size()-test_dist-1; ++i)
    {
        for (int j = i+1; j< (int)state.size()-test_dist; ++j)
        {
            for (int o = 0; o<test_dist; ++o)
            {
                if((state[i+o] != state[j+o]).max())
                    goto skip;
            }
            first_i = i;
            rep_indices.push_back(j);
skip:       ;
        }
        if(first_i != -1)
            break;
    }

    std::cout<<"first_i : "<<first_i <<'\n';
    for (int i = 0; i<std::min(5,(int)rep_indices.size()-1); ++i)
        std::cout<<rep_indices[i]<< " ";
    std::cout<<'\n';
    for (int i = 0; i<std::min(5,(int)rep_indices.size()-1); ++i)
        std::cout<<rep_indices[i+1]- rep_indices[i]<< " ";

    std::cout<<'\n';
    for (int i = 0; i<std::min(5,(int)rep_indices.size()-1); ++i)
        std::cout<<heights[rep_indices[i+1]]- heights[rep_indices[i]]<< " ";

    print(map, heights[first_i], heights[rep_indices[0]], 20);

    print(map, heights[rep_indices[1]], heights[rep_indices[2]], 20);


    std::cout<<"heights[first_i]: "<<heights[first_i]<<'\n';
    std::cout<<"heights[rep_indices[0]]: "<<heights[rep_indices[0]]<<'\n';
    std::cout<<"heights[rep_indices[1]]: "<<heights[rep_indices[1]]<<'\n';
    std::cout<<"heights[rep_indices[2]]: "<<heights[rep_indices[2]]<<'\n';


    int period_length = rep_indices[1] - rep_indices[0];
    long long int no_rocks = 1000000000000;
    std::cout<<"no_rocks : "<<no_rocks <<'\n';
    long long int no_rocks_wo_pinit = 1000000000000 - first_i;
    std::cout<<"no_rocks_wo_pinit : "<<no_rocks_wo_pinit <<'\n';
    long long int number_of_periods = no_rocks_wo_pinit / period_length;
    std::cout<<"number_of_periods : "<<number_of_periods <<'\n';
    long long int mod = no_rocks_wo_pinit % period_length;
    std::cout<<"mod : "<<mod <<'\n';
    long long int left_for_manual = no_rocks_wo_pinit - number_of_periods * period_length;
    long long int period_height = heights[rep_indices[1]] - heights[rep_indices[0]];
    std::cout<<"period_height : "<<period_height <<'\n';
    std::cout<<"left_for_manual : "<<left_for_manual <<'\n';

    map.clear();
    map.push_back(std::vector<char> {'+', '-', '-', '-', '-', '-', '-', '-', '+'} );
    state.clear();
    heights.clear();
    last_rock = 0;
    last_gust = 0;

    for (int i = 0; i< first_i + left_for_manual; ++i)
    {
        drop_rock(map, state, heights);
        //print(map);
    }

    long long int manual_height = heights[heights.size()-1] - heights[first_i];
    std::cout<<"manual_height : "<<manual_height <<'\n';

    //print(map, heights[rep_indices[20]], heights[rep_indices[30]], 20);


    return  heights[first_i] - 1 + number_of_periods*period_height + manual_height;
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
}
