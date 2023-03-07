#pragma once

#include <ctime>
#include <iostream>
struct
{
    using clock_t = std::clock_t;
    clock_t cl;
    double total = 0;
    void begin() { cl = clock(); }
    void end() { total += double(clock() - cl) / CLOCKS_PER_SEC; }
    void show() { std::cout << total << std::endl; }
} cl;