#include "FiniteFields.h"
#include <string.h>     /* strcat */

FiniteField::FiniteField(int fieldSize, vector<u64> num){


    this->fieldSize = fieldSize;

    assert(fieldSize >= num.size());

    this->num = (u64*)calloc(fieldSize, sizeof(u64));

    for (int i = 0; i < num.size(); i++){
        this->num[i] = num[i];
    }

}
FiniteField::FiniteField(int fieldSize){

    this->fieldSize = fieldSize;
    this->num = (u64*)calloc(fieldSize, sizeof(u64));

}

FiniteField::~FiniteField(){
    if (num != nullptr)
        free(num);
}

void FiniteField::init(int fieldSize){
    if (num == nullptr)
        free(num);

    this->fieldSize = fieldSize;
    this->num = (u64*)calloc(fieldSize, sizeof(u64));
}

FiniteField::FiniteField(){

    this->fieldSize = 0;
    this->num = nullptr;
}



void FiniteField::addC2(FiniteField& p1, FiniteField& p2, FiniteField& out){

	assert(p1.fieldSize == p2.fieldSize);
	u64 carry = 0, t1;

	if (out.fieldSize < p1.fieldSize)
		out.init(p1.fieldSize);

	for (int i = 0; i < p1.fieldSize; i++){
		out.num[i] = p1.num[i] ^ p2.num[i];		
	}
}

void FiniteField::addC10( FiniteField& p1,  FiniteField& p2, FiniteField& out){
    
    assert(p1.fieldSize == p2.fieldSize);
    
    if (out.fieldSize < p1.fieldSize)
        out.init(p1.fieldSize);
        
    u64 carry = 0, t1;

    for (int i = 0; i < p1.fieldSize; i++){
        t1 = p1.num[i];

        out.num[i] += p1.num[i] + p2.num[i];

        if (( out.num[i] < t1 ||( carry && out.num[i] == t1 )  ) && i + 1 < p1.fieldSize ){
            // carry. 
            carry = 1;
            out.num[i+1]++;
        }
        else{
            carry = 0;
        }
        
    }
}

void FiniteField::multiplyC2(FiniteField& p1, FiniteField& p2, FiniteField& out){



}

void FiniteField::print64(){

    for (int i = fieldSize - 1; i >= 0 ; i--){
        printf("%016llx ", num[i] );
    }
    printf("`\n");
}
const char *byte_to_binary(int x)
{
    static char b[30];
    b[0] = '\0';

    int z;
    for (z = 128*128*2; z > 0; z >>= 1)
    {
        strcat_s(b, ((x & z) == z) ? "1" : "0");
        
    }

    return b;
}
void FiniteField::randomize64(){

    static std::default_random_engine generator;
    static std::uniform_int_distribution<u64> distribution(0, (u64)-1);

    printf("maxRand %x\n", RAND_MAX);
    for (int i = 0; i < fieldSize; i++){
        /*unsigned int r0 = rand();
        unsigned int r1 = rand();
        unsigned int r2 = rand();
        unsigned int r3 = rand();

        printf("r0 %04x : %s\n", r0, byte_to_binary(r0));
        printf("r1 %04x : %s\n", r1, byte_to_binary(r1));
        printf("r2 %04x : %s\n", r2, byte_to_binary(r2));
        printf("r3 %04x : %s\n", r3, byte_to_binary(r3));

        u64 n = ((((u64)r3 << 48) | (u64)r2 << 32) | (u64)r1 << 16) | r0;*/

        u64 n = distribution(generator);
        num[i] = n;

    }
}

void FiniteField::print16(){
    for (int i = fieldSize - 1; i >= 0; i--){

        printf("%02x ", num[i]);
    }
    printf("`\n");
}

void FiniteField::randomize16(){
    for (u64 i = 0; i < fieldSize; i++){

        char n = (char)rand();
        num[i] = n;

    }
}


