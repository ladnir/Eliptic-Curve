#include "FiniteFields.h"
#include <random>



int main(int argc, char* argv){
        
    
    auto ffp1 = FiniteFieldPoint(4);
    auto ffp2 = FiniteFieldPoint(4);
    auto ffp3 = FiniteFieldPoint(4);

    ffp1.randomize64();
    ffp2.randomize64();
    ffp1.print64();
    ffp2.print64();

    FiniteFieldPoint::add(ffp1,ffp2,ffp3);

    ffp3.print64();
    return 1;
}