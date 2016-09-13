#include "HomerCryptoHeaderOnlyAPI.h"

#include "HomerCryptoAPI.h"

int EncipherBufferA(
	const ProgramOptions & i_oOptions,
	const char * i_pInputBuffer,
	size_t i_nInputSize,
	char * o_pOutputBuffer, 
	size_t i_nOutputSize, 
	const char * i_pKey,
	size_t i_nKeyBufferSize)
{
	return EncipherBufferH(
		i_oOptions,
		i_pInputBuffer,
		i_nInputSize,
		o_pOutputBuffer, 
		i_nOutputSize, 
		i_pKey,
		i_nKeyBufferSize);
}

int EncipherFileA(
	const string & i_strInputFile,
	const string & i_strOutputFile,
	const ProgramOptions & i_oOptions,
	const string & i_strKeyFile,
	size_t & io_nOriginalFileSize)
{
	return EncipherFileH(
		i_strInputFile,
		i_strOutputFile,
		i_oOptions,
		i_strKeyFile,
		io_nOriginalFileSize);
}

//Returns zero if there was an error with the options.
size_t EvalKeyBufferSizeA(
	const ProgramOptions & i_oOptions)
{
	return EvalKeyBufferSizeH(i_oOptions);
}

size_t EvalSizeAfterPaddingA(
	const ProgramOptions & i_oOptions, 
	size_t i_nBufferSize)
{
	return EvalSizeAfterPaddingH(
		i_oOptions, 
		i_nBufferSize);
}

int GenerateSymmetricKeyA(
	const ProgramOptions & i_oOptions,
	uint64_t i_nSeed,
	char * o_pKeyBuffer,
	size_t i_nKeyBufferSize)
{
	return GenerateSymmetricKeyH(
		i_oOptions,
		i_nSeed,
		o_pKeyBuffer,
		i_nKeyBufferSize);
}

int GenerateAsymmetricKeyPairA(
	const ProgramOptions & i_oOptions,
	uint64_t i_nSeed,
	char * o_pPublicKeyBuffer,
	size_t i_nPublicKeyBufferSize,
	char * o_pPrivateKeyBuffer,
	size_t i_nPrivateKeyBufferSize)
{
	return GenerateAsymmetricKeyPairH(
		i_oOptions,
		i_nSeed,
		o_pPublicKeyBuffer,
		i_nPublicKeyBufferSize,
		o_pPrivateKeyBuffer,
		i_nPrivateKeyBufferSize);
}