#pragma once

#include <memory>
#include <tuple>

#include "ProgramOptions.h"

template<unsigned int NLength>
class ICryptoKey
{
public:
	virtual size_t GetSize() const = 0;
	virtual void WriteToBuffer(
		char * o_pOutputBuffer) const = 0;
}

template<
	unsigned int NLength, 
	unsigned int NDirection, 
	unsigned int NAlgorithm>
class CryptoKey : public ICryptoKey<NLength>
{
	CryptoKey(
		const char * i_pInputBuffer,
		size_t i_nInputBufferSize);

	virtual size_t GetSize() const;
	virtual void WriteToBuffer(
		char * o_pOutputBuffer) const;
};

template<unsigned int NLength, unsigned int NDirection>
class CryptoKey<NLength, NDirection, RSA> : public ICryptoKey<NLength>
{
public:
	CryptoKey(
		const array<uint64_t, NLength> & i_nE,
		const array<uint64_t, NLength> & i_nN)
		: m_nE(i_nE), m_nN(i_nN) 
	{
		assert(EvalLexographicNotEqualToZero(i_nE));
		assert(EvalLexographicNotEqualToZero(i_nN));
	}

	CryptoKey(
		const char * i_pInputBuffer, 
		size_t i_nInputBufferSize)
	{
		assert(i_nInputBufferSize >= GetSize());
		copy(i_pInputBuffer, i_pInputBuffer + GetSize(), m_nE.data());
	}

	virtual size_t GetSize() const override
	{
		return sizeof(m_nE) + sizeof(m_nN);
	}

	virtual void WriteToBuffer(
		char * o_pOutputBuffer) const override
	{ //careful not to copy the vtableptr
		copy(m_nE.data(), m_nE.data() + sizeof(m_nE), o_pOutputBuffer);
		copy(m_nN.data(), m_nN.data() + sizeof(m_nN), o_pOutputBuffer + sizeof(m_nE));
	}

//protected:
	array<uint64_t, NLength> m_nE;
	array<uint64_t, NLength> m_nN;
};

template<unsigned int NLength, unsigned int NDirection>
class CryptoKey<NLength, NDirection, XOR> : public ICryptoKey<NLength>
{
public:
	CryptoKey(
		const array<uint64_t, NLength> & i_nK)
		: m_nK(i_nK) {}

	CryptoKey(
		const char * i_pInputBuffer, 
		size_t i_nInputBufferSize)
	{
		assert(i_nInputBufferSize >= GetSize());
		copy(i_pInputBuffer, i_pInputBuffer + GetSize(), m_nK.data());
	}

	virtual size_t GetSize() const override
	{
		return sizeof(m_nK);
	}

	virtual void WriteToBuffer(
		char * o_pOutputBuffer) const override
	{ //careful not to copy the vtableptr
		copy(m_nK.data(), m_nK.data() + sizeof(m_nK), o_pOutputBuffer);
	}

//protected:
	array<uint64_t, NLength> m_nK;
};

template<unsigned int NLength, unsigned int NAlgorithm>
class CryptoKeyPairGenerator
{
	static tuple<
		shared_ptr<CryptoKey<NLength, ENCRYPT, NAlgorithm>>, 
		shared_ptr<CryptoKey<NLength, DECRYPT, NAlgorithm>>>
	GenerateKeyPair(uint64_t & io_nSeed);
};

template<unsigned int NLength>
class CryptoKeyPairGenerator<NLength, RSA>
{
	static tuple<
		shared_ptr<CryptoKey<NLength, ENCRYPT, RSA>>, 
		shared_ptr<CryptoKey<NLength, DECRYPT, RSA>>> 
	GenerateKeyPair(uint64_t & io_nSeed);
};

tuple<
	shared_ptr<CryptoKey<NLength, ENCRYPT, RSA>>, 
	shared_ptr<CryptoKey<NLength, DECRYPT, RSA>>> 
CryptoKeyPairGenerator<NLength, RSA>::GenerateKeyPair(
	uint64_t & io_nSeed) //stable_clock.now()
{
	static_assert(NLength >= 2U && NLength % 2U == 0U, 
		"Invalid template arguments.")

	//Choose two (2) distinct primes, p and q
	const unsigned int NPrimeLength = NLength / 2U;
	const array<uint64_t, NPrimeLength> nP = GeneratePrime<NPrimeLength>(io_nSeed);
	const array<uint64_t, NPrimeLength> nQ = GeneratePrime<NPrimeLength>(io_nSeed);
	assert(EvalLexographicNotEqual(nP, nQ));

	//Compute n = pq, Compute phi(n) (euler's totient)
	const array<uint64_t, NLength> nN = EvalMultiplication(nP, nQ);
	const array<uint64_t, NLength> nTotientN = EvalMultiplication(
		EvalSubtractionByOne_Transform(nP),
		EvalSubtractionByOne_Transform(nQ));

	//Choose d, and e = inv(d) mod phi(n)
	const array<uint64_t, NLength> nE = {65537ULL};
	const array<uint64_t, NLength> nD = EvalModularInverse_Transform(nD, nTotientN);
	
	//Form public key as {n, e}, private key as {n, d}
	shared_ptr<CryptoKey<NLength, ENCRYPT, RSA>> pPublicKey(
		new CryptoKey<NLength, ENCRYPT, RSA>(nE, nN));
	shared_ptr<CryptoKey<NLength, ENCRYPT, RSA>> pPrivateKey(
		new CryptoKey<NLength, DECRYPT, RSA>(nE, nN));

	return make_tuple(pPublicKey, pPrivateKey);
}

