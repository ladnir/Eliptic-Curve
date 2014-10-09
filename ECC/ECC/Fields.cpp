//#include "Fields.h"
#include "BitIterator.cpp"
#include <string.h>     /* strcat */

#include <iostream>
#include <map>

template<class T>
Field<T>::Field(int bitCount){
    construct(bitCount);
}

template<class T>
Field<T>::Field(int bitCount, InputType it,string num){
    construct(bitCount);

    Field<T>::bitIterator bitIter( getLSB());

    if(it == Base2){
        assert(num.size() <= bitCount);
        for (int i = (int)num.size() - 1; i > -1; i--){
            if (num[i] == '1')
            {
                bitIter.flipBit();
            }
            ++bitIter;
        }
    }
    else{
        assert(0);
    }
}

template<class T>
 Field<T>::Field(const  Field<T>& copy){
    construct(copy.mBitCount);

    for (int i = 0; i < mWordCount; i++)
    {
        num[i] = copy(i);
    }
}

template<class T>
 Field<T>::~Field(){
    if (num != nullptr){
        //cout << "free " << (void*)num << endl;
        free( num);
       aMals --;
    }
}

template<class T>
void Field<T>::construct(int bitCount)
{
    mWordSize = (sizeof(T)* 8);
    mWordCount = bitCount / mWordSize;
    if (mWordCount * mWordSize < bitCount) mWordCount++;

    mBitCount = bitCount;

    this->num = (T*)calloc(mWordCount, sizeof(T));
    aMals++;
}

template<class T>
void  Field<T>::add(const  Field<T>& augend, 
                         const  Field<T>& addend, 
                                Field<T>& sum){

    assert(augend.mWordCount == addend.mWordCount);
    assert(augend.mWordCount == sum.mWordCount);

    for (int i = 0; i < augend.mWordCount; i++){
        sum(i) = augend(i) ^ addend(i);
    }

    assert(sum.checkHighBits());
}

template<class T>
void Field<T>::multiply(const  Field<T>& multiplicand, 
                              const  Field<T>& multiplier, 
                                     Field<T>& product){

     Field<T> tempProd(multiplicand.mBitCount * 2);
     Field<T> doubleP1(multiplicand.mBitCount * 2);
     Field<T> multiplierCopy(multiplier.mBitCount);

    for (int i = 0; i < multiplicand.mWordCount; i++){
        doubleP1(i) = multiplicand(i);
        multiplierCopy(i) = multiplier(i);
    }

    for (int i = 0; i < multiplicand.mBitCount; i++){
        if (multiplierCopy(0) & 1){
            add(tempProd, doubleP1, tempProd);

        }
        doubleP1 <<= 1;
        multiplierCopy >>= 1;
    }
    Field<T> quotient(product.mBitCount);
    division(tempProd, getIrrPoly(multiplicand.mBitCount), quotient, product);
}

template<class T>
void Field<T>::square(const Field<T>& base,
                                  Field<T>& product)
{
    // TODO optimize this.
    Field<T>::multiply(base, base, product);
}

template<class T>
void  Field<T>::modulus( Field<T>& m, const  Field<T>& divisor){
    
     Field<T> quotient(divisor.mBitCount);
     Field<T> remainder(divisor.mBitCount);

    division(m, divisor, quotient, remainder);
    m.mBitCount = divisor.mBitCount;
    m.mWordCount = divisor.mWordCount;
    m.mWordSize = divisor.mWordSize;
    m.steal(remainder);
}

template<class T>
void Field<T>::division(const Field<T>& dividend,
                              const Field<T>& divisor, 
                                    Field<T>& quotient , 
                                    Field<T>& remainder)
{
    assert(!divisor.isZero() && "Divide by zero error.");
    quotient.clear();
    if (dividend.isZero()){
        remainder.clear();
        return;
    }

    int dividendLeadingZeros = 0;
    int divisorLeadingZeros = 0;
    int shifts = 0;

    Field<T> accumulator(dividend);
    Field<T> divisorShift(dividend.mBitCount);

    Field<T>::bitIterator quotientBI = quotient.getLSB();
    Field<T>::bitIterator accumulatorBI = accumulator.getMSB();
    Field<T>::bitIterator divisorBI = divisor.getMSB();

    for (int i = 0; i < divisor.mWordCount; i++)
        divisorShift(i) = divisor(i);

    ////////////////////////////////////////////////////////
    //if (Field<T>::show)
    //{
    //	cout << "division" << endl;
    //	cout << "  " << dividend << endl;
    //	cout << "/ " << divisorShift << endl;
    //}
    ////////////////////////////////////////////////////////

    while (*accumulatorBI == 0){
        dividendLeadingZeros++;
        --accumulatorBI;
    }

    while (*divisorBI == 0){
        divisorLeadingZeros++;
        --divisorBI;
    }

    shifts = dividend.mBitCount - dividendLeadingZeros - divisor.mBitCount + divisorLeadingZeros;

    quotientBI.goToBit(shifts);
    divisorShift <<= shifts;

    while (shifts >= 0){
        if (*accumulatorBI != 0)
        {
            quotientBI.flipBit();
            add(accumulator, divisorShift, accumulator);
        }
        --accumulatorBI;
        --quotientBI;
        divisorShift >>= 1;
        shifts--;
    }

    for (int i = 0; i < divisor.mWordCount; i++)
        remainder(i) = accumulator(i);

    assert(remainder.checkHighBits());
}

