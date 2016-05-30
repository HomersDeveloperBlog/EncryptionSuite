#ifndef JTH_PADDING_H
#define JTH_PADDING_H

//With both, we need to record the original size so we can trim the padding.
//That means the encrypted file needs to be accompanied / concatenated with some accessory information.
//In other words, we need an unpad function
//Might be able to work in writing a header into padding.

//Check we are handling bytelengths not a multiple of largest unsigned int
//Make an OO structure to handle padding differences

template<unsigned int NLength>
class BlockLoader
{
	public:
	virtual vector<array<uint64_t, NLength>> PreparePaddedBlocks(
		const char * i_achPlainText,
		size_t i_nBufferByteCount);
};

template<unsigned int NLength>
vector<array<uint64_t, NLength>> BlockLoader<NLength>::reparePaddedBlocks(
	const char * i_achPlainText,
	size_t i_nBufferByteCount) //Number of characters, not necessarily 8 bits each
{
	size_t nBlockByteCount = sizeof(uint64_t) * NLength;
	size_t nBufferBlockCount = 
		i_nBufferByteCount / nBlockSizeInBytes;
	size_t nBufferRemainderByteCount = 
		i_nBufferByteCount % nBlockSizeInBytes;
	size_t nOutputBlockCount = 
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

#endif //JTH_PADDING_H