#pragma once
#include<stdio.h>
#include<tuple>

class Base{
    char* _p;
    unsigned int _size;
    public:
    Base(unsigned int size):_p(new char[size]()), _size(size){
    }
    virtual ~Base(){
        delete[] _p;
        printf("del Base...");
    }
    auto getP(){ return std::tuple(_p, _size); }

    virtual void print() = 0;

};

class Derived :public Base{
    char* dp;
    public:
    Derived(int size):Base(size){
        auto [p, s] = getP();
        dp = p;
    }
    ~Derived() override{
        printf("del Derived...");
    }

    void print() override{
        auto [p, s] = getP();
        for (short i = 0; i < s; i++)
            printf("%c, ", dp[i]);
        printf("\n");
    }
};