#include "Tests.h"

#include "FiniteFields.cpp"

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

        FiniteField<T> num1(16);
        FiniteField<T> num2(16);
        FiniteField<T> prod(16);

        num1(0) = 14;
        num2(0) = 9;

        FiniteField<T>::multiply(num1, num2, prod);

        FiniteField<T> quo(16);
        FiniteField<T> rem(16);


        FiniteField<T>::division(prod, num2, quo, rem);

        if (!(rem.isZero()) || !(quo == num1))
        {
            FiniteField<T>::show = true;
            FiniteField<T>::multiply(num1, num2, prod);
            FiniteField<T>::division(prod, num2, quo, rem);
            FiniteField<T>::show = false;

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

    FiniteField<T> num1(16, Base2, "0000011000000111");
    FiniteField<T> num2(16, base2, "0000000000000001");
    FiniteField<T> prod(16);



    FiniteField<T> prodTest(num1);
    

    while (!*prod.getMSB())
    {
        FiniteField<T>::multiply(num1, num2, prod);

        result = prod == prodTest;

        if (!result)
        {
            FiniteField<T>::show = true;
            FiniteField<T>::multiply(num1, num2, prod);
            FiniteField<T>::show = false;

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
    FiniteField<T> num1(16, Base2, "0010001100101001");
    cout << "num1 " << num1 << endl;
    FiniteField<T> num2(16, Base2, "0000000000000100");
    cout << "num2 " << num2 << endl;

    FiniteField<T> prod(16);

    FiniteField<T>::multiply(num1, num2, prod);
    cout << prod << " = " << num1 << " * " << num2 << endl;

    return true;
}

template<class T>
bool Tests<T>::testMultiply2()
{

    FiniteField<T> num1(9, Base2,      "01010011"); // 83
    FiniteField<T> num2(9, Base2,      "11001010"); // 202
    FiniteField<T> knownProd(9, Base2, "00000001"); // 1
    FiniteField<T> prod(9);

    //FiniteField<T> quo(9);
    //FiniteField<T> rem(9);
    //FiniteField<T>::show = true;
    FiniteField<T>::multiply(num1, num2, prod);

    cout << "prod " << prod << endl;
    cout << "Kprod " << knownProd << endl;

    assert(knownProd == prod);
    return true;
}

template<class T>
bool Tests<T>::testMultiply3()
{

    FiniteField<T> num1(10, Base2, "01010011"); // 
    FiniteField<T> num2(10);//, Base2, "11001010"); // 
    FiniteField<T> knownProd(10, Base2, "00000001"); // 
    FiniteField<T> prod(10);

    FiniteField<T>::bruteForceInvert(num1,num2);

    FiniteField<T>::multiply(num1, num2, prod);

    cout << "prod " << prod << endl;
    cout << "Kprod " << knownProd << endl;

    assert(knownProd == prod);
    return true;
}

template<class T>
bool Tests<T>::testWikiDivision()
{


    FiniteField<T> num1(9,Base2, "01010011"); // 83
    FiniteField<T> num2(9,Base2, "11001010"); // 202

    //FiniteField<T> quo(9);
    //FiniteField<T> rem(9);
    FiniteField<T> prod(9);
    //FiniteField<T>::show = true;
    FiniteField<T>::division(num1, num2, prod);

    cout << prod << endl;

    return true;
}

template<class T>
bool Tests<T>::testWikiInverse()
{
    FiniteField<T> base(9, Base2, "01010011"); // 83
    FiniteField<T> knownInv(9, Base2, "11001010"); // 202
    FiniteField<T> inv(9);

    FiniteField<T>::invert(base, inv);

    assert(inv == knownInv);
    return inv == knownInv;
}

template<class T>
bool Tests<T>::testWikiBruteForceInverse()
{
    FiniteField<T> base(9, Base2, "01010011"); // 83
    FiniteField<T> knownInv(9, Base2, "11001010"); // 202
    FiniteField<T> inv(9);

    FiniteField<T>::bruteForceInvert(base, inv);

    assert(inv == knownInv);
    return inv == knownInv;
}

template<class T>
bool Tests<T>::testGCD()
{
    FiniteField<T> a(10, Base2, "01010011"); // 83
    FiniteField<T> b(10, Base2, "11001010"); // 202
    FiniteField<T> gcd(10);
    FiniteField<T> aCo(10);
    FiniteField<T> bCo(10);

    FiniteField<T> aProd(10);
    FiniteField<T> bProd(10);
    FiniteField<T> sum(10);


    FiniteField<T>::extGCD(a, b, gcd, aCo, bCo);

    cout << "a " << a << endl;
    cout << "b " << b << endl;
    cout << "gcd " << gcd << endl;
    cout << "aCo " << aCo << endl;
    cout << "bCo " << bCo << endl;

    FiniteField<T>::multiply(a, aCo, aProd);
    FiniteField<T>::multiply(b, bCo, bProd);

    FiniteField<T>::add(aProd, bProd, sum);

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
        FiniteField<T> a(9);
        FiniteField<T> aInverse(9);
        FiniteField<T> aBFInverse(9);
        FiniteField<T> gcd(9);

        FiniteField<T>& irrPoly = FiniteField<T>::getIrrPoly(9);
        FiniteField<T> irrPolyCo(9);

        for (int i = 0; i < 10000; i++)
        {
            a.randomize();
            if (a.isZero()) continue;

            FiniteField<T>::bruteForceInvert(a,aBFInverse);

            FiniteField<T>::extGCD(irrPoly, a, gcd, irrPolyCo, aInverse);

            if (!(aBFInverse == aInverse)){
                cout << "bf Inverse  = " << aBFInverse << endl;
                cout << "gcd Inverse = " << aInverse << endl;

            assert( 0 && "testGCDvsBFInverse test failed");
            }
        }
    }

    int memLeaks = FiniteField<T>::aMals - (int)((map<int, FiniteField<T>*>*)FiniteField<T>::irrPolys)->size();
    assert(memLeaks == 0);

    return true;
}

template<class T>
bool Tests<T>::testIsReducable()
{
    FiniteField<T>& knownIrrPoly = FiniteField<T>::getIrrPoly(9);
    //FiniteField<T> knownIrrPoly(9);
    //knownIrrPoly(0) = 7;

    assert(!knownIrrPoly.isReducable());
    return !knownIrrPoly.isReducable();
}

template<class T>
bool Tests<T>::testFindIrrPoly()
{
    FiniteField<T>& knownIrrPoly = FiniteField<T>::getIrrPoly(9);
    FiniteField<T> irrPoly(9);

    FiniteField<T>::findIrrPoly(irrPoly);

    assert(knownIrrPoly == irrPoly);
    return (knownIrrPoly == irrPoly);
}
