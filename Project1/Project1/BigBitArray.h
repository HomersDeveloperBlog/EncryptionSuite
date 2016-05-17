//You currently make assumptions about machine word size. Don't.
//Transform into class implementation.
//Compiler assume statements for nonzero template parameters

#ifndef JTH_BIGBITARRAY_H
#define JTH_BIGBITARRAY_H

#include "assert.h"
#include "stdint.h"
#include <array>
#include <tuple>

#include "LinkerMacros.h"

using namespace std;

template<unsigned int NLength>
class BigBitArray
{
	BigBitArray(uint64_t i_nA)
	{
		anWords = MultiWordIntegerChangeLength<NLength>(i_nA);
	}

	BigBitArray(const array<uint64_t, NLength> & i_anA)
	{
		anWords = i_anA;
	}

	uint64_t & operator[](uint64_t i_nIndex)
	{
		return anWords[i_nIndex];
	}

	const uint64_t operator[](uint64_t i_nIndex) const
	{
		return (*this)[i_nIndex];
	}

protected:
	array<uint64_t, NLength> anWords;
};

template<unsigned int NLength>
void MultiWordIntegerFillWithZeros( //%need in place and transform
	array<uint64_t, NLength> & io_anA)
{
	for(unsigned int nWord = 0U; 
		nWord < NLength;
		++nWord) 
	{
		io_anA[nWord] = 0ULL;
	}
}

template<unsigned int NOutputLength, unsigned int NInputLength>
array<uint64_t, NOutputLength> MultiWordIntegerChangeLength(
	const array<uint64_t, NInputLength> & i_anA)
{
	array<uint64_t, NOutputLength> anReturnValue;

	//Project common length
	unsigned int nWord = 0U;
	for(; nWord < min(NInputLength, NOutputLength);
		++nWord)
	{
		anReturnValue[nWord] = i_anA[nWord];
	}
	
	//Add padding if injecting
	for(; nWord < NOutputLength;
		++nWord)
	{
		anReturnValue[nWord] = 0ULL;
	}

	//Check truncated words are zero
	for(; nWord < max(NInputLength, NOutputLength);
		++nWord)
	{
		//assert(!anReturnValue[nWord]);
	}
	
	return anReturnValue;
}

template<unsigned int NOutputLength>
array<uint64_t, NOutputLength> MultiWordIntegerChangeLength(
	uint64_t i_anA)
{
	array<uint64_t, 1U> anTemp = {i_anA};
	return MultiWordIntegerChangeLength<NOutputLength>(anTemp);
}

//Most significant non-equal position, or 0U
template<unsigned int NLength>
unsigned int EvalLexographicComparisonIndex(
	const array<uint64_t, NLength> & i_anA,
	const array<uint64_t, NLength> & i_anB)
{
	unsigned int nWord = NLength - 1U;
	for(; nWord > 0U && i_anA[nWord] == i_anB[nWord];
		--nWord) {}
	assert(nWord < NLength);

	return nWord;
}

template<unsigned int NLength>
bool EvalLexographicLessThan(
	const array<uint64_t, NLength> & i_anA,
	const array<uint64_t, NLength> & i_anB)
{
	unsigned int nWord = EvalLexographicComparisonIndex(
		i_anA,
		i_anB);

	return i_anA[nWord] < i_anB[nWord];
}

template<unsigned int NLength>
bool EvalLexographicGreaterThan(
	const array<uint64_t, NLength> & i_anA,
	const array<uint64_t, NLength> & i_anB)
{
	unsigned int nWord = EvalLexographicComparisonIndex(
		i_anA,
		i_anB);

	return i_anA[nWord] > i_anB[nWord];
}

template<unsigned int NLength>
bool EvalLexographicLessThanEqualTo(
	const array<uint64_t, NLength> & i_anA,
	const array<uint64_t, NLength> & i_anB)
{
	return !EvalLexographicGreaterThan(i_anA, i_anB);
}

template<unsigned int NLength>
bool EvalLexographicGreaterThanEqualTo(
	const array<uint64_t, NLength> & i_anA,
	const array<uint64_t, NLength> & i_anB)
{
	return !EvalLexographicLessThan(i_anA, i_anB);
}

