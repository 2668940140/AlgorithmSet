#pragma once
#include "container.hpp"
#include "debug.hpp"

class rbtree_t : public container_t
{
    void *root = nullptr;

public:
    virtual int const *find(int x) const override;

    virtual void insert(int x) override;

    virtual void remove(int x) override;

    virtual ~rbtree_t();
};