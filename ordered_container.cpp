/**
 * output : 0 x  insert x  (MINX < x < MAXX)
 * 1 x remove a x, if x does not exist, do noting
 * 1 x  Ask for the numbers strictly smaller than x, if x does not exist, expect -1
*/

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <set>

constexpr inline int MINX = 1;
constexpr inline int MAXX = 100;
constexpr inline int OPN = 1e5;

enum {INSERT, REMOVE, ASK};

int main()
{
    srand(time(NULL));
    std::multiset<int> set;

    
}