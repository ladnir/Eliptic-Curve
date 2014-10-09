#include "ECPoint.h"


ECPoint::ECPoint(int field) 
                : x(field + 1)
                , y(field + 1)
{
}


ECPoint::~ECPoint()
{
}

/** sum = augend + addend 
*/
void ECPoint::add(const ECPoint& augend,
                  const ECPoint& addend,
                        ECPoint& sum)
{
    Field lamda(sum.x.mBitCount);

    Field temp1(sum.x.mBitCount);
    Field temp2(sum.x.mBitCount);


    if (!(augend == addend)){
        //           (augend.y + addend.y) 
        // lamda =  -----------------------
        //           (augend.x + addend.x)

        Field::add(augend.x, addend.x, temp1);
        Field::invert(temp1, temp2);
        Field::add(augend.y, addend.y, temp1);
        Field::multiply(temp1, temp2, lamda);
    }
    else{
        //           augend.y  
        // lamda =  ----------  +  augend.x 
        //           augend.x 

        Field::invert(augend.x, temp1);
        Field::multiply(temp1,augend.y, temp2);
        Field::add(temp2, augend.x, lamda);
    }

    Field::square(lamda, temp1);
    Field::add(lamda, temp1   , temp1);
    Field::add(temp1, augend.x, temp1);
    Field::add(temp1, addend.x, temp1);
    Field::add(temp1, a       , sum.x); // set sum.x late

    Field::add(augend.x, sum.x, temp1);
    Field::multiply(temp1, lamda,temp2);
    Field::add(temp2,sum.x,temp2);
    Field::add(temp2, augend.y, sum.y);

}


void ECPoint::multiply(const ECPoint& multiplicand,
                             Field&   scale,
                             ECPoint& product)
{
    product.O();

    Field::bitIterator scaleIter = scale.getMSB();

    while (scaleIter.isInRnage())
    {
        ECPoint::pointDouble(product);
        if (*scaleIter) ECPoint::add(product,multiplicand, product);
    }
}