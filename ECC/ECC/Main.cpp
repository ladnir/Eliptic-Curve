#include "FiniteFields.cpp"
#include <random>

#include <stdint.h>


int main(int argc, char* argv){
        
    
    FiniteField<uint8_t> ffp1(16);
    FiniteField<uint8_t> num(16);
    FiniteField<uint8_t> ffp2(16);
    FiniteField<uint8_t> quo(16);
    FiniteField<uint8_t> rem(16);

    FiniteField<uint8_t> sum(16);

    ffp1.randomize();
    ffp2.randomize();

    num(0) = 2;
    //FiniteField<uint8_t>::multiply(ffp1,num,prod);
    FiniteField<uint8_t> prod(ffp1.mBitCount);
    //prod <<= 2

	//ffp1(0) = (uint8_t)-1;
	//ffp1(1) = (uint8_t)-1;
	//ffp2(0) = (uint8_t)-1;
	//ffp2(1) = (uint8_t)-1;

    FiniteField<uint8_t>::multiply(ffp1, ffp2, prod);

    printf("\n = \n q ");
    quo.bitPrint();
    printf("\n r ");
    rem.bitPrint();


    //FiniteField<uint8_t>::add(prod, rem, sum);

    //printf("\nsum  ");
    //sum.bitPrint();

    //printf("\nffp1 ");
    //ffp1.bitPrint();

    //FiniteField<unsigned char>::multiply(ffp1, ffp2, ffp3);
    //FiniteField<uint8_t>::extGCD(ffp1,ffp2,ffp3);
    return 1;
}