template<class T>
void Field<T>::invert(const Field<T>& base, Field<T>& inverse)
{
    Field<T> gcd(base.mBitCount);
    Field<T> aCo(base.mBitCount);

    extGCD(getIrrPoly(base.mBitCount), base, gcd, aCo, inverse);
}

template<class T>
void Field<T>::bruteForceInvert(const Field<T>& base, Field<T>& inverse)
{
    assert(!base.isZero() && "No inverse for element 0. bruteForceInvert()");
    inverse.clear();

    Field<T> prod(base.mBitCount);
    Field<T> knownProd(base.mBitCount);
    knownProd(0) = 1;

    while (!( prod == knownProd))
    {
        inverse++;
        if (inverse.isZero()){
            cout << "No Inverse for " << base << endl;
            cout << "mod " << getIrrPoly(base.mBitCount) << endl;
            assert(0 && "No Inverse, BruteForceInvert()" );
            return;
        }

        Field<T>::multiply(base, inverse, prod);
    }
}

template<class T>
void Field<T>::extGCD(const Field<T>& a, 
                            const Field<T>& b, 
                                  Field<T>& gcd,
                                  Field<T>& aCoefficient,
                                  Field<T>& bCoefficient)
{
    assert(!a.isZero() && "No inverse for a = element 0, extGCD()");
    assert(!b.isZero() && "No inverse for b = element 0, extGCD()");

    vector<Field<T>> remainders;
    vector<Field<T>> scalers;

    Field<T> aCopy(a);
    Field<T> bCopy(b);

    remainders.push_back(aCopy);
    remainders.push_back(bCopy);

    // Compute the Gcd
    int step = 1;
    while( ! remainders.back().isZero())
    {   
        // leftHandSide = multiple * scaler + remainder
        remainders.emplace_back(a.mBitCount);
        scalers.emplace_back(a.mBitCount);
        step++;

        division(remainders[step - 2], remainders[step - 1], scalers[step - 2], remainders[step]);
        if (Field<T>::show)
                cout << (int)remainders[step - 2](0) << " = " << (int)remainders[step - 1](0) << " * " << (int)scalers[step - 2](0) << " + " << (int)remainders[step](0) << endl;
    }

    // get rid of the remainder == 0 step.

    vector<Field<T>>::iterator scalIter = scalers.end();
    vector<Field<T>>::iterator remIter = remainders.end();
    --scalIter;
    --remIter;
    gcd.copy(*(--remIter));


    if (scalers.size() == 1){
        
        if (a < b){
            aCoefficient.clear();
            aCoefficient(0) = 1;
            bCoefficient.clear();
        }
        else{
            bCoefficient.clear();
            bCoefficient(0) = 1;
            aCoefficient.clear();
        }
        return;
        //extGCD(a,b,gcd,aCoefficient,bCoefficient);
    }

    // Compute the linear combination. (extended part)
    // i.e.   h,k  s.t.  h*a + k*b = gcd  
#pragma region Extended
#ifdef _DEBUG
    Field<T> leftProd(a.mBitCount);
    Field<T> rightProd(a.mBitCount);
    Field<T> temp(a.mBitCount);
#endif
    Field<T> rightScale(a.mBitCount);
    Field<T> leftScale(a.mBitCount);

    rightScale.steal(*(--scalIter));
    leftScale(0) = 1;

    vector<Field<T>>::iterator rightSub = (--remIter);
    vector<Field<T>>::iterator leftSub = (--remIter);

#ifdef _DEBUG
    // test the eq.  h*a + k*b = gcd  
    Field<T>::multiply(*leftSub, leftScale, leftProd);
    Field<T>::multiply(*rightSub, rightScale, rightProd);
    Field<T>::add(leftProd, rightProd, temp);
    //if (Field<T>::show){
    //cout << endl;
    //cout << (unsigned int)gcd(0) << " = " << (unsigned int)leftScale(0) << " * " << (unsigned int)(*leftSub)(0) << " + " << (unsigned int)rightScale(0) << " * " << (unsigned int)(*rightSub)(0) << endl;
    ////}

    //cout << endl;
    //cout << (unsigned int)gcd(0) << " == " << (unsigned int)temp(0) << endl;
    assert(gcd == temp);
#endif

    while (remIter > remainders.begin())
    {
        // substite right.
        rightSub = --remIter;

        // group the terms with the left side.
        // leftScale = leftScale + nextScale * rightScale
        //cout << "S " << *(scalIter-1) << endl;
        Field<T>::multiply(*(--scalIter), rightScale, temp);
        Field<T>::add(leftScale, temp, leftScale);

#ifdef _DEBUG
        // test the eq.  h*a + k*b = gcd  
        Field<T>::multiply(*leftSub, leftScale, leftProd);
        Field<T>::multiply(*rightSub, rightScale, rightProd);
        Field<T>::add(leftProd, rightProd, temp);

        //if (Field<T>::show){
        //cout << endl;
        //cout << (unsigned int)gcd(0) << " = " << (unsigned int)leftScale(0) << " * " << (unsigned int)(*leftSub)(0) << " + " << (unsigned int)rightScale(0) << " * " << (unsigned int)(*rightSub)(0) << endl;
        ////}
        assert(gcd == temp);
#endif

        if (remIter == remainders.begin()) break; // break if we are done.

        // substite left
        leftSub = --remIter;
        int i =0;
        // group the terms with the right side.
        // rightScale = rightScale + nextScale *leftScale
        //cout << "S " << *(scalIter - 1) << endl;
        Field<T>::multiply(*(--scalIter), leftScale, temp);
        Field<T>::add(rightScale, temp, rightScale);

#ifdef _DEBUG
        // test the eq.  h*a + k*b = gcd  
        Field<T>::multiply(*leftSub, leftScale, leftProd);
        Field<T>::multiply(*rightSub, rightScale, rightProd);
        Field<T>::add(leftProd, rightProd, temp);
        //cout << endl;
        //cout << (unsigned int)gcd(0) << " = " << (unsigned int)leftScale(0) << " * " << (unsigned int)(*leftSub)(0) << " + " << (unsigned int)rightScale(0) << " * " << (unsigned int)(*rightSub)(0) << endl;
        assert(gcd == temp);
#endif
    }

    if (a == *leftSub){
        aCoefficient.steal(leftScale);
        bCoefficient.steal(rightScale);
    }
    else{
        aCoefficient.steal(rightScale);
        bCoefficient.steal(leftScale);
    }
#pragma endregion
}

