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
    std::vector<unsigned int> elf_calories;
    unsigned int current_elf_calories = 0;
    for (std::string s; getline(ifs, s);)
    {
        if (s!="")
            current_elf_calories += std::stoi(s);
        else
        {
            elf_calories.push_back(current_elf_calories);
            current_elf_calories = 0;
        }
    }
    elf_calories.push_back(current_elf_calories);

    unsigned int max_calories = *std::max_element(elf_calories.begin(), elf_calories.end()); 
    std::cout<<"Max calories elf has: "<< max_calories <<'\n'; 
    std::sort(elf_calories.begin(), elf_calories.end(), std::greater<int>());
    std::cout<<"Top three are: \n";
    unsigned int top_three_total = 0;
    for (int i=0; i<3; ++i)
    {
        top_three_total += elf_calories[i];
        std::cout<<elf_calories[i]<<'\n';
    }
    std::cout<<"top three total: "<< top_three_total <<'\n'; 
}
