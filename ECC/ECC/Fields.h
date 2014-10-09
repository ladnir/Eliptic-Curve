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
class Field
{
public:
    Field(int bitCount);
    Field(int bitCount, InputType inputType, string num);
	Field(const Field<T>& copy);

    void construct(int);

	~Field();


	static void add(const Field<T>& augend, 
					const Field<T>& addend, 
						  Field<T>& sum);


	static void multiply(const Field<T>& multiplicand, 
						 const Field<T>& multiplier, 
							   Field<T>& product);

	static void square(const Field<T>& base, 
							 Field<T>& product);

	static void division(const Field<T>& dividend,
						 const Field<T>& divisor,
							   Field<T>& quotient,
							   Field<T>& remainder);

	static void modulus(      Field<T>& number, 
						const Field<T>& mod);

	static void invert(const Field<T>& base, 
							 Field<T>& inverse);

	static void bruteForceInvert(const Field<T>& base,
									   Field<T>& inverse);

	static void extGCD(const Field<T>& a, 
					   const Field<T>& b, 
							 Field<T>& gcd,
							 Field<T>& aCoefficient,
							 Field<T>& bCoefficient);

    static void findIrrPoly(Field<T>& irrPoly);

	T& operator()(const int&)const;
	void operator<<=(const int&);
	void operator>>=(const int&);
	void operator++(int);
	bool operator==(const Field<T>&)const;
    bool operator<(const Field<T>&)const;

    bool isReducable() const;
	bool isZero()const;

	void randomize();
	void clear();
	void copy(const Field<T>&);
    void steal(Field<T>&);
    bool checkHighBits() const;

	void print() const;

	static Field<T>& getIrrPoly(int bitCount);

  

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
bool Field<T>::show = false;

template<class T> 
int Field<T>::aMals = 0;

template<class T>
void* Field<T>::irrPolys(nullptr);


