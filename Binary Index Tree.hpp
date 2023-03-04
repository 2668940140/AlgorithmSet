#include <vector>
#include <cstddef>

//T is the value type
//change and range sum in O(n^2)
//range in [1,size]
class BIT
{
    using vector = std::vector<int>;
    vector array;
    size_t MAXN=0;

private:
    inline int lowbit(int x) const {return x & (-x);}

public:
    BIT(size_t size) : MAXN(size) {array.assign(size+1,0);}  
    void reset(size_t size) noexcept {MAXN=size;array.assign(size+1,0);};
    
    //modify [index] dirextly
    void modify(int index, int x)
    {
        int changeVal = x-array[index];
        for (int i=index;i<=MAXN;i+= lowbit(i))
        {
            array[i]+=changeVal;
        }
    }

    //[index] += changeVal
    void increment(int index, int changeVal)
    {
        for (int i=index;i<=MAXN;i+= lowbit(i))
        {
            array[i]+=changeVal;
        }
    }

    //sum of [1,n]
    int sum(int n) const
    {
        int out = 0;
        for (int i=n;i>0;i-=lowbit(i))
        {
            out += array[i];
        }
        return out;
    }

    //sum of [l,r]
    int sum(int l, int r) const
    {
        return sum(r)-sum(l-1);
    }

    //single point query
    int operator[](int index) const { return sum(index,index);}
};