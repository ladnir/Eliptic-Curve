//#include "FiniteFields.cpp"
#include <random>

#include <stdint.h>

#include "Tests.cpp"


int main(int argc, char* argv){
    typedef uint8_t T;

	//Tests<T>::testBruteForceGCD();
    Tests<T>::testGCD();
	//Tests<T>::testMultiply1();
    //Tests<T>::testGCDvsBFInverse();

    return 1;
}