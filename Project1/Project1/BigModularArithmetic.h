#pragma once

#include "BigUnsignedInteger.h"
#include "BigSignedInteger.h"

//A = (A * B) % C
template<unsigned int NLength>
array<uint64_t, NLength> MultiWordIntegerModularMultiplication(
	const array<uint64_t, NLength> & i_anA,
	const array<uint64_t, NLength> & i_anB,
	const array<uint64_t, NLength> & i_anC)
{
	return MultiWordIntegerChangeLength<NLength>(
		MultiWordIntegerModularReduction(
			MultiWordIntegerMultiply(i_anA, i_anB),
			MultiWordIntegerChangeLength<2U * NLength>(i_anC)));
}

//A = (A * B) % C
template<unsigned int NLength>
void MultiWordIntegerModularMultiplicationInPlace(
	array<uint64_t, NLength> & io_anA,
	const array<uint64_t, NLength> & i_anB,
	const array<uint64_t, NLength> & i_anC)
{
	io_anA = MultiWordIntegerModularMultiplication(
		io_anA,
		i_anB,
		i_anC);
}

//%wrap the core routine ("safety-off") version in the error handling, and use as default.
template<unsigned int NLength>
void MultiWordIntegerModularExponentiationInPlace(
	array<uint64_t, NLength> & io_anA, //Base. //%Aliasing.
	array<uint64_t, NLength> i_anB, //Exponent. Local copy.
	const array<uint64_t, NLength> & i_anC) //Modulus. Undefined for zero modulus.
{
	//Handle zero modulus. Input is not modified.
	assert(!EvalLexographicEqualToZero(i_anC));
	if(EvalLexographicEqualToZero(i_anC))
	{
		return;
	}

	//Handle zero base. Answer is zero (do nothing; return).
	MultiWordIntegerModularReductionInPlace(io_anA, i_anC);
	if(EvalLexographicEqualToZero(io_anA))
	{
		return;
	}

	//Initialize exponentiation. Copy A then assign A to one.
	array<uint64_t, NLength> anCurrentBitBasePower = io_anA;
	MultiWordIntegerFillWithZeros(io_anA); //Set io_anA to 1U. //%need constant initialization functions. UINT_MAX and 1.
	MultiWordIntegerAddOneInPlace(io_anA); //%need to do this better. Use assignment to first 1 bit power?

	//Handle zero exponent, Answer is defined to be one.
	MultiWordIntegerModularReductionInPlace(i_anB, i_anC);
	if(EvalLexographicEqualToZero(i_anB))
	{
		MultiWordIntegerFillWithZeros(io_anA);
		return;
	}
	
	for(unsigned int nCurrentWord = 0U;
		nCurrentWord < NLength;
		++nCurrentWord)
	{
		for(uint64_t nCurrentBitMask = 1ULL;
			nCurrentBitMask; //Once the last bit falls of the end of the word
			nCurrentBitMask <<= 1U)
		{
			//If current position is set in the exponent, multiply A by our working base power.
			if(i_anB[nCurrentWord] & nCurrentBitMask)
			{
				MultiWordIntegerModularMultiplicationInPlace(
					io_anA, 
					anCurrentBitBasePower, 
					i_anC);
			}

			//Square anWorkingBase, reduce the result modulo C
			MultiWordIntegerModularMultiplicationInPlace( //%squaring version would be nice
				anCurrentBitBasePower,
				anCurrentBitBasePower,
				i_anC);
		}
	}
}

//%this needs to be fixed up. 
uint64_t SingleWordIntegerGreatestCommonDivisor(
	uint64_t i_nA, 
	uint64_t i_nB) //Undefined behaviour for zero modulus.
{
	//assert(i_nA >= i_nB);
	if(i_nA < i_nB)
	{ //%is this really necessary. won't the first round swap?
		uint64_t nSwap = i_nA;
		i_nA = i_nB;
		i_nB = nSwap;
	}

	uint64_t nR_Last = i_nA;
	uint64_t nR_Current = i_nB;
	while(nR_Current)
	{
		uint64_t nR_Next = nR_Last % nR_Current;
		nR_Last = nR_Current;
		nR_Current = nR_Next;
	}

	return nR_Last;
}

//Note this can take as many as 98 iterations per 64-bit word, but on average about 37. 
template<unsigned int NLength>
array<uint64_t, NLength> MultiWordIntegerGreatestCommonDivisor(
	const array<uint64_t, NLength> & i_nA,
	const array<uint64_t, NLength> & i_nB)
{
	assert(EvalLexographicGreaterThanEqualTo(i_nA, i_nB)); //%Make "safety off" version
	if(EvalLexographicLessThan(i_nA, i_nB))
	{
		const array<uint64_t, NLength> nSwap = i_nA;
		i_nA = i_nB;
		i_nB = nSwap;
	}

	//Euclid's algorithm
	array<uint64_t, NLength> nR_Last = i_nA; //%do swapping here at assignment so i dont' need a local copy?
	array<uint64_t, NLength> nR_Current = i_nB;
	if(!EvalLexographicEqualToZero(nR_Current)) //Will cause us to return 0 B is zero.
	{
		while(EvalLexographicEqualToZero(nR_Last))
		{
			const array<uint64_t, NLength> nR_Next = MultiWordIntegerModularReduction(nR_Last, nR_Current);
			nR_Last = nR_Current;
			nR_Current = nR_Next;
		}
	}

	return nR_Last;
}

