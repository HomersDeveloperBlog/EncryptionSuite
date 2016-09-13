#pragma once

#include "BigUnsignedInteger.h"

enum SignedIntegerPart : uint8_t 
{
	SIGN_BIT = 0U, UINT_PART, 
	SIGNED_INTEGER_PART_COUNT
};

const bool POSITIVE_SIGN_BIT = false;
const bool NEGATIVE_SIGN_BIT = true;

//sign of the overflow is indicated by the sign of the result.
INLINE uint64_t SignedSingleWordIntegerAddWithCarryInPlace(
	tuple<bool, uint64_t> & io_nA, 
	const tuple<bool, uint64_t> i_nB)
{
	uint64_t nCarry;
	if(get<SIGN_BIT>(io_nA) == get<SIGN_BIT>(i_nB))
	{//Integer signs were same sign
		nCarry = SingleWordIntegerAddWithCarryInPlace(
			get<UINT_PART>(io_nA), 
			get<UINT_PART>(i_nB));
	}
	else
	{
		nCarry = 0U; //Overflow ruled out
		if(get<UINT_PART>(i_nB) < get<UINT_PART>(io_nA))
		{ 
			get<UINT_PART>(io_nA) -= get<UINT_PART>(i_nB);
			//get<SIGN_BIT>(io_nA) = get<SIGN_BIT>(io_nA);
		}
		else
		{
			get<UINT_PART>(io_nA) = get<UINT_PART>(i_nB)
				- get<UINT_PART>(io_nA);
			get<SIGN_BIT>(io_nA) = get<SIGN_BIT>(i_nB);
		}
	}

	return nCarry;
}

//sign of the overflow is indicated by the sign of the result.
template<unsigned int NLength>
uint64_t SignedMultiWordIntegerAddWithCarryInPlace(
	tuple<bool, array<uint64_t, NLength>> & io_nA, 
	tuple<bool, array<uint64_t, NLength>> i_nB) //Local copy
{
	uint64_t nCarry;
	if(get<SIGN_BIT>(io_nA) == get<SIGN_BIT>(i_nB))
	{ //Integer signs were same sign
		nCarry = MultiWordIntegerAddInPlace(
			get<UINT_PART>(io_nA), 
			get<UINT_PART>(i_nB));
	}
	else
	{
		nCarry = 0U; //Overflow ruled out
		if(EvalLexographicLessThan(
			get<UINT_PART>(i_nB), 
			get<UINT_PART>(io_nA)))
		{
			uint64_t nReturnValue = MultiWordIntegerSubtractInPlace(
				get<UINT_PART>(io_nA), 
				get<UINT_PART>(i_nB));
			assert(!nReturnValue);
		}
		else
		{
			uint64_t nReturnValue = MultiWordIntegerSubtractInPlace(
				get<UINT_PART>(i_nB), 
				get<UINT_PART>(io_nA));
			assert(!nReturnValue);

			get<UINT_PART>(io_nA) = get<UINT_PART>(i_nB);
			get<SIGN_BIT>(io_nA) = get<SIGN_BIT>(i_nB);
		}
	}

	return nCarry;
}

INLINE void NegateSignBitInPlace(
	bool & io_bSignA)
{ //Toggle sign bit
	io_bSignA ^= NEGATIVE_SIGN_BIT; //^= true
}

INLINE bool NegateSignBit(
	bool io_bSignA)
{ //Toggle sign bit
	NegateSignBitInPlace(io_bSignA);
	return io_bSignA;
}

//Return 0 if B > A, 1 otherwise.
INLINE uint64_t SignedSingleWordIntegerSubtractWithCarryInPlace(
	tuple<bool, uint64_t> & io_nA, 
	const tuple<bool, uint64_t> i_nB) //local copy
{
	NegateSignBit(get<SIGN_BIT>(i_nB));

	return SignedSingleWordIntegerAddWithCarryInPlace(
		io_nA,
		i_nB);
}

//Return 0 if B > A, 1 otherwise.
template<unsigned int NLength>
uint64_t SignedMultiWordIntegerSubtractWithCarryInPlace(
	tuple<bool, array<uint64_t, NLength>> & io_nA, 
	const tuple<bool, array<uint64_t, NLength>> i_nB) //local copy
{
	NegateSignBit(get<SIGN_BIT>(i_nB));

	return SignedMultiWordIntegerAddWithCarryInPlace(
		io_nA,
		i_nB);
}