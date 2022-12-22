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
typedef long long int llint;
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

std::vector<std::vector<char>> dir2char = 
{{' ','^',' '},
 {'<',' ','>'},
 {' ','v',' '},
};

std::vector<std::vector<int>> dir2score = 
{{-1, 3,-1},
 { 2,-1, 0},
 {-1, 1,-1},
};

void cross_corner(int & x, int & y, int & x_dir, int & y_dir)
{
    int from_x = x;
    int from_y = y;
    if (x == 1 && y >= 151 && y <= 200)
    {
        //face1 left
        x = y-100;
        y = 1;
        x_dir = 0;
        y_dir = 1;
        return;
    }
    if (x == 1 && y >= 101 && y <= 150)
    {
        //face2 left
        y = 51 - (y - 100);
        x = 51;
        x_dir = 1;
        y_dir = 0;
        return;
    }
    if (x == 51 && y>=1 && y <= 50)
    {
        //face5 left
        y = 151 - y;
        x = 1;
        x_dir = 1;
        y_dir = 0;
        return;
    }
    if (x == 51 && y>=51 && y <= 100)
    {
        //face4 left
        x = y-50;
        y = 101;
        x_dir = 0;
        y_dir = 1;
        return;
    }
    if (x == 50 && y >= 151 && y <= 200)
    {
        //face 1 right
        x = y-100;
        y = 151;
        x_dir = 0;
        y_dir = -1;
        return;
    }
    if (x == 100 && y>=51 && y <= 100)
    {
        //face4 right
        x = y + 50;
        y = 50;
        x_dir = 0;
        y_dir = -1;
        return;
    }
    if (x == 100 && y>=101 && y <= 150)
    {
        //face3 right
        y = 151 - y;
        x = 150;
        x_dir = -1;
        y_dir = 0;
        return;
    }
    if (x == 150 && y>=1 && y <= 50)
    {
        //face6 right
        y = 151 - y;
        x = 100;
        x_dir = -1;
        y_dir = 0;
        return;
    }
    if(y == 1 && x >= 51 && x <= 100)
    {
        //face5 top
        y = x + 100;
        x = 1;
        x_dir = 1;
        y_dir = 0;
        return;
    }
    if(y == 1 && x >= 101 && x <= 150)
    {
        //face6 top
        x = x-100;
        y = 200;
        x_dir = 0;
        y_dir = -1;
        return;
    }
    if(y == 50 && x >= 101 && x <= 150)
    {
        //face6 bot
        y = x - 50;
        x = 100;
        x_dir = -1;
        y_dir = 0;
        return;
    }
    if(y == 101 && x >= 1 && x <= 50)
    {
        //face2 top
        y = x + 50;
        x = 51;
        x_dir = 1;
        y_dir = 0;
        return;
    }
    if(y == 150 && x >= 51 && x <= 100)
    {
        //face3 bot
        y = x + 100;
        x = 50;
        x_dir = -1;
        y_dir = 0;
        return;
    }
    if(y == 200 && x >= 1 && x <= 50)
    {
        //face1 bot
        x = x + 100;
        y = 1;
        y_dir = 1;
        x_dir = 0;
        return;
    }
    std::cout<<"\nfrom_x: "<<from_x<<" from_y: "<<from_y<<'\n';
    std::cout<<"x: "<<x<<" y: "<<y<<'\n';
}

void pm(std::vector<std::vector<char>> const & map, std::vector<std::vector<char>> const & mmap)
{
    for (size_t y=0; y<map.size(); ++y)
    {
        for (size_t x=0; x<map[0].size(); ++x)
        {
            if (mmap[y][x] != ' ')
            {
                std::cout<<mmap[y][x];
                continue;
            }
            std::cout<<map[y][x];
        }
        std::cout<<'\n';
    }
}

void move(std::vector<std::vector<char>> const & map,
        std::vector<std::vector<char>> & mmap,
        int & x_pos, int & y_pos,
        int & x_dir, int & y_dir, int dist)
{

    while (dist > 0)
    {
        //pm(map, mmap);
        int n_x = x_pos + x_dir;
        int n_y = y_pos + y_dir;
        --dist;

        switch (map[n_y][n_x])
        {
            case '.':
                mmap[n_y][n_x] = dir2char[y_dir+1][x_dir+1];
                x_pos = n_x;
                y_pos = n_y;
                continue;
            case '#':
                dist = 0;
                continue;
            case ' ':
                int n_x_dir = -5;
                int n_y_dir = -5;
                n_x = x_pos;
                n_y = y_pos;
                cross_corner(n_x, n_y, n_x_dir, n_y_dir);
                if (map[n_y][n_x] == '.')
                {
                    x_pos = n_x;
                    y_pos = n_y;
                    x_dir = n_x_dir;
                    y_dir = n_y_dir;
                    mmap[n_y][n_x] = dir2char[y_dir+1][x_dir+1];
                } else
                    dist = 0;
                continue;
        }
    }
}

