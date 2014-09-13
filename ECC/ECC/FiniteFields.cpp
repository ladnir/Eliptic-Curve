//#include "FiniteFields.h"
#include "BitIterator.cpp"
#include <string.h>     /* strcat */

#include <iostream>
#include <map>

template<class T>
FiniteField<T>::FiniteField(int bitCount){
    construct(bitCount);
}

template<class T>
FiniteField<T>::FiniteField(int bitCount, InputType it,string num){
    construct(bitCount);

    FiniteField<T>::bitIterator bitIter( getLSB());

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
 FiniteField<T>::FiniteField(const  FiniteField<T>& copy){
    construct(copy.mBitCount);

    for (int i = 0; i < mWordCount; i++)
    {
        num[i] = copy(i);
    }
}

template<class T>
 FiniteField<T>::~FiniteField(){
    if (num != nullptr){
        //cout << "free " << (void*)num << endl;
        free( num);
       aMals --;
    }
}

template<class T>
void FiniteField<T>::construct(int bitCount)
{
    mWordSize = (sizeof(T)* 8);
    mWordCount = bitCount / mWordSize;
    if (mWordCount * mWordSize < bitCount) mWordCount++;

    mBitCount = bitCount;

    this->num = (T*)calloc(mWordCount, sizeof(T));
    aMals++;
}

template<class T>
void  FiniteField<T>::add(const  FiniteField<T>& augend, 
                         const  FiniteField<T>& addend, 
                                FiniteField<T>& sum){

    assert(augend.mWordCount == addend.mWordCount);
    assert(augend.mWordCount == sum.mWordCount);

    for (int i = 0; i < augend.mWordCount; i++){
        sum(i) = augend(i) ^ addend(i);
    }

    assert(sum.checkHighBits());
}

template<class T>
void FiniteField<T>::multiply(const  FiniteField<T>& multiplicand, 
                              const  FiniteField<T>& multiplier, 
                                     FiniteField<T>& product){

     FiniteField<T> tempProd(multiplicand.mBitCount * 2);
     FiniteField<T> doubleP1(multiplicand.mBitCount * 2);
     FiniteField<T> multiplierCopy(multiplier.mBitCount);

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
    FiniteField<T> quotient(product.mBitCount);
    division(tempProd, getIrrPoly(multiplicand.mBitCount), quotient, product);
}

template<class T>
void  FiniteField<T>::modulus( FiniteField<T>& m, const  FiniteField<T>& divisor){
    
     FiniteField<T> quotient(divisor.mBitCount);
     FiniteField<T> remainder(divisor.mBitCount);

    division(m, divisor, quotient, remainder);
    m.mBitCount = divisor.mBitCount;
    m.mWordCount = divisor.mWordCount;
    m.mWordSize = divisor.mWordSize;
    m.steal(remainder);
}

template<class T>
void FiniteField<T>::division(const FiniteField<T>& dividend,
                              const FiniteField<T>& divisor, 
                                    FiniteField<T>& quotient , 
                                    FiniteField<T>& remainder)
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

    FiniteField<T> accumulator(dividend);
    FiniteField<T> divisorShift(dividend.mBitCount);

    FiniteField<T>::bitIterator quotientBI = quotient.getLSB();
    FiniteField<T>::bitIterator accumulatorBI = accumulator.getMSB();
    FiniteField<T>::bitIterator divisorBI = divisor.getMSB();

    for (int i = 0; i < divisor.mWordCount; i++)
        divisorShift(i) = divisor(i);

    ////////////////////////////////////////////////////////
    //if (FiniteField<T>::show)
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
            //if (FiniteField<T>::show)
            //{
            //	cout << "a " << accumulator << " , " << quotientBI << endl;
            //	cout << "^ " << divisorShift << endl;
            //}
            quotientBI.flipBit();
            add(accumulator, divisorShift, accumulator);
        }
   //     else{
            //if (FiniteField<T>::show)
            //{
            //	cout << "- " << accumulator << " , " << quotientBI << endl;
            //}
   //     }
        --accumulatorBI;
        --quotientBI;
        divisorShift >>= 1;
        shifts--;
    }
    //if (FiniteField<T>::show)
    //{
    //	cout << "r " << accumulator << " ,q" << quotient << endl;
    //}

    for (int i = 0; i < divisor.mWordCount; i++)
        remainder(i) = accumulator(i);

    assert(remainder.checkHighBits());
}

