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

typedef std::vector<std::vector<std::vector<char>>> Map;
typedef std::vector<std::vector<char>> Row;

std::vector<Map> maps;

void pm(Map const & map)
{
    std::cout<<'\n';
    for (int y=0; y<(int)map.size(); ++y)
    {
        for (int x=0; x<(int)map[0].size(); ++x)
        {
            switch (map[y][x].size())
            {
                case 0:
                    if(x==0 || y==0 ||
                       x==(int)map[0].size()-1 || y==(int)map.size() -1)
                        std::cout<<'#';
                    else
                        std::cout<<'.';
                    break;
                case 1:
                    std::cout<<map[y][x][0];
                    break;
                default:
                    std::cout<<map[y][x].size();
            }
        }
        std::cout<<'\n';
    }
}


bool pat(int x, int y, int t)
{
    static std::unordered_map<char,std::vector<int>> char2dir 
    {{'^',{0,-1}},
     {'>',{1,0}},
     {'v',{0,1}},
     {'<',{-1,0}},
    };
    while (t>(int)maps.size()-1)
    {
        Map & map = maps[maps.size()-1];
        Map next_t = Map(map.size(), Row(map[0].size(), std::vector<char>()));
        for (int y=1; y<(int)map.size()-1; ++y)
        {
            for (int x=1; x<(int)map[0].size()-1; ++x)
            {
                for (char c : map[y][x])
                {
                    auto dir = char2dir[c];
                    int n_x = x + dir[0];
                    int n_y = y + dir[1];
                    if (n_x == 0 || n_x == (int)map[0].size()-1 ||
                        n_y == 0 || n_y == (int)map.size()-1)               
                    {
                        switch (c)
                        {
                            case '^':
                                n_y = map.size()-2;
                                break;
                            case '>':
                                n_x = 1;
                                break;
                            case 'v':
                                n_y = 1;
                                break;
                            case '<':
                                n_x = map[0].size()-2;
                                break;
                        }
                    }
                    next_t[n_y][n_x].push_back(c);
                }
            }
        }
        maps.push_back(std::move(next_t));
    }
    return maps[t][y][x].size() == 0;
}


luint run(std::string const filename)
{
    std::ifstream ifs {filename};
    int y = 0;
    maps.clear();
    Map map;
    int x_start = 0;
    int x_end = 0;
    while (ifs.peek() != EOF)
    {
        std::string line;
        std::getline(ifs, line);
        std::vector<std::vector<char>> row;
        for (int x=0; x<(int)line.size(); ++x)
        {
            if(y==0 && line[x] == '.')
                x_start = x;
            //next if is dumb but the last time we have a . is the exit
            if(line[x] == '.')
                x_end = x;
            if (line[x] != '.' && line[x] != '#')
                row.push_back(std::vector<char> {line[x]});
            else
                row.push_back(std::vector<char> {});
        }
        map.push_back(row);
        ++y;
    }
    int y_start = 0;
    int y_end = map.size()-1;
    std::cout<<"start: ("<<x_start<<","<<y_start<<")\n";
    std::cout<<"end: ("<<x_end<<","<<y_end<<")\n";
    maps.push_back(std::move(map));

    for (int i=0; i<3; ++i)
    {
        pat(1,1,i);
        pm(maps[maps.size()-1]);
    }

    return 0;
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
