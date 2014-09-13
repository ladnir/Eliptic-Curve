//#include "FiniteFields.cpp"
#include <random>

#include <stdint.h>

#include "Tests.cpp"


int main(int argc, char* argv){
    typedef uint64_t T;

	//Tests<T>::testBruteForceGCD();
    Tests<T>::testGCD();
    //Tests<T>::testMultiply3();
    //Tests<T>::testGCDvsBFInverse();
    Tests<T>::testIsReducable();
    Tests<T>::testFindIrrPoly();

    return 1;
}