template<class T>
void FiniteField<T>::invert(const FiniteField<T>& base, FiniteField<T>& inverse)
{
    FiniteField<T> gcd(base.mBitCount);
    FiniteField<T> aCo(base.mBitCount);

    extGCD(getIrrPoly(base.mBitCount), base, gcd, aCo, inverse);
}

template<class T>
void FiniteField<T>::bruteForceInvert(const FiniteField<T>& base, FiniteField<T>& inverse)
{
    assert(!base.isZero() && "No inverse for element 0. bruteForceInvert()");
    inverse.clear();

    FiniteField<T> prod(base.mBitCount);
    FiniteField<T> knownProd(base.mBitCount);
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

        FiniteField<T>::multiply(base, inverse, prod);
    }
}

template<class T>
void FiniteField<T>::extGCD(const FiniteField<T>& a, 
                            const FiniteField<T>& b, 
                                  FiniteField<T>& gcd,
                                  FiniteField<T>& aCoefficient,
                                  FiniteField<T>& bCoefficient)
{
    assert(!a.isZero() && "No inverse for a = element 0, extGCD()");
    assert(!b.isZero() && "No inverse for b = element 0, extGCD()");

    vector<FiniteField<T>> remainders;
    vector<FiniteField<T>> scalers;

    FiniteField<T> aCopy(a);
    FiniteField<T> bCopy(b);

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
        if (FiniteField<T>::show)
                cout << (int)remainders[step - 2](0) << " = " << (int)remainders[step - 1](0) << " * " << (int)scalers[step - 2](0) << " + " << (int)remainders[step](0) << endl;
    }

    // get rid of the remainder == 0 step.

    vector<FiniteField<T>>::iterator scalIter = scalers.end();
    vector<FiniteField<T>>::iterator remIter = remainders.end();
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
    FiniteField<T> leftProd(a.mBitCount);
    FiniteField<T> rightProd(a.mBitCount);
    FiniteField<T> temp(a.mBitCount);
#endif
    FiniteField<T> rightScale(a.mBitCount);
    FiniteField<T> leftScale(a.mBitCount);

    rightScale.steal(*(--scalIter));
    leftScale(0) = 1;

    vector<FiniteField<T>>::iterator rightSub = (--remIter);
    vector<FiniteField<T>>::iterator leftSub = (--remIter);

#ifdef _DEBUG
    // test the eq.  h*a + k*b = gcd  
    FiniteField<T>::multiply(*leftSub, leftScale, leftProd);
    FiniteField<T>::multiply(*rightSub, rightScale, rightProd);
    FiniteField<T>::add(leftProd, rightProd, temp);
    //if (FiniteField<T>::show){
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
        FiniteField<T>::multiply(*(--scalIter), rightScale, temp);
        FiniteField<T>::add(leftScale, temp, leftScale);

#ifdef _DEBUG
        // test the eq.  h*a + k*b = gcd  
        FiniteField<T>::multiply(*leftSub, leftScale, leftProd);
        FiniteField<T>::multiply(*rightSub, rightScale, rightProd);
        FiniteField<T>::add(leftProd, rightProd, temp);

        //if (FiniteField<T>::show){
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
        FiniteField<T>::multiply(*(--scalIter), leftScale, temp);
        FiniteField<T>::add(rightScale, temp, rightScale);

