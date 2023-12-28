#pragma once
#include "debug.hpp"
class container_t
{
public:
    virtual int const *find(int x) const = 0;

    virtual void insert(int x) = 0;

    virtual void remove(int x) = 0;

    virtual ~container_t() = default;
};