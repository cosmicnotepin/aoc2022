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


luint run(std::string const filename)
{
    std::ifstream ifs {filename};
    std::regex pat {R"((\d+),(\d+),(\d+))"};
    std::smatch sm;
    while (ifs.peek() != EOF)
    {
        std::string line;
        std::getline(ifs, line);
        std::regex_match(line, sm, pat);
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
