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

typedef long unsigned int luint;

class Monkey
{
    public:
        Monkey(std::queue<int> items, std::function<int(int)> inspect, std::function<int(int)> get_target):
            items{items},
            inspect{inspect},
            get_target{get_target} {};

        std::queue<int> items;
        std::function<int(int)> inspect;
        std::function<int(int)> get_target;
        int number_inspections = 0;
};

std::queue<int> parse_items(std::string line)
{
    static std::regex pat {R"(\d+)"};
    std::queue<int> items;
    for (auto itit = std::sregex_iterator(line.begin(), line.end(), pat);
            itit != std::sregex_iterator(); ++itit)
        items.push(stoi((*itit)[0]));

    return items;
}

std::function<int(int)> parse_inspect(std::string line)
{
    std::function<int(int)> inspect;
    std::function<int(int,int)> fun;
    if (line[23] == '*')
        fun = std::multiplies<int> {};
    if (line[23] == '+')
        fun = std::plus<int> {};

    if (line[25] == 'o')
        inspect = [fun](int par){return fun(par, par);};
    else
    {
        int val = stoi(line.substr(25));
        inspect = [fun,val](int par){return fun(par, val);};
    }

    return inspect;
}

luint run(std::string const filename)
{
    std::ifstream ifs {filename};
    if(!ifs)
    {
        std::cerr<<"could not open "<<filename<<'\n';
        exit(1);
    }

    std::vector<Monkey> monkeys;
    while (ifs.peek() != EOF)
    {
        std::string line;
        std::getline(ifs, line);
        if(line == "")
            continue;

        std::getline(ifs, line);
        std::queue<int> items = parse_items(line);

        std::getline(ifs, line);
        std::function<int(int)> inspect = parse_inspect(line);

        std::getline(ifs, line);
        int divisor = stoi(line.substr(21));
        std::getline(ifs, line);
        int target_1 = stoi(line.substr(29));
        std::getline(ifs, line);
        int target_2 = stoi(line.substr(30));
        std::function<int(int)> get_target = [divisor, target_1, target_2](int par)
        {
            return (par % divisor) ? target_2 : target_1;
        };
        monkeys.emplace_back(items, inspect, get_target);
    }

    for (int i=0; i<20; ++i)
        for (Monkey & m : monkeys)
            while (m.items.size() > 0) {
                int worry_level = m.inspect(m.items.front()) / 3;
                ++m.number_inspections;
                m.items.pop();
                size_t target_index = m.get_target(worry_level);
                monkeys[target_index].items.push(worry_level);
            }

    std::sort(monkeys.begin(), monkeys.end(),
            [](const Monkey & m_1, const Monkey & m_2) {
                return m_1.number_inspections > m_2.number_inspections;
            });

    return monkeys[0].number_inspections * monkeys[1].number_inspections;
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
}
