#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>

int main()
{
    std::ifstream ifs {"input"};
    std::vector<unsigned int> elves {{0}};
    for (std::string s; getline(ifs, s);) {
        if (s!="")
            *(elves.end()-1) += std::stoi(s);
        else
            elves.push_back(0);
    }

    int max_calories = *std::max_element(elves.begin(), elves.end());
    std::cout<<"p1: "<<max_calories <<'\n';

    std::sort(elves.begin(), elves.end(), std::greater<int>());
    int top_three_total = std::accumulate(elves.begin(), elves.begin()+3, 0);
    std::cout<<"p2: "<<top_three_total<<"\n";
}
