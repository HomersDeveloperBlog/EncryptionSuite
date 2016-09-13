#pragma once

#include "BigBitArray.h"

template<unsigned int NLength>
uint64_t MultiWordIntegerAddOneInPlace(
	array<uint64_t, NLength> & io_anA)
{
	++io_anA[0U];
	
	unsigned int nWord = 0U;
	for(; nWord < NLength - 1U && !io_anA[nWord];) 
	{
		++(io_anA[++nWord]);
	}

	return (nWord == NLength - 1U
		&& !io_anA[nWord]);
}

INLINE uint64_t SingleWordIntegerAddWithCarryInPlace(
	uint64_t & io_nA, 
	const uint64_t i_nB)
{
	io_nA += i_nB;
	return (io_nA < i_nB); //Return carry
}

template<unsigned int NLength>
uint64_t MultiWordIntegerAddInPlace(
	array<uint64_t, NLength> & io_anA, //%aliasing
	const array<uint64_t, NLength> & i_anB)
{
	uint64_t nCarry = 0U;
	for(unsigned int nWord = 0U;
		nWord < NLength;
		++nWord)
	{
		uint64_t nPartialSummand = i_anB[nWord] + nCarry;
		io_anA[nWord] += nPartialSummand;
		nCarry = (nPartialSummand < i_anB[nWord])
			|| (io_anA[nWord] < nPartialSummand);
	}

	return nCarry;
}

template<unsigned int NLength>
void MultiWordIntegerNegateInPlace(
	array<uint64_t, NLength> & io_anA)
{
	MultiWordIntegerBitwiseNOTInPlace(io_anA);
	MultiWordIntegerAddOneInPlace(io_anA);
}

//In the process of changing it to be 1 if B > A
//Return 0 if B > A, 1 otherwise.
template<unsigned int NLength>
uint64_t MultiWordIntegerSubtractInPlace(
	array<uint64_t, NLength> & io_anA, 
	array<uint64_t, NLength> i_anB) //Local copy
{
	MultiWordIntegerNegateInPlace(i_anB);
	return !MultiWordIntegerAddInPlace(io_anA, i_anB);
}

//Try Kochanski multiplication. It may be linear in the word counts; ours is quadratic.
//Try using carry-save adders to avoid O(n) carry propagation (mostly branchmaggedon)

INLINE tuple<uint64_t, uint64_t> SingleWordIntegerMultiplyIntoDoubleWord(
	uint64_t i_nA,
	uint64_t i_nB)
{
	uint64_t anA_High = i_nA >> 32U;
	uint64_t anB_High = i_nB >> 32U;
	uint64_t anA_Low = i_nA & 0x00000000FFFFFFFF; //(UINT64_MAX >> 32)
	uint64_t anB_Low = i_nB & 0x00000000FFFFFFFF;

	//Compute middle "word", which is split between high and low.
	uint64_t nLocalResult_Mid = anA_High * anB_Low;
	uint64_t nCarry = SingleWordIntegerAddWithCarryInPlace(nLocalResult_Mid, anA_Low * anB_High);
	nCarry <<= 32U;

	//Compute low 64 bits of 128 bit result
	uint64_t nLocalResult_Low = anA_Low * anB_Low;
	nCarry += SingleWordIntegerAddWithCarryInPlace(nLocalResult_Low, (nLocalResult_Mid << 32U));

	//Compute high 64 bits of 128 bit result
	uint64_t nLocalResult_High = anA_High * anB_High + (nLocalResult_Mid >> 32U) + nCarry; //Can't overflow; 

	return make_tuple(nLocalResult_High, nLocalResult_Low);
}

