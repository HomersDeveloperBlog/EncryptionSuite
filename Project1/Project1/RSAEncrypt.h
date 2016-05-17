#pragma once

//We need a generic encryptor class
//Possibly some more differentiation symmetric/asymmetric

template<unsigned int NLength>
class RSABlockCipher //%At the moment, this could just be a namespace
{
public:
	//Might have to use a typedef for generic naming of subclasses.
	//If we choose a namespace instead of a class, we cannot template the key.
	//Might need the outer class to have factory functions for the ciphers.

	struct RSAPublicKey
	{ //Could be moved out?
		array<uint64_t, NLength> m_nE;
		array<uint64_t, NLength> m_nN;
	};

	struct RSAPrivateKey
	{
		array<uint64_t, NLength> m_nD;
		array<uint64_t, NLength> m_nN;
	};

	class RSABlockEncryptor
	{
	public:
		RSABlockEncryptor(const RSAPublicKey & i_oKey)
		 : m_oKey(i_oKey) {};

		array<uint64_t, NLength> EncipherBlock(
			const array<uint64_t, NLength> & i_anMessageBlock)
		{
			return EncryptBlock(i_anMessageBlock);
		}

	private:
		array<uint64_t, NLength> EncryptBlock(
			const array<uint64_t, NLength> & i_nM)
		{
			//Encrypt as m^e mod n
			return EvalModularExponentiation(
				i_nM, 
				m_oKey.m_nE, 
				m_oKey.m_nN);
		}

	protected:
		RSAPublicKey m_oKey;
	};

	class RSABlockDecryptor
	{
	public:
		RSABlockDecryptor(const RSAPrivateKey & i_oKey)
		 : m_oKey(i_oKey) {};
		
		array<uint64_t, NLength> EncipherBlock(
			const array<uint64_t, NLength> & i_anMessageBlock)
		{
			return DecryptBlock(i_anMessageBlock);
		}

	private:
		void DecryptBlock(
			const array<uint64_t, NLength> & i_nM)
		{
			//Decrypt as m^d mod n
			return EvalModularExponentiation(
				i_nM, 
				oPrivateKey.m_nD,
				oPrivateKey.m_nN); 
			
			//%Should use chinese remainder theorem instead
		}

	protected:
		RSAPrivateKey m_oKey;
	};

	tuple<
		RSAEncryptor<NLength>::PublicKey, 
		RSAEncryptor<NLength>::PrivateKey> 
		GenerateRSAKeys() //should accept seed
	{
		static_assert(NLength >= 2U && NLength % 2U == 0U, 
			"Invalid template arguments.")

		//Choose two (2) distinct primes, p and q
		const unsigned int NPrimeLength = NLength / 2U;
		const array<uint64_t, 8U> nP = GeneratePrime<NPrimeLength>(); //should accept seed
		const array<uint64_t, 8U> nQ = GeneratePrime<NPrimeLength>();

		//Compute n = pq
		const array<uint64_t, 16U> nN = EvalMultiplication(nP, nQ);

		//Compute phi(n) (euler's totient)
		const array<uint64_t, 16U> nTotientN = EvalMultiplication(
			EvalSubtractionByOne_Transform(nP),
			EvalSubtractionByOne_Transform(nQ));

		//Choose d, and e = inv(d) mod phi(n)
		const array<uint64_t, 16U> nE = {65537ULL}; //%Probably should be a random number (N, E coprime)
		const array<uint64_t, 16U> nD = EvalModularInverse_Transform(nD, nTotient);

		//Form public key as {n, e}, private key as {n, d}
		return std::make_tuple(
			RSAEncryptor<NLength>::PublicKey(nE, nN), //Here's a good argument for having the key types accessible at this level.
			RSAEncryptor<NLength>::PrivateKey(nD, nN));
	}
};