#ifdef _DEBUG
        // test the eq.  h*a + k*b = gcd  
        FiniteField<T>::multiply(*leftSub, leftScale, leftProd);
        FiniteField<T>::multiply(*rightSub, rightScale, rightProd);
        FiniteField<T>::add(leftProd, rightProd, temp);
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
void FiniteField<T>::findIrrPoly(FiniteField<T>& irrPoly)
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
void FiniteField<T>::operator<<=(const int& shifts){

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
void FiniteField<T>::operator>>=(const int& shifts){

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
T& FiniteField<T>::operator()(const int & i) const
{
    assert(i <= mWordCount);
    return num[i];
}

template<class T>
void FiniteField<T>::operator++(int)
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
bool FiniteField<T>::operator==(const FiniteField<T>& cmp) const
{
    assert(mBitCount == cmp.mBitCount);

    for (int i = 0; i < mWordCount; i++){
        if (num[i] != cmp(i)) return false;
    }

    assert(checkHighBits());
    return true;
}

template<class T>
bool FiniteField<T>::operator<(const FiniteField<T>& cmp)const
{
    for (int i = 0; i < mWordCount; i++)
    {
        if (num[i] < cmp(i)) return true;
    }
    return false;
}

template <class T>
bool FiniteField<T>::checkHighBits() const
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
 FiniteField<T>&  FiniteField<T>::getIrrPoly(int bitCount)
{

    if (irrPolys == nullptr){
        irrPolys = (void*) new map<int, FiniteField<T>*>();
    }

    map<int, FiniteField<T>*>& irrPolyMap = *reinterpret_cast<map<int, FiniteField<T>*>*>(irrPolys);

    map<int, FiniteField<T>*>::iterator ipi = irrPolyMap.find(bitCount);

    FiniteField<T>* ip;
    if (ipi == irrPolyMap.end()){
              
        if (bitCount == 9){
            ip = new FiniteField<T>(bitCount, Base2, "100011011");
        }
        else if (bitCount == 10){
            
            ip = new FiniteField<T>(bitCount);
            FiniteField<T>::findIrrPoly(*ip);
        }
        else{
            assert(0 && "undefined irrPoly");
        }
        irrPolyMap.insert(pair<int, FiniteField<T>*>(bitCount, ip));

        cout << " new " << bitCount << " bit irrPoly " << endl;
    }
    else{
        ip = ipi->second;
    }

    return *ip;
}

template<class T>
bool FiniteField<T>::isReducable() const
{
    FiniteField<T> scanner(this->mBitCount);
    FiniteField<T> quo(this->mBitCount);
    FiniteField<T> rem(this->mBitCount);

    scanner(0) = 2;

    while (!(scanner == *this)){



        FiniteField<T>::division(*this,scanner,quo,rem);
        if (rem.isZero()) {
            return true;
        }
        scanner++;
    }
    return false;
}


template<class T>
void  FiniteField<T>::clear()
{
    for (int i = 0; i < mWordCount; i++)
        num[i] = 0;
}

template<class T>
void  FiniteField<T>::copy(const  FiniteField<T>& source)
{
    assert(mBitCount == source.mBitCount);
    for (int i = 0; i < source.mWordCount; i++)
        num[i] = source(i);

    assert(checkHighBits());
}

template<class T>
void FiniteField<T>::steal(FiniteField<T>& source)
{
    assert(mBitCount == source.mBitCount);
    if (num != nullptr) free( num); 
    aMals --;
    num =  source.num;
    source.num = nullptr;
}

template<class T>
void  FiniteField<T>::print() const
{
    for (int i = mWordCount - 1; i >= 0 ; i--)
        printf("%016llx ", num[i] );
    printf("`\n");
}

template<class T>
bool  FiniteField<T>::isZero()const
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
void  FiniteField<T>::randomize(){
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
typename  FiniteField<T>::bitIterator  FiniteField<T>::getLSB() const
{
    return  FiniteField<T>::bitIterator(*this);
}

template<class T>
typename  FiniteField<T>::bitIterator  FiniteField<T>::getMSB() const
{
     FiniteField<T>::bitIterator MSB(*this);
    MSB.goToMSB();
    return MSB;
}

template<class T>
typename  FiniteField<T>::bitIterator  FiniteField<T>::getBit(int bit) const
{
     FiniteField<T>::bitIterator bitIter(*this);
    bitIter.goToBit(bit);
    return bitIter;
}

template<class T>
ostream& operator << (ostream& stream,const  FiniteField<T>& field)
{
     FiniteField<T>::bitIterator bits = field.getMSB();

    while (bits >= field.getLSB()){
        stream << *bits;
        --bits;
    }
    stream << "'";

    return stream;
}