tuple<int64_t, int64_t, int64_t> SingleWordIntegerExtendedEuclideanAlgorithm(
	const int64_t i_nA,
	const int64_t i_nB)
{
	if(!!(i_nA % i_nB))
	{
		int64_t nGCD, nX, nY;
		tie(nGCD, nX, nY) = SingleWordIntegerExtendedEuclideanAlgorithm(
			i_nB, 
			i_nA % i_nB);

		return make_tuple(nGCD, nY, nX - nY * (i_nA / i_nB));
	}
	else
    {
		return make_tuple(i_nB, 0LL, 1LL);
    }
}

int64_t SingleWordIntegerModularInverse(
	const int64_t i_nA, 
	const int64_t i_nModulus)
{
	int64_t nGCD, nX, nY;
	tie(nGCD, nX, nY) = SingleWordIntegerExtendedEuclideanAlgorithm(
		i_nA,
		i_nModulus);
    assert(nGCD == 1LL);

	if(nX < 0) 
	{ 
		nX += i_nModulus;
	}
	assert(nX > 0);

    return nX;
}

template<unsigned int NLength>
tuple<
	array<uint64_t, NLength>, 
	tuple<bool, array<uint64_t, NLength>>, 
	tuple<bool, array<uint64_t, NLength>>>
MultiWordIntegerExtendedEuclideanAlgorithm(
	const array<uint64_t, NLength> i_nA,
	const array<uint64_t, NLength> i_nB)
{
	array<uint64_t, NLength> nADivB, nAModB;
	tie(nADivB, nAModB) = MultiWordIntegerDivision(i_nA, i_nB);

	if(!EvalLexographicEqualToZero(nAModB))
	{
		array<uint64_t, NLength> nGCD;
		tuple<bool, array<uint64_t, NLength>> nX, nY;
		tie(nGCD, nX, nY) = MultiWordIntegerExtendedEuclideanAlgorithm(
			i_nB,
			nAModB);

		SignedMultiWordIntegerSubtractWithCarryInPlace(
				nX,
				make_tuple(
					get<SIGN_BIT>(nY),
					MultiWordIntegerChangeLength<NLength>( 
						MultiWordIntegerMultiply( //Overflow should not be possible.
							get<UINT_PART>(nY), 
							nADivB))));

		return make_tuple(nGCD, nY, nX);
	}
	else
    {
		return make_tuple(
			i_nB, 
			make_tuple(
				POSITIVE_SIGN_BIT, 
				MultiWordIntegerChangeLength<NLength>(0ULL)), 
			make_tuple(
				POSITIVE_SIGN_BIT, 
				MultiWordIntegerChangeLength<NLength>(1ULL)));
    }
}

template<unsigned int NLength>
array<uint64_t, NLength> MultiWordIntegerModularInverse(
	const array<uint64_t, NLength> & i_nA, 
	const array<uint64_t, NLength> & i_nModulus)
{
	array<uint64_t, NLength> nGCD;
	tuple<bool, array<uint64_t, NLength>> nX, nY;
    tie(nGCD, nX, nY) = MultiWordIntegerExtendedEuclideanAlgorithm(
		i_nA,
		i_nModulus);

	const array<uint64_t, NLength> nGCDAnswer = {0x1ULL}; //%use temporary from uint constructor
    assert(EvalLexographicEqualTo(nGCD, nGCDAnswer)); 

	if(get<SIGN_BIT>(nX) == NEGATIVE_SIGN_BIT)
	{
		uint64_t nCarry = SignedMultiWordIntegerAddWithCarryInPlace(
			nX, 
			make_tuple(static_cast<bool>(POSITIVE_SIGN_BIT), i_nModulus));
		assert(!nCarry);
	}
	assert(get<SIGN_BIT>(nX) == POSITIVE_SIGN_BIT);

    return get<UINT_PART>(nX);
}

