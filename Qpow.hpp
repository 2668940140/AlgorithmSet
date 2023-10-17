#pragma once

inline long long qpow(int base, int exp)
{
    long long out = 1;
    while (exp)
    {
        if (exp & 1)
            out *= base;
        exp >>= 1;
        base *= base;
    }
    return out;
}