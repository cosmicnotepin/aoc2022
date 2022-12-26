#include <iostream>
#include <fstream>

int score(std::string filename)
{
    std::ifstream ifs {filename};
    int score_p1 = 0;
    int score_p2 = 0;
    for (std::string s; getline(ifs, s);)
    {
        char op = s[0] - 'A';
        char me = s[2] - 'X';
        score_p1 += ((me-op+4)%3)*3 + me + 1;
        score_p2 += ((op + me + 5)%3) + 1 + me*3;
    }
    std::cout<<"p1: "<<score_p1<<'\n';
    std::cout<<"p2: "<<score_p2<<'\n';
    return 0;
}

int main(int argc, char** argv)
{
    score("input_t1");
    score("input");
}


