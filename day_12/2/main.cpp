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

        bool operator ==(const Patch & rhs) const {return this->x == rhs.x && this->y == rhs.y;};
        bool operator !=(const Patch & rhs) const {return !(*this==rhs);};
        //bool operator <(const Patch & rhs) const {return this->pathlength < rhs.pathlength;};

        char height;
        luint x;
        luint y;
        luint pathlength = ULONG_MAX;
        Patch * previous;
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
    while (*current != *start)
    {
        current = current->previous;
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
        if (*current == *end)
            return path_length(map, current, start);
        auto neighbours = get_neighbours(map, *current);
        for (Patch*  neighbour : neighbours)
        {
            if (current->height >= neighbour->height - 1 && neighbour->pathlength > current->pathlength + 1)
            {
                neighbour->pathlength = current->pathlength + 1;
                neighbour->previous = current;
                todo.insert(neighbour);
            }
        }
    }
    return ULONG_MAX;
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
    luint shortest_shortest = ULONG_MAX;
    for (size_t y = 0; y<map.size(); ++y)
    {
        for (size_t x = 0; x<map.size(); ++x)
        {
            if (map[y][x].height == 'a')
            {
                //std::cout<<"shortest_shortest : "<<shortest_shortest <<'\n';
                map[y][x].pathlength = 0;
                shortest_shortest = std::min(shortest_shortest, shortest_path_length(map, &map[y][x], &end));
                for (size_t y = 0; y<map.size(); ++y)
                {
                    for (size_t x = 0; x<map.size(); ++x)
                    {
                        map[y][x].pathlength = ULONG_MAX;
                    }
                }
            }

        }
    }
    return shortest_shortest;
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
}
