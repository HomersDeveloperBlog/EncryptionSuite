//Derive from bigbitarray and implement using bitwise operations

#ifndef JTH_BIGGALOISFIELD_H
#define JTH_BIGGALOISFIELD_H

#include "BigBitArray.h"

template<unsigned int NLength>
void DoGaloisFieldMultiplication(
	const array<uint64_t, NLength> & i_anA,
	const array<uint64_t, NLength> & i_anB, //%need a reduction polynomial
	array<uint64_t, NLength> & o_anC) //%aliasing
{

}

template<unsigned int NLength>
void DoGaloisFieldAddition(
	const array<uint64_t, NLength> & i_anA,
	const array<uint64_t, NLength> & i_anB,
	array<uint64_t, NLength> & o_anC) //%aliasing
{
	for(unsigned int nWord = 0U;
		nWord < NLength;
		++nWord)
	{
		o_anC[nWord] = i_anB[nWord] ^ i_an[nWord]; 
	}
}

#endif //#ifndef JTH_BIGGALOISFIELD_H