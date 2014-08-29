#include "FiniteFields.h"
#include <string.h>     /* strcat */

#include <windows.h> // WinApi header

//template<class T>
//FiniteField<T>* FiniteField<T>::irrPoly = nullptr;

template<class T>
FiniteField<T>::FiniteField(int bitCount){

    mWordSize = (sizeof(T)* 8);
    mWordCount = bitCount / mWordSize;
    if (mWordCount * mWordSize < bitCount) mWordCount++;

    mBitCount = bitCount;

    this->num = (T*)calloc(mWordCount, sizeof(T));

}
template<class T>
FiniteField<T>::FiniteField(const FiniteField<T>& copy){

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
void FiniteField<T>::add(const FiniteField<T>& augend, 
                         const FiniteField<T>& addend, 
                               FiniteField<T>& sum){

    assert(augend.mWordCount == addend.mWordCount);

    for (int i = 0; i < augend.mWordCount; i++){
        sum(i) = augend(i) ^ addend(i);
    }
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
    for (int i = 0; i < mWordCount -1; i++){
        num[i] >>= shifts;

        shiftIn = num[i + 1] << (sizeof(T) * 8 - shifts);

        num[i] ^= shiftIn;
    }
    num[mWordCount - 1] >>= shifts;
}

template<class T>
void FiniteField<T>::multiply(const FiniteField<T>& multiplicand, 
                                const FiniteField<T>& multiplier, 
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

            printf(" ");
            doubleP1.bitPrint();
            printf("\t");

            multiplierCopy.bitPrint();
        }
        else{
            printf("\t\t\t\t\t");

                multiplierCopy.bitPrint();
        }
        printf("\n");
        doubleP1 <<= 1;
        multiplierCopy >>= 1;
    }
    printf("^___________________________________________________\n ");
    tempProd.bitPrint();
    printf("\n\n");


    FiniteField<T>&  tempMod = getIrrPoly(multiplicand.mBitCount);


    modulus(tempProd, tempMod);


    printf("\n^___________________________________________________\n ");
    printf("                  ");
    tempProd.bitPrint();  
    printf("  % ");
    tempMod.bitPrint();
    printf("\n\n");

    for (int i = 0; i < tempProd.mWordCount; i++)
    {
        product(i) = tempProd(i);
    }
    //product.mBitCount = tempProd.mBitCount;
    //product.mWordCount = tempProd.mWordCount;
}

template<class T>
FiniteField<T>& FiniteField<T>::getIrrPoly(int bitCount){

    FiniteField<T>* irrPoly = new FiniteField<T>(bitCount);
    for (int i = 0; i < bitCount; i++){
        irrPoly->num[i] = (T) i * 3 + 4;
    }

    // set top bit.
    irrPoly->num[irrPoly->mWordCount - 1] = (T)-1;

    return *irrPoly;
}

//const char *byte_to_binary(int x)
//{
//    static char b[30];
//    b[0] = '\0';
//
//    for (int z = 128; z > 0; z >>= 1)
//    {
//        strcat_s(b, ((x & z) == z) ? "1" : "0");
//
//    }
//
//    return b;
//}

