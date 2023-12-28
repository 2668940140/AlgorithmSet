#include "rbtree.hpp"
#include "debug.hpp"

#include <cassert>
#include <utility>
#include <deque>

// in real world, should write two version of find instead of using const_cast
#define ROOT (reinterpret_cast<node_t *&>(const_cast<void *&>(root)))

enum color_t {RED, BLACK};
namespace insert_info_ns
{
    enum insert_info_t {    NOTHING     ,    // this node does not need to do anything
                            INSERTED    ,    // inserted a red at the son or it turns to RED
                            L           ,    // inserted a red at the l of a red son
                            R           ,    // inserted a red at the r of a red son
                            LL          ,    // inserted a red at the l of a l red son
                            LR          ,    // inserted a red at the r of a l red son
                            RL          ,    // inserted a red at the l of a r red son
                            RR          ,    // inserted a red at the r of a r red son
                       };
}


/**
 * If a son is nullptr, it means it's a black leaf.
*/
struct node_t
{
    color_t color = RED;
    int black_height = 1;
    std::pair<int,int> value = std::make_pair(0,1);
    node_t *l = nullptr;
    node_t *r = nullptr;

    node_t() = default;
    explicit node_t(int val)
    {
        value = std::make_pair(val,1);
    }
    ~node_t()
    {
        delete l;
        delete r;
    }
};

int insert(int x, node_t *& now);
void update_black_height(node_t *node);
int insert_handle(node_t *&node, int type);
void push_black_down(node_t *node);
void rotate(node_t *&node, node_t *node_t::* same_p); // l is zig, r is zag
node_t *&find(int x, node_t *&now);
node_t *&find_max(node_t *&now);
void remove_node(node_t *&node, node_t *&root);
void remove_handle(node_t *node, node_t *&root);
int check(node_t *now);

rbtree_t::~rbtree_t()
{
    delete ROOT;
}

void rbtree_t::insert(int x)
{
    ::insert(x, ROOT);
    ROOT->color = BLACK;
    check(ROOT);
}

color_t get_color(node_t *node)
{
    if (node == nullptr) return BLACK;
    else return node->color;
}

int const *rbtree_t::find(int x) const
{
    node_t *node = ::find(x, ROOT);
    return node == nullptr ? nullptr : &node->value.first;
}

void rbtree_t::remove(int x)
{
    node_t *&node = ::find(x, ROOT);
    if (node != nullptr && --node->value.second)
    {
        return;
    }
    else
    {
        ::remove_node(node, ROOT);
        check(ROOT);
    }
}

void update_black_height(node_t *node)
{
    if (node == nullptr) return;
    int left_height, right_height;
    if (node->l == nullptr)
    {
        left_height = 1;
    }
    else
    {
        left_height = node->l->black_height + node->l->color;
    }
    if (node->r == nullptr)
    {
        right_height = 1;
    }
    else
    {
        right_height = node->r->black_height + node->r->color;
    }
    assert(left_height == right_height);
    node->black_height = left_height;
}

int insert(int  x, node_t *& now)
{
    using namespace insert_info_ns;
    if (now == nullptr)
    {
        now = new node_t(x);
        return INSERTED;
    }
    insert_info_t inserted_place;
    insert_info_t info;
    if (x < now->value.first)
    {
        info = static_cast<insert_info_t>(insert(x, now->l));
        inserted_place = L;
    }
    else if (x > now->value.first)
    {
        info = static_cast<insert_info_t>(insert(x, now->r));
        inserted_place = R;
    }
    else
    {
        now->value.second++;
        return NOTHING;
    }

    if (info == NOTHING)
    {
        return NOTHING;
    }
    else if (info == INSERTED)
    {
        if (now->color == BLACK)
        {
            return NOTHING;
        }
        else
        {
            return inserted_place;
        }
    }
    else
    {
        if (inserted_place == L)
        {
            if (info == L)
            {
                return insert_handle(now, LL);
            }
            else
            {
                return insert_handle(now, LR);
            }
        }
        else
        {
            if (info == L)
            {
                return insert_handle(now, RL);
            }
            else
            {
                return insert_handle(now, RR);
            }
        }
    }
}

int insert_handle(node_t *&node, int type)
{
using namespace insert_info_ns;
assert(node->color == BLACK);
if (type == LL)
{
    if (get_color(node->r) == RED)
    {
        push_black_down(node);
        return INSERTED;
    }
    else
    {
INSERT_LLB_LABEL:
        rotate(node, &node_t::l);
        // node has changed
        node->r->color = RED;
        node->color = BLACK;
        return NOTHING;
    }
}
else if (type == LR)
{
    if (get_color(node->r) == RED)
    {
        push_black_down(node);
        return INSERTED;
    }
    else
    {
        rotate(node->l, &node_t::r);
        goto INSERT_LLB_LABEL;
    }
}
else if (type == RL)
{
    if (get_color(node->l) == RED)
    {
        push_black_down(node);
        return INSERTED;
    }
    else
    {
        rotate(node->r, &node_t::l);
        goto INSERT_RRB_LABEL;
    }
}
else if (type == RR)
{
    if (get_color(node->l) == RED)
    {
        push_black_down(node);
        return INSERTED;
    }
    else
    {
INSERT_RRB_LABEL:
        rotate(node, &node_t::r);
        // node has changed
        node->l->color = RED;
        node->color = BLACK;
        return NOTHING;
    }
}
else
{
    assert(false);
}
}


