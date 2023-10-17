#include <vector>
#include <cmath>
#include <algorithm>
/*
Function f(x,y)=f(y,x)
has the property f(f(x,y),z)=f(x,f(y,z))
which means f(x,y,z....)=f(any oder of x,y,z.....)
f(f(SetA),f(SetB))=f(SetA U SetB)
*/

template<typename T , typename BiF>
class ST
{
private:
    std::vector<std::vector<T>> storage;
    size_t m,n;
    BiF biF;
public:
    ST(const T* begin,const T* end)
    {
        n = end - begin + 1;
        m = static_cast<size_t>(ceil(log2(n)));
        storage.assign(m,std::vector<T>(n,T()));
        std::copy(begin,end,storage[0].begin());
        for (int i=1;i<m;i++)
            for (int j=0;j+(1<<i)-1<n;j++)
            {
                storage[i][j]=biF(storage[i-1][j],storage[i-1][j+(1<<(i-1))]);
            }
    }

    //ans in [i,j]
    T query(int i, int j)
    {
        int len = static_cast<int>(log2(j-i+1));
        return biF(storage[len][i],storage[len][j-(1<<len)+1]);
    }
};