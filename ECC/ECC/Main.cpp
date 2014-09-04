#include "FiniteFields.cpp"
#include <random>

#include <stdint.h>


int main(int argc, char* argv){
        
    
    FiniteField<uint8_t> ffp1(16);
    FiniteField<uint8_t> ffp2(16);
    FiniteField<uint8_t> gcd(16);
    FiniteField<uint8_t> aCo(16);
    FiniteField<uint8_t> bCo(16);

    FiniteField<uint8_t> sum(16);

    ffp1.randomize();
    ffp2.randomize();

    //ffp1(0) = (uint8_t)-1;
    //ffp1(1) = (uint8_t)-1;
    /*ffp2(0) = (uint8_t)-1;
    ffp2(1) = (uint8_t)-1;*/

    //FiniteField<uint8_t>::multiply(ffp1, ffp2, prod);
	FiniteField<uint8_t>::extGCD(ffp1, ffp2, gcd, aCo, bCo);

	cout << "a " << ffp1 << endl;
	cout << "b " << ffp2 << endl;
	cout << "gcd " << gcd << endl;
	cout << "aCo " << aCo << endl;
	cout << "bCo " << bCo << endl;

    //FiniteField<uint8_t>::add(prod, rem, sum);

    //printf("\nsum  ");
    //sum.bitPrint();

    //printf("\nffp1 ");
    //ffp1.bitPrint();

    //FiniteField<unsigned char>::multiply(ffp1, ffp2, ffp3);
    //FiniteField<uint8_t>::extGCD(ffp1,ffp2,ffp3);
    return 1;
}