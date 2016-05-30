#ifndef JTH_PRIMENUMBERGENERATOR_H
#define JTH_PRIMENUMBERGENERATOR_H

#include <random>

template<unsigned int NLength, typename FGenerator>
array<uint64_t, NLength> GenerateRandomBigUInt(
	const FGenerator & i_fGenerator)
{
	//We need to set the range of these numbers to not include small numbers.
	//Or numbers with large/small parity
	//And has not been generated previously (this word)
	//Note that the std mersenne twister is unchecked code.
	//Require return type of generator to exactly return a full length int.
	//The generated RSA primes should not be the same.

	array<uint64_t, NLength> nBigRandomNumber;
	for(unsigned int nWord = 0U;
		nWord < NLength;
		nWord++)
	{
		nBigRandomNumber[nWord] = fGenerator();
	}
}

template<unsigned int NLength, typename FGenerator>
array<uint64_t, NLength> GenerateRandomBigOddUInt(
	const FGenerator & i_fGenerator)
{
	array<uint64_t, NLength> nReturnValue = GenerateRandomBigUInt<NLength>(i_fGenerator);
	anReturnValue[0U] |= 0x0000000000000001ULL; //Assign lsb to 1

	return anReturnValue;
}

template<unsigned int NLength, typename FGenerator>
array<uint64_t, NLength> GenerateRandomBigEvenUInt(
	const FGenerator & i_fGenerator)
{
	array<uint64_t, NLength> nReturnValue = GenerateRandomBigUInt<NLength>(i_fGenerator);
	anReturnValue[0U] &= 0xfffffffffffffffeULL; //Assign lsb to 0

	return anReturnValue;
}

//If p passes the test, it has at least 3/4 chance of being prime.
template<unsigned int NLength>
bool IsMillerRabinWitnessToPrime(
	const array<uint64_t, NLength> & i_nP,
	const array<uint64_t, NLength> & i_nPMinusOne,
	const array<uint64_t, NLength> & i_nN,
	const array<uint64_t, NLength> & i_nD
	const unsigned int i_nS)
{
	//n^d = 1 mod p (OR other test)
	array<uint64_t, NLength> nTestOne = EvalModularExponentiation(nN, nD, i_nP);
	if(EvalLexographicEqualsToOne(nTestOne))
	{
		return true; //Test pass, nN is witness to the primality of nP
	}
	
	//n^((2^j)*d) = -1 mod p , for some j in [0,s), you must test all until 1 passes
	for(unsigned int nJ = 0U;
		nJ < i_nS;
		++nJ)
	{
		unsigned int nWordShift = nJ / 64U;
		unsigned int nBitShift = nJ % 64U; //It is impossible for this to overflow (since it is less than s)
		array<uint64_t, NLength> nTwoPowerJTimesD = EvalRightShiftBits_Transform(
			nD, nWordShift, nBitShift);
		array<uint64_t, NLength> nTestTwo = EvalModularExponentiation_Transform(
			nN, nTwoPowerJTimesD, i_nP);
		
		if(EvalLexographicEquals(nTestTwo, nPMinusOne))
		{
			return true; //Test passes, nN is witness to the primality of nP
		}
	}

	return false;
}

template<unsigned int NLength>
bool IsMillerRabinPrime(
	const array<uint64_t, NLength> & i_nP,
	const unsigned int i_nWitnessThreshold)
{
	assert(!EvalLexographicEqualsToZero(i_nP));
	if(EvalLexographicLessThanEqualTo(1U)) 
	{//%should commit to > or >= for comparing against constants
		return false;
	}
	else if(EvalLexographicLessThanEqualTo(3U))
	{ //2 and 3 are prime.
		return true;
	}

	//Subtract one
	array<uint64_t, NLength> nPMinusOne = EvalSubtractionByOne(i_nP);

	//Find largest power of two dividing p-1 (by counting left zeroes)
	unsigned int nLSWord = 16U, nLSBit = 64U;
	tie(nLSWord, nLSBit) = EvalLeastSignificantBit(nPMinusOne); //%make sure this - 1 happens.
	assert(nLSBit != 64U && nLSWord != 16U); 
	unsigned int nS = nLSWord * 64U + nLSBit; //%confirm this will not overflow

	//Call this (2^s) * d = p - 1. Find d by shifting s bits over.
	array<uint64_t, 16U> nD = EvalLeftShiftBits(
		nPMinusOne, 
		nLSWord, 
		nLSBit);

	for(unsigned int nWitnessTest = 0U;
		nWitnessTest < i_nWitnessThreshold;
		++nWitnessTest)
	{
		array<uint64_t, NLength> nN = EvalModularReduction_InPlace(
			GenerateRandomBigUInt<NLength>(i_fGenerator), 
			i_nP); //%We should not repeat numbers, however unlikely.
		
		if(!IsMillerRabinWitnessToPrime(
			i_nP,
			nPMinusOne,
			nN,
			nD,
			nS))
		{
			return false;
		}
	}
	
	return true;
}

template<NLength>
array<uint64_t, NLength> GeneratePrime()
{
	//Generate a random number
	size_t nSeed = stable_clock.now(); //tied to time.
	mt19937_64 fGenerator(nSeed);

	//Generate candidate prime
	array<uint64_t, NLength> nP = GenerateRandomBigOddUInt<NLength>(i_fGenerator);

	for(unsigned int nPrimeAttempt = 0U;
		nPrimeAttempt < 1000U;
		++nPrimeAttempt)
	{
		if(IsMillerRabinPrime(nP, 50U))
		{
			return nP;
		}

		//If the test fails, we decrement p (by two) and try again.
		for(unsigned int nSub = 0U;
			nSub < 2U;
			++nSub)
		{
			EvalSubtractionByOne_InPlace(nP);
			assert(!EvalLexographicEqualToZero(nP)); //Check overflow while incrementing.
		}
	}

	//Failed to generate prime
	assert(false);
	return MultiWordIntegerConstantZero();
}

#endif JTH_PRIMENUMBERGENERATOR_H