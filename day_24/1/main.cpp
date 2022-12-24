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

class valarray_comp
{

    public:
        bool operator()(const std::valarray<int> & lhs, const std::valarray<int> & rhs) const
        {
            for (int i=0; i<(int)lhs.size(); ++i)
                if (lhs[i] > rhs[i])
                    return true;
            return false;
        };
};
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
                    //if(x==0 || y==0 ||
                    //   x==(int)map[0].size()-1 || y==(int)map.size() -1)
                    //    std::cout<<'#';
                    //else
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


bool pat(std::valarray<int> pos)
{
    static std::unordered_map<char,std::vector<int>> char2dir 
    {{'^',{0,-1}},
     {'>',{1,0}},
     {'v',{0,1}},
     {'<',{-1,0}},
    };
    while (pos[0]>(int)maps.size()-1)
    {
        Map & map = maps[maps.size()-1];
        Map next_t = Map(map.size(), Row(map[0].size(), std::vector<char>()));

        for (int y : {0, (int)map.size() -1})
            for (auto & col : next_t[y])
                col.push_back('#');

        for (auto & row : next_t)
        {
            row[0].push_back('#');
            row[map[0].size()-1].push_back('#');
        }
                

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
    //pm(maps[pos[0]]);
    //std::cout<<"for pos:"<<pos[0]<<","<<pos[1]<<","<<pos[2]<<'\n';
    //bool res = maps[pos[0]][pos[2]][pos[1]].size() == 0;
    //std::cout<<"res : "<<res <<'\n';
    return maps[pos[0]][pos[2]][pos[1]].size() == 0;
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
            if (line[x] != '.')// && line[x] != '#')
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
    map[y_start][x_start].push_back('#');
    maps.push_back(std::move(map));
    pm(maps[maps.size()-1]);
    //std::string blah;
    //std::cin>>blah;


    //for (int i=0; i<3; ++i)
    //{
    //    pat(std::valarray {i,1,1});
    //    pm(maps[maps.size()-1]);
    //}

    auto comp = [](std::valarray<int> const & lhs,
                                    std::valarray<int> const & rhs)
    {
        //std::cout<<"("<<lhs[0]<<","<<lhs[1]<<","<<lhs[2]<<"), ";
        //std::cout<<"("<<rhs[0]<<","<<rhs[1]<<","<<rhs[2]<<"), "<<'\n';
        for (int i=0; i<(int)lhs.size(); ++i)
        {
            if (lhs[i] == rhs[i])
                continue;
            return lhs[i] < rhs[i];
        }
        return false;
    };
    std::set<std::valarray<int>, decltype(comp)> todo;
    //todo.insert(std::valarray {1,4,1});
    //todo.insert(std::valarray {1,3,1});
    //todo.insert(std::valarray {2,3,2});
    //todo.insert(std::valarray {3,2,3});
    //todo.insert(std::valarray {4,1,4});

    //std::for_each(todo.begin(), todo.end(), [](auto e)
    //        {std::cout<<"("<<e[0]<<","<<e[1]<<","<<e[2]<<"), ";});
    //std::cout<<std::endl;
    //abort();



    std::vector<std::valarray<int>> next_dir {
        {1,  0,  1},
        {1,  1,  0},
        {1, -1,  0},
        {1,  0,  0},
        {1,  0, -1}
    };
    std::valarray<int> start_pos {0,x_start, y_start};
    if (pat(start_pos+next_dir[0]))
        todo.insert(start_pos+next_dir[0]);
    else
        todo.insert(start_pos+next_dir[3]);

    while (todo.size() > 0)
    {
        //std::for_each(todo.begin(), todo.end(), [](auto e)
        //        {std::cout<<e[0]<<", ";});
        //std::cout<<'\n';
        //std::cout<<'\n';
        auto pos = *todo.begin();
        //std::cout<<"for pos:"<<pos[0]<<","<<pos[1]<<","<<pos[2]<<'\n';
        todo.erase(todo.begin());
        for (auto dir : next_dir)
        {
            auto n_pos = pos + dir;
            if (n_pos[1] == x_end && n_pos[2] == y_end)
                return n_pos[0];
            if (pat(n_pos))
                todo.insert(n_pos);
        }
        //std::cout<<"todo.size() : "<<todo.size() <<'\n';
        //for (auto e : todo)
        //    std::cout<<"("<<e[0]<<","<<e[1]<<","<<e[2]<<"), ";
        //std::cout<<'\n';
        //std::string blah;
        //std::cin>>blah;
    }
    return 0;
}

int main(int argc, char** argv)
{
    auto start = std::chrono::steady_clock::now();

    auto test_result1 = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result1<<'\n';
    auto test_result2 = run("input_t2");
    std::cout<<"input_t2 result: "<<test_result2<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    //568 too high
}
