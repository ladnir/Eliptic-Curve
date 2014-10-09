#pragma once
#include "Fields.h"

template<class T>
class BitIterator
{
public:
	BitIterator(const Field<T>&);

	void goToBit(const int&);
	void goToMSB();
	void goToLSB();
    
    bool isInRnage() const;
	bool hasLesserBits() const;
	bool hasGreaterBits() const;

	inline bool operator*() const;
	BitIterator<T>& operator++();
    BitIterator<T>& operator--();

	bool operator<=(const BitIterator<T>&) const;
	bool operator>=(const BitIterator<T>&) const;
	bool operator==(const BitIterator<T>&) const;

	void flipBit();
	//void setBit(); 
	//void clearBit();

private:
	const Field<T>& mField;
	T mMask;
	int mWordIdx;
	int mBitIdx;
};
