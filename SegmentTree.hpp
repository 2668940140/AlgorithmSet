#include <vector>
#include <cstddef>
#include <cmath>

template<typename ll = long long>
struct STree
{
private:
    std::vector<ll> tree;
    std::vector<ll> mark;
    const size_t SIZE;
    const size_t LEN;

private:
    void build(ll l, ll r, ll idx, ll* ar) //[l,r]
    {
        if (r == l)
            tree[idx]=ar[l];
        else
        {
            int mid = l + (r-l)/2;
            build(l,mid,idx*2,ar);
            build(mid+1,r,idx*2+1,ar);
            tree[idx]=tree[idx*2]+tree[idx*2+1];
        }
    }

    void pushDown(ll idx, ll len)
    {
        mark[idx*2] += mark[idx];
        mark[idx*2+1] += mark[idx];
        tree[idx*2] += mark[idx] * (len - len/2);
        tree[idx*2+1]+=mark[idx]*(len/2);
        mark[idx]=0;
    }

    void update(ll l, ll r, ll cl, ll cr, ll idx,ll val)
    {
        if (r<cl||l>cr) return;
        if (l>=cl&&r<=cr)
        {
            mark[idx]+=val;
            tree[idx]+=val*(r-l+1);
            return;
        }
        else
        {
            ll mid = l + (r - l) / 2;
            pushDown(idx,r-l+1);
            update(l,mid,cl,cr,idx*2,val);
            update(mid+1,r,cl,cr,idx*2+1,val);
            tree[idx] = tree[idx*2]+tree[2*idx+1];
        }
    }
    
    ll query(ll l, ll r, ll cl, ll cr, ll idx)
    {
        if (r<cl||l>cr) return 0;
        if (l>=cl&&r<=cr)
        {
            return tree[idx];
        }
        else
        {
            pushDown(idx,r-l+1);
            ll mid = l + (r-l)/2;
            return query(l,mid,cl,cr,idx*2)+query(mid+1,r,cl,cr,idx*2+1);
        }
    }
public:

    STree(ll* begin, ll* end) : SIZE(size_t(ceil(log2(end-begin)+1.0)*(end-begin))) , LEN(end-begin)//avoid pushDown exceeded range
    {
        tree.assign(SIZE,0);
        mark.assign(SIZE,0);
        build(0,end-begin-1,1,begin);
    }

    ll ask(ll l, ll r) //[l,r]
    {
        return query(0,LEN-1,l,r,1);
    }

    void modify(ll l, ll r, ll val)
    {
        update(0,LEN-1,l,r,1,val);
    }
};
