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
#include <unordered_set>
#include <functional>
#include <limits>
#include <algorithm>
#include <chrono>
#include <valarray>

typedef long unsigned int luint;
typedef long long int llint;
typedef std::string ss;
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


std::unordered_map<std::string, std::function<std::string()>> monkeys;

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
            std::string val = sm[2];
            auto monkey_do = [val]()
            {
                return val;
            };
            monkeys.insert({sm[1], std::function<std::string()> {monkey_do}});
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
            
            auto monkey_do = [op, lhs, rhs, fun]() mutable
            {
                auto lm = monkeys[lhs]();
                auto rm = monkeys[rhs]();

                try
                {
                    return std::to_string(fun(std::stoll(lm), std::stoll(rm)));
                } catch (std::invalid_argument const & e)
                {
                    ;
                }
                return "(" + lm + " " + op + " " + rm + ")";
            };
            if (sm[1] == "root")
            {
                auto monkey_do2 = [op, lhs, rhs]() mutable
                {
                    auto lm = monkeys[lhs]();
                    auto rm = monkeys[rhs]();
                    return lm + " " + "==" + " " + rm;
                };
                monkeys.insert({sm[1], std::function<std::string()> {monkey_do2}});
                continue;
            }
            monkeys.insert({sm[1], std::function<std::string()> {monkey_do}});
        }
    }

    auto monkey_do = []()
    {
        return "humn";
    };
    monkeys.insert_or_assign("humn", std::function<std::string()> {monkey_do});

    std::string res = monkeys["root"]();
    //std::cout<<res<<'\n';
    std::regex pat_res {R"((.*?) == (.*?))"};
    std::smatch sm_res;
    std::regex_match(res, sm_res, pat_res);
    //std::cout<<"succ : "<<succ <<'\n';
    std::string to_peel = sm_res[1];
    //std::cout<<"to_peel : "<<to_peel <<'\n';
    llint rhs = stoll(sm_res[2]);
    //std::cout<<"rhs : "<<rhs <<'\n';
    //std::cout<<"to_peel.substr(-2,2) : "<<to_peel.substr(to_peel.size()-2,2) <<'\n';
    std::regex pat_left {R"(\((\d+) (.) (.*))"};
    std::regex pat_right {R"((.+?) (.) (\d+)\))"};
    while (to_peel.substr(0,1) == "(")
    {
        //std::cout<<to_peel<<" == " << rhs<<'\n';
        llint val;
        std::string op;
        if (std::regex_match(to_peel, sm, pat_left))
        {
            val = std::stoll(sm[1]);
            op = sm[2];
            to_peel = sm[3];
            to_peel.pop_back();
            if (op == "+")
            {
                rhs -= val;
            }
            if (op == "-")
            {
                rhs = -1 *(rhs-val);
            }
            if (op == "/")
            {
                rhs = val/rhs;
            }
            if (op == "*")
            {
                rhs = rhs/val;
            }

        } else
        {
            std::regex_match(to_peel, sm, pat_right);
            val = std::stoll(sm[3]);
            op = sm[2];
            to_peel = sm[1];
            to_peel = to_peel.substr(1);
            if (op == "-")
            {
                rhs += val;
            }
            if (op == "/")
            {
                rhs *= val;
            }
            if (op == "+")
            {
                rhs -= val;
            }
            if (op == "*")
            {
                rhs /= val;
            }
        }

    }
    //std::cout<<to_peel<<" == " << rhs<<'\n';
    return rhs;
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
