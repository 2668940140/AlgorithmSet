#include <random>
#include <ctime>
#include <set>
#include <cstdio>
#include <cassert>

#include "container.hpp"
#include "bst.hpp"
#include "rbtree.hpp"

constexpr int kMinx = 0;
constexpr int kMaxx = 1e2;
constexpr int kOpn = 1e5;

enum {INSERT, DELETE, FIND};

template <typename Container>
void uniform_random_test();

template <typename Container>
void sequential_test();
int main(int, char**)
{
    uniform_random_test<rbtree_t>();
    sequential_test<rbtree_t>();
}

template <typename Container>
void uniform_random_test()
{
    std::minstd_rand rnd(0);
    std::uniform_int_distribution<int> number_gen(kMinx, kMaxx);
    std::uniform_int_distribution<int> op_gen(0, 2);
    
    container_t *c = new Container;
    clock_t start_time = clock();
#if DEBUG
    std::multiset<int> s;
    for (int i = 0; i < kOpn; i++)
    {
        int x = number_gen(rnd);
        int op = op_gen(rnd);
        switch (op)
        {
            case INSERT:
            s.insert(x);
            c->insert(x);
            break;
            case DELETE:
            if (auto p = s.find(x); p != s.end())
            {
                s.erase(p);
            }
            c->remove(x);
            break;
            case FIND:
            auto p1 = s.find(x);
            auto p2 = c->find(x);
            assert(p1 == s.end() && p2 == nullptr ||
            p1 != s.end() && p2 != nullptr);
            break;
        }
    }
#else
    for (int i = 0; i < kOpn; i++)
    {
        int x = number_gen(rnd);
        int op = op_gen(rnd);
        switch (op)
        {
            case INSERT:
            c->insert(x);
            break;
            case DELETE:
            c->remove(x);
            break;
            case FIND:
            auto p2 = c->find(x);
            break;
        }
    }
#endif
    clock_t end_time = clock();
    delete c;
    double kops = (double)kOpn / (end_time - start_time) * CLOCKS_PER_SEC / 1000;
    printf("uniform_random_test: %.1lf kops\n", kops);
}

template <typename Container>
void sequential_test()
{
    std::minstd_rand rnd(0);
    std::uniform_int_distribution<int> op_gen(0, 2);
    
    container_t *c = new Container;
    clock_t start_time = clock();
#if DEBUG
    std::multiset<int> s;
    for (int i = 0; i < kOpn; i++)
    {
        int x = (double) i / kOpn * (kMaxx - kMinx) + kMinx;
        int op = op_gen(rnd);
        switch (op)
        {
            case INSERT:
            s.insert(x);
            c->insert(x);
            break;
            case DELETE:
            if (auto p = s.find(x); p != s.end())
            {
                s.erase(p);
            }
            c->remove(x);
            break;
            case FIND:
            auto p1 = s.find(x);
            auto p2 = c->find(x);
            assert(p1 == s.end() && p2 == nullptr ||
            p1 != s.end() && p2 != nullptr);
            break;
        }
    }
#else
    for (int i = 0; i < kOpn; i++)
    {
        int x = (double) i / kOpn * (kMaxx - kMinx) + kMinx;
        int op = op_gen(rnd);
        switch (op)
        {
            case INSERT:
            c->insert(x);
            break;
            case DELETE:
            c->remove(x);
            break;
            case FIND:
            auto p2 = c->find(x);
            break;
        }
    }
#endif
    clock_t end_time = clock();
    delete c;
    double kops = (double)kOpn / (end_time - start_time) * CLOCKS_PER_SEC / 1000;
    printf("sequential_test: %.1lf kops\n", kops);
}