template<unsigned int NLength>
bool EvalLexographicEqualTo(
	const array<uint64_t, NLength> & i_anA,
	const array<uint64_t, NLength> & i_anB)
{
	bool bReturnValue = true;
	for(unsigned int nWord = 0U; 
		nWord < NLength;
		++nWord) 
	{
		bReturnValue &= (i_anA[nWord] == i_anB[nWord]);
	}

	return bReturnValue;
}

template<unsigned int NLength>
bool EvalLexographicEqualToZero(
	const array<uint64_t, NLength> & i_anA)
{
	uint64_t nTestWord = 0ULL;
	for(unsigned int nWord = 0U; 
		nWord < NLength;
		++nWord) 
	{
		nTestWord |= i_anA[nWord];
	}

	return !nTestWord;
}

template<unsigned int NLength>
bool EvalLexographicNotEqualTo(
	const array<uint64_t, NLength> & i_anA,
	const array<uint64_t, NLength> & i_anB)
{
	return !EvalLexographicEqualTo(i_anA, i_anB);
}

template<unsigned int NLength>
unsigned int EvalMostSignificantWord(
	const array<uint64_t, NLength> & i_anA)
{
	for(unsigned int nMSWord = NLength;
		nMSWord-- > 0;)
	{
		if(i_anA[nMSWord])
		{
			return nMSWord;
		}
	}

	return NLength;
}

//%should be test-exported instead of inlined. also in cpp
//A binary search would be an improvement
INLINE unsigned int EvalMostSignificantBit(
	uint64_t i_nA)
{
	if(i_nA)
	{
		unsigned int nMSBit = 63U;
		uint64_t nCurrentPlaceMask = 1ULL << 63U;

		while(!(nCurrentPlaceMask & i_nA))
		{
			nCurrentPlaceMask >>= 1U;
			--nMSBit;
		}

		return nMSBit;
	}
	
	return 64U;
}

template<unsigned int NLength>
tuple<unsigned int, unsigned int> EvalMostSignificantBit(
	const array<uint64_t, NLength> & i_anA)
{
	unsigned int nMSWord = EvalMostSignificantWord(i_anA);
	unsigned int nMSBit = 64U;
	
	if(nMSWord < NLength)
	{
		nMSBit = EvalMostSignificantBit(i_anA[nMSWord]);
	}

	return make_pair(nMSWord, nMSBit);
}

template<unsigned int NLength>
unsigned int EvalLeastSignificantWord(
	const array<uint64_t, NLength> & i_anA)
{
	unsigned int nLSWord = 0U; 
	for(; nLSWord < NLength && !i_anA[nLSWord];
		++nLSWord) {}

	return nLSWord;
}

INLINE unsigned int EvalLeastSignificantBit(
	uint64_t i_nA)
{
	if(i_nA)
	{
		unsigned int nLSBit = 0U;
		uint64_t nCurrentPlaceMask = 1ULL;
	
		while(!(nCurrentPlaceMask & i_nA))
		{
			nCurrentPlaceMask <<= 1U;
			++nLSBit;
		}

		return nLSBit;
	}

	return 64U;
}

template<unsigned int NLength>
tuple<unsigned int, unsigned int> EvalLeastSignificantBit(
	const array<uint64_t, NLength> & i_anA)
{
	unsigned int nLSWord = EvalLeastSignificantWord(i_anA);
	unsigned int nLSBit = 64U;
	
	if(nLSWord < NLength)
	{
		nLSBit = EvalLeastSignificantBit(i_anA[nMSWord]);
	}

	return make_pair(nLSWord, nLSBit);
}

template<unsigned int NLength>
void MultiWordIntegerRightShiftWordsInPlace(
	array<uint64_t, NLength> & io_anA,
	unsigned int i_nWordShiftCount)
{
	assert(i_nWordShiftCount < NLength);

	if(i_nWordShiftCount)
	{
		//Starting at the most significant word, shift right
		for(unsigned int nWord = NLength; 
			nWord-- > i_nWordShiftCount;)
		{
			io_anA[nWord] = io_anA[nWord - i_nWordShiftCount];
		}

		//Then fill words < nWords with zero
		for(unsigned int nWord = i_nWordShiftCount;
			nWord-- > 0U; )
		{
			io_anA[nWord] = 0ULL;
		}
	}
}

