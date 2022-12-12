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

class Patch
{
    public:
        Patch(char height, luint x, luint y):
            height{height},
            x{x},
            y{y}
        {};

        char height;
        luint x;
        luint y;
        luint pathlength = ULONG_MAX;
        luint prev_x;
        luint prev_y;
};

class Patch_Compare
{
    public:
        bool operator()(Patch const * lhs, Patch const * rhs) const {return lhs->pathlength < rhs->pathlength;}
};


std::list<Patch*> get_neighbours(std::vector<std::vector<Patch>>& map, const Patch patch)
{
    std::list<Patch*> neighbours;
    if (patch.x != 0)
        neighbours.push_back(&map[patch.y][patch.x - 1]);
    if (patch.y != 0)
        neighbours.push_back(&map[patch.y - 1][patch.x]);
    if (patch.x + 1 < map[0].size())
        neighbours.push_back(&map[patch.y][patch.x + 1]);
    if (patch.y + 1 < map.size())
        neighbours.push_back(&map[patch.y + 1][patch.x]);
    return neighbours;
}

luint path_length(const std::vector<std::vector<Patch>>& map, const Patch * end, const Patch * start)
{
    const Patch* current = end;
    luint steps = 0;
    while (!(current->x == start->x && current->y == start->y))
    {
        current = &map[current->prev_y][current->prev_x];
        ++steps;
    }
    return steps;
}


luint shortest_path_length(std::vector<std::vector<Patch>>& map, Patch * start, const Patch * end)
{
    std::multiset<Patch*, Patch_Compare> todo;
    todo.insert(start);
    while (todo.size() > 0)
    {
        Patch* current = *todo.begin();
        todo.erase(todo.begin());
        if (current->x == end->x and current->y == end->y)
            return path_length(map, current, start);
        auto neighbours = get_neighbours(map, *current);
        for (Patch*  neighbour : neighbours)
        {
            if (current->height >= neighbour->height - 1 && neighbour->pathlength > current->pathlength + 1)
            {
                neighbour->pathlength = current->pathlength + 1;
                neighbour->prev_x = current->x;
                neighbour->prev_y = current->y;
                todo.insert(neighbour);
            }
        }
    }
    return 0;
}

luint run(std::string const filename)
{
    std::ifstream ifs {filename};
    if(!ifs)
    {
        std::cerr<<"could not open "<<filename<<'\n';
        exit(1);
    }

    size_t y = 0;
    Patch start {'a', 0, 0};
    Patch end {'z', 0, 0};
    std::vector<std::vector<Patch>> map;
    while (ifs.peek() != EOF)
    {
        std::vector<Patch> map_row;
        std::string line;
        std::getline(ifs, line);
        size_t x = 0;
        for (char c : line)
        {
            if (c == 'S')
            {
                start.x = x;
                start.y = y;
                start.pathlength = 0;
                map_row.push_back(start);
            } else if (c == 'E')
            {
                end.x = x;
                end.y = y;
                map_row.push_back(end);
            } else
            {
                map_row.push_back(Patch(c, x, y));
            }
            ++x;
        }
        map.push_back(map_row);
        ++y;
    }

    std::cout<<"start: "<<start.x<<", "<<start.y<<'\n';
    std::cout<<"end: "<<end.x<<", "<<end.y<<'\n';
    std::cout<<"map_x: "<<map[map.size()-1].size()<<'\n';
    std::cout<<"map_y: "<<map.size()<<'\n';
    std::cout<<"N S: "<<get_neighbours(map, start).size()<<'\n';
    std::cout<<"N E: "<<get_neighbours(map, end).size()<<'\n';
    return shortest_path_length(map, &start, &end);
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
}
