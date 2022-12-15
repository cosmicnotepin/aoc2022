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

luint run(std::string const filename, lint row)
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
        lint sensor_y = std::stoi((*coords)[2]);
        max_x = std::max(max_x, sensor_x);
        max_y = std::max(max_y, sensor_y);
        min_x = std::min(min_x, sensor_x);
        min_y = std::min(min_y, sensor_y);
        std::valarray<lint> sensor {sensor_x, sensor_y};
        sensors.push_back(sensor);

        ++coords;
        std::valarray<lint> beacon {std::stoi((*coords)[1]), std::stoi((*coords)[2])};
        beacons.push_back(beacon);
        std::valarray<lint> dist = beacon - sensor;

        lint mh_reach = std::abs(dist[0]) + std::abs(dist[1]);
        mh_reaches.push_back(mh_reach);
        max_mh_reach = std::max(max_mh_reach, mh_reach);
    }

    lint offset_x_pos = min_x - max_mh_reach;

    luint not_possbible = 0;
    for (lint x = offset_x_pos; x<max_x + max_mh_reach; ++x)
    {
        for (size_t i =0; i<sensors.size(); ++i)
        {
            luint x_diff = std::abs(sensors[i][0] - x);
            luint y_diff = std::abs(sensors[i][1] - row);
            if (mh_reaches[i] >= (x_diff + y_diff))
            {
                ++not_possbible;
                break;
            }
        }
        
    }
    std::cout<<"not_possbible : "<<not_possbible <<'\n';

    std::set<std::valarray<lint>, valarray_comp> beacons_in_row;
    auto row_filter = [row](std::valarray<lint> a){return (a[1] == row);};
    for (auto b : beacons | std::views::filter(row_filter))
        beacons_in_row.insert(b);

    std::cout<<"beacons_in_row : "<<beacons_in_row.size() <<'\n';
    return not_possbible - beacons_in_row.size();
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1", 10);
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input", 2000000);
    std::cout<<"input result: "<<result<<'\n';
}
