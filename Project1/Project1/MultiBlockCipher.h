#pragma once

#include <memory>

#include "RandomNumberUtility.h"
#include "ProgramOptions.h"
#include "BufferCipher.h" //%if we split off IBufferCipher, we could make a lighter include

using namespace std;

template<unsigned int NLength>
class IMultiBlockCipher
{
public:
	template<typename TInputBlockIterator, typename TOutputBlockIterator>
	TOutputBlockIterator EncipherBlocks(
		const TInputBlockIterator i_itPlainTextBlocksBegin,
		const TInputBlockIterator i_itPlainTextBlocksEnd,
		const TOutputBlockIterator o_itCipherTextBlocksBegin) const;

protected:
	array<uint64_t, NLength> EncipherBlockFromStream(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentCipherText,
		const array<uint64_t, NLength> & i_anPreviousCipherText) const;

	virtual array<uint64_t, NLength> InitializeIV() const = 0;

	virtual array<uint64_t, NLength> PreBlockRoutine(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentPlainText,
		const array<uint64_t, NLength> & i_anPreviousCipherText) const = 0;
	
	virtual array<uint64_t, NLength> PostBlockRoutine(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentCipherText,
		const array<uint64_t, NLength> & i_anPreviousCipherText) const = 0;
}

template<unsigned int NLength>
array<uint64_t, NLength> IMultiBlockCipher<NLength>::EncipherBlockFromStream(
	const array<uint64_t, NLength> & i_anPreviousPlainText,
	const array<uint64_t, NLength> & i_anCurrentPlainText,
	const array<uint64_t, NLength> & i_anPreviousCipherText) const
{
	return PostBlockRoutine(
		i_anPreviousPlainText,
		m_pBlockCipher->EncipherBlock(
			PreBlockRoutine(
				i_anPreviousPlainText,
				i_anCurrentPlainText,
				i_anPreviousCipherText)),
		i_anPreviousCipherText);
}

template<
	unsigned int NLength, 
	typename TInputBlockIterator,
	typename TOutputBlockIterator>
TOutputBlockIterator IMultiBlockCipher<NLength>::EncipherBlocks(
	const TInputBlockIterator i_itPlainTextBlocksBegin,
	const TInputBlockIterator i_itPlainTextBlocksEnd,
	const TOutputBlockIterator o_itCipherTextBlocksBegin) const
{
	array<uint64_t, NLength> anPreviousPlainText = InitializeIV();
	array<uint64_t, NLength> anPreviousCipherText = InitializeIV();

	TInputBlockIterator itCurrentPlainText = i_itPlainTextBlocksBegin;
	TOutputBlockIterator itCurrentCipherText = o_itCipherTextBlocksBegin;
	for(; itCurrentPlainText != i_itPlainTextBlocksEnd;
		anPreviousPlainText = *itCurrentPlainText, ++itCurrentPlainText,
		anPreviousCipherText = *itCurrentCipherText, ++itCurrentCipherText)
	{
		*itCurrentCipherText = EncipherBlockFromStream(
			i_anPreviousPlainText,
			*itCurrentPlainText,
			i_anPreviousCipherText);
	}

	return itCurrentCipherText;
};

template<
	unsigned int NLength, 
	unsigned int NDirection, 
	unsigned int NBlockMode>
class MultiBlockCipher : public IMultiBlockCipher<NLength>
{
public:
	MultiBlockCipher(shared_ptr<BlockCipher<NLength>> i_pBlockCipher);

protected:
	virtual array<uint64_t, NLength> InitializeIV() const override;

	virtual array<uint64_t, NLength> PreBlockRoutine(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentPlainText,
		const array<uint64_t, NLength> & i_anPreviousCipherText) const override;
	
	virtual array<uint64_t, NLength> PostBlockRoutine(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentCipherText,
		const array<uint64_t, NLength> & i_anPreviousCipherText) const override;

protected:
	shared_ptr<IBlockCipher<NLength>> m_pBlockCipher;
};

template<unsigned int NLength, unsigned int NDirection>
class MultiBlockCipher<NLength, NDirection, ELECTRONIC_CODEBOOK> : public IMultiBlockCipher<NLength>
{
public:
	MultiBlockCipher(
		shared_ptr<BlockCipher<NLength>> i_pBlockCipher)
		: m_pBlockCipher(i_pBlockCipher)
	{
		assert(m_pBlockCipher.get());
	}

protected:
	virtual array<uint64_t, NLength> InitializeIV() const override
	{
		array<uint64_t, NLength> anReturnValue = {}; //Zeros
		return anReturnValue;
	}

	virtual array<uint64_t, NLength> PreBlockRoutine(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentPlainText,
		const array<uint64_t, NLength> & i_anPreviousCipherText) const override
	{
		return i_anCurrentPlainText;
	}
	
