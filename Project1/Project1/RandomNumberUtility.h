#pragma once

#include <random>

template<unsigned int NLength, typename FGenerator>
array<uint64_t, NLength> GenerateRandomBigUInt(
	uint64_t & io_nSeed)
{
	mt19937_64 fGenerator(io_nSeed);
	array<uint64_t, NLength> nP = GenerateRandomBigOddUInt<NLength>(fGenerator);
	io_nSeed = fGenerator();

	return nP;
}

template<unsigned int NLength, typename FGenerator>
array<uint64_t, NLength> GenerateRandomBigUInt(
	const FGenerator & i_fGenerator)
{
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
	uint64_t & io_nSeed)
{
	array<uint64_t, NLength> nReturnValue
	 = GenerateRandomBigUInt<NLength>(io_nSeed);
	anReturnValue[0U] |= 0x0000000000000001ULL; //Assign lsb to 1

	return anReturnValue;
}

template<unsigned int NLength, typename FGenerator>
array<uint64_t, NLength> GenerateRandomBigEvenUInt(
	uint64_t & io_nSeed)
{
	array<uint64_t, NLength> nReturnValue
	 = GenerateRandomBigUInt<NLength>(io_nSeed);
	anReturnValue[0U] &= 0xfffffffffffffffeULL; //Assign lsb to 0

	return anReturnValue;
}

inline void FillRandomBuffer(
	uint64_t & io_nSeed,
	char * o_pBuffer,
	size_t i_nBufferSize)
{
	mt19937_64 fGenerator(io_nSeed);

	size_t nWordSize = sizeof(uint64_t);
	size_t nWordCount = i_nBufferSize / nWordSize;
	for(size_t nWord = 0U;
		nWord < nWordCount;
		nWord++)
	{
		o_pBuffer[nWord * nWordSize] = fGenerator();
	}

	size_t nCharacterRemainderCount = i_nBufferSize % nWordSize;
	if(nCharacterRemainderCount)
	{
		uint64_t nRandomWord = fGenerator();
		memcpy_s(
			o_pBuffer + nWord * nWordCount,
			nCharacterRemainderCount,
			&nRandomWord,
			nCharacterRemainderCount);
	}

	io_nSeed = fGenerator();
}