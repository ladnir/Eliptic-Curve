#include "FiniteFields.h"
#include <random>



int main(int argc, char* argv){
        
    
    auto ffp1 = FiniteField(4);
    auto ffp2 = FiniteField(4);
    auto ffp3 = FiniteField(4);

    ffp1.randomize64();
    ffp2.randomize64();
    ffp1.print64();
    ffp2.print64();

    FiniteField::add(ffp1,ffp2,ffp3);

    ffp3.print64();
    return 1;
}