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
#include <valarray>
#include <chrono>

typedef long unsigned int luint;
typedef long long int llint;
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

llint snafu2dec(std::string snafu)
{
    static std::map<char, llint> sd2dd {
        {'0', 0},
        {'-', -1},
        {'=', -2},
        {'1', 1},
        {'2', 2}
    };
    llint res = 0;
    for (int i = 0; i<(int)snafu.size(); ++i)
        res += pow(5,snafu.size()-1-i) * sd2dd[snafu[i]];
    return res;
}

llint max_red(llint exp)
{
    llint max_red = 0;
    for (int j = 0; j<=exp; ++j)
        max_red += pow(5,j) * -2;
    return max_red;
}

llint usb(llint exp)
{
    return  pow(5,exp+1) - 1 + max_red(exp);
}

llint hexp(llint dec)
{
    for (int i = 0; i<1000; ++i)
        if(usb(i) >= dec)
            return i;
    return -1;
}

std::string dec2snafu(llint dec)
{
    static std::map<llint, char> dd2sd {
        { 0, '0'},
            {-1, '-'},
            {-2, '='},
            { 1, '1'},
            { 2, '2'}
    };
    //find highest snafu digit position
    llint exp = hexp(dec);
    //std::cout<<"exp : "<<exp <<'\n';
    std::string ret = "";
    for (int i=exp; i>=0; --i)
    {
        if (dec >= 0)
        {
            if (usb(i-1) < dec)
            {
                llint digit = 0;
                while ((dec -= pow(5,i)) >= max_red(i-1))
                {
                    ++digit;
                }
                dec += pow(5,i);
                ret += dd2sd[digit];
            }
            else
                ret+='0';
        }
        else // dec-rest < 0
        {
            if (std::abs(dec) < usb(i-1))
            {
                ret+='0';
                continue;
            }
            dec += pow(5,i);
            if( dec >= -usb(i-1))
            {
                ret+='-';
                continue;
            }
            dec += pow(5,i);
            ret+='=';

        }
        //std::cout<<"dec-rest: "<<dec<<'\n';
    }
    return ret;
}

std::string run(std::string const filename)
{
    std::ifstream ifs {filename};
    llint decres = 0;
    while (ifs.peek() != EOF)
    {
        std::string line;
        std::getline(ifs, line);
        //std::cout<<snafu2dec(line)<<'\n';
        decres += snafu2dec(line);
        //std::cout<<line<<" : "<<snafu2dec(line)<< " : "<<dec2snafu(snafu2dec(line))<<'\n';
    }

    return dec2snafu(decres);
}

int main(int argc, char** argv)
{
    for (llint i : {1, 2, 3, 4, 5, 6})
    {
        std::string res = dec2snafu(i);
        //std::cout<<i<<" : "<<res<<'\n';
    }
    //abort();
    auto start = std::chrono::steady_clock::now();

    auto test_result = run("input_t1");
    std::cout<<"input_t1 result: "<<test_result<<'\n';
    auto result = run("input");
    std::cout<<"input result: "<<result<<'\n';

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}
