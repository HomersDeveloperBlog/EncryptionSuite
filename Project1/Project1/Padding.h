#pragma once

//Serve up blocks, properly padded
//We could wrap in cipher text stealing, or zero padding.
//With both, we need to record the original size so we can trim the padding.
//That means the encrypted file needs to be accompanied / concatenated with some accessory information.

template<unsigned int NLength>
vector<array<uint64_t, NLength>> PreparePaddedBlocks(
	const char * i_achPlainText,
	const size_t i_nBufferByteCount) //Number of characters, not necessarily 8 bits each
{
	const size_t nBlockByteCount = sizeof(uint64_t) * NLength;
	const size_t nBufferBlockCount = 
		i_nBufferByteCount / nBlockSizeInBytes;
	const size_t nBufferRemainderByteCount = 
		i_nBufferByteCount % nBlockSizeInBytes;
	const size_t nOutputBlockCount = 
		nBufferBlockCount + !!(nBufferRemainderByteCount);

	vector<array<uint64_t, NLength>> vectReturnValue;
	vectReturnValue.reserve(nOutputBlockCount));

	char * pCurrentBlock = i_achPlainText;
	for(size_t nBlock = 0U;
		nBlock < nBufferBlockCount;
		++nBlock, pCurrentBlock += nBlockByteCount)
	{
		array<uint64_t, NLength> achBlockBuffer;
		memcpy_s(
			achBlockBuffer.data(),
			nBlockByteCount,
			pCurrentBlock,
			nBlockByteCount);

		vectReturnValue.push_back(achBlockBuffer);
	}
	
	//Last block. Zero padding
	array<uint64_t, NLength> achBlockBuffer = {0x0ULL};
	memcpy_s(
		achBlockBuffer.data(),
		nBlockByteCount,
		pCurrentBlock,
		nBufferRemainderByteCount);

	vectReturnValue.push_back(achBlockBuffer);

	////Last block. Ciphertext stealing.
	//const size_t nStolenByteCount = 
	//	nBlockByteCount - nBufferRemainderByteCount;

	//array<uint64_t, NLength> achBlockBuffer;
	//memcpy_s(
	//	achBlockBuffer.data(),
	//	nBlockByteCount,
	//	pCurrentBlock - nStolenByteCount,
	//	nBlockByteCount);

	//vectReturnValue.push_back(achBlockBuffer);

	return vectReturnValue
}