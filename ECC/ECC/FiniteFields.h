#pragma once
#define __STDC_FORMAT_MACROS


#include <inttypes.h>
#include <stdint.h>

#include <cmath>
#include <list>
#include <vector>
#include <cassert>
#include <random>


using namespace std;

template<class T>
class FiniteField
{
public:
    FiniteField(int);
    FiniteField(const FiniteField<T>&);
    ~FiniteField();


    static void add(const FiniteField<T>& augend, 
                    const FiniteField<T>& addend, 
                          FiniteField<T>& sum);

    static void multiply(const FiniteField<T>& multiplicand, 
                         const FiniteField<T>& multiplier, 
                               FiniteField<T>& product);

    static void modulus(      FiniteField<T>& number, 
                        const FiniteField<T>& mod);

    static void division(const FiniteField<T>& dividend,
                         const FiniteField<T>& divisor,
                               FiniteField<T>& quotient,
                               FiniteField<T>& remainder);

    static void invert(const FiniteField<T>& base, 
                             FiniteField<T>& inverse);

    static void extGCD(const FiniteField<T>& a, 
                       const FiniteField<T>& b, 
                             FiniteField<T>& gcd,
                             FiniteField<T>& aCoefficient,
                             FiniteField<T>& bCoefficient);
    
    
    T& operator()(const int&)const;
    void operator<<=(const int&);
    void operator>>=(const int&);

    bool operator==(const FiniteField<T>&)const;
    bool isZero()const;

    void randomize();
    void clear();

    void bitPrint() const;
    void bitPrint(int) const;
    void print() const;

    static FiniteField<T>& getIrrPoly(int bitCount);
    
    //static FiniteField<T>* irrPoly;
    int mWordSize;
    int mWordCount;
    int mBitCount;
    T* num;
};

