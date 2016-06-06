#ifndef JTH_PADDING_H
#define JTH_PADDING_H

//With both, we need to record the original size so we can trim the padding.
//That means the encrypted file needs to be accompanied / concatenated with some accessory information.

//%In other words, we need an unpad function

template<unsigned int NLength>
class BlockLoader
{
public:
	tuple<vector<array<uint64_t, NLength>>, size_t> PreparePaddedBlocks(
		const char * i_achPlainText,
		size_t i_nBufferByteCount);
	
	virtual array<uint64_t, NLength> FinalBlock(
		const char * i_pCurrentBlock,
		size_t i_nBufferRemainderByteCount) = 0;
};

template<unsigned int NLength>
tuple<vector<array<uint64_t, NLength>>, size_t> BlockLoader<NLength>::PreparePaddedBlocks(
	const char * i_achPlainText,
	size_t i_nBufferByteCount) //Number of characters, not necessarily 8 bits each
{
	const size_t nBlockByteCount = sizeof(uint64_t) * NLength;
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

	array<uint64_t, NLength> achBlockBuffer = FinalBlock(
		pCurrentBlock, 
		nBufferRemainderByteCount);
	vectReturnValue.push_back(achBlockBuffer);

	size_t nPaddingByteCount = nBlockByteCount - i_nBufferRemainderByteCount;
	return make_tuple(vectReturnValue, nPaddingByteCount);
}

template<unsigned int NLength>
class ZeroPaddingLoader : BlockLoader<NLength>
{
	virtual array<uint64_t, NLength> FinalBlock(
		const char * i_pCurrentBlock,
		size_t i_nBufferRemainderByteCount) override
	{
		assert(i_pCurrentBlock);

		const size_t nBlockByteCount = sizeof(uint64_t) * NLength;
		array<uint64_t, NLength> achBlockBuffer = {0x0ULL};
		memcpy_s(
			achBlockBuffer.data(),
			nBlockByteCount,
			i_pCurrentBlock,
			i_nBufferRemainderByteCount);

		return achBlockBuffer;
	}
};

template<unsigned int NLength>
class CipherTextStealingLoader : BlockLoader<NLength>
{
	virtual array<uint64_t, NLength> FinalBlock(
		const char * i_pCurrentBlock,
		size_t i_nBufferRemainderByteCount) override
	{
		assert(i_pCurrentBlock);

		const size_t nBlockByteCount = sizeof(uint64_t) * NLength;
		size_t nStolenByteCount = nBlockByteCount - i_nBufferRemainderByteCount;
		
		array<uint64_t, NLength> achBlockBuffer;
		memcpy_s(
			achBlockBuffer.data(),
			nBlockByteCount,
			i_pCurrentBlock - nStolenByteCount,
			nBlockByteCount);

		return achBlockBuffer;
	}
};

#endif //JTH_PADDING_H