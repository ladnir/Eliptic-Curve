#include "Tests.h"

#include "Fields.cpp"

template<class T>
bool Tests<T>::runAll()
{
    bool result = true;

    result &= testMultiply();
    result &= testGCD();
    result &= testMultDivide();

    return result;
}

template<class T>
bool Tests<T>::testMultDivide()
{

        Field<T> num1(16);
        Field<T> num2(16);
        Field<T> prod(16);

        num1(0) = 14;
        num2(0) = 9;

        Field<T>::multiply(num1, num2, prod);

        Field<T> quo(16);
        Field<T> rem(16);


        Field<T>::division(prod, num2, quo, rem);

        if (!(rem.isZero()) || !(quo == num1))
        {
            Field<T>::show = true;
            Field<T>::multiply(num1, num2, prod);
            Field<T>::division(prod, num2, quo, rem);
            Field<T>::show = false;

            cout << endl;
            cout << "  " << num1 << endl;
            cout << "* " << num2 << endl;
            cout << "= " << prod << endl;
            
            cout << endl;
            cout << "  " << prod << endl;
            cout << "/ " << num2 << endl;
            cout << "= " << quo << endl;
            cout << "r " << rem << endl;

        }
    

    return true;
}

template<class T>
bool Tests<T>::testMultiply()
{
    bool result;

    Field<T> num1(16, Base2, "0000011000000111");
    Field<T> num2(16, base2, "0000000000000001");
    Field<T> prod(16);



    Field<T> prodTest(num1);
    

    while (!*prod.getMSB())
    {
        Field<T>::multiply(num1, num2, prod);

        result = prod == prodTest;

        if (!result)
        {
            Field<T>::show = true;
            Field<T>::multiply(num1, num2, prod);
            Field<T>::show = false;

            cout << "  " << num1 << endl;
            cout << "* " << num2 << endl;
            cout << "= " << prod << endl;

            cout << endl;
            cout << "c " << prodTest << endl;
            cout << "multiply failed" << endl;
        }


        num2 <<= 1;
        prodTest <<= 1;
    }

    return result;
}


template<class T>
bool Tests<T>::testMultiply1()
{
    Field<T> num1(16, Base2, "0010001100101001");
    cout << "num1 " << num1 << endl;
    Field<T> num2(16, Base2, "0000000000000100");
    cout << "num2 " << num2 << endl;

    Field<T> prod(16);

    Field<T>::multiply(num1, num2, prod);
    cout << prod << " = " << num1 << " * " << num2 << endl;

    return true;
}

template<class T>
bool Tests<T>::testMultiply2()
{

    Field<T> num1(9, Base2,      "01010011"); // 83
    Field<T> num2(9, Base2,      "11001010"); // 202
    Field<T> knownProd(9, Base2, "00000001"); // 1
    Field<T> prod(9);

    //Field<T> quo(9);
    //Field<T> rem(9);
    //Field<T>::show = true;
    Field<T>::multiply(num1, num2, prod);

    cout << "prod " << prod << endl;
    cout << "Kprod " << knownProd << endl;

    assert(knownProd == prod);
    return true;
}

template<class T>
bool Tests<T>::testMultiply3()
{

    Field<T> num1(10, Base2, "01010011"); // 
    Field<T> num2(10);//, Base2, "11001010"); // 
    Field<T> knownProd(10, Base2, "00000001"); // 
    Field<T> prod(10);

    Field<T>::bruteForceInvert(num1,num2);

    Field<T>::multiply(num1, num2, prod);

    cout << "prod " << prod << endl;
    cout << "Kprod " << knownProd << endl;

    assert(knownProd == prod);
    return true;
}

template<class T>
bool Tests<T>::testWikiDivision()
{


    Field<T> num1(9,Base2, "01010011"); // 83
    Field<T> num2(9,Base2, "11001010"); // 202

    //Field<T> quo(9);
    //Field<T> rem(9);
    Field<T> prod(9);
    //Field<T>::show = true;
    Field<T>::division(num1, num2, prod);

    cout << prod << endl;

    return true;
}

template<class T>
bool Tests<T>::testWikiInverse()
{
    Field<T> base(9, Base2, "01010011"); // 83
    Field<T> knownInv(9, Base2, "11001010"); // 202
    Field<T> inv(9);

    Field<T>::invert(base, inv);

    assert(inv == knownInv);
    return inv == knownInv;
}

template<class T>
bool Tests<T>::testWikiBruteForceInverse()
{
    Field<T> base(9, Base2, "01010011"); // 83
    Field<T> knownInv(9, Base2, "11001010"); // 202
    Field<T> inv(9);

    Field<T>::bruteForceInvert(base, inv);

    assert(inv == knownInv);
    return inv == knownInv;
}

template<class T>
bool Tests<T>::testGCD()
{
    Field<T> a(10, Base2, "01010011"); // 83
    Field<T> b(10, Base2, "11001010"); // 202
    Field<T> gcd(10);
    Field<T> aCo(10);
    Field<T> bCo(10);

    Field<T> aProd(10);
    Field<T> bProd(10);
    Field<T> sum(10);


    Field<T>::extGCD(a, b, gcd, aCo, bCo);

    cout << "a " << a << endl;
    cout << "b " << b << endl;
    cout << "gcd " << gcd << endl;
    cout << "aCo " << aCo << endl;
    cout << "bCo " << bCo << endl;

    Field<T>::multiply(a, aCo, aProd);
    Field<T>::multiply(b, bCo, bProd);

    Field<T>::add(aProd, bProd, sum);

    cout << endl;
    cout << "aPo " << aProd << endl;
    cout << "bPo " << bProd << endl;

    cout << endl;
    cout << "sum " << sum << endl;
    cout << "gcd " << gcd << endl;

    assert(sum == gcd);

    return sum == gcd;
}

template<class T>
bool Tests<T>::testGCDvsBFInverse()
{
    {
        Field<T> a(9);
        Field<T> aInverse(9);
        Field<T> aBFInverse(9);
        Field<T> gcd(9);

        Field<T>& irrPoly = Field<T>::getIrrPoly(9);
        Field<T> irrPolyCo(9);

        for (int i = 0; i < 10000; i++)
        {
            a.randomize();
            if (a.isZero()) continue;

            Field<T>::bruteForceInvert(a,aBFInverse);

            Field<T>::extGCD(irrPoly, a, gcd, irrPolyCo, aInverse);

            if (!(aBFInverse == aInverse)){
                cout << "bf Inverse  = " << aBFInverse << endl;
                cout << "gcd Inverse = " << aInverse << endl;

            assert( 0 && "testGCDvsBFInverse test failed");
            }
        }
    }

    int memLeaks = Field<T>::aMals - (int)((map<int, Field<T>*>*)Field<T>::irrPolys)->size();
    assert(memLeaks == 0);

    return true;
}

template<class T>
bool Tests<T>::testIsReducable()
{
    Field<T>& knownIrrPoly = Field<T>::getIrrPoly(9);
    //Field<T> knownIrrPoly(9);
    //knownIrrPoly(0) = 7;

    assert(!knownIrrPoly.isReducable());
    return !knownIrrPoly.isReducable();
}

template<class T>
bool Tests<T>::testFindIrrPoly()
{
    Field<T>& knownIrrPoly = Field<T>::getIrrPoly(9);
    Field<T> irrPoly(9);

    Field<T>::findIrrPoly(irrPoly);

    assert(knownIrrPoly == irrPoly);
    return (knownIrrPoly == irrPoly);
}
