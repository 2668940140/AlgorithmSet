#pragma once
#include <ctime>
#include <iostream>
namespace Debug
{
    using namespace std;
    struct
    {
        using clock_t = std::clock_t;
        clock_t cl;
        double total = 0;
        void begin() { cl = clock(); }
        void end() { total += double(clock() - cl) / CLOCKS_PER_SEC; }
        void show() { cout << total << endl; }
    } cl;

template<typename Ar>
    void showMatrix(Ar mat,int row,int col)
    {
        for (int i=0;i<row;i++)
        {
            for (int j=0;j<col;j++)
            {
                cout << mat[i][j];
            }
            cout << endl;
        }

    }

struct
{
    template <typename T ,typename... Args>
    void operator()(T arg1, Args... args)
    {
        cout << arg1;
        if constexpr(sizeof...(args)>0)
        {
            cout << ' ';
            this->operator()(args...);
        }
    }
    template <typename T>
    void operator()(T arg1)
    {
        cout << arg1 << endl;
    }
}print;

    int cnt = 0;
}