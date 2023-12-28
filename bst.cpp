#include "bst.hpp"
#include <utility>
#include <cassert>

bst_t::~bst_t()
{
    delete root;
}

bst_t::node_t::~node_t()
{
    delete l;
    delete r;
}

void bst_t::insert(int x)
{
    _insert(x, root);
}

void bst_t::_insert(int x, node_t *&now)
{
    if (now == nullptr) [[unlikely]]
    {
        now = new node_t;
        now->val = x;
        return;
    }

    if (x < now->val)
    {
        _insert(x, now->l);
    }
    else
    {
        _insert(x, now->r);
    }
}

int const *bst_t::find(int x) const
{
    node_t const * const *ptr = _find(x, root);
    if (ptr == nullptr) return nullptr;
    else return &(*ptr)->val;

}

bst_t::node_t const * const * const bst_t::_find(int x, node_t const* const& now) const {
    if (now == nullptr) return nullptr;
    else if (now->val == x) return &now;
    else if (x < now->val)
    {
        return _find(x, now->l);
    }
    else
    {
        return _find(x, now->r);
    }
}

void bst_t::remove(int x)
{
    // Shounld implement 2 versions of _find (use template this) in real word
    node_t **ptr = const_cast<node_t **>(_find(x, root));
    
    if (ptr == nullptr) return;
    node_t *& now = const_cast<node_t *&>(*ptr);
    if (now->l && now->r)
    {
        node_t *&swap_node = node_t::get_max(now->l);
        std::swap(swap_node->val, now->val);
        _remove_leaf(swap_node);
    }
    else
    {
        _remove_leaf(now);
    }
    
}

void bst_t::_remove_leaf(node_t *&now)
{
    node_t *prev_now = now;
    if (now->l == nullptr)
    {
        now = now->r;

        prev_now->r = nullptr; //avoid recursive delete
        delete prev_now;
    }
    else if (now->r == nullptr)
    {
        now = now->l;

        prev_now->l = nullptr;
        delete prev_now;
    }
    else
    {
        assert(false);
    }
}


bst_t::node_t *&bst_t::node_t::get_max(node_t *& now)
{
    node_t ** p = &now;
    while (1)
    {
        if ((*p)->r == nullptr) return *p;
        else p = &(*p)->r;
    }
}