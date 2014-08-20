#include "FiniteFields.cpp"
#include <random>

#include <stdint.h>


int main(int argc, char* argv){
        
    
    FiniteField<unsigned char> ffp1(16);
    FiniteField<unsigned char> ffp2(16);
    FiniteField<unsigned char> ffp3(16);

    ffp1.randomize();
    ffp2.randomize();

    FiniteField<unsigned char>::multiplyC2(ffp1, ffp2, ffp3);

    return 1;
}