#include "stdafx.h"
#include "CppUnitTest.h"

#include "RSAEncrypt.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

//don't forget a check that the two primes generated are not the same.
//given key pair, test cipher text and recovery of plaintext for a single block.
namespace RSAEncrypt_Test
{
	TEST_CLASS(RSAEncrypt_Test)
	{
	public:
		TEST_METHOD(SingleWordSingleBlockTest1)
		{//Single block consisting of a single word.
			uint64_t nM = 0x1001011100100011ULL;
			uint64_t nE = 0x0000000000010001ULL;
			uint64_t nN = 0x8e1ce2bd2620a57fULL;
			
			uint64_t nAnswer = 0x01732a21964a5af2ULL;
			uint64_t nX = RSAEncryptBlock(nM, nE, nN);
			Assert::IsTrue(nX == nAnswer);
		}

		TEST_METHOD(SingleWordSingleBlockTest2)
		{//Single block consisting of a single word.
			uint64_t nX = 0x01732a21964a5af2ULL;
			uint64_t nN = 0x8e1ce2bd2620a57fULL;
			uint64_t nD = 0x3b4cdc63b63fcb51ULL;
			
			uint64_t nAnswer = 0x1001011100100011ULL;
			uint64_t nM = RSADecryptBlock(nX, nD, nN);
			Assert::IsTrue(nM == nAnswer);
		}

		TEST_METHOD(MultiWordSingleBlockTest1)
		{//Single block consisting of multiple words
			array<uint64_t, 4U> nM = {0x1001011100100011ULL, 0x1001011100100011ULL, 0x1001011100100011ULL, 0x1001011100100011ULL};
			array<uint64_t, 4U> nE = {0x0000000000010001ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};

			//0xc2e36927bffd8a2c38da113e4215e9827685ca62d1c909de1e48f820575f7e17
			array<uint64_t, 4U> nN = {0x1e48f820575f7e17ULL, 0x7685ca62d1c909deULL, 0x38da113e4215e982ULL, 0xc2e36927bffd8a2cULL};

			//0x12131389d7bf9837e8b743b8130c0b8299bb7757dedda77e213ee74a1b306246
			array<uint64_t, 4U> nAnswer = {0x213ee74a1b306246ULL, 0x99bb7757dedda77eULL, 0xe8b743b8130c0b82ULL, 0x12131389d7bf9837ULL};

			array<uint64_t, 4U> nX = RSAEncryptBlock(nM, nE, nN);
			Assert::IsTrue(nX == nAnswer);
		}

		TEST_METHOD(MultiWordSingleBlockTest2)
		{//Single block consisting of multiple words
			//0x12131389d7bf9837e8b743b8130c0b8299bb7757dedda77e213ee74a1b306246
			array<uint64_t, 4U> nX = {0x213ee74a1b306246ULL, 0x99bb7757dedda77eULL, 0xe8b743b8130c0b82ULL, 0x12131389d7bf9837ULL};

			//0xc2e36927bffd8a2c38da113e4215e9827685ca62d1c909de1e48f820575f7e17
			array<uint64_t, 4U> nN = {0x1e48f820575f7e17ULL, 0x7685ca62d1c909deULL, 0x38da113e4215e982ULL, 0xc2e36927bffd8a2cULL};

			//0x4f483e4e1a6fa5574a25846044782f8eda9d2efeee3cd26309f1dcdf793371b1
			array<uint64_t, 4U> nD = {0x09f1dcdf793371b1ULL, 0xda9d2efeee3cd263ULL, 0x4a25846044782f8eULL, 0x4f483e4e1a6fa557ULL};

			//0x12131389d7bf9837e8b743b8130c0b8299bb7757dedda77e213ee74a1b306246
			array<uint64_t, 4U> nAnswer = {0x1001011100100011ULL, 0x1001011100100011ULL, 0x1001011100100011ULL, 0x1001011100100011ULL};

			array<uint64_t, 4U> nM = RSADecryptBlock(nM, nD, nN);
			Assert::IsTrue(nM == nAnswer);
		}
	};
}