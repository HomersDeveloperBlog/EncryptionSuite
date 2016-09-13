#pragma once

#include <memory>

using namespace std;

template<unsigned int NLength>
class IBlockLoader
{
public:
	template<typename TOutputBlockIterator>
	TOutputBlockIterator PreparePaddedBlocks(
		const char * i_achPlainText,
		size_t i_nBufferByteCount,
		TOutputBlockIterator i_itOutputBlocksBegin) const;

	template<unsigned int NLength, typename TInputBlockIterator>
	void PrepareUnpaddedBuffer(
		const TInputBlockIterator i_itBlocksBegin,
		const TInputBlockIterator i_itBlocksEnd,
		char * o_pOutputBuffer,
		size_t i_nOutputSize) const;

protected:
	virtual array<uint64_t, NLength> PadFinalBlock(
		const char * i_pInputBlock,
		size_t i_nBufferRemainderByteCount) const = 0;
		
	//Returns a block in which the first i_nBufferRemainderByteCount bytes are valid.
	virtual array<uint64_t, NLength> UnpadFinalBlock(
		const array<uint64_t, NLength> & i_achBlock,
		size_t i_nBufferRemainderByteCount) const = 0;
};

template<unsigned int NLength, typename TOutputBlockIterator>
TOutputBlockIterator IBlockLoader<NLength>::PreparePaddedBlocks(
	const char * i_pInputBuffer,
	size_t i_nBufferByteCount,
	const TOutputBlockIterator i_itOutputBlocksBegin)
{
	static_assert(NLength, "Zero block size not allowed.");

	assert(i_pInputBuffer);
	if(!i_pInputBuffer)
		return i_itOutputBlocksBegin;

	size_t nBlockByteCount = sizeof(uint64_t) * NLength;
	size_t nBufferWholeBlockCount = i_nBufferByteCount / nBlockByteCount;
	size_t nBufferRemainderByteCount = i_nBufferByteCount % nBlockByteCount;

	char * pInputBlock = i_pInputBuffer;
	TOutputBlockIterator itOutputBlock = i_itOutputBlocksBegin;
	for(size_t nBlock = 0U;
		nBlock < nBufferWholeBlockCount;
		++nBlock, ++itOutputBlock, pInputBlock += nBlockByteCount)
	{
		memcpy_s(
			(*itOutputBlock).data(),
			nBlockByteCount,
			pInputBlock,
			nBlockByteCount);
	}
	
	//Last block.
	if(nBufferRemainderByteCount)
	{
		(*itOutputBlock) = PadFinalBlock(
			pInputBlock, 
			nBufferRemainderByteCount);
		++itOutputBlock;
	}

	return itOutputBlock;
}

template<unsigned int NLength, typename TInputBlockIterator>
void IBlockLoader<NLength>::PrepareUnpaddedBuffer(
	const TInputBlockIterator i_itBlocksBegin,
	const TInputBlockIterator i_itBlocksEnd,
	char * o_pOutputBuffer,
	size_t i_nOutputSize)
{
	static_assert(NLength, "Zero block size not allowed.");
	assert(o_pOutputBuffer);

	size_t nBlockByteCount = sizeof(array<uint64_t, NLength>);
	size_t nBufferWholeBlockCount = i_nOutputSize / nBlockByteCount;
	size_t nBufferRemainderByteCount = i_nOutputSize % nBlockByteCount;
	size_t nInputBlockCount = distance(i_itBlocksBegin, i_itBlocksEnd);
	assert(nBufferWholeBlockCount <= nInputBlockCount
		&& nBufferWholeBlockCount >= (nInputBlockCount - 0x1ULL));
	if(!o_pOutputBuffer || !nInputBlockCount
		|| !(nBufferWholeBlockCount <= nInputBlockCount
		&& nBufferWholeBlockCount >= (nInputBlockCount - 0x1ULL))))
		return;

	//Buffer all blocks except last
	char * pOutputBlock = o_pOutputBuffer;
	TInputBlockIterator itInputBlock = i_itBlocksBegin;
	for(size_t nBlock = 0U;
		nBlock < nBufferWholeBlockCount;
		++nBlock, ++itInputBlock, pOutputBlock += nBlockByteCount)
	{
		memcpy_s(
			pOutputBlock,
			nBlockByteCount,
			(*itInputBlock).data(),
			nBlockByteCount);
	}
	
	//Last block.
	if(nBufferRemainderByteCount)
	{
		array<uint64_t, NLength> achBlockBuffer = UnpadFinalBlock(
			(*itInputBlock),
			nBufferRemainderByteCount);
		memcpy_s(
			pOutputBlock,
			nBufferRemainderByteCount,
			achBlockBuffer.data(),
			nBufferRemainderByteCount);
	}
}

template<
	unsigned int NLength, 
	unsigned int NDirection, 
	unsigned int NPadding>
class BlockLoader : public IBlockLoader<NLength>
{
protected:
	virtual array<uint64_t, NLength> PadFinalBlock(
		const char * i_pInputBlock,
		size_t i_nBufferRemainderByteCount) const override;
		
