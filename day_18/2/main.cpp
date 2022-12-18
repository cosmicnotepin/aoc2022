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
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

typedef std::valarray<int> Point;
typedef std::vector<Point> Blob;

luint run(std::string const filename)
{
    std::regex pat {R"((\d+),(\d+),(\d+))"};
    std::smatch sm;
    std::ifstream ifs {filename};
    Blob blob;
    int max_x = 0;
    int max_y = 0;
    int max_z = 0;
    while (ifs.peek() != EOF)
    {
        std::string line;
        std::getline(ifs, line);
        std::regex_match(line, sm, pat);
        //buffer of two in direction of zero
        int x = stoi(sm[1]) + 1;
        int y = stoi(sm[2]) + 1;
        int z = stoi(sm[3]) + 1;
        blob.push_back(Point{x, y, z});
        max_x = std::max(max_x, x);
        max_y = std::max(max_y, y);
        max_z = std::max(max_z, z);
    }

    //buffer of 1 around blob in direction of infinity
    max_x += 2;
    max_y += 2;
    max_z += 2;

    std::vector<std::vector<std::vector<int>>> 
        map(max_x, 
            std::vector<std::vector<int>>(max_y, 
                                          std::vector<int>(max_z, 0)
                                         )
           );

    for (auto p : blob)
        map[p[0]][p[1]][p[2]] = 1;

    int count = 0;
    std::queue<Point> todo;
    todo.push(Point{0,0});

    while (todo.size() > 0)
    {
        Point p = todo.front();
        todo.pop();
        for (int x = -1; x<2; ++x)
            for (int y = -1; y<2; ++y)
                for (int z = -1; z<2; ++z)
                {
                    int xc = p[0]+x;
                    int yc = p[1]+y;
                    int zc = p[2]+z;
                    if(std::abs(x) + std::abs(y) + std::abs(z) != 1 ||
                           xc > (int)map.size() -1 || xc < 0 || 
                           yc > (int)map[0].size() -1 || yc < 0 || 
                           zc > (int)map[0][0].size() -1 || zc < 0)
                       continue;
                    if(map[xc][yc][zc] == 1)
                        ++count;
                    if(map[xc][yc][zc] == 0)
                    {
                        map[xc][yc][zc] = -1;
                        todo.push(Point{xc,yc,zc});
                    }
                }
                       
    }

    return count;
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
}
