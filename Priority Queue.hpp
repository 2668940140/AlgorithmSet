#include <vector>
#include <concepts>
#include <cassert>
#include <utility>

template <typename T>
concept has_strong_order = requires(T a, T b) {
    {
        a < b
    } -> std::convertible_to<bool>;
};

template <has_strong_order T = int>
class priority_queue
{
private:
    std::vector<T> dat = std::vector<T>(1);


public:
    T const &front() const
    {
        assert(!empty());
        return dat[1];
    }
    bool empty() const
    {
        return dat.size()==1;
    }
    void push(T const &item)
    {
        dat.emplace_back(item);
        up_maintain(dat.size()-1);
    }
    void pop()
    {
        assert(!empty());
        std::swap(dat[1],dat.back());
        dat.resize(dat.size()-1);
        down_maintain(1);
    }

private:
    void up_maintain(int cur)
    {
        while (cur > 1)
        {
            if (dat[cur] < dat[cur / 2])
            {
                std::swap(dat[cur], dat[cur / 2]);
                cur /= 2;
            }
            else
                break;
        }
    }
    void down_maintain(int cur)
    {
        while (2*cur+1 < dat.size())
        {
            int min_son_idx = (dat[2*cur]<dat[2*cur+1]?2*cur:2*cur+1);
            if (dat[min_son_idx] < dat[cur])
            {
                std::swap(dat[cur],dat[min_son_idx]);
                cur = min_son_idx;
            }
            else
                break;
        }
        if (2*cur < dat.size() && dat[2*cur]<dat[cur])
        {
            std::swap(dat[cur],dat[2*cur]);
        }
    }
};