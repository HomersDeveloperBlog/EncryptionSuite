#pragma once

#include "LinkerMacros.h"
#include "ProgramOptions.h"

JTH_DLL_EXPORT int EncipherBufferA(
	const ProgramOptions & i_oOptions,
	const char * i_pInputBuffer,
	size_t i_nInputSize,
	char * o_pOutputBuffer, 
	size_t i_nOutputSize, 
	const char * i_pKey,
	size_t i_nKeyBufferSize);

JTH_DLL_EXPORT int EncipherFileA(
	const string & i_strInputFile,
	const string & i_strOutputFile,
	const ProgramOptions & i_oOptions,
	const string & i_strKeyFile,
	size_t & io_nOriginalFileSize);

JTH_DLL_EXPORT size_t EvalKeyBufferSizeA(
	const ProgramOptions & i_oOptions);

JTH_DLL_EXPORT size_t EvalSizeAfterPaddingA(
	const ProgramOptions & i_oOptions, 
	size_t i_nBufferSize);

JTH_DLL_EXPORT int GenerateSymmetricKeyA(
	const ProgramOptions & i_oOptions,
	uint64_t i_nSeed,
	char * o_pKeyBuffer,
	size_t i_nKeyBufferSize);

JTH_DLL_EXPORT int GenerateAsymmetricKeyPairA(
	const ProgramOptions & i_oOptions,
	uint64_t i_nSeed,
	char * o_pPublicKeyBuffer,
	size_t i_nPublicKeyBufferSize,
	char * o_pPrivateKeyBuffer,
	size_t i_nPrivateKeyBufferSize);