void turn(int & x_dir, int & y_dir, char const & dir,
        std::vector<std::vector<char>> & mmap,
        int const x_pos, int const y_pos)
{

    if (dir == 'L')
    {
        int x_dir_tmp = 0 * x_dir + 1*y_dir;
        y_dir = -1 * x_dir + 0*y_dir;
        x_dir = x_dir_tmp;
    } else if (dir == 'R')
    {
        int x_dir_tmp = 0 * x_dir + -1*y_dir;
        y_dir = 1 * x_dir + 0*y_dir;
        x_dir = x_dir_tmp;
    } else
        std::cerr<<"BS-dir for turn()\n";

    mmap[y_pos][x_pos] = dir2char[y_dir + 1][x_dir + 1];
}

luint run(std::string const filename)
{
    std::ifstream ifs {filename};
    size_t max_x = 0;
    while (ifs.peek() != EOF)
    {
        std::string line;
        std::getline(ifs, line);
        if(line.size() == 0)
            break;
        max_x = std::max(max_x, line.size());
    }

    max_x += 2;

    ifs.seekg(0);
    std::vector<std::vector<char>> map;
    std::vector<std::vector<char>> mmap;
    map.push_back(std::vector<char>(max_x, ' '));
    mmap.push_back(std::vector<char>(max_x, ' '));
    std::string moves;
    while (ifs.peek() != EOF)
    {
        std::string line;
        std::getline(ifs, line);
        if(line.size() == 0)
        {
            std::getline(ifs, line);
            moves = line;
            break;
        }
        std::vector<char> row(max_x, ' ');
        for (size_t i=0; i<line.size(); ++i)
            row[i+1] = line[i];
        map.push_back(row);
        mmap.push_back(std::vector<char>(max_x, ' '));
    }
    map.push_back(std::vector<char>(max_x, ' '));
    mmap.push_back(std::vector<char>(max_x, ' '));
    //pm(map, mmap);
    //std::cout<<moves<<'\n';

    int x_dir = 1;
    int y_dir = 0;
    int x_pos;
    for (int i=0; i<(int)map[1].size(); ++i)
        if (map[1][i] == '.')
        {
            x_pos = i;
            break;
        }
    int y_pos = 1;
    mmap[y_pos][x_pos] = '>';
    //pm(map, mmap);
    turn(x_dir, y_dir, 'L', mmap, x_pos, y_pos);
    //pm(map, mmap);
    turn(x_dir, y_dir, 'R', mmap, x_pos, y_pos);
    //pm(map, mmap);
    int moves_i = 0;
    while (moves_i < (int)moves.size())
    {
        std::string dist_s = "";
        while (std::isdigit(moves[moves_i]))
            dist_s += moves[moves_i++];
        int dist = std::stoi(dist_s);
        //std::cout<<"dist : "<<dist <<'\n';
        move(map, mmap, x_pos, y_pos, x_dir, y_dir, dist);
        if (moves_i < (int)moves.size())
            turn(x_dir, y_dir, moves[moves_i++], mmap, x_pos, y_pos);
    }
    //pm(map,mmap);

    return (llint)1000 * (llint)y_pos + (llint)4*(llint)x_pos + (llint)dir2score[y_dir+1][x_dir+1];
}

int main(int argc, char** argv)
{
    //turn test
    //for (int i : {-1,0,1})
    //    for (int j : {-1,0,1})
    //    {
    //        //std::cout<<i<<j<<'\n';
    //        if (std::abs(i) + std::abs(j) > 1)
    //            continue;
    //        int x_dir = i;
    //        int y_dir = j;
    //        std::cout<<"x_dir: "<<x_dir<<", y_dir: "<<y_dir<<'\n';
    //        turn(x_dir, y_dir, 'L');
    //        std::cout<<"turn left\n";
    //        std::cout<<"x_dir: "<<x_dir<<", y_dir: "<<y_dir<<'\n';
    //        turn(x_dir, y_dir, 'R');
    //        std::cout<<"turn right\n";
    //        std::cout<<"x_dir: "<<x_dir<<", y_dir: "<<y_dir<<'\n';
    //    }

    auto start = std::chrono::steady_clock::now();

    //auto test_result = run("input_t1");
    //std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    //not 28505
    ////102250 too low
}
