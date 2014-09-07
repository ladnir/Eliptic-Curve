//#include "FiniteFields.h"
#include "BitIterator.cpp"
#include <string.h>     /* strcat */

#include <iostream>
#include <map>

template<class T>
 FiniteField<T>::FiniteField(int bitCount){

    mWordSize = (sizeof(T)* 8);
    mWordCount = bitCount / mWordSize;
    if (mWordCount * mWordSize < bitCount) mWordCount++;

    mBitCount = bitCount;

    this->num = (T*)calloc(mWordCount, sizeof(T));

}
template<class T>
 FiniteField<T>::FiniteField(const  FiniteField<T>& copy){

    mWordSize = copy.mWordSize;
    mBitCount = copy.mBitCount;
    mWordCount = copy.mWordCount;
    
    num = (T*)malloc(sizeof(T) * mWordCount);

    for (int i = 0; i < mWordCount; i++)
    {
        num[i] = copy(i);
    }
}

template<class T>
 FiniteField<T>::~FiniteField(){
    if (num != nullptr)
        free(num);
}

template<class T>
void  FiniteField<T>::add(const  FiniteField<T>& augend, 
                         const  FiniteField<T>& addend, 
                                FiniteField<T>& sum){

    assert(augend.mWordCount == addend.mWordCount);

    for (int i = 0; i < augend.mWordCount; i++){
        sum(i) = augend(i) ^ addend(i);
    }
}

template<class T>
void  FiniteField<T>::multiply(const  FiniteField<T>& multiplicand, 
                              const  FiniteField<T>& multiplier, 
                                     FiniteField<T>& product){

     FiniteField<T> tempProd(multiplicand.mBitCount * 2);
     FiniteField<T> doubleP1(multiplicand.mBitCount * 2);
     FiniteField<T> multiplierCopy(multiplier.mBitCount);

    for (int i = 0; i < multiplicand.mWordCount; i++){
        doubleP1(i) = multiplicand(i);
        multiplierCopy(i) = multiplier(i);
    }

    T mask = 1;
    for (int i = 0; i < multiplicand.mBitCount; i++){
        

        if (multiplierCopy(0) & 1){
            add(tempProd, doubleP1, tempProd);
			if ( FiniteField<T>::show)
			{
				cout << " " << doubleP1 << "\t" << multiplierCopy << endl;
			}
        }
        else{
			if ( FiniteField<T>::show)
			{
				cout << "\t\t\t\t\t" << multiplierCopy << endl;
			}
        }

        doubleP1 <<= 1;
        multiplierCopy >>= 1;
    }
	if ( FiniteField<T>::show)
	{
		  cout << "^___________________________________________________" << endl;
		  cout << " " << tempProd << endl << endl;
	}
    FiniteField<T>&  tempMod = getIrrPoly(multiplicand.mBitCount);
	FiniteField<T> quotient(product.mBitCount);
    division(tempProd, tempMod, quotient, product);

	if ( FiniteField<T>::show)
	{
		std::cout << endl << product << endl;
	}
}

