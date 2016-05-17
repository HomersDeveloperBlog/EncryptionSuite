#include "stdafx.h"
#include "CppUnitTest.h"

#include "PrimeNumberGenerator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

//note, primes are half the quoted security factor in bits
namespace PrimeNumberGenerator_Test
{		
	TEST_CLASS(PrimeNumberGenerator_Test)
	{
	public:
		TEST_METHOD(SingleWordPrimeTest1)
		{//Single word case, trivial prime
			uint64_t nA = 0x0000000000000007ULL;
			Assert::IsTrue(IsRabinMillerPrime(nA));
		}

		TEST_METHOD(SingleWordPrimeTest2)
		{//Single word case, trivial composite
			uint64_t nA = 0x0000000000000015ULL;
			Assert::IsTrue(!IsRabinMillerPrime(nA));
		}

		TEST_METHOD(SingleWordPrimeTest3)
		{//Single word case, edge cases
			uint64_t nA = 0x0000000000000000ULL; //0x0
			Assert::IsTrue(!IsRabinMillerPrime(nA));

			uint64_t nB = 0x0000000000000001ULL; //0x1
			Assert::IsTrue(!IsRabinMillerPrime(nB));

			uint64_t nC = 0x0000000000000002ULL; //0x2
			Assert::IsTrue(!IsRabinMillerPrime(nC));

			uint64_t nD = 0x1000000000000000ULL; //even
			Assert::IsTrue(!IsRabinMillerPrime(nD));
		}

		TEST_METHOD(SingleWordPrimeTest4)
		{//Single word case, all prime
			uint64_t nA = 0xa381ae8b19c12547ULL;
			Assert::IsTrue(IsRabinMillerPrime(nA));

			uint64_t nB = 0xb04cf46e6b13241dULL;
			Assert::IsTrue(IsRabinMillerPrime(nB));

			uint64_t nC = 0xc4699f0449355117ULL;
			Assert::IsTrue(IsRabinMillerPrime(nC));

			uint64_t nD = 0xbd757030958657bfULL;
			Assert::IsTrue(IsRabinMillerPrime(nD));

			uint64_t nE = 0xa5132d6db47d5e39ULL;
			Assert::IsTrue(IsRabinMillerPrime(nE));
		}

		TEST_METHOD(SingleWordPrimeTest5)
		{//Single word case, all composite
			uint64_t nA = 0x0707cae4e9c8ccb9ULL;
			Assert::IsTrue(!IsRabinMillerPrime(nA));

			uint64_t nB = 0x1e73f70b6148e2ffULL;
			Assert::IsTrue(!IsRabinMillerPrime(nB));

			uint64_t nC = 0xdf8d12002ce6959bULL;
			Assert::IsTrue(!IsRabinMillerPrime(nC));

			uint64_t nD = 0xed8d7fbe0722ff91ULL;
			Assert::IsTrue(!IsRabinMillerPrime(nD));

			uint64_t nE = 0x9799957fbe3edf0dULL;
			Assert::IsTrue(!IsRabinMillerPrime(nE));
		}

		TEST_METHOD(MultiWordPrimeTest1)
		{//Multiword reusing single word cases, all prime
			array<uint64_t, 1U> anA = {0xa381ae8b19c12547ULL};
			Assert::IsTrue(IsRabinMillerPrime(anA));

			array<uint64_t, 2U> anB = {0xb04cf46e6b13241dULL, 0x0000000000000000ULL};
			Assert::IsTrue(IsRabinMillerPrime(anB));

			array<uint64_t, 3U> anC = {0xc4699f0449355117ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			Assert::IsTrue(IsRabinMillerPrime(anC));

			array<uint64_t, 2U> anD = {0xbd757030958657bfULL, 0x0000000000000000ULL};
			Assert::IsTrue(IsRabinMillerPrime(anD));

			array<uint64_t, 1U> anD = {0xa5132d6db47d5e39ULL};
			Assert::IsTrue(IsRabinMillerPrime(anD));
		}

		TEST_METHOD(MultiWordPrimeTest2)
		{//Multiword reusing single word cases, all composite
			array<uint64_t, 1U> anA = {0x0707cae4e9c8ccb9ULL};
			Assert::IsTrue(IsRabinMillerPrime(anA));

			array<uint64_t, 2U> anB = {0x1e73f70b6148e2ffULL, 0x0000000000000000ULL};
			Assert::IsTrue(IsRabinMillerPrime(anB));

			array<uint64_t, 3U> anC = {0xdf8d12002ce6959bULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			Assert::IsTrue(IsRabinMillerPrime(anC));

			array<uint64_t, 2U> anD = {0xed8d7fbe0722ff91ULL, 0x0000000000000000ULL};
			Assert::IsTrue(IsRabinMillerPrime(anD));

			array<uint64_t, 1U> anD = {0x9799957fbe3edf0dULL};
			Assert::IsTrue(IsRabinMillerPrime(anD));
		}

		TEST_METHOD(MultiWordPrimeTest3)
		{//Full multiword case, prime
			array<uint64_t, 2U> anA = {0x9ad46f4ef7d0e89fULL, 0x8e56bd293b2e8b2eULL}; //0x8e56bd293b2e8b2e9ad46f4ef7d0e89f [prime]
			Assert::IsTrue(IsRabinMillerPrime(anA));
		}

		TEST_METHOD(MultiWordPrimeTest4)
		{//Full multiword case, composite
			array<uint64_t, 2U> anB = {0x9ad46f4ef7d0e8a1ULL, 0x8e56bd293b2e8b2eULL}; //0x8e56bd293b2e8b2e9ad46f4ef7d0e8a1 [composite]
			Assert::IsTrue(!IsRabinMillerPrime(anB));
		}

		TEST_METHOD(MultiWordPrimeTest5)
		{//Multiword reusing single word cases, edge cases
			array<uint64_t, 2U> anA = {0x0000000000000000ULL, 0x0000000000000000ULL}; //0x0
			Assert::IsTrue(!IsRabinMillerPrime(anA));

			array<uint64_t, 2U> anB = {0x0000000000000001ULL, 0x0000000000000000ULL}; //0x1
			Assert::IsTrue(!IsRabinMillerPrime(anB));

			array<uint64_t, 2U> anC = {0x0000000000000001ULL, 0x0000000000000000ULL}; //0x2
			Assert::IsTrue(!IsRabinMillerPrime(anC));

			array<uint64_t, 2U> anD = {0x1000000000000000ULL, 0x0000000000000000ULL}; //0x3
			Assert::IsTrue(!IsRabinMillerPrime(anD));
		}

		TEST_METHOD(MultiWordPrimeTest6)
		{//Full multiword case, prime
			//0x911b43115f2e9b12ea2c945bb035c90386197fc5c2d0b95e458bfe05b73c55e9 [prime]
			array<uint64_t, 4U> anA = {0x458bfe05b73c55e9ULL, 0x86197fc5c2d0b95eULL, 0xea2c945bb035c903ULL, 0x911b43115f2e9b12ULL};
			Assert::IsTrue(IsRabinMillerPrime(anA));

			//0xa356d90fadfa7ddd06838e3eb3cc0a4f00bce711541a1677a87a57d1fda51d41 [prime]
			array<uint64_t, 4U> anB = {0xa87a57d1fda51d41ULL, 0x00bce711541a1677ULL, 0x06838e3eb3cc0a4fULL, 0xa356d90fadfa7dddULL};
			Assert::IsTrue(IsRabinMillerPrime(anB));

			//0x86b3f52876b7204297eaf6a4943acb09e1d7a402e74ac078f3aa3e0017318f79 [prime]
			array<uint64_t, 4U> anC = {0xf3aa3e0017318f79ULL, 0xe1d7a402e74ac078ULL, 0x97eaf6a4943acb09ULL, 0x86b3f52876b72042ULL};
			Assert::IsTrue(IsRabinMillerPrime(anC));

			//0xdc408b759fe8e9d6377e9848076ef25d51e8b84267a0bbad11ce8eb40e50396d [prime]
			array<uint64_t, 4U> anD = {0x11ce8eb40e50396dULL, 0x51e8b84267a0bbadULL, 0x377e9848076ef25dULL, 0xdc408b759fe8e9d6ULL};
			Assert::IsTrue(IsRabinMillerPrime(anD));

			//0xa8165a248e316eec8b01afbc02287af83b26a92fe6f1581b8f7692d1bbafdbd1 [prime]
			array<uint64_t, 4U> anE = {0x8f7692d1bbafdbd1ULL, 0x3b26a92fe6f1581bULL, 0x8b01afbc02287af8ULL, 0xa8165a248e316eecULL};
			Assert::IsTrue(IsRabinMillerPrime(anE));
		}

		TEST_METHOD(MultiWordPrimeTest7)
		{//Full multiword case, composite
			//0x6afa065208d49b90e52114ef9a86f95658bde8160ede7390a3322a10a9d35471 [composite]
			array<uint64_t, 4U> anA = {0xa3322a10a9d35471ULL, 0x58bde8160ede7390ULL, 0xe52114ef9a86f956ULL, 0x6afa065208d49b90ULL};
			Assert::IsTrue(!IsRabinMillerPrime(anA));

			//0x11ae838f1e7cf95e07600369c882fa98d8fc058dfebdc8a3d88616808a3a676a [composite]
			array<uint64_t, 4U> anB = {0xd88616808a3a676aULL, 0xd8fc058dfebdc8a3ULL, 0x07600369c882fa98ULL, 0x11ae838f1e7cf95eULL};
			Assert::IsTrue(!IsRabinMillerPrime(anB));

			//0x0c056a8c3bd742bd40d65dc1170c7a3922002db69c2d2bc3834dd0151df087a3 [composite]
			array<uint64_t, 4U> anC = {0x834dd0151df087a3ULL, 0x22002db69c2d2bc3ULL, 0x40d65dc1170c7a39ULL, 0x0c056a8c3bd742bdULL};
			Assert::IsTrue(!IsRabinMillerPrime(anC));

			//0xfe9e475554515aaeea59682595652b189b6bd14d184e141700446e198e1988b5 [composite]
			array<uint64_t, 4U> anD = {0x00446e198e1988b5ULL, 0x9b6bd14d184e1417ULL, 0xea59682595652b18ULL, 0xfe9e475554515aaeULL};
			Assert::IsTrue(!IsRabinMillerPrime(anD));

			//0xd861897dea788796fc73f764e4b9744d599ba2fae2965af27c71ae12b34a6fd7 [composite]
			array<uint64_t, 4U> anE = {0x7c71ae12b34a6fd7ULL, 0x599ba2fae2965af2ULL, 0xfc73f764e4b9744dULL, 0xd861897dea788796ULL};
			Assert::IsTrue(!IsRabinMillerPrime(anE));
		}
	};
}