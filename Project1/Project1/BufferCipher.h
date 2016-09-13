#pragma once

#include <memory>

#include "Padding.h"
#include "BlockMode.h"

using namespace std;

class IBufferCipher
{
public:
	virtual void EncipherBuffer(
		const char * i_pInputBuffer,
		size_t i_nInputSize,
		char * o_pOutputBuffer, 
		size_t i_nOutputSize) = 0;
};

template<unsigned int NLength>
class BufferCipher : public IBufferCipher<NLength>
{
public:
	BufferCipher(
		shared_ptr<IBlockLoader<NLength>> m_pBlockLoader;
		shared_ptr<IMultiBlockCipher<NLength>> m_pMultiBlockCipher;)
		: m_pBlockLoader(i_pBlockLoader),
		m_pMultiBlockCipher(i_pMultiBlockCipher)
	{
		assert(m_pBlockLoader.get() && m_pBlockCipher.get());
	}

	virtual void EncipherBuffer(
		const char * i_pInputBuffer,
		size_t i_nInputSize,
		char * o_pOutputBuffer, 
		size_t i_nOutputSize) override;

protected:
	shared_ptr<IBlockLoader<NLength>> m_pBlockLoader;
	shared_ptr<IMultiBlockCipher<NLength>> m_pMultiBlockCipher;
};

