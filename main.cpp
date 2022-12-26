#include <iostream>
#include <fstream>

void run(std::string const filename) {
    std::ifstream ifs {filename};

    for (std::string line; getline(ifs, line);) {
    }

    std::cout<<"p1: ";
    std::cout<<"\np2: ";
    std::cout<<'\n';
}

int main(int argc, char** argv)
{
    run("input_t1");
    run("input");
}