template<class T>
void Field<T>::findIrrPoly(Field<T>& irrPoly)
{
    irrPoly.clear();
    irrPoly.getMSB().flipBit();

    while (! irrPoly.isZero()){
        cout << irrPoly << endl;
        if (! irrPoly.isReducable()) return;
        irrPoly++;
    }
    irrPoly.clear();
}

template<class T>
void Field<T>::operator<<=(const int& shifts){

    assert(shifts < mBitCount);
    int s = shifts;
    int step = (s > mWordSize) ? mWordSize : s;

    while (s){
        T shiftIn;
        for (int i = mWordCount - 1; i > 0; i--){
            num[i] <<= step;
            shiftIn = num[i - 1] >> (sizeof(T)* 8 - step);
            num[i] ^= shiftIn;
        }
        num[0] <<= step;
        s -= step;
        step = (s > mWordSize) ? mWordSize : s;
    }
    // 00001111
    T mask = ((T)-1) >> ((mWordCount)* mWordSize - mBitCount);
    num[mWordCount - 1] &= mask;
    assert(checkHighBits());
}

template<class T>
void Field<T>::operator>>=(const int& shifts){

    assert(shifts < sizeof(T)* 8);

    T shiftIn;
    for (int i = 0; i < mWordCount - 1; i++){
        num[i] >>= shifts;
        shiftIn = num[i + 1] << (sizeof(T)* 8 - shifts);
        num[i] ^= shiftIn;
    }
    num[mWordCount - 1] >>= shifts;
}

template<class T>
T& Field<T>::operator()(const int & i) const
{
    assert(i <= mWordCount);
    return num[i];
}

template<class T>
void Field<T>::operator++(int)
{
    for (int i = 0; i <= mWordCount - 1; i++){
        num[i]++;
        if (num[i] != 0) break;
    }

    // 00001111
    T mask = ((T)-1) >> ((mWordCount)* mWordSize - mBitCount);
    num[mWordCount - 1] &= mask;
    assert(checkHighBits());
}

template<class T>
bool Field<T>::operator==(const Field<T>& cmp) const
{
    assert(mBitCount == cmp.mBitCount);

    for (int i = 0; i < mWordCount; i++){
        if (num[i] != cmp(i)) return false;
    }

    assert(checkHighBits());
    return true;
}

