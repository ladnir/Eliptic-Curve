#pragma once
#define __STDC_FORMAT_MACROS


#include <inttypes.h>
#include <stdint.h>

#include <cmath>
#include <vector>
#include <cassert>
#include <random>

using namespace std;

template<class T>
class FiniteField
{
public:
    FiniteField(int);
    ~FiniteField();


    static void addC2(FiniteField<T>&, FiniteField<T>&, FiniteField<T>&);

    static void multiplyC2(FiniteField<T>&, FiniteField<T>&, FiniteField<T>&);

    T getBit(int);

    void operator<<=(const int&);
    void operator>>=(const int&);

    void init(int);

    int mWordCount;
    int mBitCount;
    T* num;
    
    void bitPrint();
    void print();

    void randomize();
};