template<unsigned int NLength>
void BufferCipher<NLength>::EncipherBuffer(
	const char * i_pInputBuffer,
	size_t i_nInputSize,
	char * o_pOutputBuffer, 
	size_t i_nOutputSize)
{
	//Load blocks
	vector<array<uint64_t, NLength>> vectInputBlocks;
	vectInputBlocks.reserve(
		i_nInputSize / sizeof(array<uint64_t, NLength>)
		+ !!(i_nInputSize % sizeof(array<uint64_t, NLength>));

	m_pBlockLoader->PreparePaddedBlocks(
		i_pInputBuffer,
		i_nInputSize,
		back_inserter(vectInputBlocks));

	//Encipher the blocks
	vector<array<uint64_t, NLength>> vectOutputBlocks;
	vectOutputBlocks.reserve(vectInputBlocks.size());

	m_pMultiBlockCipher->EncipherBlocks(
		vectInputBlocks.begin(),
		vectInputBlocks.end(),
		back_inserter(vectOutputBlocks));

	//Unload blocks
	m_pBlockLoader->PrepareUnpaddedBuffer(
		vectOutputBlocks.begin(),
		vectOutputBlocks.end(),
		o_pOutputBuffer,
		i_nOutputSize);
}

template<unsigned int NLength, unsigned int NDirection>
shared_ptr<IBufferCipher> CreateBufferCipherTT(
	const ProgramOptions & i_oOptions,
	const char * i_pKeyBuffer,
	size_t i_nKeyBufferSize)
{
	shared_ptr<IMultiBlockCipher<NLength>> pMultiBlockCipher = 
		CreateMultiBlockCipherTT<NLength, NDirection>(
			i_oOptions,
			i_pKeyBuffer,
			i_nKeyBufferSize);
	
	shared_ptr<IBlockLoader<NLength>> pBlockLoader = 
		CreateBlockLoaderTT<NLength, NDirection>(
			i_oOptions);

	shared_ptr<IBufferCipher> pBufferCipher = nullptr;
	if(pBlockLoader.get() 
		&& pMultiBlockCipher.get())
	{
		pBufferCipher = shared_ptr<IBufferCipher>(
			new (nothrow) BufferCipher<NLength>(
				pBlockLoader.get(), 
				pMultiBlockCipher.get()));
	}

	return pBufferCipher;
}

template<unsigned int NLength>
shared_ptr<IBufferCipher> CreateBufferCipherT(
	const ProgramOptions i_oOptions,
	const char * i_pKeyBuffer,
	size_t i_nKeyBufferSize)
{
	switch(i_oOptions.m_eDirectionOption)
	{
		case ENCRYPT:
			return CreateBufferCipherTT<NLength, ENCRYPT>(
				i_oOptions, 
				i_pKeyBuffer, 
				i_nKeyBufferSize);
		case DECRYPT:
			return CreateBufferCipherTT<NLength, DECRYPT>(
				i_oOptions, 
				i_pKeyBuffer, 
				i_nKeyBufferSize);
		default:
			return nullptr;
	}
}

inline shared_ptr<IBufferCipher> CreateBufferCipher(
	const ProgramOptions & i_oOptions,
	const char * i_pKeyBuffer,
	size_t i_nKeyBufferSize)
{
	switch(i_oOptions.m_eBlockSizeOption)
	{
		case BLOCK_SIZE_64:
			return CreateBufferCipherT<1U>(i_oOptions, i_pKeyBuffer, i_nKeyBufferSize);
		case BLOCK_SIZE_128:
			return CreateBufferCipherT<2U>(i_oOptions, i_pKeyBuffer, i_nKeyBufferSize);
		case BLOCK_SIZE_256:
			return CreateBufferCipherT<4U>(i_oOptions, i_pKeyBuffer, i_nKeyBufferSize);
		case BLOCK_SIZE_512:
			return CreateBufferCipherT<8U>(i_oOptions, i_pKeyBuffer, i_nKeyBufferSize);
		case BLOCK_SIZE_1024:
			return CreateBufferCipherT<16U>(i_oOptions, i_pKeyBuffer, i_nKeyBufferSize);
		case BLOCK_SIZE_2048:
			return CreateBufferCipherT<32U>(i_oOptions, i_pKeyBuffer, i_nKeyBufferSize);
		case BLOCK_SIZE_4096:
			return CreateBufferCipherT<64U>(i_oOptions, i_pKeyBuffer, i_nKeyBufferSize);
		default:
			return nullptr;
	}
}

inline int EncipherBuffer(
	const ProgramOptions & i_oOptions,
	const char * i_pInputBuffer,
	size_t i_nInputSize,
	char * o_pOutputBuffer, 
	size_t i_nOutputSize, 
	const char * i_pKey,
	size_t i_nKeyBufferSize)
{
	//Input validation
	assert(i_oOptions.IsValid());
	assert(i_pInputBuffer && i_nInputSize);
	assert(io_pOutputBuffer && i_nOutputSize);
	assert(i_pKey && i_nKeyBufferSize);
	assert(i_nKeyBufferSize >= EvalKeyBufferSize(i_oOptions));
	assert(i_oOptions.m_eDirectionOption != ENCRYPT
		|| i_nOutputSize >= EvalSizeAfterPadding(i_nInputSize, i_oOptions));
	//%Output size should be >= floor(size / block size) * block size and 
	// <= ceil(size / block size) * block size to indicate padding if decrypting
	//%Buffers should not alias eachother

	if(!(i_oOptions.IsValid()
		&& i_pInputBuffer && i_nInputSize
		&& io_pOutputBuffer && i_nOutputSize
		&& i_pKey && i_nKeyBufferSize
		&& i_nKeyBufferSize >= EvalKeyBufferSize(i_oOptions)
		&& (i_oOptions.m_eDirectionOption != ENCRYPT
		|| i_nOutputSize >= EvalSizeAfterPadding(i_nInputSize, i_oOptions))))
		return FAILURE;
	
	//Create buffer cipher from options
	shared_ptr<IBufferCipher> pBufferCipher = CreateBufferCipher(
		i_oOptions,
		i_pKeyBuffer,
		i_nKeyBufferSize);
	assert(pBufferCipher.get());
	if(!pBufferCipher.get())
		return FAILURE;

	//Encipher the buffer
	pBufferCipher->EncipherBuffer(
		i_pInputBuffer,
		i_nInputSize,
		o_pOutputBuffer, 
		i_nOutputSize);

	return SUCCESS;
}