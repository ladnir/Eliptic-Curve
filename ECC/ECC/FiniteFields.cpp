#include "FiniteFields.h"
#include <string.h>     /* strcat */

template<class T>
FiniteField<T>* FiniteField<T>::irrPoly = nullptr;

template<class T>
FiniteField<T>::FiniteField(int bitCount){


    mWordCount = bitCount / (sizeof(T) * 8);
    if (mWordCount * (sizeof(T)* 8) < bitCount) mWordCount++;

    mBitCount = bitCount;

    this->num = (T*)calloc(mWordCount, sizeof(T));

}

template<class T>
FiniteField<T>::~FiniteField(){
    if (num != nullptr)
        free(num);
}

template<class T>
void FiniteField<T>::addC2(FiniteField<T>& p1, FiniteField<T>& p2, FiniteField<T>& out){

    assert(p1.mWordCount == p2.mWordCount);

    for (int i = 0; i < p1.mWordCount; i++){
        out.num[i] = p1.num[i] ^ p2.num[i];		
    }
}


template<class T>
void FiniteField<T>::operator<<=(const int& shifts){

    assert(shifts < sizeof(T)* 8);

    T shiftIn;
    for (int i = mWordCount - 1; i > 0; i--){
        num[i] <<= shifts;

        shiftIn = num[i - 1] >> (sizeof(T)* 8 - shifts);

        num[i] ^= shiftIn;
    }

    num[0] <<= shifts;
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
void FiniteField<T>::multiplyC2(FiniteField<T>& p1, FiniteField<T>& p2, FiniteField<T>& out){

    FiniteField<T> tempProd(p1.mBitCount * 2);
    FiniteField<T> doubleP1(p1.mBitCount * 2);
    FiniteField<T> p2Copy(p2.mBitCount);

    for (int i = 0; i < p1.mWordCount; i++){
        doubleP1.num[i] = p1.num[i];
        p2Copy.num[i] = p2.num[i];
    }

    T mask = 1;
    for (int i = 0; i < p1.mBitCount; i++){
        

        if (p2Copy.num[0] & 1){
            //tempProd ^= doubleP1;
            addC2(tempProd, doubleP1, tempProd);
            /*for (int j = p1.mBitCount; j > i; j--)
            p1.bitPrint();*/
            printf(" ");
            doubleP1.bitPrint();
            printf("\t");

            p2Copy.bitPrint();
        }
        else{
            printf("\t\t\t\t\t");

                p2Copy.bitPrint();
        }
        printf("\n");
        doubleP1 <<= 1;
        p2Copy >>= 1;
    }
    printf("^___________________________________________________\n ");
    tempProd.bitPrint();
    printf("\n\n");

    modulus(tempProd, p1.getIrrPoly());


    printf("^___________________________________________________\n ");
    tempProd.bitPrint();
    printf("\n\n");
}

template<class T>
FiniteField<T>& FiniteField<T>::getIrrPoly(){

    if (FiniteField<T>::irrPoly == nullptr){
        FiniteField<T>::irrPoly = new FiniteField<T>(mBitCount);
        for (int i = 0; i < mWordCount; i++){
            irrPoly->num[i] = (T) i * 3 + 4;
        }
    }

    return *FiniteField<T>::irrPoly;
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
void FiniteField<T>::modulus(FiniteField<T>& element, const FiniteField<T>& mod){
    
    T mask = ((T)1) << (sizeof(T)*8 -1 );

    //FiniteField<T> elemShift(element.mBitCount);
    //for (int i = 0;i<element.mWordCount;i++) 
    //    elemShift.num[i] = element.num[i];
    //

    FiniteField<T> modShift(element.mBitCount);
    for (int i = 0; i < mod.mWordCount; i++)
        modShift.num[i] = (T)3 * i + 4;
    

    printf(" ");
    int shifts = 0;
    // shift the modulus to have it's most significant 1  in the MSB position.
    while ((modShift.num[modShift.mWordCount - 1] & mask) == 0){
        modShift <<= 1;
        //modShift.bitPrint();
        //printf("\n");
        shifts++;
    }
    

    for (int wordIdx = element.mWordCount - 1; wordIdx >=0 && shifts; wordIdx--){

        mask = ((T)1) << (sizeof(T)* 8 - 1);

        for (int bitIdx = sizeof(T)* 8; bitIdx >= 0 && shifts; wordIdx--, shifts--){
            if (element.num[wordIdx] & mask){
                {
                    unsigned char x = mask;
                    static char b[30];
                    b[0] = '\0';

                    for (int z = 128; z > 0; z >>= 1)
                    {
                        strcat_s(b, ((x & z) == z) ? "1" : "0");

                    }
                    printf("\n ");
                    element.bitPrint();
                    printf("\n %s \n--", b);
                }
                printf("\n^");
                modShift.bitPrint();
                //element ^= modShift;
                addC2(element, modShift, element);
                printf("\n ");
                element.bitPrint();
            }
            else{
                printf("\n");
            }

            mask >>=1;
            modShift>>=1;
        }
    }

    T* remainder = (T*)malloc(mod.mWordCount * sizeof(T));
    for (int i = 0; i < mod.mWordCount; i++)
        remainder[i] = element.num[i];
    
    delete element.num;

    element.num = remainder;
}






template<class T>
void FiniteField<T>::bitPrint(){

    for (int i = mWordCount - 1; i >= 0; i--){

        for (int j = sizeof(T)-1; j >= 0; j--){

            
            uint8_t  bt = *(((uint8_t*)&num[i]) + j);

            char b[9];
            b[0] = '\0';
            for (int z = 128; z > 0; z >>= 1)
            {
                strcat_s(b, ((bt & z) == z) ? "1" : "0");

            }
            printf("%s", b);
        }
    }
    printf("'");
}


template<class T>
void FiniteField<T>::print(){

    for (int i = mWordCount - 1; i >= 0 ; i--){
        printf("%016llx ", num[i] );
    }
    printf("`\n");
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



