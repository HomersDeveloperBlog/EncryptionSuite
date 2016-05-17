#include "stdafx.h"
#include "CppUnitTest.h"

#include "BigModularArithmetic.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BigModularArithmetic_Test
{
	TEST_CLASS(BigModularArithmetic_Test)
	{
	public:
		TEST_METHOD(MultiWordModularExponentiation1)
		{
			array<uint64_t, 4U> anA = {0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000001ULL};
			array<uint64_t, 4U> anB = {0x0000000000000010ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			array<uint64_t, 4U> anC = {0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000001ULL};
			array<uint64_t, 4U> anAnswer = {0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			
			MultiWordIntegerModularExponentiationInPlace(anA, anB, anC);
			Assert::IsTrue(anA == anAnswer);
		}

		TEST_METHOD(MultiWordModularExponentiation2)
		{//Squaring, no modular reduction.
			array<uint64_t, 4U> anA = {0xa7040737b3e82214ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			array<uint64_t, 4U> anB = {0x0000000000000002ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			array<uint64_t, 4U> anC = {0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL};
			array<uint64_t, 4U> anAnswer = {0xa6aa2ebdc0c95190ULL, 0x6cf6417ae6aa93acULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			
			MultiWordIntegerModularExponentiationInPlace(anA, anB, anC);
			Assert::IsTrue(anA == anAnswer);
		}

		TEST_METHOD(MultiWordModularExponentiation3)
		{//Small integer base / exponent, no modular reduction.
			array<uint64_t, 4U> anA = {0x0000000000000002ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			array<uint64_t, 4U> anB = {0x0000000000000080ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			array<uint64_t, 4U> anC = {0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL};
			array<uint64_t, 4U> anAnswer = {0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000001ULL, 0x0000000000000000ULL};
			
			MultiWordIntegerModularExponentiationInPlace(anA, anB, anC);
			Assert::IsTrue(anA == anAnswer);
		}
		
		TEST_METHOD(MultiWordModularExponentiation4)
		{//Small integer base / exponent, no modular reduction.
			array<uint64_t, 4U> anA = {0x0000000000000003ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			array<uint64_t, 4U> anB = {0x000000000000008fULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			array<uint64_t, 4U> anC = {0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL};
			
			//0x00000006466cb651c73dda5aace9d78a59164677dd5244858950fd9bea63f06b
			array<uint64_t, 4U> anAnswer = {0x8950fd9bea63f06bULL, 0x59164677dd524485ULL, 0xc73dda5aace9d78aULL, 0x00000006466cb651ULL};
			
			MultiWordIntegerModularExponentiationInPlace(anA, anB, anC);
			Assert::IsTrue(anA == anAnswer);
		}

		TEST_METHOD(MultiWordModularExponentiation5)
		{//Small integer base / exponent, modular reduction only after the last multiplication (once)
			array<uint64_t, 4U> anA = {0x0000000000000003ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			array<uint64_t, 4U> anB = {0x000000000000008fULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			array<uint64_t, 4U> anC = {0x6cf6417ae6aa93acULL, 0xc1520cb661487cb3ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			
			//0x39f34e7a8e1b09834f2f8ea270c01397
			array<uint64_t, 4U> anAnswer = {0x4f2f8ea270c01397ULL, 0x39f34e7a8e1b0983ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			
			MultiWordIntegerModularExponentiationInPlace(anA, anB, anC);
			Assert::IsTrue(anA == anAnswer);
		}

		TEST_METHOD(MultiWordModularExponentiation6)
		{//Squaring with resulting size exceeding input. one trivial modular reduction.
			array<uint64_t, 2U> anA = {0xa6aa2ebdc0c95190ULL, 0x6cf6417ae6aa93acULL}; //0x6cf6417ae6aa93aca6aa2ebdc0c95190
			array<uint64_t, 2U> anB = {0x0000000000000002ULL, 0x0000000000000000ULL}; //0x2
			array<uint64_t, 2U> anC = {0x0000000000000000ULL, 0x0000000000000001ULL}; //0x0000000000000001 0000000000000000
			array<uint64_t, 2U> anAnswer = {0xa700c7492e1c7100ULL, 0x0000000000000000ULL};
			
			MultiWordIntegerModularExponentiationInPlace(anA, anB, anC);
			Assert::IsTrue(anA == anAnswer);
		}

		TEST_METHOD(MultiWordModularExponentiation7)
		{//Cubing with resulting size exceeding input twice, two trivial modular reductions.
			array<uint64_t, 2U> anA = {0xa6aa2ebdc0c95190ULL, 0x6cf6417ae6aa93acULL}; //0x6cf6417ae6aa93aca6aa2ebdc0c95190
			array<uint64_t, 2U> anB = {0x0000000000000003ULL, 0x0000000000000000ULL}; //0x2
			array<uint64_t, 2U> anC = {0x0000000000000000ULL, 0x0000000000000001ULL}; //0x0000000000000001 0000000000000000
			array<uint64_t, 2U> anAnswer = {0xbbd636d5a8c09000ULL, 0x0000000000000000ULL};
			
			MultiWordIntegerModularExponentiationInPlace(anA, anB, anC);
			Assert::IsTrue(anA == anAnswer);
		}

		TEST_METHOD(MultiWordModularExponentiation8)
		{ //message, public exponent, n = pq, p,q are prime, modulus
			array<uint64_t, 4U> anA = {0x044d78bf180e5692ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL}; //0x044d78bf180e5692
			array<uint64_t, 4U> anB = {0x0000000000010001ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL}; //0x10001
			array<uint64_t, 4U> anC = {0xcdc20e19f289f6afULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL}; //0xcdc20e19f289f6af
			array<uint64_t, 4U> anAnswer = {0x513aa3e9d41dea67ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL}; //0x513aa3e9d41dea67
			
			MultiWordIntegerModularExponentiationInPlace(anA, anB, anC);
			Assert::IsTrue(anA == anAnswer);
		}

		TEST_METHOD(MultiWordModularExponentiation9)
		{
			//0xc01b1f26e3e5f46c96dbe96d18952ad4ed7ac14d9ab9a79fa7040737b3e82214
			array<uint64_t, 4U> anA = {0xa7040737b3e82214ULL, 0xed7ac14d9ab9a79fULL, 0x96dbe96d18952ad4ULL, 0xc01b1f26e3e5f46cULL};
			
			//0xc5ad1731ccf8b5ccda8cbe3f6a3f94af977a9c8b61aa09552eb39905e0307811
			array<uint64_t, 4U> anB = {0x2eb39905e0307811ULL, 0x977a9c8b61aa0955ULL, 0xda8cbe3f6a3f94afULL, 0xc5ad1731ccf8b5ccULL};
			
			//0xc7e02cb8452b12ac6da7fbfe0a9a9291a1563a0370da47d24caaa30edc50de3f
			array<uint64_t, 4U> anC = {0x4caaa30edc50de3fULL, 0xa1563a0370da47d2ULL, 0x6da7fbfe0a9a9291ULL, 0xc7e02cb8452b12acULL};

			//0xb5923b4896d0611ed24686d328b13cedec0b86b92a259244838b69ff60a41226
			array<uint64_t, 4U> anAnswer = {0x838b69ff60a41226ULL, 0xec0b86b92a259244ULL, 0xd24686d328b13cedULL, 0xb5923b4896d0611eULL};
			
			MultiWordIntegerModularExponentiationInPlace(anA, anB, anC);
			Assert::IsTrue(anA == anAnswer);
		}

		TEST_METHOD(SingleWordGCD1)
		{
			uint64_t anA = 80ULL;
			uint64_t anB = 60ULL;
			uint64_t anAnswer = 20ULL;
			
			uint64_t anC = SingleWordIntegerGreatestCommonDivisor(anA, anB);
			Assert::IsTrue(anC == anAnswer);
		}

		TEST_METHOD(SingleWordGCD2)
		{
			uint64_t anA = 39088169ULL; //Fibonacci numbers
			uint64_t anB = 24157817ULL;
			uint64_t anAnswer = 1ULL; //Fibonnaci numbers are coprime
			
			uint64_t anC = SingleWordIntegerGreatestCommonDivisor(anA, anB);
			Assert::IsTrue(anC == anAnswer);
		}

		TEST_METHOD(SingleWordGCD3)
		{
			uint64_t anA = 0xd7b67dee9ecde453ULL; //Big random number
			uint64_t anB = 0xd5da5d8ba5f95d42ULL;
			uint64_t anAnswer = 0x0000000000000003ULL;
			
			uint64_t anC = SingleWordIntegerGreatestCommonDivisor(anA, anB);
			Assert::IsTrue(anC == anAnswer);
		}

		TEST_METHOD(SingleWordGCD4)
		{
			uint64_t anA = 0xd5da5d8ba5f95d42ULL;
			uint64_t anB = 0xd7b67dee9ecde453ULL;
			uint64_t anAnswer = 0x0000000000000003ULL;
			
			uint64_t anC = SingleWordIntegerGreatestCommonDivisor(anA, anB);
			Assert::IsTrue(anC == anAnswer);
		}

		TEST_METHOD(SingleWordModularInverse1)
		{
			uint64_t anA = 60ULL;
			uint64_t anB = 83ULL; //prime
			uint64_t anAnswer = 18ULL;
			
			uint64_t anC = SingleWordIntegerModularInverse(anA, anB);
			Assert::IsTrue((anC * anA) % anB == 1ULL);
			Assert::IsTrue(anC == anAnswer);
		}

		TEST_METHOD(SingleWordModularInverse2)
		{
			uint64_t anA = 0x0000000000000025ULL; //prime
			uint64_t anB = 0x0000000000000b89ULL; //prime
			uint64_t anAnswer = 0x000000000000004fdULL;
			
			uint64_t anC = SingleWordIntegerModularInverse(anA, anB);
			Assert::IsTrue((anC * anA) % anB == 1ULL);
			Assert::IsTrue(anC == anAnswer);
		}

		TEST_METHOD(SingleWordModularInverse3)
		{
			uint64_t anA = 0x0000000000000777ULL;
			uint64_t anB = 0x0000000000000b89ULL; //prime
			uint64_t anAnswer = 0x00000000000008abULL;
			
			//In this case, x_current (==anB) - x_last is the appropriate solution
			uint64_t anC = SingleWordIntegerModularInverse(anA, anB);
			Assert::IsTrue((anC * anA) % anB == 1ULL);
			Assert::IsTrue(anC == anAnswer);
		}

		TEST_METHOD(SingleWordModularInverse4)
		{
			uint64_t anA = 0x0000000000000779ULL; //prime
			uint64_t anB = 0x0000000000000b89ULL; //prime
			uint64_t anAnswer = 0x0000000000000185ULL;
			
			//In this case, x_current (==anB) - x_last is the appropriate solution
			uint64_t anC = SingleWordIntegerModularInverse(anA, anB);
			Assert::IsTrue((anC * anA) % anB == 1ULL);
			Assert::IsTrue(anC == anAnswer);
		}

		TEST_METHOD(SingleWordModularInverse5)
		{
			uint64_t anA = 0x0000000000000065ULL;
			uint64_t anB = 0x00000000000002cfULL; //prime
			uint64_t anAnswer = 0x000000000000012bULL;
			
			uint64_t anC = SingleWordIntegerModularInverse(anA, anB);
			Assert::IsTrue((anC * anA) % anB == 1ULL);
			Assert::IsTrue(anC == anAnswer);
		}

		TEST_METHOD(SingleWordModularInverse6)
		{
			uint64_t anA = 0x0000000001709e79ULL; //Fibonnaci numbers
			uint64_t anB = 0x0000000002547029ULL; //not prime
			uint64_t anAnswer = anA;

			uint64_t anC = SingleWordIntegerModularInverse(anA, anB);
			Assert::IsTrue((anC * anA) % anB == 1ULL);
			Assert::IsTrue(anC == anAnswer);
		}

		TEST_METHOD(SingleWordModularInverse7)
		{
			uint64_t anA = 0x0000000001709e79ULL;
			uint64_t anB = 0x0000000002547041ULL; //prime
			uint64_t anAnswer = 0x0000000001d0c3e2ULL;

			uint64_t anC = SingleWordIntegerModularInverse(anA, anB);
			Assert::IsTrue((anC * anA) % anB == 1ULL);
			Assert::IsTrue(anC == anAnswer);
		}

		TEST_METHOD(MultiWordModularInverse1)
		{ //A single word example, but presented with multiple words.
			array<uint64_t, 4U> anA = {0x0000000001709e79ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			array<uint64_t, 4U> anB = {0x0000000002547041ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			array<uint64_t, 4U> anAnswer = {0x0000000001d0c3e2ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};

			array<uint64_t, 4U> anC = MultiWordIntegerModularInverse(anA, anB);
			Assert::IsTrue(anC == anAnswer);

			//Verification of inverse.
			array<uint64_t, 4U> anProductAnswer = {0x0000000000000001ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			array<uint64_t, 4U> anProduct = MultiWordIntegerChangeLength<4U>(MultiWordIntegerMultiply(anC, anA));
			MultiWordIntegerModularReductionInPlace(anProduct, anB);
			Assert::IsTrue(anProduct == anProductAnswer);
		}

		TEST_METHOD(MultiWordModularInverse2)
		{
			//0x71779ae0e56a038a0df530d0a959ce532afdfe86e9a3ee393b56229d8ded0f65
			array<uint64_t, 4U> anA = {0x3b56229d8ded0f65ULL, 0x2afdfe86e9a3ee39ULL, 0x0df530d0a959ce53ULL, 0x71779ae0e56a038aULL};
			
			//0x77e123c061e4c1e7b8a25eac0b245ac40a7f60282736e54228b61315b6aa836d
			array<uint64_t, 4U> anB = {0x28b61315b6aa836d, 0x0a7f60282736e542,	0xb8a25eac0b245ac4,	0x77e123c061e4c1e7};
			
			//0x1382c004c2f02efee4af5b59a35e479130a99ec3f90d086304dbeaaaf0e2876d
			array<uint64_t, 4U> anAnswer = {0x04dbeaaaf0e2876d, 0x30a99ec3f90d0863,	0xe4af5b59a35e4791,	0x1382c004c2f02efe};

			array<uint64_t, 4U> anC = MultiWordIntegerModularInverse(anA, anB);
			Assert::IsTrue(anC == anAnswer);

			//Verification of inverse.
			array<uint64_t, 4U> anProductAnswer = {0x0000000000000001ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			array<uint64_t, 4U> anProduct = MultiWordIntegerChangeLength<4U>(MultiWordIntegerMultiply(anC, anA));
			MultiWordIntegerModularReductionInPlace(anProduct, anB);
			Assert::IsTrue(anProduct == anProductAnswer);
		}
	};
}