import std;

int main(int argc, char** argv)
{
    std::string filename = "input";
    if (argc == 2)
        filename = argv[1];

    std::ifstream ifs {filename};
    if(!ifs)
    {
        std::cerr<<"could not open "<<filename<<'\n';
        exit(1);
    }
    std::vector<unsigned int> elf_calories{0};
    for (std::string s; getline(ifs, s);)
    {
        if (s=="")
            elf_calories.push_back(0);
        else
             elf_calories[elf_calories.size() - 1] += std::stoi(s);
    }

    unsigned int max_calories = *std::max_element(elf_calories.begin(), elf_calories.end()); 
    std::cout<<"Max calories elf has: "<< max_calories <<'\n'; 
    std::sort(elf_calories.begin(), elf_calories.end(), std::greater<int>());
    unsigned int top_three_total = std::accumulate(elf_calories.begin(), elf_calories.begin()+3, 0);
    std::cout<<"Top three are: "<<top_three_total<<"\n";
}
