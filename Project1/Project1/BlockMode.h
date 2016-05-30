#ifndef JTH_BLOCKMODE_H
#define JTH_BLOCKMODE_H

template<unsigned int NLength>
class MultiBlockCipher
{
public:
	template<typename TInputBlockIterator, typename TOutputBlockIterator>
	TOutputBlockIterator EncipherBuffer(
		const TInputBlockIterator i_itPlainTextBlocksBegin,
		const TInputBlockIterator i_itPlainTextBlocksEnd,
		const TOutputBlockIterator o_itCipherTextBlocksBegin);

private:
	virtual array<uint64_t, NLength> InitializeIV() = 0;

	virtual array<uint64_t, NLength> PreBlockRoutine(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentPlainText,
		const array<uint64_t, NLength> & i_anPreviousCipherText) = 0;
	
	virtual array<uint64_t, NLength> PostBlockRoutine(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentCipherText,
		const array<uint64_t, NLength> & i_anPreviousCipherText) = 0;
	
	array<uint64_t, NLength> EncipherBlockFromStream(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentCipherText,
		const array<uint64_t, NLength> & i_anPreviousCipherText);

protected:
	BlockCipher<NLength> * m_pBlockCipher;
}

template<unsigned int NLength>
array<uint64_t, NLength> MultiBlockCipher<NLength>::EncipherBlockFromStream(
	const array<uint64_t, NLength> & i_anPreviousPlainText,
	const array<uint64_t, NLength> & i_anCurrentPlainText,
	const array<uint64_t, NLength> & i_anPreviousCipherText)
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

template<unsigned int NLength, typename TInputBlockIterator, typename TOutputBlockIterator>
TOutputBlockIterator MultiBlockCipher<NLength>::EncipherBuffer(
	const TInputBlockIterator i_itPlainTextBlocksBegin,
	const TInputBlockIterator i_itPlainTextBlocksEnd,
	const TOutputBlockIterator o_itCipherTextBlocksBegin)
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

template<unsigned int NLength>
class ECBCipher : MultiBlockCipher<NLength>
{
public:
	ECBCipher(BlockCipher<NLength> * i_pBlockCipher)
	 : m_pBlockCipher(i_pBlockCipher)
	{
		assert(m_pBlockCipher);
	}

private:
	virtual array<uint64_t, NLength> InitializeIV()
	{
		array<uint64_t, NLength> anReturnValue = {}; //Zeros
		return anReturnValue;
	}

	virtual array<uint64_t, NLength> PreBlockRoutine(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentPlainText,
		const array<uint64_t, NLength> & i_anPreviousCipherText)
	{
		return i_anCurrentPlainText;
	}
	
	virtual array<uint64_t, NLength> PostBlockRoutine(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentCipherText,
		const array<uint64_t, NLength> & i_anPreviousCipherText)
	{
		return i_anCurrentCipherText;
	}
};

template<unsigned int NLength>
class CBCForwardCipher : MultiBlockCipher<NLength>
{
public:
	CBCForwardCipher(BlockCipher<NLength> * i_pBlockCipher)
	 : m_pBlockCipher(i_pBlockCipher)
	{
		assert(m_pBlockCipher);
	}

private:
	const uint64_t nCBCIVSeed = 0x29e84ddc44974ce5ULL;

	virtual array<uint64_t, NLength> InitializeIV()
	{//Use mersenne twister on fixed seed to generate additional words.
		mt19937_64 fGenerator(nCBCIVSeed);
		array<uint64_t, NLength> anIV = {nCBCIVSeed}; //%pull this into randomnumbergenerator.h
		for(unsigned int nWord = 1U;
			nWord < NLength;
			++nWord)
		{
			anIV[nWord] = fGenerator();
		}
		
		return anIV;
	}

	virtual array<uint64_t, NLength> PreBlockRoutine(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentPlainText,
		const array<uint64_t, NLength> & i_anPreviousCipherText)
	{//Take the previous cipher text and XOR it with the current plain text
		return MultiWordIntegerBitwiseXOR(
			i_anPreviousCipherText, 
			i_anCurrentPlainText);
	}
	
	virtual array<uint64_t, NLength> PostBlockRoutine(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentCipherText,
		const array<uint64_t, NLength> & i_anPreviousCipherText)
	{ //Do nothing
		return i_anCurrentCipherText;
	}
}

template<unsigned int NLength>
class CBCBackwardCipher : MultiBlockCipher<NLength>
{
public:
	CBCBackwardCipher(BlockCipher<NLength> * i_pBlockCipher)
	 : m_pBlockCipher(i_pBlockCipher)
	{
		assert(m_pBlockCipher);
	}

private:
	const uint64_t nCBCIVSeed = 0x29e84ddc44974ce5ULL;

	virtual array<uint64_t, NLength> InitializeIV()
	{//Use mersenne twister on fixed seed to generate additional words.
		mt19937_64 fGenerator(nCBCIVSeed);
		array<uint64_t, NLength> anIV = {nCBCIVSeed}; //%pull this into randomnumbergenerator.h
		for(unsigned int nWord = 1U;
			nWord < NLength;
			++nWord)
		{
			anIV[nWord] = fGenerator();
		}
		
		return anIV;
	}

	virtual array<uint64_t, NLength> PreBlockRoutine(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentPlainText,
		const array<uint64_t, NLength> & i_anPreviousCipherText)
	{ //Do nothing
		return i_anCurrentPlainText;
	}
	
	virtual array<uint64_t, NLength> PostBlockRoutine(
		const array<uint64_t, NLength> & i_anPreviousPlainText,
		const array<uint64_t, NLength> & i_anCurrentCipherText,
		const array<uint64_t, NLength> & i_anPreviousCipherText)
	{ //Take the previous plain text and XOR it with the current cipher text
		return MultiWordIntegerBitwiseXOR(
			i_anPreviousPlainText, 
			i_anCurrentCipherText);;
	}
}

#endif //JTH_BLOCKMODE_H