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

    FiniteField<T>& getIrrPoly();

    static void addC2(FiniteField<T>&, FiniteField<T>&, FiniteField<T>&);
    static void multiplyC2(FiniteField<T>&, FiniteField<T>&, FiniteField<T>&);
    static void modulus(FiniteField<T>&, const FiniteField<T>&);

    void operator<<=(const int&);
    void operator>>=(const int&);



    void bitPrint();
    void print();

    void randomize();
    
    static FiniteField<T>* irrPoly;

    int mWordCount;
    int mBitCount;
    T* num;
};




