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


std::unordered_map<std::string, std::function<llint()>> monkeys;

llint run(std::string const filename)
{
    std::ifstream ifs {filename};
    std::regex pat {R"((.+): (.+) (.) (.+))"};
    std::regex pat_simp {R"((.+): (\d+))"};
    std::smatch sm;
    monkeys.clear();
    while (ifs.peek() != EOF)
    {
        std::string line;
        std::getline(ifs, line);
        if (std::regex_match(line, sm, pat_simp))
        {
            llint val = stoi(sm[2]);
            auto monkey_do = [val]()
            {
                return val;
            };
            monkeys.insert({sm[1], std::function<llint()> {monkey_do}});
        } 
        else {
            std::regex_match(line, sm, pat);
            std::string op = sm[3];
            std::string lhs = sm[2];
            std::string rhs = sm[4];
            std::function<llint(llint,llint)> fun = std::divides<llint> {};
            if (op == "+")
                fun = std::plus<llint> {};
            if (op == "-")
                fun = std::minus<llint> {};
            if (op == "*")
                fun = std::multiplies<llint> {};
            
            auto monkey_do = [fun, lhs, rhs]()
            {
                return fun(monkeys[lhs](),monkeys[rhs]());
            };
            monkeys.insert({sm[1], std::function<llint()> {monkey_do}});
        }
    }

    return monkeys["root"]();
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