template<class T>
bool Field<T>::operator<(const Field<T>& cmp)const
{
    for (int i = 0; i < mWordCount; i++)
    {
        if (num[i] < cmp(i)) return true;
    }
    return false;
}

template <class T>
bool Field<T>::checkHighBits() const
{
#ifdef _DEBUG

    T mask = -1; // 11111111

    // 11100000
    int ones = (mWordCount)* mWordSize - mBitCount;
    int zeros = sizeof(T)* 8 - ones;

    mask <<= zeros;

    return(!(mask & num[mWordCount - 1]));
#else 
    return true;
#endif
}

template<class T>
 Field<T>&  Field<T>::getIrrPoly(int bitCount)
{

    if (irrPolys == nullptr){
        irrPolys = (void*) new map<int, Field<T>*>();
    }

    map<int, Field<T>*>& irrPolyMap = *reinterpret_cast<map<int, Field<T>*>*>(irrPolys);

    map<int, Field<T>*>::iterator ipi = irrPolyMap.find(bitCount);

    Field<T>* ip;
    if (ipi == irrPolyMap.end()){
              
        if (bitCount == 9){
            ip = new Field<T>(bitCount, Base2, "100011011");
        }
        else if (bitCount == 10){
            
            ip = new Field<T>(bitCount);
            Field<T>::findIrrPoly(*ip);
        }
        else{
            assert(0 && "undefined irrPoly");
        }
        irrPolyMap.insert(pair<int, Field<T>*>(bitCount, ip));

        cout << " new " << bitCount << " bit irrPoly " << endl;
    }
    else{
        ip = ipi->second;
    }

    return *ip;
}

template<class T>
bool Field<T>::isReducable() const
{
    Field<T> scanner(this->mBitCount);
    Field<T> quo(this->mBitCount);
    Field<T> rem(this->mBitCount);

    scanner(0) = 2;

    while (!(scanner == *this)){



        Field<T>::division(*this,scanner,quo,rem);
        if (rem.isZero()) {
            return true;
        }
        scanner++;
    }
    return false;
}


template<class T>
void  Field<T>::clear()
{
    for (int i = 0; i < mWordCount; i++)
        num[i] = 0;
}

template<class T>
void  Field<T>::copy(const  Field<T>& source)
{
    assert(mBitCount == source.mBitCount);
    for (int i = 0; i < source.mWordCount; i++)
        num[i] = source(i);

    assert(checkHighBits());
}

template<class T>
void Field<T>::steal(Field<T>& source)
{
    assert(mBitCount == source.mBitCount);
    if (num != nullptr) free( num); 
    aMals --;
    num =  source.num;
    source.num = nullptr;
}

template<class T>
void  Field<T>::print() const
{
    for (int i = mWordCount - 1; i >= 0 ; i--)
        printf("%016llx ", num[i] );
    printf("`\n");
}

template<class T>
bool  Field<T>::isZero()const
{
    bool result =  true;
    int i = 0;
    for (; i < mWordCount; i++)
    {
        if (num[i] != 0){
            result =  false;
            break;
        }
    }
    assert(checkHighBits());

    return result;
}

template<class T>
void  Field<T>::randomize(){
 #if 1    
    for (int i = 0; i < mWordCount; i++)
        num[i] = (T)rand();
    
    // 00001111
    T mask = ((T)-1) >> ( (mWordCount)* mWordSize - mBitCount + 1);
    num[mWordCount - 1] &= mask;
    assert(checkHighBits());
#else
    static std::default_random_engine generator;
    static std::uniform_int_distribution<T> distribution(0, (T)-1);
    for (int i = 0; i < mWordCount; i++)
        num[i] = distribution(generator);

    // 00001111
    T mask = (T)-1 >> (mWordCount)* mWordSize - mBitCount;
    num[mWordCount - 1] &= mask;
    assert(checkHighBits());
#endif
}

template<class T>
typename  Field<T>::bitIterator  Field<T>::getLSB() const
{
    return  Field<T>::bitIterator(*this);
}

template<class T>
typename  Field<T>::bitIterator  Field<T>::getMSB() const
{
     Field<T>::bitIterator MSB(*this);
    MSB.goToMSB();
    return MSB;
}

template<class T>
typename  Field<T>::bitIterator  Field<T>::getBit(int bit) const
{
     Field<T>::bitIterator bitIter(*this);
    bitIter.goToBit(bit);
    return bitIter;
}

template<class T>
ostream& operator << (ostream& stream,const  Field<T>& field)
{
     Field<T>::bitIterator bits = field.getMSB();

    while (bits >= field.getLSB()){
        stream << *bits;
        --bits;
    }
    stream << "'";

    return stream;
}