template<unsigned int NLength>
array<uint64_t, NLength> MultiWordIntegerRightShiftWords(
	const array<uint64_t, NLength> & i_anA,
	unsigned int i_nWordShiftCount)
{
	array<uint64_t, NLength> anAShifted(i_anA);
	MultiWordIntegerRightShiftWordsInPlace(
		anAShifted,
		i_nWordShiftCount);

	return anAShifted;
}

//Note: This operation is defined to be a 'right shift' and does not agree with the C++ >> operator.
//Words are ordered left to right LS first. Bits within words are defined to be ordered this way too.
//According to this defition, the function performs a right shift.
template<unsigned int NLength>
void MultiWordIntegerRightShiftBitsInPlace(
	array<uint64_t, NLength> & io_anA,
	unsigned int i_nBitShiftCount, //Invokes undefined behaviour for shifting more bits than word size.
	unsigned int i_nLeftZeroWords = 0U)
{
	assert(i_nBitShiftCount < 64U);
	
	if(i_nBitShiftCount)
	{
		unsigned int nMSBSaveShiftAmount = 64U - i_nBitShiftCount;
		io_anA[NLength - 1U] <<= i_nBitShiftCount; //First shift on most significant word. No save/carry.

		//Subsequent words.
		for(unsigned int nWord = NLength - 1U;
			nWord-- > i_nLeftZeroWords;)
		{
			uint64_t nNewLSBits = io_anA[nWord] >> nMSBSaveShiftAmount; //Save the most significant 64 - n bits
			io_anA[nWord] <<= i_nBitShiftCount; //Shift right >> n bits
			io_anA[nWord + 1U] |= nNewLSBits; //Insert the saved bits into the least significant slot.
		}
	}
}

template<unsigned int NLength>
array<uint64_t, NLength> MultiWordIntegerRightShiftBits(
	const array<uint64_t, NLength> & i_anA,
	unsigned int i_nBitShiftCount,
	unsigned int i_nRightSignificantWords = 0U)
{
	array<uint64_t, NLength> anAShifted(i_anA);
	MultiWordIntegerRightShiftBits(
		anAShifted,
		i_nBitShiftCount,
		i_nRightSignificantWords);

	return anAShifted;
}

template<unsigned int NLength>
void MultiWordIntegerRightShiftInPlace(
	array<uint64_t, NLength> & io_anA,
	unsigned int i_nWordShiftCount,
	unsigned int i_nBitShiftCount)
{
	MultiWordIntegerRightShiftWordsInPlace(
		io_anA,
		i_nWordShiftCount);

	MultiWordIntegerRightShiftBitsInPlace(
		io_anA,
		i_nBitShiftCount,
		i_nWordShiftCount);
}

template<unsigned int NLength>
array<uint64_t, NLength> MultiWordIntegerRightShift(
	const array<uint64_t, NLength> & i_anA,
	unsigned int i_nWordShiftCount,
	unsigned int i_nBitShiftCount)
{
	array<uint64_t, NLength> anAShifted(i_anA);
	MultiWordIntegerRightShiftInPlace(
		anAShifted,
		i_nWordShiftCount,
		i_nBitShiftCount);

	return anAShifted;
}

template<unsigned int NLength>
void MultiWordIntegerRightShiftByOneBitInPlace(
	array<uint64_t, NLength> & io_anA)
{
	io_anA[NLength - 1U] <<= 1U; //First shift on most significant word. No save/carry.

	for(unsigned int nWord = NLength - 1U;
		nWord-- > 0;) 
	{
		uint64_t nNewLSBit = io_anA[nWord] >> 63U; //Save the most significant bit
		io_anA[nWord] <<= 1U; //Shift right >> n bits
		io_anA[nWord + 1U] |= nNewLSBit; //Insert the saved bits into the least significant slot.
	}
}

template<unsigned int NLength>
array<uint64_t, NLength> MultiWordIntegerRightShiftByOneBit(
	const array<uint64_t, NLength> & i_anA)
{
	array<uint64_t, NLength> anAShifted(i_anA);
	MultiWordIntegerRightShiftByOneBitInPlace(anAShifted);

	return anAShifted;
}