//Note this can take as many as 98 iterations per 64-bit word, but on average about 37. 
//template<unsigned int NLength>
//array<array<uint64_t, NLength>, 3U> MultiWordIntegerBezoutNumbers(
//	const array<uint64_t, NLength> & i_nA,
//	const array<uint64_t, NLength> & i_nB)
//{
//	assert(EvalLexographicGreaterThanEqualTo(i_nA, i_nB));
//	array<array<uint64_t, NLength>, 98*NLength> anQuotientStack; 
//	//%its almost certainly easier to do this in tail code to a recursive version.
//	//%since the numbers will get smaller, we don't need all this space,
//	//%we can eventually use arithmetic on smaller integers then to save time.
//
//	//Euclid's algorithm
//	array<uint64_t, NLength> nR_Last = i_nA;
//	array<uint64_t, NLength> nR_Current = i_nB;
//	while(!EvalLexographicEqualToZero(nR_Last))
//	{
//		const array<uint64_t, NLength> nR_Next = MultiWordIntegerModularReduction(nR_Last, nR_Current);
//		const array<uint64_t, NLength> nQ = (nR_Last - nR_Next) / nRCurrent; //%We could get modulus from quotient, but not vice versa.
//		
//		nR_Last = nR_Current;
//		nR_Current = nR_Next;
//
//		assert(nIterationCount < 98*NLength); //%pushback
//		anQuotientStack[nIterationCount] = nQ;
//		nIterationCount++;
//	}
//
//	array<uint64_t, NLength> nY = MultiWordIntegerFillWithZeros();
//	array<uint64_t, NLength> nX = MultiWordIntegerConstantOne();
//	for(int nQuotient = nIterationCount - 2U;
//		nQuotient > 0;
//		--nQuotient)
//	{//%we cannot do negative numbers with arithmetic
//		const array<uint64_t, NLength> nX_Next = nY - anQuotientStack[nQuotient] * nX;
//		nY = nX;
//		nX = nX_Next;
//	}
//
//	return make_tuple(nR_Last, nX, nY); //GCD, A bezout, B bezout (inv(A) mod B)
//}

////http://mathforum.org/library/drmath/view/51675.html
////This is apparently a backtrack free implementation 
//uint64_t SingleWordIntegerModularInverse(
//	const uint64_t i_nA,
//	const uint64_t i_nB) //%modulus
//{
//	assert(i_nB >= i_nA);
//	//Handle A == 0, B == 0, A > B;
//
//	uint64_t nR_Last = i_nB;
//	uint64_t nR_Current = i_nA;
//
//	uint64_t nX_Last = 0ULL;
//	uint64_t nX_Current = 1ULL;
//
//	while(nR_Current)
//	{
//		//Bezout numbers (not really though)
//		const uint64_t nQ = nR_Last / nR_Current;
//		const uint64_t nX_Next = nQ * nX_Current + nX_Last; //%overflow
//		nX_Last = nX_Current;
//		nX_Current = nX_Next;
//
//		//GCD remainders
//		const uint64_t nR_Next = nR_Last % nR_Current;
//		nR_Last = nR_Current;
//		nR_Current = nR_Next;
//	}
//
//	//Verify that GCD = 1; A^-1 < B
//	assert(nR_Last == 1ULL); //%if not, there are multiple inverses
//	assert(nX_Last < i_nB); //%modular reduction should not be necessary
//
//	//Inverse is nX_Last
//	return nX_Last;
//}

//template<unsigned int NLength>
//array<uint64_t, NLength> MultiWordIntegerModularInverse(
//	const array<uint64_t, NLength> & i_nA,
//	const array<uint64_t, NLength> & i_nB) //%modulus
//{
//	assert(EvalLexographicGreaterThanEqualTo(i_nA, i_nB)); //%for inverse problem, this is reversed.
//	//Handle A==0, B==0;
//
//	array<uint64_t, NLength> nR_Last = i_nA; //%for inverse problem, this is reversed.
//	array<uint64_t, NLength> nR_Current = i_nB;
//
//	array<uint64_t, NLength> nX_Last = MultiWordIntegerFillWithZeros();
//	array<uint64_t, NLength> nX_Current = MultiWordIntegerConstantOne();
//
//	while(!EvalLexographicEqualToZero(nR_Current))
//	{
//		const array<uint64_t, NLength> nQ = MultiWordIntegerDivision(nR_Last, nR_Current); //%we need quotient
//		const array<uint64_t, NLength> nX_Next = MultiWordIntegerChangeLength MultiWordIntegerMultiply(nQ, nX_Current) + nX_Last; //%check overflow
//		nX_Last = nX_Current;
//		nX_Current = nX_Next;
//
//		const array<uint64_t, NLength> nR_Next = MultiWordIntegerModularReduction(nR_Last, nR_Current); //%We could get modulus from quotient, but not vice versa.
//		nR_Last = nR_Current;
//		nR_Current = nR_Next;
//	}
//
//	//Verify that GCD = 1; A^-1 < B
//	assert(nR_Last == MultiWordIntegerConstantOne());
//	assert(EvalLexographicLessThan(nX_Last, i_nB));
//
//	//Inverse is nX_Last
//	return nX_Last;
//}