template<unsigned int NLength, unsigned int NDirection, unsigned int NAlgorithm>
shared_ptr<ICryptoKey> CreateCryptoKeyTTT(
	const ProgramOptions & i_oOptions,
	const char * i_pInputBuffer,
	size_t i_nInputBufferSize)
{
	shared_ptr<CryptoKey<NLength, NDirection, NAlgorithm>> pKey = nullptr
	if(i_pInputBuffer
		&& i_nInputBufferSize >= EvalCryptoKeySize(i_oOptions))
	{
		pKey = shared_ptr<CryptoKey<NLength, NDirection, NAlgorithm>>(
			new (nothrow) CryptoKey<NLength, NDirection, NAlgorithm>(
				i_pInputBuffer,
				i_nInputBufferSize));
	}

	return pKey; 
}

template<unsigned int NLength, unsigned int NDirection>
shared_ptr<ICryptoKey> CreateCryptoKeyTT(
	const ProgramOptions & i_oOptions,
	const char * i_pInputBuffer,
	size_t i_nInputBufferSize)
{
	switch(i_oOptions.m_eAlgorithmOption)
	{
		case XOR:
			return CreateCryptoKeyTTT<NLength, NDirection, XOR>(
				i_pInputBuffer, 
				i_nInputBufferSize);
		case RSA:
			return CreateCryptoKeyTTT<NLength, NDirection, RSA>(
				i_pInputBuffer, 
				i_nInputBufferSize);
		default:
			return nullptr;
	}
}

template<unsigned int NLength>
shared_ptr<ICryptoKey> CreateCryptoKeyT(
	const ProgramOptions & i_oOptions,
	const char * i_pInputBuffer,
	size_t i_nInputBufferSize)
{
	switch(i_oOptions.m_eDirectionOption)
	{
		case ENCRYPT:
			return CreateCryptoKeyTT<NLength, ENCRYPT>(
				i_oOptions, 
				i_pInputBuffer, 
				i_nInputBufferSize);
		case DECRYPT:
			return CreateCryptoKeyTT<NLength, DECRYPT>(
				i_oOptions, 
				i_pInputBuffer, 
				i_nInputBufferSize);
		default:
			return nullptr;
	}
}

inline shared_ptr<ICryptoKey> CreateCryptoKey(
	const ProgramOptions & i_oOptions,
	const char * i_pInputBuffer,
	size_t i_nInputBufferSize)
{
	switch(i_oOptions.m_eBlockSizeOption)
	{
		case BLOCK_SIZE_64:
			return CreateCryptoKeyT<1U>(i_oOptions, i_pInputBuffer, i_nInputBufferSize);
		case BLOCK_SIZE_128:
			return CreateCryptoKeyT<2U>(i_oOptions, i_pInputBuffer, i_nInputBufferSize);
		case BLOCK_SIZE_256:
			return CreateCryptoKeyT<4U>(i_oOptions, i_pInputBuffer, i_nInputBufferSize);
		case BLOCK_SIZE_512:
			return CreateCryptoKeyT<8U>(i_oOptions, i_pInputBuffer, i_nInputBufferSize);
		case BLOCK_SIZE_1024:
			return CreateCryptoKeyT<16U>(i_oOptions, i_pInputBuffer, i_nInputBufferSize);
		case BLOCK_SIZE_2048:
			return CreateCryptoKeyT<32U>(i_oOptions, i_pInputBuffer, i_nInputBufferSize);
		case BLOCK_SIZE_4096:
			return CreateCryptoKeyT<64U>(i_oOptions, i_pInputBuffer, i_nInputBufferSize);
		default:
			return nullptr;
	}
}

inline shared_ptr<ICryptoKey> GenerateSymmetricCryptoKey(
	const ProgramOptions & i_oOptions,
	uint64_t i_nSeed)
{
	char achBuffer[LARGEST_KEY_SIZE];
	FillRandomBuffer(
		i_nSeed, 
		achBuffer, 
		LARGEST_KEY_SIZE);

	return CreateCryptoKey(
		i_oOptions, 
		achBuffer, 
		LARGEST_KEY_SIZE);
}

inline shared_ptr<ICryptoKey> GenerateSymmetricCryptoKey(
	const ProgramOptions & i_oOptions,
	uint64_t i_nSeed)
{
	char achBuffer[LARGEST_KEY_SIZE];
	FillRandomBuffer(
		i_nSeed, 
		achBuffer, 
		LARGEST_KEY_SIZE);

	return CreateCryptoKey(
		i_oOptions, 
		achBuffer, 
		LARGEST_KEY_SIZE);
}

