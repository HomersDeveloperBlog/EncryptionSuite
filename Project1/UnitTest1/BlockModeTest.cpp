#include "stdafx.h"
#include "CppUnitTest.h"

#include <vector>

#include "BlockMode.h"
#include "XOREncrypt.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BlockMode_Test
{
	TEST_CLASS(BlockMode_Test)
	{
	public:
		TEST_METHOD(ECBXOREncryptTest1)
		{ 
			array<uint64_t, 2U> nK = {0x4a80f93e2d5cd16aULL, 0xe44a27d2df41f7d8ULL}; //0xe44a27d2df41f7d84a80f93e2d5cd16a
			vector<array<uint64_t, 2U>> anM = 
			{
				{0x4599e65642762e15ULL, 0xb96b3351336dbf91ULL}, //0xb96b3351336dbf914599e65642762e15
				{0xfac489d38f5a7df0ULL, 0x7d27883993ddda2dULL}, //0x7d27883993ddda2dfac489d38f5a7df0
				{0x98c4bba115472eeaULL, 0x723b79549c278485ULL}, //0x723b79549c27848598c4bba115472eea
				{0xa6fef5177f0c62efULL, 0x804dbe05dac791fcULL}  //0x804dbe05dac791fca6fef5177f0c62ef
			};
			
			vector<array<uint64_t, 2U>> anAnswer =
			{//M[i] ^ K
				{0x0F191F686F2AFF7FULL, 0x5D211483EC2C4849ULL}, //0x5D211483EC2C48490F191F686F2AFF7F
				{0xB04470EDA206AC9AULL, 0x996DAFEB4C9C2DF5ULL}, //0x996DAFEB4C9C2DF5B04470EDA206AC9A
				{0xD244429F381BFF80ULL, 0x96715E864366735DULL}, //0x96715E864366735DD244429F381BFF80
				{0xEC7E0C295250B385ULL, 0x640799D705866624ULL}  //0x640799D705866624EC7E0C295250B385
			};

			EBCBlockMode<4U> oBlockMode;
			vector<array<uint64_t, 4U>> anX(anM.size());
			vector<array<uint64_t, 4U>>::iterator itXEnd = XORCipher.EncryptBlockArray(
				anM.begin(),
				anM.end(),
				nK,
				oBlockMode,
				anX.begin());

			Assert::IsTrue(equals(
				anX.begin(),
				itXEnd,
				anAnswer.begin(),
				anAnswer.end()));
		}

		TEST_METHOD(ECBXORDecryptTest1)
		{//Single block consisting of multiple words
			array<uint64_t, 2U> nK = {0x4a80f93e2d5cd16aULL, 0xe44a27d2df41f7d8ULL}; //0xe44a27d2df41f7d84a80f93e2d5cd16a
			vector<array<uint64_t, 2U>> anX = 
			{
				{0x0F191F686F2AFF7FULL, 0x5D211483EC2C4849ULL}, //0x5D211483EC2C48490F191F686F2AFF7F
				{0xB04470EDA206AC9AULL, 0x996DAFEB4C9C2DF5ULL}, //0x996DAFEB4C9C2DF5B04470EDA206AC9A
				{0xD244429F381BFF80ULL, 0x96715E864366735DULL}, //0x96715E864366735DD244429F381BFF80
				{0xEC7E0C295250B385ULL, 0x640799D705866624ULL}  //0x640799D705866624EC7E0C295250B385
			};

			vector<array<uint64_t, 2U>> anAnswer =
			{
				{0x4599e65642762e15ULL, 0xb96b3351336dbf91ULL}, //0xb96b3351336dbf914599e65642762e15
				{0xfac489d38f5a7df0ULL, 0x7d27883993ddda2dULL}, //0x7d27883993ddda2dfac489d38f5a7df0
				{0x98c4bba115472eeaULL, 0x723b79549c278485ULL}, //0x723b79549c27848598c4bba115472eea
				{0xa6fef5177f0c62efULL, 0x804dbe05dac791fcULL}  //0x804dbe05dac791fca6fef5177f0c62ef
			};

			vector<array<uint64_t, 4U>> anM(anX.size());
			vector<array<uint64_t, 4U>>::iterator itMEnd = XORCipher.DecryptBlockArray(
				anX.begin(),
				anX.end(),
				nK,
				anM.begin());

			Assert::IsTrue(equal(
				anM.begin(),
				itMEnd,
				anAnswer.begin(),
				anAnswer.end()));
		}

		TEST_METHOD(CBCXOREncryptTest1)
		{ //Strong test of CBC block mode encryption
			array<uint64_t, 2U> nK = {0x4a80f93e2d5cd16aULL, 0xe44a27d2df41f7d8ULL}; //0xe44a27d2df41f7d84a80f93e2d5cd16a
			vector<array<uint64_t, 2U>> anM = 
			{
				{0x4599e65642762e15ULL, 0xb96b3351336dbf91ULL}, //0xb96b3351336dbf914599e65642762e15
				{0xfac489d38f5a7df0ULL, 0x7d27883993ddda2dULL}, //0x7d27883993ddda2dfac489d38f5a7df0
				{0x98c4bba115472eeaULL, 0x723b79549c278485ULL}, //0x723b79549c27848598c4bba115472eea
				{0xa6fef5177f0c62efULL, 0x804dbe05dac791fcULL}  //0x804dbe05dac791fca6fef5177f0c62ef
			};
			
			vector<array<uint64_t, 2U>> anECBAnswer =
			{//M[i] ^ K =
				{0x0F191F686F2AFF7FULL, 0x5D211483EC2C4849ULL}, //0x5D211483EC2C48490F191F686F2AFF7F
				{0xB04470EDA206AC9AULL, 0x996DAFEB4C9C2DF5ULL}, //0x996DAFEB4C9C2DF5B04470EDA206AC9A
				{0xD244429F381BFF80ULL, 0x96715E864366735DULL}, //0x96715E864366735DD244429F381BFF80
				{0xEC7E0C295250B385ULL, 0x640799D705866624ULL}  //0x640799D705866624EC7E0C295250B385
			};

			//EBC Encryption is Enc(M[i]) = M[i] ^ K
			//CBC Encryption is Enc(M[i] ^ X[i-1]), X[-1] = IV; = (M[i] ^ X[i-1]) ^ K = (M[i] ^ K) ^ X[i-1]
			//CBC[i] = EBC[i] ^ CBC[i-1], CBC[-1]
			//So lets first get ECB then use it to derive CBC. Allows some economy in these tests.

			vector<array<uint64_t, 2U>> anCBCAnswer =
			{
				{0x0000000000000000ULL, 0x0000000000000000ULL}, //(M[0] ^ IV) ^ K = 
				{0x0000000000000000ULL, 0x0000000000000000ULL}, //(M[1] ^ X[0]) ^ K
				{0x0000000000000000ULL, 0x0000000000000000ULL}, 
				{0x0000000000000000ULL, 0x0000000000000000ULL}
			};

			CBCBlockMode<4U> oBlockMode;
			vector<array<uint64_t, 4U>> anX(anM.size());
			vector<array<uint64_t, 4U>>::iterator itXEnd = XORCipher.EncryptBlockArray(
				anM.begin(),
				anM.end(),
				nK,
				oBlockMode,
				anX.begin());

			Assert::IsTrue(equals(
				anX.begin(),
				itXEnd,
				anCBCAnswer.begin(),
				anCBCAnswer.end()));
		}

		TEST_METHOD(CBCXORDecryptTest1)
		{//Single block consisting of multiple words
			array<uint64_t, 2U> nK = {0x4a80f93e2d5cd16aULL, 0xe44a27d2df41f7d8ULL}; //0xe44a27d2df41f7d84a80f93e2d5cd16a
			vector<array<uint64_t, 2U>> anX = 
			{
				{0x0000000000000000ULL, 0x0000000000000000ULL}, 
				{0x0000000000000000ULL, 0x0000000000000000ULL}, 
				{0x0000000000000000ULL, 0x0000000000000000ULL}, 
				{0x0000000000000000ULL, 0x0000000000000000ULL}
			};

			vector<array<uint64_t, 2U>> anAnswer =
			{
				{0x4599e65642762e15ULL, 0xb96b3351336dbf91ULL}, //0xb96b3351336dbf914599e65642762e15
				{0xfac489d38f5a7df0ULL, 0x7d27883993ddda2dULL}, //0x7d27883993ddda2dfac489d38f5a7df0
				{0x98c4bba115472eeaULL, 0x723b79549c278485ULL}, //0x723b79549c27848598c4bba115472eea
				{0xa6fef5177f0c62efULL, 0x804dbe05dac791fcULL}  //0x804dbe05dac791fca6fef5177f0c62ef
			};

			vector<array<uint64_t, 4U>> anM(anX.size());
			vector<array<uint64_t, 4U>>::iterator itMEnd = XORCipher.DecryptBlockArray(
				anX.begin(),
				anX.end(),
				nK,
				anM.begin());

			Assert::IsTrue(equal(
				anM.begin(),
				itMEnd,
				anAnswer.begin(),
				anAnswer.end()));
		}
	};
}