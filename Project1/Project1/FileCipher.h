#pragma once

//%right now there is only an api call for this functionality
//%when I get a moment, I would like to make a FileCipher class
//%also the extra bytes due to padding are poorly handled

inline int EncipherFile(
	const string & i_strInputFile,
	const string & i_strOutputFile,
	const ProgramOptions & i_oOptions,
	const string & i_strKeyFile,
	size_t & io_nOriginalFileSize)
{
	//Load source key into buffer.
	size_t nExpectedKeySize = EvalKeyBufferSize(i_oOptions);
	size_t nKeyBufferSize = 0x0ULL;
	shared_ptr<char> pKeyBuffer = nullptr;
	tie(pKeyBuffer, nKeyBufferSize) = ReadFileIntoBuffer(i_strKeyFile);
	assert(pKeyBuffer && nKeyBufferSize == nExpectedKeySize);
	if(!pKeyBuffer.GET() 
		|| nKeyBufferSize != nExpectedKeySize)
		return FAILURE;

	//Caution. We will be destructing this same pointer twice and relying
	// on destruction order. First one wipes, second one (from read file) deletes.
	shared_ptr<char> pKeyBufferScopeGuard(
		pInputBuffer.get(),
		[=](char * i_pBuffer) 
	{
		memset(i_pBuffer, 0, nKeyBufferSize);
	});

	//Load source file, key into buffer.
	size_t nInputBufferSize = 0x0ULL;
	shared_ptr<char> pInputBuffer = nullptr;
	tie(pInputBuffer, nInputBufferSize) = ReadFileIntoBuffer(i_strInputFile);
	assert(pInputBuffer && nInputBufferSize);
	if(!pInputBuffer || !nInputBufferSize)
		return FAILURE;
		 
	shared_ptr<char> pInputBufferScopeGuard(
		pInputBuffer.get(),
		[=](char * i_pBuffer) 
	{
		memset(i_pBuffer, 0, nInputBufferSize);
	});

	//Allocate output buffer
	size_t nOutputBufferSize = (i_oOptions.m_eDirectionOption == ENCRYPT)
		? (EvalSizeAfterPadding(nInputBufferSize, i_oOptions))
		: (io_nOriginalFileSize);
	shared_ptr<char> pOutputBuffer = shared_ptr<char>(
		new char[nOutputBufferSize], 
		[=](char * i_pBuffer) 
	{
		memset(i_pBuffer, 0, nOutputBufferSize);
		delete [] i_pBuffer;
		i_pBuffer = nullptr;
	});

	//Call program using program options struct and loaded buffers.
	int nReturnValue = EncipherBuffer(
		i_oOptions,
		pInputBuffer.get(),
		nInputBufferSize,
		pOutputBuffer.get(),
		nOutputBufferSize, 
		pKeyBuffer.get(),
		nKeyBufferSize);
	if(nReturnValue == FAILURE)
		return FAILURE;

	//Write output buffer to target file.
	size_t nBytesWritten = WriteFileFromBuffer(
		i_strOutputFile,
		pOutputBuffer.get(),
		nOutputBufferSize);
	assert(nBytesWritten == nOutputBufferSize);
	if(nBytesWritten != nOutputBufferSize)
		return FAILURE;

	//If encrypting, set original file size.
	if(i_oOptions.m_eDirectionOption == ENCRYPT)
	{
		io_nOriginalFileSize = nInputBufferSize;
	}

	return SUCCESS;
}