#pragma once

#include "FileLoader.h"
#include "RandomNumberUtility.h"
#include "FileCipher.h"
#include "BufferCipher.h"
#include "BlockLoader.h"
#include "CryptoKey.h"

inline int EncipherBufferH(
	const ProgramOptions & i_oOptions,
	const char * i_pInputBuffer,
	size_t i_nInputSize,
	char * o_pOutputBuffer, 
	size_t i_nOutputSize, 
	const char * i_pKey,
	size_t i_nKeyBufferSize)
{
	return EncipherBuffer(
		i_oOptions,
		i_pInputBuffer,
		i_nInputSize,
		o_pOutputBuffer, 
		i_nOutputSize, 
		i_pKey,
		i_nKeyBufferSize);
}

inline int EncipherFileH(
	const string & i_strInputFile,
	const string & i_strOutputFile,
	const ProgramOptions & i_oOptions,
	const string & i_strKeyFile,
	size_t & io_nOriginalFileSize)
{
	return EncipherFile(
		i_strInputFile,
		i_strOutputFile,
		i_oOptions,
		i_strKeyFile,
		io_nOriginalFileSize);
}

inline size_t EvalSizeAfterPaddingH(
	const ProgramOptions & i_oOptions, 
	size_t i_nBufferSize)
{
	return EvalSizeAfterPadding(
		i_oOptions, 
		i_nBufferSize);
}

inline size_t EvalKeyBufferSizeH(
	const ProgramOptions & i_oOptions)
{
	return EvalKeyBufferSize(i_oOptions);
}

inline int GenerateSymmetricKeyH(
	const ProgramOptions & i_oOptions,
	uint64_t i_nSeed,
	char * o_pKeyBuffer,
	size_t i_nKeyBufferSize)
{
	return GenerateSymmetricKey(
		i_oOptions,
		i_nSeed,
		o_pKeyBuffer,
		i_nKeyBufferSize);
}

inline int GenerateAsymmetricKeyPairH(
	const ProgramOptions & i_oOptions,
	uint64_t i_nSeed,
	char * o_pPublicKeyBuffer,
	size_t i_nPublicKeyBufferSize,
	char * o_pPrivateKeyBuffer,
	size_t i_nPrivateKeyBufferSize)
{
	return GenerateAsymmetricKeyPair(
		i_oOptions,
		i_nSeed,
		o_pPublicKeyBuffer,
		i_nPublicKeyBufferSize,
		o_pPrivateKeyBuffer,
		i_nPrivateKeyBufferSize);
}