void rotate(node_t *&node, node_t *node_t::* same_p)
{
    node_t *node_t::* other_p = &node_t::l == same_p ? &node_t::r : &node_t::l;
    assert(node && node->*same_p);
    node_t *p_same = node->*same_p, *mid = node;
    node_t *tmp = p_same->*other_p;
    p_same->*other_p = mid;
    mid->*same_p = tmp;
    node = p_same;
}

void push_black_down(node_t *node)
{
    node->l->color = BLACK;
    node->r->color = BLACK;
    node->color = RED;
    node->black_height++;
}

node_t *&find(int x, node_t * &now)
{
    if (now == nullptr || x == now->value.first) return now;
    if (x < now->value.first)
    {
        return find(x, now->l);
    }
    else
    {
        return find(x, now->r);
    }
}
node_t *&find_max(node_t *&now)
{
    if (now->r == nullptr) return now;
    else return find_max(now->r);
}

void remove_node(node_t *&node, node_t *&root)
{
    if (node == nullptr) return;
    if (node->l && node->r)
    {
        node_t *&swap_node = find_max(node->l);
        node->value = swap_node->value;
        remove_node(swap_node, root);
    }
    else if (node->l)
    {
        assert(get_color(node) == BLACK);
        node_t *to_delete = node;
        node = node->l;
        assert(node->color == RED);
        node->color = BLACK;

        to_delete->l = nullptr;
        delete to_delete;
    }
    else if (node->r)
    {
        assert(get_color(node) == BLACK);
        node_t *to_delete = node;
        node = node->r;
        assert(node->color == RED);
        node->color = BLACK;

        to_delete->r = nullptr;
        delete to_delete;
    }
    else
    {
        if (!(node == root || get_color(node) == RED))
        {
            remove_handle(node, root);
        }
        delete node;
        node = nullptr;
    }
}

void remove_handle(node_t *node, node_t *&root)
{
    std::deque<node_t *> path{};
    node_t *p = nullptr;
    node_t *now = root;

    while (now != node)
    {
        path.push_back(p);
        if (node->value.first <= now->value.first)
        {
            p = now;
            now = now->l;
        }
        else
        {
            p = now;
            now = now->r;
        }
    }
    node_t * node_t::* same_p, * node_t::*other_p;
REMOVE_FLAG0:
    if (p == nullptr) return;
    if (now == p->l)
    {
        same_p = &node_t::l;
        other_p = &node_t::r;
    }
    else
    {
        same_p = &node_t::r;
        other_p = &node_t::l;
    }
REMOVE_FLAG1:
    node_t *s = p->*other_p;

    if (get_color(s) == RED)
    {
        rotate(path.back()?(path.back()->l == p ? path.back()->l : path.back()->r):*&root, other_p);
        std::swap(p->color, s->color);

        path.push_back(s);
        s = p->*other_p;
        goto REMOVE_FLAG1;
    }
    else if (get_color(p) == RED && get_color(s) == BLACK
    && get_color(s->l) == BLACK && get_color(s->r) == BLACK)
    {
        std::swap(s->color, p->color);
        p->black_height--;
    }
    else if (get_color(p) == BLACK && get_color(s) == BLACK
    && get_color(s->l) == BLACK && get_color(s->r) == BLACK)
    {
        s->color = RED;
        p->black_height--;

        now = p;
        p = path.back();
        path.pop_back();
        goto REMOVE_FLAG0;
    }
    else if (get_color(s->*other_p) == RED)
    {
REMOVE_FLAG2:
        node_t *n = s->*other_p;
        rotate(path.back()?(path.back()->l == p ? path.back()->l : path.back()->r):*&root, other_p);
        std::swap(p->color, s->color);
        s->black_height++;
        p->black_height--;
        n->color = BLACK;
    }
    else if (get_color(s->*same_p) == RED)
    {
        node_t *n = s->*same_p;
        rotate(p->*other_p, same_p);
        std::swap(s->color, n->color);

        s=p->*other_p;
        goto REMOVE_FLAG2;
    }
    else
    {
        assert(0);
    }
    
}

int check(node_t *now)
{
    #if DEBUG
        if (now == nullptr)
            return 0;
        else
        {
            int l = check(now->l);
            int r = check(now->r);
            assert(l + get_color(now->l) == r + get_color(now->r) &&
            l + get_color(now->l)  == now->black_height);
            return now->black_height;
        }
    #else
        return -1;
    #endif
}