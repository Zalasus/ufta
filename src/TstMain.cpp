

#include <iostream>

#include "StateSet.h"
#include "StateIterator.h"

using namespace Ufta;

int main()
{
    StateSet<uint8_t> set;
    
    set.addDiscreteState(10, 10);
    set.setBitmask("100X00XX");

    StateIterator<uint8_t> it(&set);
    while(!it.end())
    {
        std::cout << (int)*it << std::endl;
    
        ++it;
    }

    std::cout << "StateSet has " << sizeof(set) << " bytes" << std::endl;

    return 0;
}