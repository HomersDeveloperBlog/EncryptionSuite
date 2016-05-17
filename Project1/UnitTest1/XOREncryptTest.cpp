#include "stdafx.h"
#include "CppUnitTest.h"

#include "XOREncrypt.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace XOREncrypt_Test
{
	TEST_CLASS(XOREncrypt_Test)
	{
	public:
		TEST_METHOD(SingleWordSingleBlockTest1)
		{
			uint64_t nM = 0x5b756bd5c482505dULL; //0x5b756bd5c482505d
			uint64_t nK = 0x82a207e362d294feULL; //0x82a207e362d294fe
			
			uint64_t nAnswer = 0xd9d76c36a650C4a3ULL; //0xd9d76c36a650C4a3
			uint64_t nX = XOREncryptor.EncryptBlock(nM, nK);
			Assert::IsTrue(nX == nAnswer);
		}

		TEST_METHOD(SingleWordSingleBlockTest2)
		{
			uint64_t nX = 0xd9d76c36a650C4a3ULL; //0xd9d76c36a650C4a3
			uint64_t nK = 0x82a207e362d294feULL; //0x82a207e362d294fe
			
			uint64_t nAnswer = 0x5b756bd5c482505dULL; //0x5b756bd5c482505d
			uint64_t nM = XOREncryptor.DecryptBlock(nX, nK);
			Assert::IsTrue(nM == nAnswer);
		}

		TEST_METHOD(MultiWordSingleBlockTest1)
		{//Single block consisting of multiple words
			array<uint64_t, 2U> nM = {0xe6693fb529bf57f2ULL, 0xa6a7320309d300a3ULL}; //0xa6a7320309d300a3e6693fb529bf57f2
			array<uint64_t, 2U> nK = {0xdda089fc06dc1896ULL, 0x94c66ba262ae8252ULL}; //0x94c66ba262ae8252dda089fc06dc1896
			array<uint64_t, 2U> nAnswer = {0x3bc9b6492f634f64ULL, 0x326159a16b7d82f1ULL}; //0x326159a16b7d82f13bc9b6492f634f64

			array<uint64_t, 2U> nX = XOREncryptor.EncryptBlock(nM, nK);
			Assert::IsTrue(nX == nAnswer);
		}

		TEST_METHOD(MultiWordSingleBlockTest2)
		{//Single block consisting of multiple words
			array<uint64_t, 4U> nX = {0x3bc9b6492f634f64ULL, 0x326159a16b7d82f1ULL}; //0x326159a16b7d82f13bc9b6492f634f64
			array<uint64_t, 4U> nK = {0xdda089fc06dc1896ULL, 0x94c66ba262ae8252ULL}; //0x94c66ba262ae8252dda089fc06dc1896
			array<uint64_t, 4U> nAnswer = {0xe6693fb529bf57f2ULL, 0xa6a7320309d300a3ULL}; //0xa6a7320309d300a3e6693fb529bf57f2
			
			array<uint64_t, 4U> nM = XOREncryptor.DecryptBlock(nM, nK);
			Assert::IsTrue(nM == nAnswer);
		}
	};
}