template<class T>
void  FiniteField<T>::modulus( FiniteField<T>& m, const  FiniteField<T>& divisor){
    
     FiniteField<T> quotient(divisor.mBitCount);
     FiniteField<T> remainder(divisor.mBitCount);

    division(m, divisor, quotient, remainder);

    /*delete m.num;
    m.num = remainder.num;
    remainder.num = nullptr;
*/
    m.clear();

    for (int i = 0; i < remainder.mWordCount; i++)
    {
        m(i) = remainder(i);
    }
    m.mBitCount = remainder.mBitCount;
    m.mWordCount = remainder.mWordCount;
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
	if (FiniteField<T>::show)
	{
		cout << "division" << endl;
		cout << "  " << dividend << endl;
		cout << "/ " << divisorShift << endl;
	}
    ////////////////////////////////////////////////////////

    while (*accumulatorBI == 0){
        dividendLeadingZeros++;
        accumulatorBI--;
    }

    while (*divisorBI == 0){
        divisorLeadingZeros++;
        divisorBI--;
    }

    shifts = dividend.mBitCount - dividendLeadingZeros - divisor.mBitCount + divisorLeadingZeros;

    quotientBI.goToBit(shifts);
    divisorShift <<= shifts;

    while (shifts >= 0){
        if (*accumulatorBI != 0)
        {
			if (FiniteField<T>::show)
			{
				cout << "a " << accumulator << " , " << quotientBI << endl;
				cout << "^ " << divisorShift << endl;
			}
            quotientBI.flipBit();
            add(accumulator, divisorShift, accumulator);
        }
        else{
			if (FiniteField<T>::show)
			{
				cout << "- " << accumulator << " , " << quotientBI << endl;
			}
        }
        accumulatorBI--;
        quotientBI--;
        divisorShift >>= 1;
        shifts--;
    }
	if (FiniteField<T>::show)
	{
		cout << "r " << accumulator << " ,q" << quotient << endl;
	}

    for (int i = 0; i < divisor.mWordCount; i++)
        remainder(i) = accumulator(i);

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
	inverse.clear();

	FiniteField<T> prod(base.mBitCount);
	FiniteField<T> knownProd(base.mBitCount);
	knownProd(0) = 1;

	while (!( prod == knownProd))
	{
		inverse++;
		assert(!inverse.isZero());

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

    vector<FiniteField<T>*> remainders;
    vector<FiniteField<T>*> scalers;

    FiniteField<T> aCopy(a);
    FiniteField<T> bCopy(b);

	FiniteField<T> z(a.mBitCount);

    remainders.push_back(&aCopy);
    scalers.push_back(&z);

	
    remainders.push_back(&bCopy);
    scalers.push_back(&z);

    // Compute the Gcd
    int step;
    for (step = 1; ! remainders[step]->isZero(); step++)
    {   
        // leftHandSide = multiple * scaler + remainder

        FiniteField<T>* leftHandSide = remainders[step - 1];
        FiniteField<T>* multiple     = remainders[step];

        FiniteField<T>* scaler       = new FiniteField<T>(a.mBitCount);
        FiniteField<T>* remainder    = new FiniteField<T>(a.mBitCount);


        division(*leftHandSide, *multiple, *scaler, *remainder);

		cout << *leftHandSide << " = " << *multiple << " * " << *scaler << " + " << *remainder << endl;
		testEq(*leftHandSide, *multiple, *scaler, *remainder);

        scalers.push_back(scaler);
        remainders.push_back(remainder);
    }

    // get rid of the remainder == 0 step.
    step--;
    remainders.pop_back();
    scalers.pop_back();

    // Copy the Gcd out.
    for (int i = 0; i < remainders.back()->mWordCount; i++)
    {
        gcd(i) = remainders.back()->num[i];
    }

    // Compute the linear combination. (extended part)
    // i.e.   h,k  s.t.  h*a + k*b = gcd  
   /* int aIdx = step -2;
    int bIdx = step -1;

    aCoefficient.clear();
    aCoefficient(0) = 1;
    bCoefficient.copy(*scalers[step]);

    FiniteField<T> temp(a.mBitCount);

    while (aIdx)
    {
        multiply(*scalers[bIdx], bCoefficient, temp );
        add(aCoefficient, temp, aCoefficient);

        bIdx -= 2;

        if (bIdx == 0)
            break;

        multiply(*scalers[aIdx], aCoefficient, temp);
        add(bCoefficient, temp, bCoefficient);
        aIdx -=2;
    }

    FiniteField<T> at(a.mBitCount);
    FiniteField<T> bt(a.mBitCount);
*/
  /*  multiply(a, aCoefficient, at);
    multiply(b, bCoefficient, bt);
    assert( at == bt);*/

}

template<class T>
void FiniteField<T>::bruteForceGCD(const FiniteField<T>& a, 
								   const FiniteField<T>& b, 
								         FiniteField<T>& gcd)
{
	FiniteField<T> scanner(a.mBitCount);
	scanner(0) = 1;

	FiniteField<T> aRem(a.mBitCount);
	FiniteField<T> bRem(a.mBitCount);

	FiniteField<T> quo(a.mBitCount);


	while (!scanner.isZero()){
		
		FiniteField<T>::division(a, scanner, quo, aRem);
		FiniteField<T>::division(b, scanner, quo, bRem);

		if (aRem.isZero() && bRem.isZero()){
			cout << "g " << scanner << endl;
			gcd.copy(scanner);
		}
		cout << "  " << scanner << endl;
		scanner++;

	}
}
template<class T>
void testEq(FiniteField<T>& lhs, FiniteField<T>& mul, FiniteField<T>& scl, FiniteField<T>& rem)
{
#ifdef _DEBUG 
	FiniteField<T> prod(lhs.mBitCount);
	FiniteField<T> sum(lhs.mBitCount);

	FiniteField<T>::multiply(mul, scl, prod);
	FiniteField<T>::add(prod, rem, sum);

	if (! (sum == lhs))
	{
		cout << "n eq " << endl;
		cout << sum << " = " << mul << " * " << scl << " + " << rem << endl;
		cout << prod << " = " << mul << " * " << scl << endl;
	}
#endif
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
	for (int i = 0; i < mWordCount; i++)
	{
		num[i]++;
		if (num[i] != 0) return;
	}
}

template<class T>
bool FiniteField<T>::operator==(const FiniteField<T>& cmp) const
{
    assert(mBitCount == cmp.mBitCount);

    for (int i = 0; i < mWordCount; i++)
    {
        if (num[i] != cmp(i)) return false;
    }
    return true;
}

template<class T>
 FiniteField<T>&  FiniteField<T>::getIrrPoly(int bitCount)
 {
	/*map<int, FiniteField<T>>::iterator irrPoly = irrPolies.find(bitCount);

	if (irrPoly != irrPolies.end())
	{
		return *(irrPoly->second);
	}
	else{*/

		FiniteField<T>* ip = new FiniteField<T>(bitCount);
		//irrPolies.emplace(bitCount, ip);

		// 100011011
		if (bitCount == 9)
		{
			ip->operator()(0) = 27;
			ip->operator()(1) = 1;

		}
		else{
			assert(0 && "undefined irrPoly");
		}

		return *ip;
	//}
}

template<class T>
void  FiniteField<T>::clear()
{
    for (int i = 0; i < mWordCount; i++){
        num[i] = 0;
    }

}

template<class T>
void  FiniteField<T>::copy(const  FiniteField<T>& source)
{
    assert(mBitCount == source.mBitCount);

    for (int i = 0; i < source.mWordCount; i++)
    {
        num[i] = source(i);
    }
}

//template<class T>
//void  FiniteField<T>::bitPrint()const
//{
//    bitPrint(-1);
//}
//
//template<class T>
//void  FiniteField<T>::bitPrint(int idx)const
//{
//
//    HAPDLE hConsole = GetStdHandle(STD_OUTPUT_HAPDLE);
//
//    int curIdx = mWordCount * sizeof(T) * 8 - 1;
//
//	 FiniteField<T>::bitIterator bits = this->getMSB();
//
//	while (bits >= this->getLSB())
//	{
//        if (curIdx == idx )SetConsoleTextAttribute(hConsole, 2);
//        
//		cout << *bits;
//		bits--;
//
//        if (curIdx == idx)SetConsoleTextAttribute(hConsole, 7);
//        curIdx--;
//	}
//
//    printf("'");
//}

template<class T>
void  FiniteField<T>::print() const
{

    for (int i = mWordCount - 1; i >= 0 ; i--){
        printf("%016llx ", num[i] );
    }
    printf("`\n");
}

template<class T>
bool  FiniteField<T>::isZero()const
{
    for (int i = 0; i < mWordCount - 1; i++)
    {
        if (num[i] != 0) return false;
    }

	if ()
    return true;
}

template<class T>
void  FiniteField<T>::randomize(){

    //static std::default_random_engine generator;
    //static std::uniform_int_distribution<T> distribution(0, (T)-1);

    for (int i = 0; i < mWordCount; i++){

        //T n = distribution(generator);
        T n = (T)rand();
        num[i] = n;

    }
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

    while (bits >= field.getLSB())
    {
        stream << *bits;
        bits--;

    }

    stream << "'";

    return stream;
}