//import std;

#include <iostream>
#include <unordered_map>
#include <fstream>

unsigned int score_round(const std::string& round)
{
    static std::unordered_map<char, unsigned int> play_scores {{'X', 1}, {'Y', 2}, {'Z', 3}};
    static std::unordered_map<std::string, unsigned int> outcomes {{"A X", 3}, {"A Y", 6}, {"A Z", 0},
                                                                   {"B X", 0}, {"B Y", 3}, {"B Z", 6},
                                                                   {"C X", 6}, {"C Y", 0}, {"C Z", 3},};
    return outcomes[round] + play_scores[round[2]]; 
}

unsigned int score(std::string filename)
{
    std::ifstream ifs {filename};
    if(!ifs)
    {
        std::cerr<<"could not open "<<filename<<'\n';
        exit(1);
    }
    unsigned int final_score = 0;
    for (std::string s; getline(ifs, s);)
    {
        unsigned int round_score = score_round(s);
        final_score += round_score;
    }
    return final_score;
}

int main(int argc, char** argv)
{
    std::cout<<"input_t1 score: "<<score("input_t1")<<'\n';
    std::cout<<"input score: "<<score("input")<<'\n';
}
