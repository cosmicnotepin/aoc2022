#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <regex>
#include <stack>
#include <sstream>
#include <list>
#include <string>
#include <memory>

typedef long unsigned int luint;

class Node
{
    public:
        Node(std::string name, luint size, std::shared_ptr<Node> parent, bool is_dir) : name{name}, size{size}, parent{parent}, is_dir{is_dir} {};
        Node(std::string name, bool is_dir) : name{name}, size{size}, parent{parent}, is_dir(is_dir) {};
        std::string name;
        luint size = 0;
        std::unordered_map<std::string, std::shared_ptr<Node>> children;
        std::shared_ptr<Node> parent;
        bool is_dir = false;
};

void print_all(std::shared_ptr<Node> n, std::string spaces)
{
    std::cout<<spaces<<n->name;
    if (n->children.size() > 0)
        std::cout<<" : "<<n->size<<'\n';
    else
        std::cout<<'\n';
    for (const auto & [key, value] : n->children)
        print_all(value, spaces+"  ");
}

luint rec_size(std::shared_ptr<Node> root)
{
    for (auto & [key, value] : root->children)
        root->size += rec_size(value);
    return root->size;
}


luint sum_smaller_equal(std::shared_ptr<Node> root, luint threshold)
{
    luint ret = 0;
    if (root->is_dir && root->size <= threshold)
        ret += root->size;
    for (auto & [key, value] : root->children)
        ret += sum_smaller_equal(value, threshold);
    return ret;
}

luint find_best_del_dir(std::shared_ptr<Node> root, luint best_so_far, luint needed_free)
{
    if (!root->is_dir)
        return best_so_far;

    if (root->size < needed_free)
        return best_so_far;

    luint ret = best_so_far;
    for (auto & [key, value] : root->children)
        ret = std::min(ret, find_best_del_dir(value, best_so_far, needed_free));
   ret = std::min(ret, root->size); 
   return ret;
}

luint run(std::string const filename)
{
    std::ifstream ifs {filename};
    if(!ifs)
    {
        std::cerr<<"could not open "<<filename<<'\n';
        exit(1);
    }

    std::string line; 
    getline(ifs, line);
    std::shared_ptr<Node> root = std::make_shared<Node>("/", true);//first line is always cd /
    std::shared_ptr<Node> current_node = root;
    while (ifs.peek() != EOF)
    {
        std::cout<<"==========current tree==========\n";
        print_all(current_node, "");

        getline(ifs, line);
        if (line.substr(0, 4) == "$ cd")
        {
            std::string param = line.substr(5);
            if (param == "..")
            {
                current_node = current_node->parent;
            }
            else
            {
                current_node = current_node->children[param];
            }
            
        } 
        else if (line.substr(0, 4) == "$ ls")
        {
            while (ifs.peek() != '$' && ifs.peek() != EOF)
            {
                std::string line; 
                getline(ifs, line);
                std::stringstream ss(line);
                std::string dir_size;
                std::string name;
                ss>>dir_size;
                ss>>name;
                if(dir_size == "dir")
                {
                    std::shared_ptr<Node> n = std::make_shared<Node>(name, 0, current_node, true);
                    current_node->children.insert({n->name, n});
                } else
                {
                    std::shared_ptr<Node> n = std::make_shared<Node>(name, stoul(dir_size), current_node, false);
                    current_node->children.insert({n->name, n});
                }
            }
        }
    }
    print_all(root, "");
    rec_size(root);
    print_all(root, "");
    luint total = 70000000;
    luint min_free = 30000000;
    luint cur_free = total - root->size;
    luint needed_free = min_free - cur_free;
    return find_best_del_dir(root, root->size, needed_free); 
}

int main(int argc, char** argv)
{
    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';
}