template<unsigned int NLength>
void MultiWordIntegerLeftShiftWordsInPlace(
	array<uint64_t, NLength> & io_anA,
	unsigned int i_nWordShiftCount)
{
	assert(i_nWordShiftCount < NLength);
	
	if(i_nWordShiftCount)
	{
		//Starting at the most significant word, shift right
		unsigned int nWord = 0U;
		for(; nWord < NLength - i_nWordShiftCount;
			++nWord)
		{
			io_anA[nWord] = io_anA[nWord + i_nWordShiftCount];
		}

		//First fill words < nWords with zero
		for(; nWord < NLength;
			++nWord)
		{
			io_anA[nWord] = 0ULL;
		}
	}
}

//This wrapper form results in a few extra assignments 
template<unsigned int NLength>
array<uint64_t, NLength> MultiWordIntegerLeftShiftWords(
	const array<uint64_t, NLength> & i_anA,
	unsigned int i_nWordShiftCount)
{
	array<uint64_t, NLength> anAShifted(i_anA);
	MultiWordIntegerLeftShiftWordsInPlace(
		anAShifted,
		i_nWordShiftCount);

	return anAShifted;
}

//Note: This operation is defined to be a 'left shift' and does not agree with the C++ << operator.
//Words are ordered left to right LS first. Bits within words are defined to be ordered this way too.
//According to this defition, the function performs a left shift.
template<unsigned int NLength>
void MultiWordIntegerLeftShiftBitsInPlace(
	array<uint64_t, NLength> & io_anA,
	unsigned int i_nBitShiftCount, //Invokes undefined behaviour for shifting more bits than word size.
	unsigned int i_nLeftSignificantWords = NLength)
{
	assert(i_nBitShiftCount < 64U);
	assert(i_nLeftSignificantWords && i_nLeftSignificantWords <= NLength);

	if(i_nBitShiftCount)
	{
		unsigned int nLSBitsSaveShiftAmount = 64U - i_nBitShiftCount;
		io_anA[0U] >>= i_nBitShiftCount; //First shift least significant word. No save/carry.

		for(unsigned int nWord = 1U;
			nWord < i_nLeftSignificantWords;
			++nWord)
		{
			uint64_t nNewMSBit = io_anA[nWord] << nLSBitsSaveShiftAmount; //Save the most significant bit
			io_anA[nWord] >>= i_nBitShiftCount; //Shift right >> n bits
			io_anA[nWord - 1U] |= nNewMSBit; //Insert the saved bits into the least significant slot.
		}
	}
}

template<unsigned int NLength>
void MultiWordIntegerLeftShiftBits(
	const array<uint64_t, NLength> & i_anA,
	unsigned int i_nBitShiftCount,
	unsigned int i_nLeftSignificantWords = NLength)
{
	array<uint64_t, NLength> i_anAShifted(i_anA);
	MultiWordIntegerLeftShiftBitsInPlace(
		i_anAShifted,
		i_nBitShiftCount,
		i_nLeftSignificantWords);

	return i_anAShifted;
}

template<unsigned int NLength>
void MultiWordIntegerLeftShiftInPlace(
	array<uint64_t, NLength> & io_anA,
	unsigned int i_nWordShiftCount,
	unsigned int i_nBitShiftCount)
{
	MultiWordIntegerLeftShiftWordsInPlace(
		io_anA,
		i_nWordShiftCount);
	
	MultiWordIntegerLeftShiftBitsInPlace(
		io_anA,
		i_nBitShiftCount,
		i_nWordShiftCount);
}

template<unsigned int NLength>
array<uint64_t, NLength> MultiWordIntegerLeftShift(
	const array<uint64_t, NLength> & i_anA, 
	unsigned int i_nWordShiftCount,
	unsigned int i_nBitShiftCount)
{
	array<uint64_t, NLength> anAShifted(i_anA);
	MultiWordIntegerLeftShiftInPlace(
		anAShifted,
		i_nWordShiftCount);

	return anAShifted;
}

template<unsigned int NLength>
void MultiWordIntegerLeftShiftByOneBitInPlace(
	array<uint64_t, NLength> & io_anA)
{
	io_anA[0U] >>= 1U; //First shift least significant word. No save/carry.

	for(unsigned int nWord = 1U;
		nWord < NLength;
		++nWord)
	{
		uint64_t nNewMSBit = io_anA[nWord] << 63U; //Save the most significant bit
		io_anA[nWord] >>= 1U; //Shift right >> n bits
		io_anA[nWord - 1U] |= nNewMSBit; //Insert the saved bits into the least significant slot.
	}
}

