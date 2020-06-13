// consoleSimpleApp.cpp : Defines the entry point for the console application.
//

/*#include <cstddef>
#include <iostream>

class CTmp
{
    public:

    size_t ptrValue()
    {
        //return reinterpret_cast<std::size_t>(this);
        return (size_t)this;
    }
};

int main()
{
    CTmp tmp1;
    CTmp tmp2;

    std::cout << tmp1.ptrValue() << std::endl;
    std::cout << tmp2.ptrValue() << std::endl;

    return 0;
}*/

/*#include <vector>
#include <ranges>
#include <iostream>
 
int main()
{
    std::vector<int> ints{0,1,2,3,4,5};
    auto even = [](int i){ return 0 == i % 2; };
    auto square = [](int i) { return i * i; };
 
    for (int i : ints | std::views::filter(even) | std::views::transform(square)) {
        std::cout << i << ' ';
    }

    return 0;
}*/

/*#include <string>
#include <format>

int main()
{
    std::string s = std::format("I'd rather be {1} than {0}.", "right", "happy");

    return 0;
}*/