//The second carry can overflow if the inner loop executes more times than our word size.
template<unsigned int NLength>
array<uint64_t, 2U * NLength> MultiWordIntegerMultiply(
	const array<uint64_t, NLength> & i_anA,
	const array<uint64_t, NLength> & i_anB)
{
	array<uint64_t, 2U * NLength> anReturnValue;

	uint64_t nBase = 0ULL;
	uint64_t nCarry = 0ULL;
	uint64_t nSecondCarry = 0ULL;

	unsigned int nOutputIndex = 0U;
	for(; nOutputIndex < NLength;
		++nOutputIndex)
	{
		//Perform carry rotation
		nBase = nCarry; //%can we refactor this into a word rotate?
		nCarry = nSecondCarry; //%just refactor this into index arithmetic. zero all before loop. increment index to do all this.
		nSecondCarry = 0ULL;

		//Perform convolution
		for(unsigned int nKernelIndex = 0U;
			nKernelIndex <= nOutputIndex; //Prevent index underflow in B, where entries would be 0 anyway
			++nKernelIndex)
		{
			//Compute anReturnValue[nOutputIndex] += i_anA[nKernelIndex] * i_anB[nOutputIndex - nKernelIndex];
			uint64_t nLocalResult_High, nLocalResult_Low;
			tie(nLocalResult_High, nLocalResult_Low) = SingleWordIntegerMultiplyIntoDoubleWord( //%make this template specialization of current function?
				i_anA[nKernelIndex], 
				i_anB[nOutputIndex - nKernelIndex]);

			//Accumulate onto output and carrys
			assert(nSecondCarry < UINT64_MAX - 1U); //Second carry will not overflow

			nBase += nLocalResult_Low; //%can we refactor this into a multiword add?
			nCarry += (nBase < nLocalResult_Low);
			nSecondCarry += (nCarry < static_cast<uint64_t>(nBase < nLocalResult_Low));
			nCarry += nLocalResult_High;
			nSecondCarry += (nCarry < nLocalResult_High);
		}

		//Assign output; perform carry rotation
		anReturnValue[nOutputIndex] = nBase;
	}

	for(; nOutputIndex < 2U * NLength - 1U;
		++nOutputIndex)
	{
		//Perform carry rotation
		nBase = nCarry; //%can we refactor this into a word rotate?
		nCarry = nSecondCarry;
		nSecondCarry = 0ULL;

		//Perform convolution
		for(unsigned int nKernelIndex = nOutputIndex - NLength + 1U; //Skip the entries where A is zero.
			nKernelIndex < NLength; //Prevent index overflow in A, where entries would be 0 anyway
			++nKernelIndex)
		{
			//Compute anReturnValue[nOutputIndex] += i_anA[nKernelIndex] * i_anB[nOutputIndex - nKernelIndex];
			uint64_t nLocalResult_High, nLocalResult_Low;
			tie(nLocalResult_High, nLocalResult_Low) = SingleWordIntegerMultiplyIntoDoubleWord(
				i_anA[nKernelIndex], 
				i_anB[nOutputIndex - nKernelIndex]);

			//Accumulate onto output and carrys
			assert(nSecondCarry < UINT64_MAX - 1U); //Second carry will not overflow

			nBase += nLocalResult_Low; //%can we refactor this into a multiword add?
			nCarry += (nBase < nLocalResult_Low);
			nSecondCarry += (nCarry < static_cast<uint64_t>(nBase < nLocalResult_Low)); //% -_-
			nCarry += nLocalResult_High;
			nSecondCarry += (nCarry < nLocalResult_High);
		}

		//Assign output
		anReturnValue[nOutputIndex] = nBase;
	}

	//The final place is just the carry from the second highest. 
	anReturnValue[nOutputIndex] = nCarry;
	assert(!nSecondCarry); //%second carry is guarunteed 0?

	return anReturnValue;
}

template<unsigned int NLength>
tuple<array<uint64_t, NLength>, array<uint64_t, NLength>> MultiWordIntegerDivision(
	array<uint64_t, NLength> i_anA,
	array<uint64_t, NLength> i_anB) //Local copy
{
	assert(!EvalLexographicEqualToZero(i_anB)); //undefined behaviour if i_anB == 0;

	//Extract most significant places (word and bit)
	unsigned int nMSWord_A, nMSBit_A, nMSWord_B, nMSBit_B;
	tie(nMSWord_A, nMSBit_A) = EvalMostSignificantBit(i_anA);
	tie(nMSWord_B, nMSBit_B) = EvalMostSignificantBit(i_anB);

	//Handle negative shift requirement (would mean B > A, so no reduction necessary)
	if((nMSWord_A < nMSWord_B)
		|| (nMSWord_A == nMSWord_B && nMSBit_A < nMSBit_B)
		|| (nMSWord_A == NLength && nMSBit_A == 64U)
		|| (nMSWord_B == NLength && nMSBit_B == 64U))
	{
		array<uint64_t, NLength> nQ; 
		MultiWordIntegerFillWithZeros(nQ);

		return make_tuple(nQ, i_anA);
	}

	//Compute alignment shift
	unsigned int nWordShiftCount = nMSWord_A - nMSWord_B;
	unsigned int nBitShiftCount = nMSBit_A - nMSBit_B;
	if(nMSBit_A < nMSBit_B)
	{
		--nWordShiftCount;
		nBitShiftCount += 64U;
	}

	//Align B with A, initialize quotient
	array<uint64_t, NLength> nQ; 
	MultiWordIntegerFillWithZeros(nQ);
	array<uint64_t, NLength> nB_Shifted = i_anB;
	MultiWordIntegerRightShiftInPlace(
		nB_Shifted,
		nWordShiftCount,
		nBitShiftCount);

	//Reduce A by B, bit by bit.
	size_t nIterationCount = 64U * (nWordShiftCount) + nBitShiftCount + 1U;
	for(size_t nIteration = 0U;
		nIteration < nIterationCount;
		++nIteration)
	{
		if(EvalLexographicGreaterThanEqualTo(i_anA, nB_Shifted))
		{
			MultiWordIntegerSubtractInPlace(i_anA, nB_Shifted);
			MultiWordIntegerSetBitInPlace(nQ, nIterationCount - (nIteration + 1U));
		}

		MultiWordIntegerLeftShiftByOneBitInPlace(nB_Shifted);
	}

	return make_tuple(nQ, i_anA);
}

template<unsigned int NLength>
void MultiWordIntegerModularReductionInPlace(
	array<uint64_t, NLength> & io_anA,
	const array<uint64_t, NLength> & i_anB)
{
	tie(ignore, io_anA) = MultiWordIntegerDivision(io_anA, i_anB);
}

template<unsigned int NLength>
array<uint64_t, NLength> MultiWordIntegerModularReduction(
	const array<uint64_t, NLength> & i_anA,
	const array<uint64_t, NLength> & i_anB)
{
	array<uint64_t, NLength> anReducedA(i_anA);
	MultiWordIntegerModularReductionInPlace(
		anReducedA,
		i_anB);

	return anReducedA;
}

//TODO:
//Need alias control
//Need to ensure all index unsigned ints in control logic cannot overflow.