	virtual array<uint64_t, NLength> UnpadFinalBlock(
		const array<uint64_t, NLength> & i_achBlock,
		size_t i_nBufferRemainderByteCount) const override;
};

template<unsigned int NLength>
class BlockLoader<NLength, ENCRYPT, ZERO_PADDING> : public IBlockLoader<NLength>
{
protected:
	virtual array<uint64_t, NLength> PadFinalBlock(
		const char * i_pInputBlock,
		size_t i_nBufferRemainderByteCount) const override
	{
		assert(i_nBufferRemainderByteCount);

		array<uint64_t, NLength> achBlockBuffer = {0x0ULL};
		memcpy_s(
			achBlockBuffer.data(),
			sizeof(array<uint64_t, NLength>),
			i_pInputBlock,
			i_nBufferRemainderByteCount);

		return achBlockBuffer;
	}
	
	virtual array<uint64_t, NLength> UnpadFinalBlock(
		const array<uint64_t, NLength> & i_achBlock,
		size_t i_nBufferRemainderByteCount) const override
	{
		assert(false && !i_nBufferRemainderCount);
		return i_achBlock;
	}
};

template<unsigned int NLength>
class BlockLoader<NLength, DECRYPT, ZERO_PADDING> : public IBlockLoader<NLength>
{
protected:
	virtual array<uint64_t, NLength> PadFinalBlock(
		const char * i_pInputBlock,
		size_t i_nBufferRemainderByteCount) const override
	{
		assert(false && !i_nBufferRemainderCount);

		array<uint64_t, NLength> achBlockBuffer = {};
		return achBlockBuffer;
	}
	
	virtual array<uint64_t, NLength> UnpadFinalBlock(
		const array<uint64_t, NLength> & i_achBlock,
		size_t i_nBufferRemainderByteCount) const override
	{
		return i_achBlock;
	}
};

template<unsigned int NLength>
class BlockLoader<NLength, ECRYPT, CIPHERTEXT_STEALING> : public IBlockLoader<NLength>
{
protected:
	virtual array<uint64_t, NLength> PadFinalBlock(
		const char * i_pInputBlockBuffer,
		size_t i_nBufferRemainderByteCount) const override
	{
		size_t nBlockByteCount = sizeof(uint64_t) * NLength;
		const size_t nStolenByteCount = nBlockByteCount - i_nBufferRemainderByteCount;
		assert(nStolenByteCount < nBlockByteCount);

		array<uint64_t, NLength> achOutputBlock;
		memcpy_s(
			achOutputBlock.data(),
			nBlockByteCount,
			i_pInputBlockBuffer - nStolenByteCount,
			nBlockByteCount);

		return achOutputBlock;
	}
		
	virtual array<uint64_t, NLength> UnpadFinalBlock(
		const array<uint64_t, NLength> & i_achBlock,
		size_t i_nBufferRemainderByteCount) const override
	{
		assert(false && !i_nBufferRemainderCount);
		return i_achBlock;
	}
};

template<unsigned int NLength>
class BlockLoader<NLength, DECRYPT, CIPHERTEXT_STEALING> : public IBlockLoader<NLength>
{
protected:
	virtual array<uint64_t, NLength> PadFinalBlock(
		const char * i_pInputBlock,
		size_t i_nBufferRemainderByteCount) const override
	{
		assert(false && !i_nBufferRemainderCount);

		array<uint64_t, NLength> achBlockBuffer = {};
		return achBlockBuffer;
	}
		
	virtual array<uint64_t, NLength> UnpadFinalBlock(
		const array<uint64_t, NLength> & i_achInputBlock,
		size_t i_nBufferRemainderByteCount) const override
	{
		size_t nBlockByteCount = sizeof(uint64_t) * NLength;
		const size_t nStolenByteCount = nBlockByteCount - i_nBufferRemainderByteCount;
		assert(nStolenByteCount < nBlockByteCount);
		
		array<uint64_t, NLength> achOutputBlock;
		memcpy_s(
			achOutputBlock.data(),
			nBlockByteCount,
			i_achInputBlock.data() + nStolenByteCount,
			i_nBufferRemainderByteCount);

		return achOutputBlock;
	}
};

template<unsigned int NLength, unsigned int NDirection>
shared_ptr<IBlockLoader<NLength>> CreateBlockLoaderTT(
{
	switch(i_oOptions.m_ePaddingOption)
	{
		case ZERO_PADDING:
			return shared_ptr<IBlockLoader<NLength>>(
				new (nothrow) BlockLoader<NLength, NDirection, ZERO_PADDING>());
		case CIPHERTEXT_STEALING:
			return shared_ptr<IBlockLoader<NLength>>(
				new (nothrow) BlockLoader<NLength, NDirection, CIPHERTEXT_STEALING>());
		default:
			return nullptr;
	}
}

//There is no EvalSizeBeforePadding. Must be entered properly from caller.
inline size_t EvalSizeAfterPadding(
	const ProgramOptions & i_oOptions, 
	size_t i_nBufferSize)
{
	size_t nBlockSize = EvalBlockSize(i_oOptions);
	size_t nReturnValue = i_nInputSize / nBlockSize
		+ !!(i_nInputSize % nBlockSize);
}