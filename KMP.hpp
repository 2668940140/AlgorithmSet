#include<vector>
#include<cstring>
#include<cassert>

std::vector<int> KMP(char const* src, char const* pattern)
{
    using namespace std;
    int n=strlen(src),m=strlen(pattern);
    assert(m);
    vector<int> ret;
    vector<int> N(m);

    for(int i=1;i<m;i++)
    {
        int j=i;
        bool found=false;
        while (j)
        {
            if(pattern[i]==pattern[N[j-1]])
            {
                found=true;
                break;
            }
            j=N[j-1];
        }
        N[i]=found?N[j-1]+1:0;
    }

    int i=0,j=0;
    while(i+j<=n) //保证
    {
        if(j<m&&i+j<n&&src[i+j]==pattern[j])
        {
            j++;
        }
        else
        {
            if(j==m)
            {
                ret.emplace_back(i);
            }
            if(j)
            {
                i+=(j-N[j-1]);
                j=N[j-1];
            }
            else
            {
                i++;
            }
        }
    }
    return ret;
}