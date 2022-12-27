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

typedef long unsigned int luint;

struct Patch
{
    int x;
    int y;
    int pl;
};

void run(std::string const filename) {
    std::ifstream ifs {filename};
    Patch start {0, 0, 0};
    Patch end {0, 0, 0};
    std::vector<std::string> map;
    size_t y = 0;
    for (std::string line; std::getline(ifs, line);) {
        for (size_t x=0; x<line.size(); ++x) {
            if (line[x] == 'S')
            {
                start.x = x;
                start.y = y;
                line[x] = 'a';
            } else if (line[x] == 'E')
            {
                end.x = x;
                end.y = y;
                line[x] = 'z';
            }
        }
        map.push_back(line);
        ++y;
    }

    bool visited[map.size()][map[0].size()];
    memset(visited, 0, map.size()*map[0].size());

    std::deque<Patch> todo;
    todo.push_back(start);
    int n[4][2] {{-1,0},{0,-1},{1,0},{0,1}};
    while (todo.size() > 0)
    {
        Patch cur = *(todo.begin());
        todo.erase(todo.begin());
        if (cur.x == end.x and cur.y == end.y) {
            std::cout<<"p1: "<<cur.pl<<'\n';
            break;
        }
        for (auto d : n) {
            int nx = cur.x + d[0];
            int ny = cur.y + d[1];
            if ( nx >= 0 &&
                    ny >= 0 &&
                    nx < (int)map[0].size() &&
                    ny < (int)map.size() &&
                    map[cur.y][cur.x] + 1 >= map[ny][nx] &&
                    !(visited[ny][nx])) {
                todo.push_back(Patch(nx,ny,cur.pl+1));
                visited[ny][nx] = true;
            }
        }
    }
}

int main(int argc, char** argv)
{
    run("input_t1");
    run("input");
    
}
