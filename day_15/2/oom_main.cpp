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
#include <numeric>
#include <ranges>
#include <chrono>

typedef long unsigned int luint;
typedef long int lint;

class valarray_comp
{

    public:
        bool operator()(const std::valarray<lint> & lhs, const std::valarray<lint> & rhs) const
        {
            using namespace std;
            return to_string(lhs[0])+to_string(lhs[1]) < to_string(rhs[0])+to_string(rhs[1]); 
        };
};

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


void print_map(const auto & map)
{
    std::cout<<"\nthe map\n";
    for (auto row : map)
    {
        for (auto column : row)
        {
            std::cout<<column;
        }
        std::cout<<'\n';
    }
}

luint run(std::string const filename, lint max_search)
{
    std::ifstream ifs {filename};
    if(!ifs)
    {
        std::cerr<<"could not open "<<filename<<'\n';
        exit(1);
    }

    std::regex pat {R"(x=(-?\d*?), y=(-?\d*?)(:|$))"};
    std::vector<std::valarray<lint>> sensors;
    std::vector<std::valarray<lint>> beacons;
    std::vector<luint> mh_reaches;
    lint max_x = std::numeric_limits<lint>::min(); 
    lint max_y = std::numeric_limits<lint>::min(); 
    lint min_x = std::numeric_limits<lint>::max(); 
    lint min_y = std::numeric_limits<lint>::max(); 
    lint max_mh_reach = 0;
    while (ifs.peek() != EOF)
    {
        std::string line;
        std::getline(ifs, line);

        auto coords = std::sregex_iterator(line.begin(), line.end(), pat);
        lint sensor_x = std::stoi((*coords)[1]);
        //std::cout<<"sensor_x : "<<sensor_x <<'\n';
        lint sensor_y = std::stoi((*coords)[2]);
        //std::cout<<"sensor_y : "<<sensor_y <<'\n';
        max_x = std::max(max_x, sensor_x);
        max_y = std::max(max_y, sensor_y);
        min_x = std::min(min_x, sensor_x);
        min_y = std::min(min_y, sensor_y);
        std::valarray<lint> sensor {sensor_x, sensor_y};
        sensors.push_back(sensor);

        ++coords;
        //std::cout<<"(*coords)[1]: "<<(*coords)[1]<<'\n';
        //std::cout<<"(*coords)[2]: "<<(*coords)[2]<<'\n';
        std::valarray<lint> beacon {std::stoi((*coords)[1]), std::stoi((*coords)[2])};
        beacons.push_back(beacon);
        std::valarray<lint> dist = beacon - sensor;

        lint mh_reach = std::abs(dist[0]) + std::abs(dist[1]);
        mh_reaches.push_back(mh_reach);
        max_mh_reach = std::max(max_mh_reach, mh_reach);
    }

    //offset everything to be able to use positions as indices 
    //into std::<vector>std::vector<bool>> and to not waste space
    lint offset_x_pos = min_x - max_mh_reach;
    lint offset_y_pos = min_y - max_mh_reach;


    lint maxlines = 1;
    lint blocks = max_search / maxlines;
    for (lint b = 0; b<blocks + 1; ++b)
    {
        auto start = std::chrono::steady_clock::now();
        std::vector<std::vector<bool>> map(maxlines, std::vector<bool>(max_search + 1));
        lint offset = b * maxlines;
        //print_map(map);
        for (size_t i = 0; i<sensors.size(); ++i)
        {
            lint s_y = sensors[i][1];
            lint s_x = sensors[i][0];
            lint reach = mh_reaches[i];
            for (lint y = -reach; y<reach+1; ++y)
            {
                lint m_y = s_y + y;
                if (m_y < 0 + offset || m_y >= offset + maxlines)
                    continue;

                lint rest_reach = reach - std::abs(y); 
                for (lint x = -rest_reach; x<rest_reach+1; ++x)
                {
                    lint m_x = s_x + x;
                    if (m_x < 0 || m_x > max_search + 1)
                        continue;
                    //std::cout<<"m_y: "<<m_y<<'\n';
                    //std::cout<<"offset: "<<offset<<'\n';
                    //std::cout<<"m_x : "<<m_x <<'\n';
                    map[m_y-offset][m_x] = true;
                }
            }
        }
        for (size_t y = 0; y<map.size(); ++y)
            for (size_t x = 0; x<map[0].size(); ++x)
                if (map[y][x] == false)
                    return x * 4000000 + y + offset;
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    }

        


    return 0;
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1", 20);
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input", 4000000);
    std::cout<<"input result: "<<result<<'\n';
}
