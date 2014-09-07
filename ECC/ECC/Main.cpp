//#include "FiniteFields.cpp"
#include <random>

#include <stdint.h>

#include "Tests.cpp"

int main(int argc, char* argv){
        
	Tests<uint8_t>::testBruteForceGCD();
	//Tests<uint8_t>::testMultiply1();

    return 1;
}