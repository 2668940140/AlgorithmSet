#pragma once
#include "container.hpp"
#include "debug.hpp"
class bst_t : public container_t
{
    struct node_t
    {
        node_t *l = nullptr;
        int val = 0;
        node_t *r = nullptr;
        static node_t *&get_max(node_t *&now);
        ~node_t();
    } *root = nullptr;

    void _insert(int x, node_t *&now);

    node_t const * const * const _find(int x, node_t const* const& now) const;

    void _remove_leaf(node_t *&node);
public:
    virtual int const *find(int x) const override;

    virtual void insert(int x) override;

    virtual void remove(int x) override;

    virtual ~bst_t();
};