	array<uint64_t, NLength> PostBlockRoutine(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentCipherText,
		const array<uint64_t, NLength> & i_anPreviousCipherText) const override
	{
		return i_anCurrentCipherText;
	}

protected:
	shared_ptr<IBlockCipher<NLength>> m_pBlockCipher;
};

template<unsigned int NLength>
class MultiBlockCipher<NLength, ENCRYPT, CIPHER_BLOCK_CHAINING> : public IMultiBlockCipher<NLength>
{
public:
	MultiBlockCipher(
		shared_ptr<BlockCipher<NLength>> i_pBlockCipher)
		: m_pBlockCipher(i_pBlockCipher)
	{
		assert(m_pBlockCipher.get());
	}

protected:
	virtual array<uint64_t, NLength> InitializeIV() const override
	{//Use mersenne twister on fixed seed to generate additional words.
		uint64_t nCBCIVSeed = 0x29e84ddc44974ce5ULL;
		return GenerateRandomBigUInt<NLength>(nCBCIVSeed);
	}

	virtual array<uint64_t, NLength> PreBlockRoutine(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentPlainText,
		const array<uint64_t, NLength> & i_anPreviousCipherText) const override
	{//Take the previous cipher text and XOR it with the current plain text
		return MultiWordIntegerBitwiseXOR(
			i_anPreviousCipherText, 
			i_anCurrentPlainText);
	}
	
	virtual array<uint64_t, NLength> PostBlockRoutine(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentCipherText,
		const array<uint64_t, NLength> & i_anPreviousCipherText) const override
	{ //Do nothing
		return i_anCurrentCipherText;
	}

protected:
	shared_ptr<IBlockCipher<NLength>> m_pBlockCipher;
};

template<unsigned int NLength>
class MultiBlockCipher<NLength, DECRYPT, CIPHER_BLOCK_CHAINING> : public IMultiBlockCipher<NLength>
{
public:
	MultiBlockCipher(
		shared_ptr<BlockCipher<NLength>> i_pBlockCipher)
		: m_pBlockCipher(i_pBlockCipher)
	{
		assert(m_pBlockCipher.get());
	}

protected:
	virtual array<uint64_t, NLength> InitializeIV() const override
	{//Use mersenne twister on fixed seed to generate additional words.
		uint64_t nCBCIVSeed = 0x29e84ddc44974ce5ULL;
		return GenerateRandomBigUInt<NLength>(nCBCIVSeed);
	}
 
	virtual array<uint64_t, NLength> MultiBlockCipher<
		NLength, DECRYPT, CIPHER_BLOCK_CHAINING>::PreBlockRoutine(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentPlainText,
		const array<uint64_t, NLength> & i_anPreviousCipherText) const override
	{ //Do nothing
		return i_anCurrentPlainText;
	}

	virtual array<uint64_t, NLength> MultiBlockCipher<
		NLength, DECRYPT, CIPHER_BLOCK_CHAINING>::PostBlockRoutine(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentCipherText,
		const array<uint64_t, NLength> & i_anPreviousCipherText) const override
	{ //Take the previous plain text and XOR it with the current cipher text
		return MultiWordIntegerBitwiseXOR(
			i_anPreviousPlainText, 
			i_anCurrentCipherText);;
	}

protected:
	shared_ptr<IBlockCipher<NLength>> m_pBlockCipher;
};

template<unsigned int NLength, unsigned int NDirection>
shared_ptr<IMultiBlockCipher<NLength>> CreateMultiBlockCipherTT(
	const ProgramOptions & i_oOptions,
	const IBlockCipher<NLength> * i_pBlockCipher)
{
	switch(i_oOptions.m_eBlockModeOption)
	{
		case ELECTRONIC_CODEBOOK:
			return shared_ptr<IMultiBlockCipher<NLength>>(
				new (nothrow) MultiBlockCipher<NLength, NDirection, ELECTRONIC_CODEBOOK>());
		case CIPHER_BLOCK_CHAINING:
			return shared_ptr<IMultiBlockCipher<NLength>>(
				new (nothrow) MultiBlockCipher<NLength, NDirection, CIPHER_BLOCK_CHAINING>());
		default:
			return nullptr;
	}
}

template<unsigned int NLength, unsigned int NDirection>
shared_ptr<IMultiBlockCipher<NLength>> CreateMultiBlockCipherTT(
	const ProgramOptions & i_oOptions,
	const char * i_pKeyBuffer,
	size_t i_nKeyBufferSize)
{
	shared_ptr<IBlockCipher<NLength>> pBlockCipher = 
		CreateBlockCipherTT<NLength, NDirection>(
			i_oOptions,
			i_pKeyBuffer,
			i_nKeyBufferSize);

	shared_ptr<IMultiBlockCipher<NLength>> pMultiBlockCipher = nullptr;
	if(pBlockCipher.get())
	{
		pMultiBlockCipher = CreateMultiBlockCipherTT<NLength, NDirection>(
			i_oOptions, 
			pBlockCipher.get());
	}
	
	return pMultiBlockCipher;
}