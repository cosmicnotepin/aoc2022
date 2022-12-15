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

bool check(const lint & mx, const lint & my, 
        const std::vector<std::valarray<lint>> & sensors,
        const std::vector<lint> & mh_reaches)
{
    bool it = true;
    //std::cout<<"testing: "<<mx<<','<<my<<'\n';
    for (size_t j = 0; j<sensors.size(); ++j)
    {
        //std::cout<<"sensor: "<<i<<" check: "<<j<<'\n';
        lint s_y = sensors[j][1];
        lint s_x = sensors[j][0];
        lint reach = mh_reaches[j];
        //std::cout<<"reach : "<<reach <<'\n';
        lint dist = std::abs(s_y - my) + std::abs(s_x - mx);
        //std::cout<<"dist : "<<dist <<'\n';
        if (dist <= reach)
        {
            //std::cout<<"sensor: "<<i<<" is reached by: "<<j<<'\n';
            it = false;
            break;
        }
    }
    return it;
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
    std::vector<lint> mh_reaches;
    while (ifs.peek() != EOF)
    {
        std::string line;
        std::getline(ifs, line);

        auto coords = std::sregex_iterator(line.begin(), line.end(), pat);
        lint sensor_x = std::stoi((*coords)[1]);
        lint sensor_y = std::stoi((*coords)[2]);
        std::valarray<lint> sensor {sensor_x, sensor_y};
        sensors.push_back(sensor);

        ++coords;
        std::valarray<lint> beacon {std::stoi((*coords)[1]), std::stoi((*coords)[2])};
        std::valarray<lint> dist = beacon - sensor;

        lint mh_reach = std::abs(dist[0]) + std::abs(dist[1]);
        mh_reaches.push_back(mh_reach);
    }

    for (size_t i=0; i<sensors.size(); ++i)
    {
        for (lint y = -mh_reaches[i]-1; y<=mh_reaches[i]+1; ++y)
        {
            lint my = sensors[i][1] + y;
            lint rest_reach = mh_reaches[i] + 1 - std::abs(y); 
            lint mx = sensors[i][0] - rest_reach;
            bool it = check(mx,my,sensors,mh_reaches);
            if (!it)
            {
                mx = sensors[i][0] + rest_reach;
                it = check(mx,my,sensors,mh_reaches);
            }
            if (it && mx > 0 && mx<max_search + 1 && my > 0 and my<max_search + 1)
            {
                std::cout<<"mx : "<<mx <<'\n';
                std::cout<<"my : "<<my <<'\n';
                return mx * 4000000 + my;
            }
        }
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
