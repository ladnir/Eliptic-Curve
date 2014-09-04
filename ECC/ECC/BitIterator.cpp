#include "BitIterator.h"
//#include "FiniteFields.cpp"
#include <windows.h> // WinApi header

template<class T>
BitIterator<T>::BitIterator(const FiniteField<T>& field) 
							: mField(field)
							, mWordIdx(0)
							, mBitIdx(0)
							, mMask(1)
{}

template<class T>
void BitIterator<T>::goToBit(const int& idx)
{
	mBitIdx = idx % mField.mWordSize;
	mWordIdx = idx / mField.mWordSize;

	mMask <<= mBitIdx;
}

template<class T>
void BitIterator<T>::goToMSB()
{
	goToBit(mField.mBitCount - 1);
}

template<class T>
void BitIterator<T>::goToLSB()
{
	mBitIdx = 0;
	mWordIdx = 0;
	mMask = 1;
}

template<class T>
bool BitIterator<T>::hasLesserBits() const
{
	return   mWordIdx == 0 
		  && mBitIdx  == 0;
}

template<class T>
bool BitIterator<T>::hasGreaterBits() const
{
	return   mWordIdx == (mField.mWordCount - 1) 
		  && mBitIdx  == (mField.mWordSize  - 1);
}

template<class T>
bool BitIterator<T>::operator*() const
{
	return ((mField(mWordIdx) & mMask) != 0);
}

template<class T>
void BitIterator<T>::operator++(int)
{
	if (mBitIdx != mField.mWordSize - 1){
		// shift the mask inside a word
		mBitIdx++;
		mMask <<= 1;
	}else{
		// shift the mask to the start 
		//	of the next work
		mWordIdx++;
		mBitIdx = 0;
		mMask = 1;
	}
	//__asm{ rol mMask, 1 }
}

template<class T>
void BitIterator<T>::operator--(int)
{
	if (mBitIdx != 0){
		// shift the mask inside a word
		mBitIdx--;
		mMask >>= 1;
	}
	else{
		// shift the mask to the start 
		//	of the next work
		mWordIdx--;
		mBitIdx = mField.mWordSize - 1;
		mMask = (T) (1 << (sizeof(T) * 8 - 1));
	}
	//__asm{ ror mMask, 1 }
}

template<class T>
bool BitIterator<T>::operator<=(const BitIterator<T>& rhs) const
{
	if (mWordIdx < rhs.mWordIdx) return true;
	if (mWordIdx == rhs.mWordIdx && mBitIdx <= rhs.mBitIdx) return true;
	return false;
}

template<class T>
bool BitIterator<T>::operator>=(const BitIterator<T>& rhs) const
{
	if (mWordIdx > rhs.mWordIdx) return true;
	if (mWordIdx == rhs.mWordIdx && mBitIdx >= rhs.mBitIdx) return true;
	return false;
}

template<class T>
bool BitIterator<T>::operator==(const BitIterator<T>& rhs) const
{
	if (mWordIdx == rhs.mWordIdx && mBitIdx == rhs.mBitIdx) return true;
	return false;
}


template<class T>
void BitIterator<T>::flipBit()
{
	mField(mWordIdx) ^= mMask;
}

template<class T>
ostream& operator << (ostream& stream, const BitIterator<T>& bitIter)
{
	BitIterator<T> scanner(bitIter);
	BitIterator<T> lsb(bitIter);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	scanner.goToMSB();
	lsb.goToLSB();
	while (scanner >= lsb)
	{
		if (scanner == bitIter) SetConsoleTextAttribute(hConsole, 2);

		stream << *scanner;

		if (scanner == bitIter) SetConsoleTextAttribute(hConsole, 7);
		scanner--;
	}
	stream << "'";

	return stream;
}