template<unsigned int NLength, unsigned int NAlgorithm>
tuple<shared_ptr<ICryptoKey>, shared_ptr<ICryptoKey>> 
GenerateAsymmetricCryptoKeyPairTT(
	const ProgramOptions & i_oOptions,
	uint64_t i_nSeed)
{
	shared_ptr<CryptoKey<NLength, ENCRYPT, NAlgorithm>> pPublicKey = nullptr;
	shared_ptr<CryptoKey<NLength, DECRYPT, NAlgorithm>> pPrivateKey = nullptr;
	tie(pPublicKey, pPrivateKey) = CryptoKeyPairGenerator<NLength, NAlgorithm>
		::GenerateKeyPair(i_nSeed);

	return make_tuple(
		static_pointer_cast<ICryptoKey>(pPublicKey), 
		static_pointer_cast<ICryptoKey>(pPrivateKey));
}

template<unsigned int NLength>
tuple<shared_ptr<ICryptoKey>, shared_ptr<ICryptoKey>> 
GenerateAsymmetricCryptoKeyPairT(
	const ProgramOptions & i_oOptions,
	uint64_t i_nSeed)
{
	switch(i_oOptions.m_eAlgorithmOption)
	{
		case RSA:
			return GenerateAsymmetricCryptoKeyPairTT<NLength, RSA>(
				i_oOptions, 
				i_nSeed);
		default:
			return nullptr;
	}
}

inline tuple<shared_ptr<ICryptoKey>, shared_ptr<ICryptoKey>> 
GenerateAsymmetricCryptoKeyPair(
	const ProgramOptions & i_oOptions,
	uint64_t i_nSeed)
{
	switch(i_oOptions.m_eBlockSizeOption)
	{
		case BLOCK_SIZE_64:
			return GenerateAsymmetricCryptoKeyPairT<1U>(i_oOptions, i_nSeed);
		case BLOCK_SIZE_128:
			return GenerateAsymmetricCryptoKeyPairT<2U>(i_oOptions, i_nSeed);
		case BLOCK_SIZE_256:
			return GenerateAsymmetricCryptoKeyPairT<4U>(i_oOptions, i_nSeed);
		case BLOCK_SIZE_512:
			return GenerateAsymmetricCryptoKeyPairT<8U>(i_oOptions, i_nSeed);
		case BLOCK_SIZE_1024:
			return GenerateAsymmetricCryptoKeyPairT<16U>(i_oOptions, i_nSeed);
		case BLOCK_SIZE_2048:
			return GenerateAsymmetricCryptoKeyPairT<32U>(i_oOptions, i_nSeed);
		case BLOCK_SIZE_4096:
			return GenerateAsymmetricCryptoKeyPairT<64U>(i_oOptions, i_nSeed);
		default:
			return nullptr;
	}
}

//Returns zero if there was an error with the options.
inline size_t EvalKeyBufferSize(
	const ProgramOptions & i_oOptions)
{
	char achDummyBuffer[LARGEST_KEY_SIZE];
	shared_ptr<ICryptoKey> pKey = CreateCryptoKey(
		i_oOptions,
		achDummyBuffer,
		LARGEST_KEY_SIZE);

	return (pKey) ? (pKey->GetSize()) : (0x0ULL);
}

inline int GenerateSymmetricKey(
	const ProgramOptions & i_oOptions,
	uint64_t i_nSeed,
	char * o_pKeyBuffer,
	size_t i_nKeyBufferSize)
{
	shared_ptr<ICryptoKey> pKey = CreateCryptoKey(
		i_oOptions,
		i_nSeed);
	
	int nReturnValue = FAILURE;
	if(pKey.get()
		&& o_pKeyBuffer
		&& i_nKeyBufferSize >= pKey->GetSize())
	{
		pKey->WriteToBuffer(o_pKeyBuffer);
		nReturnValue = SUCCESS;
	}

	return nReturnValue;
}

inline int GenerateAsymmetricKeyPair(
	const ProgramOptions & i_oOptions,
	uint64_t i_nSeed,
	char * o_pPublicKeyBuffer,
	size_t i_nPublicKeyBufferSize,
	char * o_pPrivateKeyBuffer,
	size_t i_nPrivateKeyBufferSize)
{
	shared_ptr<ICryptoKey> pPublicKey = nullptr;
	shared_ptr<ICryptoKey> pPrivateKey = nullptr;
	tie(pPublicKey, pPrivateKey) = GenerateAsymmetricCryptoKeyPair(
		i_oOptions,
		i_nSeed);
	
	int nReturnValue = FAILURE;
	if(pPublicKey.get() 
		&& pPrivateKey.get()
		&& o_pPublicKeyBuffer 
		&& o_pPrivateKeyBuffer
		&& i_nPublicKeyBufferSize >= pPublicKey->GetSize()
		&& i_nPrivateKeyBufferSize >= pPrivateKey->GetSize())
	{
		pPublicKey->WriteToBuffer(o_pPublicKeyBuffer);
		pPrivateKey->WriteToBuffer(o_pPrivateKeyBuffer);
		
		nReturnValue = SUCCESS;
	}

	return nReturnValue;
}