template<class T>
void FiniteField<T>::clear()
{
    for (int i = 0; i < mWordCount; i++){
        num[i] = 0;
    }
    
}
template<class T>
void FiniteField<T>::modulus(FiniteField<T>& m, const FiniteField<T>& divisor){
    
    FiniteField<T> quotient = FiniteField<T>(divisor.mBitCount);
    FiniteField<T> remainder = FiniteField<T>(divisor.mBitCount);

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
T& FiniteField<T>::operator()(const int & i) const
{
    assert( i <= mWordCount);
    return num[i];
}

template<class T>
void FiniteField<T>::division(const FiniteField<T>& dividend,
                              const FiniteField<T>& divisor, 
                                    FiniteField<T>& quotient , 
                                    FiniteField<T>& remainder)
{
    T mask = ((T)1) << (sizeof(T)* 8 - 1);
    printf("division\n  ");
    dividend.bitPrint();
    printf("\n/ ");
    divisor.bitPrint();


    FiniteField<T> addShift(divisor.mBitCount);
    FiniteField<T> accumulator(dividend);
    FiniteField<T> divisorShift(dividend.mBitCount);

    for (int i = 0; i < divisor.mWordCount; i++)
        divisorShift(i) = divisor(i);

    addShift(0) = 1;
    int shifts = 0;

    printf("\n  ");
    divisorShift.bitPrint();
    printf("\n  ");

    // shift the modulus to have it's most significant 1  in the MSB position.
    while((divisorShift(divisorShift.mWordCount - 1) & mask) == 0){
        divisorShift <<= 1;

        divisorShift.bitPrint();
        printf("\n  ");
        shifts++;
    }
    if (shifts > 0){
        addShift <<= (shifts );
    }

    quotient.clear();
    assert( !addShift.isZero());

    for (int wordIdx = accumulator.mWordCount - 1; wordIdx >= 0 && shifts >=0; wordIdx--){

        mask = ((T)1) << (sizeof(T)* 8 - 1);

        for (int bitIdx = sizeof(T)* 8 - 1; bitIdx >= 0 && shifts >= 0; bitIdx--, shifts--){
            if (accumulator(wordIdx ) & mask)
            {
                /*printf("\nq ");
                quotient.bitPrint();*/
                printf("\na ");
                accumulator.bitPrint();

                printf(" , ");
                quotient.bitPrint(shifts);

                printf("\n^ ");
                divisorShift.bitPrint();

                printf(" , ");
                addShift.bitPrint();

                add(quotient, addShift, quotient);
                add(accumulator, divisorShift, accumulator);
            }
            else{
                printf("\n-\t\t\t\t\t");
                printf(", ");
                quotient.bitPrint(shifts );
            }

            mask >>= 1;
            addShift >>= 1;
            divisorShift >>= 1;
        }
    }

    for (int i = 0; i < divisor.mWordCount; i++)
        remainder(i) = accumulator(i);

}


template<class T>
void FiniteField<T>::invert(const FiniteField<T>& base, FiniteField<T>& inverse)
{
    FiniteField<T> gcd(base.mBitCount);
    FiniteField<T> aCo(base.mBitCount);

    extGCD(getIrrPoly(), base, gcd, aCo, inverse);
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

    remainders.push_back(&a);
    scalers.push_back(nullptr);

    remainders.push_back(&b);
    scalers.push_back(nullptr);

    // Compute the Gcd
    int step;
    for (step = 1; remainders[step] != 0; step++)
    {   
        // leftHandSide = multiple * scaler + remainder

        FiniteField<T>* leftHandSide = remainders[step - 1];
        FiniteField<T>* multiple     = remainders[step];

        FiniteField<T>* scaler       = new FiniteField<T>(a.mBitCount);
        FiniteField<T>* remainder    = new FiniteField<T>(a.mBitCount);

        division(*leftHandSide, *multiple, *scaler, *remainder);

        scalers.push_back(scale);
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
    int aIdx = step -2;
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
        {
            //assert(0);//?
            break;
        }

        multiply(*scalers[aIdx], aCoefficient, temp);
        add(bCoefficient, temp, bCoefficient);
        aIdx -=2;
    }

    FiniteField<T> at(a.mBitCount);
    FiniteField<T> bt(a.mBitCount);

    multiply(a, aCoefficient, at);
    multiply(b, bCoefficient, bt);
    assert( at == bt);

}



template<class T>
void FiniteField<T>::bitPrint()const
{

    bitPrint(-1);
}
template<class T>
void FiniteField<T>::bitPrint(int idx)const
{

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int curIdx = mWordCount * sizeof(T) * 8 - 1;

    for (int i = mWordCount - 1; i >= 0; i--){

        for (int j = sizeof(T)-1; j >= 0; j--){


            uint8_t  bt = *(((uint8_t*)&num[i]) + j);

            char b[9];
            b[0] = '\0';
            for (int z = 128; z > 0; z >>= 1)
            {
                if (curIdx == idx ){
                    SetConsoleTextAttribute(hConsole, 2);
                }
                printf((((bt & z) == z) ? "1" : "0"));

                if (curIdx == idx){
                    SetConsoleTextAttribute(hConsole, 15);
                }
                curIdx--;
            }
            printf(" ");
        }
    }
    printf("'");
}


template<class T>
void FiniteField<T>::print() const
{

    for (int i = mWordCount - 1; i >= 0 ; i--){
        printf("%016llx ", num[i] );
    }
    printf("`\n");
}




template<class T>
bool FiniteField<T>::isZero()const
{
    for (int i = 0; i < mWordCount; i++)
    {
        if (num[i] != 0) return false;
    }
    return true;
}
template<class T>
void FiniteField<T>::randomize(){

    //static std::default_random_engine generator;
    //static std::uniform_int_distribution<T> distribution(0, (T)-1);

    for (int i = 0; i < mWordCount; i++){

        //T n = distribution(generator);
        T n = (T)rand();
        num[i] = n;

    }
}



