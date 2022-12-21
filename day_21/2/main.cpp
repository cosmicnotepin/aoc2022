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
typedef long long int llint;
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


std::unordered_map<std::string, std::function<std::pair<llint, bool>()>> monkeys;

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
                return std::pair(val, false);
            };
            monkeys.insert({sm[1], std::function<std::pair<llint, bool>()> {monkey_do}});
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
            
            std::pair<llint, bool> res;
            bool dirty = true;
            auto monkey_do = [fun, lhs, rhs, res, dirty]() mutable
            {
                if(!dirty)
                    return res;
                auto lm = monkeys[lhs]();
                auto rm = monkeys[rhs]();
                dirty = lm.second || rm.second;
                res = std::pair(fun(lm.first,rm.first), dirty);
                return res;
            };
            if (sm[1] == "root")
            {
                auto monkey_do2 = [fun, lhs, rhs, res, dirty]() mutable
                {
                    if(!dirty)
                        return res;
                    auto lm = monkeys[lhs]();
                    auto rm = monkeys[rhs]();
                    dirty = lm.second || rm.second;
                    res = std::pair(lm.first - rm.first, dirty);
                    return res;
                };
                monkeys.insert({sm[1], std::function<std::pair<llint, bool>()> {monkey_do2}});
                continue;
            }
            monkeys.insert({sm[1], std::function<std::pair<llint, bool>()> {monkey_do}});
        }
    }

//230587548238637679 : res.first = 8;
//230587548238637678 : res.first = 28;
//230587548238637680 : res.first = -10;
    llint max = std::numeric_limits<llint>::max();
    llint test = max/2 - max/4;
    auto monkey_do = [test]()
    {
        return std::pair(test, true);
    };
    monkeys.insert_or_assign("humn", std::function<std::pair<llint,bool>()> {monkey_do});

    std::pair<llint, bool> res = monkeys["root"]();
    while (res.first != 0)
    {
        std::cout<<"res.first : "<<res.first <<'\n';
        if (std::abs(res.first) < 10)
        {
            ++test;
        }
        else
        {
            if(res.first < 0)
            {
                max = test;
                test = test/2;
            }
            std::cout<<"max : "<<max <<'\n';
            std::cout<<"test : "<<test <<'\n';
            if(res.first > 0)
                test += (max-test)/2;
        }
        std::cout<<"test : "<<test <<'\n';
        auto monkey_do = [test]()
        {
            return std::pair(test, true);
        };
        monkeys.insert_or_assign("humn", std::function<std::pair<llint,bool>()> {monkey_do});
        res = monkeys["root"]();
    }

    return test;
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    //auto result = run("input");
    //std::cout<<"input result: "<<result<<'\n';
}
