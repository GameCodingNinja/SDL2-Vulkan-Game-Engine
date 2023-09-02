// consoleSimpleApp.cpp : Defines the entry point for the console application.
//

#include <cstddef>
#include <iostream>
#include <cstdint>

class CTmp
{
    public:

    enum theEnum: uint64_t
    {
        dummy1,
        dummy2,
        dummy3,
        dummy4,
        dummy5,
        dummy6,
        dummy7,
    };

    theEnum dummy;
};

int main()
{
    std::cout << sizeof(CTmp) << std::endl;


    return 0;
}

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