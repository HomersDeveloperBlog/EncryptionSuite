#include "stdafx.h"
#include "CppUnitTest.h"

#include "BigBitArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BigBitArray_Test
{		
	TEST_CLASS(BigBitArray_Test)
	{
	public:
		TEST_METHOD(WordSize)
		{
			Assert::IsTrue(sizeof(uint64_t) == 8U);
		}

		TEST_METHOD(SignificantWord1)
		{
			array<uint64_t, 8U> anA = 
			{
				0x0000000000000000ULL, 0x22ca281bbb83825dULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
				0x0000000000000000ULL, 0x22ca281bbb83825dULL, 0x0000000000000000ULL, 0x0000000000000000ULL
			};

			unsigned int nMSWordAnswer = 5U;
			unsigned int nMSWord = EvalMostSignificantWord(anA);
			Assert::IsTrue(nMSWord == nMSWordAnswer);

			unsigned int nLSWordAnswer = 1U;
			unsigned int nLSWord = EvalLeastSignificantWord(anA);
			Assert::IsTrue(nLSWord == nLSWordAnswer);
		}

		TEST_METHOD(SignificantWord2)
		{
			array<uint64_t, 8U> anA = {0x0000000000000001ULL, 0x0000000000000000ULL};

			unsigned int nMSWordAnswer = 0U;
			unsigned int nMSWord = EvalMostSignificantWord(anA);
			Assert::IsTrue(nMSWord == nMSWordAnswer);

			unsigned int nLSWordAnswer = 0U;
			unsigned int nLSWord = EvalLeastSignificantWord(anA);
			Assert::IsTrue(nLSWord == nLSWordAnswer);
		}

		TEST_METHOD(SignificantBit1)
		{
			uint64_t nA = 0x22ca281bbb838240ULL;

			unsigned int nMSBitAnswer = 61U;
			unsigned int nMSBit = EvalMostSignificantBit(nA);
			Assert::IsTrue(nMSBit == nMSBitAnswer);

			unsigned int nLSBitAnswer = 6U;
			unsigned int nLSBit = EvalLeastSignificantBit(nA);
			Assert::IsTrue(nLSBit == nLSBitAnswer);
		}

		TEST_METHOD(RightWordShift1)
		{
			array<uint64_t, 8U> anA = 
			{
				0x22ca281bbb83825dULL, 0x22c6b6e166be9181ULL, 0x93a272b71668e0ebULL, 0x62dc764741c3ae08ULL, 
				0x6be2b66efb5d0647ULL, 0x38b70497c6b62adeULL, 0xe92742e1540e62e0ULL, 0xebd0f18634feec57ULL
			};

			array<uint64_t, 8U> anAnswer = 
			{
				0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 
				0x0000000000000000ULL, 0x22ca281bbb83825dULL, 0x22c6b6e166be9181ULL, 0x93a272b71668e0ebULL,
			};

			MultiWordIntegerRightShiftWordsInPlace(anA, 5U);
			Assert::IsTrue(anA == anAnswer);
		}
		
		TEST_METHOD(LeftWordShift1)
		{
			array<uint64_t, 8U> anA = 
			{
				0x22ca281bbb83825dULL, 0x22c6b6e166be9181ULL, 0x93a272b71668e0ebULL, 0x62dc764741c3ae08ULL, 
				0x6be2b66efb5d0647ULL, 0x38b70497c6b62adeULL, 0xe92742e1540e62e0ULL, 0xebd0f18634feec57ULL
			};

			array<uint64_t, 8U> anAnswer = 
			{
				0x93a272b71668e0ebULL, 0x62dc764741c3ae08ULL, 0x6be2b66efb5d0647ULL, 0x38b70497c6b62adeULL, 
				0xe92742e1540e62e0ULL, 0xebd0f18634feec57ULL, 0x0000000000000000ULL, 0x0000000000000000ULL
			};

			MultiWordIntegerLeftShiftWordsInPlace(anA, 2U);
			Assert::IsTrue(anA == anAnswer);
		}
		
		TEST_METHOD(RightBitShift1)
		{
			array<uint64_t, 4U> anA =      {0x22ca281bbb83825dULL, 0x22c6b6e166be9181ULL, 0x93a272b71668e0ebULL, 0x62dc764741c3ae08ULL};
			array<uint64_t, 4U> anAnswer = {0xca281bbb83825d00ULL, 0xc6b6e166be918122ULL, 0xa272b71668e0eb22ULL, 0xdc764741c3ae0893ULL};
			
			MultiWordIntegerRightShiftBitsInPlace(anA, 8U);
			Assert::IsTrue(anA == anAnswer);
		}

		TEST_METHOD(LeftBitShift1)
		{
			array<uint64_t, 4U> anA      = {0x22ca281bbb83825dULL, 0x22c6b6e166be9181ULL, 0x93a272b71668e0ebULL, 0x62dc764741c3ae08ULL};
			array<uint64_t, 4U> anAnswer = {0x8122ca281bbb8382ULL, 0xeb22c6b6e166be91ULL, 0x0893a272b71668e0ULL, 0x0062dc764741c3aeULL};
			
			MultiWordIntegerLeftShiftBitsInPlace(anA, 8U);
			Assert::IsTrue(anA == anAnswer);
		}

		TEST_METHOD(RightSingleBitShift1)
		{
			//0x2d581472ec8a70c429fc6ae04c6f1bfc563d52b53ead079c5e1bf1cb0a79fae7
			//0x5ab028e5d914e18853f8d5c098de37f8ac7aa56a7d5a0f38bc37e39614f3f5ce

			array<uint64_t, 4U> anA =      {0x5e1bf1cb0a79fae7ULL, 0x563d52b53ead079cULL, 0x29fc6ae04c6f1bfcULL, 0x2d581472ec8a70c4ULL};
			array<uint64_t, 4U> anAnswer = {0xbc37e39614f3f5ceULL, 0xac7aa56a7d5a0f38ULL, 0x53f8d5c098de37f8ULL, 0x5ab028e5d914e188ULL};

			MultiWordIntegerRightShiftByOneBitInPlace(anA);
			Assert::IsTrue(anA == anAnswer);
		}

		TEST_METHOD(LeftSingleBitShift1)
		{
			//0x2d581472ec8a70c429fc6ae04c6f1bfc563d52b53ead079c5e1bf1cb0a79fae7
			//0x16ac0a397645386214fe357026378dfe2b1ea95a9f5683ce2f0df8e5853cfd73

			array<uint64_t, 4U> anA =      {0x5e1bf1cb0a79fae7ULL, 0x563d52b53ead079cULL, 0x29fc6ae04c6f1bfcULL, 0x2d581472ec8a70c4ULL};
			array<uint64_t, 4U> anAnswer = {0x2f0df8e5853cfd73ULL, 0x2b1ea95a9f5683ceULL, 0x14fe357026378dfeULL, 0x16ac0a3976453862ULL};

			MultiWordIntegerLeftShiftByOneBitInPlace(anA);
			Assert::IsTrue(anA == anAnswer);
		}

		TEST_METHOD(LexographicComparison1)
		{
			array<uint64_t, 4U> anA = {0x5e1bf1cb0a79fae7ULL, 0x563d52b53ead079cULL, 0x0000000000000000ULL, 0x16ac0a3976453862ULL};
			array<uint64_t, 4U> anB = {0x5e1bf1cb0a79fae7ULL, 0x2b1ea95a9f5683ceULL, 0x0000000000000000ULL, 0x16ac0a3976453862ULL};

			unsigned int nComparisonIndex = EvalLexographicComparisonIndex(anA, anB);
			Assert::IsTrue(nComparisonIndex == 1U);

			Assert::IsTrue(EvalLexographicGreaterThanEqualTo(anA, anB));
			Assert::IsTrue(!EvalLexographicGreaterThanEqualTo(anB, anA));
			Assert::IsTrue(EvalLexographicGreaterThanEqualTo(anA, anA));

			Assert::IsTrue(EvalLexographicGreaterThan(anA, anB));
			Assert::IsTrue(!EvalLexographicGreaterThan(anB, anA));
			Assert::IsTrue(!EvalLexographicGreaterThan(anA, anA));

			Assert::IsTrue(!EvalLexographicLessThanEqualTo(anA, anB));
			Assert::IsTrue(EvalLexographicLessThanEqualTo(anB, anA));
			Assert::IsTrue(EvalLexographicLessThanEqualTo(anA, anA));

			Assert::IsTrue(!EvalLexographicLessThan(anA, anB));
			Assert::IsTrue(EvalLexographicLessThan(anB, anA));
			Assert::IsTrue(!EvalLexographicLessThan(anA, anA));

			Assert::IsTrue(EvalLexographicNotEqualTo(anA, anB));
			Assert::IsTrue(!EvalLexographicNotEqualTo(anA, anA));
			Assert::IsTrue(EvalLexographicEqualTo(anA, anA));
			Assert::IsTrue(!EvalLexographicEqualTo(anA, anB));
		}

		TEST_METHOD(LexographicComparison2)
		{
			array<uint64_t, 4U> anA = {0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};
			array<uint64_t, 4U> anB = {0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x00000a3976450002ULL};
			array<uint64_t, 4U> anC = {0x00000a3976450002ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL};

			unsigned int nComparisonIndex = EvalLexographicComparisonIndex(anA, anA);
			Assert::IsTrue(nComparisonIndex == 0U);

			nComparisonIndex = EvalLexographicComparisonIndex(anA, anB);
			Assert::IsTrue(nComparisonIndex == 3U);

			nComparisonIndex = EvalLexographicComparisonIndex(anA, anC);
			Assert::IsTrue(nComparisonIndex == 0U);

			Assert::IsTrue(EvalLexographicEqualToZero(anA));
			Assert::IsTrue(!EvalLexographicEqualToZero(anB));
		}
	};
}