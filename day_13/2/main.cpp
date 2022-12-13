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
#include <limits>
#include <algorithm>

typedef long unsigned int luint;

std::ostream & operator<<(std::ostream & os, std::strong_ordering ord)
{
    {
        if (std::strong_ordering::less == ord)
            return os<<"less";
        if (std::strong_ordering::equal == ord)
            return os<<"equal";
        return os<<"greater";
    }
}

std::vector<std::string> split_outer_level(std::string s)
{
    std::vector<std::string> ret;
    std::string current = "";
    luint level = 0;
    for (char c : s.substr(1))
    {
        if (level == 0 && (c==',' || c==']'))
        {
            ret.push_back(current);
            current = "";
            continue;
        }
        if (c=='[')
        {
            ++level;
        }
        if (c==']')
        {
            --level;
        }
        current += c;
    }
    return ret;
}


class List_Element
{
    public:
        List_Element() {};
        List_Element(const List_Element& rhs) 
        {
            this->value = rhs.value;
            for (auto & c : rhs.children)
            {
                this->children.push_back(std::make_unique<List_Element>(*(c.get())));
            }
        }

        List_Element& operator=(const List_Element& rhs) 
        {
            this->value = rhs.value;
            this->children.clear();
            for (auto & c : rhs.children)
            {
                this->children.push_back(std::make_unique<List_Element>(*(c.get())));
            }
            return *this;
        }

        List_Element(int value) : value{value} {};
        List_Element(std::unique_ptr<List_Element> && only_child) 
        {
            children.push_back(std::move(only_child));
        }

        List_Element(std::string s)
        {
            if (s == "[]")
            {
                //std::cout<<"made empty list element\n";
                return;
            }
            if (s[0] == '[')
            {
                for (std::string child_string : split_outer_level(s))
                {
                    children.push_back(make_unique<List_Element>(child_string));
                }
                //std::cout<<"made list element with: "<<children.size()<<" elements\n";
            } 
            else 
            {
                this->value = stoi(s);
                //std::cout<<"made simple element with value: "<<value<<'\n';
            }
        }

        auto operator<=>(const List_Element & rhs) const
        {
            if (this->value != -1 && rhs.value != -1)
            {
                //two values
                //std::cout<<"simple comp:\n'";
                //std::cout<<"this->value : "<<this->value <<'\n';
                //std::cout<<"rhs.value: "<<rhs.value<<'\n';
                //std::cout<<"this->value <=> rhs.value: "<<(this->value <=> rhs.value)<<'\n';
                return this->value <=> rhs.value;
            }
            if (this->value == -1 && rhs.value == -1)
            {
                //two lists
                for (size_t i=0; i<this->children.size(); ++i)
                {
                    if (i == rhs.children.size())
                        //left list ist longer
                        return std::strong_ordering::greater;

                    std::strong_ordering this_elem = *(this->children[i].get()) <=> *(rhs.children[i].get());
                    if (this_elem == std::strong_ordering::equal)
                        continue;
                    else
                        return this_elem;

                }
                if (this->children.size() < rhs.children.size())
                    //left shorter
                    return std::strong_ordering::less;
                else
                    //same size
                    return std::strong_ordering::equal;
            }
            // one list, one value
            if (this->value != -1)
            {
                //list is right, package left in in list
                return List_Element(std::make_unique<List_Element>(this->value)) <=> rhs;
            } else
            {
                //list is left, package right in list
                return *this <=> List_Element(std::make_unique<List_Element>(rhs.value));
            }
        }

        bool operator==(const List_Element & rhs) const
        {
            return ((*this<=>rhs) == std::strong_ordering::equal);
        }


        int value = -1;
        std::vector<std::unique_ptr<List_Element>> children;
};

luint run(std::string const filename)
{
    std::ifstream ifs {filename};
    if(!ifs)
    {
        std::cerr<<"could not open "<<filename<<'\n';
        exit(1);
    }

    std::vector<List_Element> all {{"[[2]]"}, {"[[6]]"}};
    while (ifs.peek() != EOF)
    {
        std::string line;
        std::getline(ifs, line);
        List_Element le1(line);
        std::getline(ifs, line);
        List_Element le2(line);
        all.push_back(le1);
        all.push_back(le2);
        std::getline(ifs, line);
    }
    //std::sort(all.begin(), all.end());
    std::ranges::sort(all);

    luint ret = 1;
    for (size_t i=0; i< all.size(); ++i)
    {
        if (all[i] == List_Element("[[2]]") || all[i] == List_Element("[[6]]"))
        {
            ret *= i+1;
        }
    }

    return ret;
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
}
