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
    FiniteField<uint8_t> prod(ffp1);
    prod <<= 2;
    prod(0) += 1;
    //ffp2.bitPrint();
    //printf(" \n/ ");
    //ffp1.bitPrint();
    //FiniteField<uint8_t>::division(prod, ffp1, quo, rem);
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