template<unsigned int NLength>
void MultiWordIntegerLeftShiftByOneBit(
	const array<uint64_t, NLength> & i_anA)
{
	array<uint64_t, NLength> anAShifted(i_anA);
	MultiWordIntegerLeftShiftByOneBitInPlace(anAShifted);

	return anAShifted;
}

template<unsigned int NLength>
void MultiWordIntegerSetBitInPlace(
	array<uint64_t, NLength> & io_nA, 
	const uint64_t i_nBitNumber)
{
	const unsigned int nWordNumber = static_cast<unsigned int>(i_nBitNumber >> 6U);
	const unsigned int nBitNumber = static_cast<unsigned int>(i_nBitNumber & 0x000000000000003fULL);
	MultiWordIntegerSetBitInPlace(io_nA, nWordNumber, nBitNumber);
}

template<unsigned int NLength>
void MultiWordIntegerSetBitInPlace(
	array<uint64_t, NLength> & io_nA, 
	const unsigned int i_nWordNumber,
	const unsigned int i_nBitNumber)
{
	assert(i_nWordNumber < NLength);
	assert(i_nBitNumber < 64U);

	io_nA[i_nWordNumber] |= (0x0000000000000001ULL << i_nBitNumber);
}

template<unsigned int NLength>
void MultiWordIntegerBitwiseNOTInPlace(
	array<uint64_t, NLength> & io_anA)
{
	for (unsigned int nWord = 0U;
		nWord < NLength;
		++nWord)
	{
		io_anA[nWord] = ~io_anA[nWord];
	}
}

template<unsigned int NLength>
array<uintmax_t, NLength> MultiWordIntegerBitwiseXOR(
	const array<uintmax_t, NLength> & i_anA,
	const array<uintmax_t, NLength> & i_anB)
{
	array<uintmax_t, NBlockWordCount> anReturnValue = i_anA;
	MultiWordIntegerBitwiseXORInPlace(anReturnValue, i_anB);

	return anReturnValue;
}

template<unsigned int NLength>
void MultiWordIntegerBitwiseXORInPlace(
	array<uintmax_t, NLength> & io_anA, //aliasing
	const array<uintmax_t, NLength> & i_anB)
{
	for(unsigned int nWord = 0U;
		nWord < NBlockWordCount;
		++nWord)
	{
		io_anA[nWord] ^= i_anB[nWord];
	}
}

template<unsigned int NLength>
void MultiWordIntegerBitwiseANDInPlace(
	array<uintmax_t, NLength> & io_anA, //aliasing
	const array<uintmax_t, NLength> & i_anB)
{
	for(unsigned int nWord = 0U;
		nWord < NBlockWordCount;
		++nWord)
	{
		io_anA[nWord] &= i_anB[nWord];
	}
}

template<unsigned int NLength>
array<uintmax_t, NLength> MultiWordIntegerBitwiseAND(
	const array<uintmax_t, NLength> & i_anA,
	const array<uintmax_t, NLength> & i_anB)
{
	array<uintmax_t, NBlockWordCount> anReturnValue = i_anA;
	MultiWordIntegerBitwiseANDInPlace(anReturnValue, i_anB);

	return anReturnValue;
}

template<unsigned int NLength>
void MultiWordIntegerBitwiseORInPlace(
	array<uintmax_t, NLength> & io_anA, //aliasing
	const array<uintmax_t, NLength> & i_anB)
{
	for(unsigned int nWord = 0U;
		nWord < NBlockWordCount;
		++nWord)
	{
		io_anA[nWord] |= i_anB[nWord];
	}
}

template<unsigned int NLength>
array<uintmax_t, NLength> MultiWordIntegerBitwiseOR(
	const array<uintmax_t, NLength> & i_anA,
	const array<uintmax_t, NLength> & i_anB)
{
	array<uintmax_t, NBlockWordCount> anReturnValue = i_anA;
	MultiWordIntegerBitwiseORInPlace(anReturnValue, i_anB);

	return anReturnValue;
}

#endif //JTH_BIGBITARRAY_H