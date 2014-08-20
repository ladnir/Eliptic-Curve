#pragma once
#define POINT int*
#define __STDC_FORMAT_MACROS
#define u64 uint64_t //unsigned char


#include <inttypes.h>
#include <stdint.h>

#include <cmath>
#include <vector>
#include <cassert>
#include <random>

using namespace std;
class FiniteField
{
public:
    FiniteField(u64, u64, int);
    FiniteField(int, vector<u64>);
    FiniteField(int);
    FiniteField();
    ~FiniteField();

	static void FiniteField::addC2(FiniteField&, FiniteField&, FiniteField&);
	static void FiniteField::addC10(FiniteField&, FiniteField&, FiniteField&);

	static void FiniteField::multiplyC2(FiniteField&, FiniteField&, FiniteField&);


    void init(int);

    int fieldSize;
    u64* num;
    

    void print64();
    void randomize64();
    void print16();
    void randomize16();
};




