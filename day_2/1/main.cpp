#include <iostream>
#include <fstream>

int score(std::string filename)
{
    std::ifstream ifs {filename};
    int final_score = 0;
    for (std::string s; getline(ifs, s);)
    {
        char op = s[0] - 'A';
        char me = s[2] - 'X';
        final_score += ((me-op+4)%3)*3 + me + 1;
    }
    return final_score;
}

int main(int argc, char** argv)
{
    std::cout<<"t1: "<<score("input_t1")<<'\n';
    std::cout<<"p1: "<<score("input")<<'\n';
}


