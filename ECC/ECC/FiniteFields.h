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
class FiniteFieldPoint
{
public:
    FiniteFieldPoint(u64, u64, int);
    FiniteFieldPoint(int, vector<u64>);
    FiniteFieldPoint(int);
    FiniteFieldPoint();
    ~FiniteFieldPoint();


    static void FiniteFieldPoint::add(const FiniteFieldPoint&, const FiniteFieldPoint&, FiniteFieldPoint&);

    void init(int);

    int fieldSize;
    u64* num;
    

    void print64();
    void randomize64();
    void print16();
    void randomize16();
};




