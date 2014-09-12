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

    FiniteField<T> num1(16);
    FiniteField<T> num2(16);
    FiniteField<T> prod(16);

    num1(0) = 7;
    num1(1) = 4;
    num2(0) = 1;

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
    FiniteField<T> num1(16);
    FiniteField<T> num2(16);
    FiniteField<T> prod(16);

    num1(0) = 41;
    num1(1) = 35;

    num2(0) = 4;
    FiniteField<T>::multiply(num1, num2, prod);
    cout << prod << " = " << num1 << " * " << num2 << endl;

    return true;
}

template<class T>
bool Tests<T>::testWikiDivision()
{


    FiniteField<T> num1(9);
    FiniteField<T> num2(9);

    num1(0) = 83; // 01010011
    num2(0) = 202; // 11001010

    //FiniteField<T> quo(9);
    //FiniteField<T> rem(9);
    FiniteField<T> prod(9);
    FiniteField<T>::show = true;
    FiniteField<T>::multiply(num1, num2, prod);

    cout << prod << endl;

    return true;
}

template<class T>
bool Tests<T>::testWikiInverse()
{
    FiniteField<T> base(9);
    FiniteField<T> inv(9);
    FiniteField<T> knownInv(9);

    base(0) = 83;
    knownInv(0) = 202;

    FiniteField<T>::invert(base, inv);

    assert(inv == knownInv);
    return inv == knownInv;
}

template<class T>
bool Tests<T>::testWikiBruteForceInverse()
{
    FiniteField<T> base(9);
    FiniteField<T> inv(9);
    FiniteField<T> knownInv(9);

    base(0) = 83;
    knownInv(0) = 202;

    FiniteField<T>::bruteForceInvert(base, inv);

    assert(inv == knownInv);
    return inv == knownInv;
}

template<class T>
bool Tests<T>::testGCD()
{
    FiniteField<T> a(9);
    FiniteField<T> b(9);
    FiniteField<T> gcd(9);
    FiniteField<T> aCo(9);
    FiniteField<T> bCo(9);

    FiniteField<T> aProd(9);
    FiniteField<T> bProd(9);
    FiniteField<T> sum(9);

    a(0) = 83;
    b(0) = 202;

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

        assert(aBFInverse == aInverse);

    }
    return true;
}