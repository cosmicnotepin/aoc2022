import std;

int main()
{
    std::cout<<"starting...\n";
    std::ifstream ifs {"input"};
    if(!ifs)
    {
        std::cerr<<"could not open input.txt";
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
}
