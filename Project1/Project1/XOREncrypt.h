#ifndef JTH_XORENCRYPT_H
#define JTH_XORENCRYPT_H

#include "BigBitArray.h"

template<unsigned int NBlockWordCount>
class XOR //: public SymmetricEncryptor<NBlockWordCount> // : BlockEncryptor<NBlockWordCount>
{
public:
	struct XORKey //: SymmetricKey<NBlockWordCount> // : BlockEncryptorKey<NBlockWordCount>
	{
		XORKey(const array<uintmax_t, NBlockWordCount> & i_nK)
		 : m_nk(i_nK) {}

		array<uintmax_t, NBlockWordCount> m_nK;
	};

	class XOREncryptor
	{
	public:
		XORBlockEncryptor(const XORKey & i_oKey)
		 : m_oKey(i_oKey) {};

		//Raw block conversion layer that does nothing here
		array<uintmax_t, NBlockWordCount> EncipherBlock(
			const array<uintmax_t, NBlockWordCount> & i_anMessageBlock)
		{
			return EncryptBlock(i_anBlock);
		}

	private:
		array<uintmax_t, NBlockWordCount> EncryptBlock(
			const array<uintmax_t, NBlockWordCount> & i_nM)
		{
			return MultiWordIntegerBitwiseXOR(
				i_nM, 
				m_oKey.m_nK);
		}

	protected:
		XORKey m_oKey;
	};

	class XORDecryptor
	{
	public:
		XORBlockDecryptor(const XORKey & i_oKey)
		 : m_oKey(i_oKey) {};

		array<uintmax_t, NBlockWordCount> EncipherBlock(
			const array<uintmax_t, NBlockWordCount> & i_anMessageBlock)
		{
			return DecryptBlock(i_anBlock);
		}

	private:
		array<uintmax_t, NBlockWordCount> DecryptBlock(
			const array<uintmax_t, NBlockWordCount> & i_nM)
		{
			return MultiWordIntegerBitwiseXOR(
				i_nM, 
				m_oKey.m_nK);
		}

	protected:
		XORKey m_oKey;
	};
};

#endif //JTH_XORENCRYPT_H