#pragma once
#define __STDC_FORMAT_MACROS


#include <inttypes.h>
#include <stdint.h>

#include <cmath>
#include <list>
#include <vector>
#include <cassert>
#include <random>

enum InputType {Base2, Base10, Base16};

template<class T>
class BitIterator;

using namespace std;

template<class T>
class FiniteField
{
public:
    FiniteField(int bitCount);
    FiniteField(int bitCount, InputType inputType, string num);
	FiniteField(const FiniteField<T>& copy);

    void construct(int);

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

	static void bruteForceInvert(const FiniteField<T>& base,
									   FiniteField<T>& inverse);

	static void extGCD(const FiniteField<T>& a, 
					   const FiniteField<T>& b, 
							 FiniteField<T>& gcd,
							 FiniteField<T>& aCoefficient,
							 FiniteField<T>& bCoefficient);

    static void findIrrPoly(FiniteField<T>& irrPoly);

	T& operator()(const int&)const;
	void operator<<=(const int&);
	void operator>>=(const int&);
	void operator++(int);
	bool operator==(const FiniteField<T>&)const;
    bool operator<(const FiniteField<T>&)const;

    bool isReducable() const;
	bool isZero()const;

	void randomize();
	void clear();
	void copy(const FiniteField<T>&);
    void steal(FiniteField<T>&);
    bool checkHighBits() const;

	void print() const;

	static FiniteField<T>& getIrrPoly(int bitCount);

  

	int mWordSize;
	int mWordCount;
	int mBitCount;
	T* num;
	static bool show;
    static int aMals;

	typedef BitIterator<T> bitIterator;

	bitIterator getLSB() const;
	bitIterator getMSB() const;
	bitIterator getBit(int) const;


    static void* irrPolys;
};

template<class T> 
bool FiniteField<T>::show = false;

template<class T> 
int FiniteField<T>::aMals = 0;

template<class T>
void* FiniteField<T>::irrPolys(nullptr);


