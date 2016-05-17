//Special thanks to the author of //https://birrell.org/andrew/ratcalc/ for giving me so many good test cases.

#include "stdafx.h"
#include "CppUnitTest.h"

#include "BigUnsignedInteger.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BigUnsignedInteger_Test
{		
	TEST_CLASS(BigUnsignedInteger_Test)
	{
	public:
		TEST_METHOD(SingleWordAdd1)
		{
			uint64_t nA = 0ULL;
			uint64_t nB = 0ULL;

			Assert::IsTrue(SingleWordIntegerAddWithCarryInPlace(nA, nB) == 0ULL);
			Assert::IsTrue(nA == 0ULL);
		}

		TEST_METHOD(SingleWordAdd2)
		{
			uint64_t nA = 0x8000000000000000ULL;
			uint64_t nB = 0x8000000000000000ULL;

			Assert::IsTrue(SingleWordIntegerAddWithCarryInPlace(nA, nB) == 1ULL);
			Assert::IsTrue(nA == 0ULL);
		}

		TEST_METHOD(MultiWordAdd1)
		{
			//0xebd0f18634feec57e92742e1540e62e038b70497c6b62ade6be2b66efb5d064762dc764741c3ae0893a272b71668e0eb22c6b6e166be918122ca281bbb83825d
			array<uint64_t, 8U> anA = 
			{
				0x22ca281bbb83825dULL, 0x22c6b6e166be9181ULL, 0x93a272b71668e0ebULL, 0x62dc764741c3ae08ULL, 
				0x6be2b66efb5d0647ULL, 0x38b70497c6b62adeULL, 0xe92742e1540e62e0ULL, 0xebd0f18634feec57ULL
			};

			//0x291e9f62f6e3c8a174facf657cf49fdefe7edeee39276aa82a9bd18f7657a0c50728469d5fc4ffc5ef7e2d77254381eccd2f67c14de1e12782bcf3db2dc8c237
			array<uint64_t, 8U> anB = 
			{
				0x82bcf3db2dc8c237ULL, 0xcd2f67c14de1e127ULL, 0xef7e2d77254381ecULL, 0x0728469d5fc4ffc5ULL, 
				0x2a9bd18f7657a0c5ULL, 0xfe7edeee39276aa8ULL, 0x74facf657cf49fdeULL, 0x291e9f62f6e3c8a1ULL
			};

			//0x114ef90e92be2b4f95e221246d10302bf3735e385ffdd9586967e87fe71b4a70c6a04bce4a188adce8320a02e3bac62d7eff61ea2b4a072a8a5871bf6e94c4494
			array<uint64_t, 8U> anAnswer = 
			{
				0xa5871bf6e94c4494ULL, 0xeff61ea2b4a072a8ULL, 0x8320a02e3bac62d7ULL, 0x6a04bce4a188adceULL, 
				0x967e87fe71b4a70cULL, 0x3735e385ffdd9586ULL, 0x5e221246d10302bfULL, 0x14ef90e92be2b4f9ULL
			};

			uint64_t nCarry = MultiWordIntegerAddInPlace(anA, anB);
			Assert::IsTrue(nCarry == 1ULL);
			Assert::IsTrue(anA == anAnswer);
		}

		TEST_METHOD(MultiWordAddOne1)
		{
			array<uint64_t, 4U> anA =      {0xffffffffffffffffULL, 0xffffffffffffffffULL, 0x00000000ffffffffULL, 0x0000000000000000ULL};
			array<uint64_t, 4U> anAnswer = {0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000100000000ULL, 0x0000000000000000ULL};
			
			MultiWordIntegerAddOneInPlace(anA);
			Assert::IsTrue(anA == anAnswer);
		}

		TEST_METHOD(MultiWordAddOne2)
		{
			array<uint64_t, 2U> anA =      {0xffffffffffffffffULL, 0xffffffffffffffffULL};
			array<uint64_t, 2U> anAnswer = {0x0000000000000000ULL, 0x0000000000000000ULL};
			
			MultiWordIntegerAddOneInPlace(anA);
			Assert::IsTrue(anA == anAnswer);
		}

		TEST_METHOD(MultiWordSubtract1)
		{
			//0xebd0f18634feec57e92742e1540e62e038b70497c6b62ade6be2b66efb5d064762dc764741c3ae0893a272b71668e0eb22c6b6e166be918122ca281bbb83825d
			array<uint64_t, 8U> anA = 
			{
				0x22ca281bbb83825dULL, 0x22c6b6e166be9181ULL, 0x93a272b71668e0ebULL, 0x62dc764741c3ae08ULL, 
				0x6be2b66efb5d0647ULL, 0x38b70497c6b62adeULL, 0xe92742e1540e62e0ULL, 0xebd0f18634feec57ULL
			};
			
			//0x291e9f62f6e3c8a174facf657cf49fdefe7edeee39276aa82a9bd18f7657a0c50728469d5fc4ffc5ef7e2d77254381eccd2f67c14de1e12782bcf3db2dc8c237
			array<uint64_t, 8U> anB = 
			{
				0x82bcf3db2dc8c237ULL, 0xcd2f67c14de1e127ULL, 0xef7e2d77254381ecULL, 0x0728469d5fc4ffc5ULL, 
				0x2a9bd18f7657a0c5ULL, 0xfe7edeee39276aa8ULL, 0x74facf657cf49fdeULL, 0x291e9f62f6e3c8a1ULL
			};

			//0xc2b252233e1b23b6742c737bd719c3013a3825a98d8ec0364146e4df850565825bb42fa9e1feae42a424453ff1255efe55974f2018dcb059a00d34408dbac026
			array<uint64_t, 8U> anAnswer =
			{
				0xa00d34408dbac026ULL, 0x55974f2018dcb059ULL, 0xa424453ff1255efeULL, 0x5bb42fa9e1feae42ULL, 
				0x4146e4df85056582ULL, 0x3a3825a98d8ec036ULL, 0x742c737bd719c301ULL, 0xc2b252233e1b23b6ULL
			};

			uint64_t nCarry = MultiWordIntegerSubtractInPlace(anA, anB);
			Assert::IsTrue(nCarry == 0ULL);
			Assert::IsTrue(anA == anAnswer);
		}

		TEST_METHOD(MultiWordSubtract2)
		{
			//0x291e9f62f6e3c8a174facf657cf49fdefe7edeee39276aa82a9bd18f7657a0c50728469d5fc4ffc5ef7e2d77254381eccd2f67c14de1e12782bcf3db2dc8c237
			array<uint64_t, 8U> anA = 
			{
				0x82bcf3db2dc8c237ULL, 0xcd2f67c14de1e127ULL, 0xef7e2d77254381ecULL, 0x0728469d5fc4ffc5ULL, 
				0x2a9bd18f7657a0c5ULL, 0xfe7edeee39276aa8ULL, 0x74facf657cf49fdeULL, 0x291e9f62f6e3c8a1ULL
			};
			
			//0xebd0f18634feec57e92742e1540e62e038b70497c6b62ade6be2b66efb5d064762dc764741c3ae0893a272b71668e0eb22c6b6e166be918122ca281bbb83825d
			array<uint64_t, 8U> anB = 
			{
				0x22ca281bbb83825dULL, 0x22c6b6e166be9181ULL, 0x93a272b71668e0ebULL, 0x62dc764741c3ae08ULL, 
				0x6be2b66efb5d0647ULL, 0x38b70497c6b62adeULL, 0xe92742e1540e62e0ULL, 0xebd0f18634feec57ULL 
			};
			
			//0x-c2b252233e1b23b6742c737bd719c3013a3825a98d8ec0364146e4df850565825bb42fa9e1feae42a424453ff1255efe55974f2018dcb059a00d34408dbac026
			//= 0x3d4daddcc1e4dc498bd38c8428e63cfec5c7da5672713fc9beb91b207afa9a7da44bd0561e0151bd5bdbbac00edaa101aa68b0dfe7234fa65ff2cbbf72453fda
			array<uint64_t, 8U> anAnswer =
			{
				0x5ff2cbbf72453fdaULL, 0xaa68b0dfe7234fa6ULL, 0x5bdbbac00edaa101ULL, 0xa44bd0561e0151bdULL, 
				0xbeb91b207afa9a7dULL, 0xc5c7da5672713fc9ULL, 0x8bd38c8428e63cfeULL, 0x3d4daddcc1e4dc49ULL
			};

			uint64_t nCarry = MultiWordIntegerSubtractInPlace(anA, anB);
			Assert::IsTrue(nCarry == 1ULL);
			Assert::IsTrue(anA == anAnswer);
		}

		TEST_METHOD(MultiWordSubtract3)
		{
			array<uint64_t, 2U> anA =      {0x00000000ffffffffULL, 0xffffffff00000000ULL};
			array<uint64_t, 2U> anB =      {0x0000000000000000ULL, 0x8000000000000000ULL};
			array<uint64_t, 2U> anAnswer = {0x00000000ffffffffULL, 0x7fffffff00000000ULL};
			
			uint64_t nCarry = MultiWordIntegerSubtractInPlace(anA, anB);
			Assert::IsTrue(nCarry == 0ULL);
			Assert::IsTrue(anA == anAnswer);
		}

		TEST_METHOD(SingleWordMultiply1)
		{
			uint64_t nA = 0x1111111111111111ULL;
			uint64_t nB = 0x1111111111111111ULL;
			
			tuple<uint64_t, uint64_t> anC = SingleWordIntegerMultiplyIntoDoubleWord(nA, nB);

			tuple<uint64_t, uint64_t> anAnswer = make_tuple(0x123456789abcdf0ULL, 0x0fedcba987654321ULL); //0x123456789abcdf00fedcba987654321
			Assert::IsTrue(anC == anAnswer);
		}

		TEST_METHOD(SingleWordMultiply2)
		{
			uint64_t nA = 0x1111111111111111ULL;
			uint64_t nB = 0ULL;
			
			tuple<uint64_t, uint64_t> anC = SingleWordIntegerMultiplyIntoDoubleWord(nA, nB);

			tuple<uint64_t, uint64_t> anAnswer = make_tuple(0ULL, 0ULL);
			Assert::IsTrue(anC == anAnswer); 
		}
		
		TEST_METHOD(SingleWordMultiply3)
		{
			uint64_t nA = 0xffffffffffffffffULL;
			uint64_t nB = 0xffffffffffffffffULL;
			
			tuple<uint64_t, uint64_t> anC = SingleWordIntegerMultiplyIntoDoubleWord(nA, nB);

			tuple<uint64_t, uint64_t> anAnswer = make_tuple(0xfffffffffffffffeULL, 0x0000000000000001ULL); //0xfffffffffffffffe0000000000000001
			Assert::IsTrue(anC == anAnswer);
		}

		TEST_METHOD(SingleWordMultiply4)
		{
			uint64_t nA = 0x955cbdff5a77e2dbULL;
			uint64_t nB = 0x3af0d94c82f366b4ULL;
			
			tuple<uint64_t, uint64_t> anC = SingleWordIntegerMultiplyIntoDoubleWord(nA, nB);

			tuple<uint64_t, uint64_t> anAnswer = make_tuple(0x226388c63ae0f0b8ULL, 0xc470129aec8fc3fcULL); //0x226388c63ae0f0b8c470129aec8fc3fc
			Assert::IsTrue(anC == anAnswer); 
		}

		TEST_METHOD(MultiWordMultiply1)
		{
			array<uint64_t, 2U> anA = {1U, 1U};
			array<uint64_t, 2U> anB = {1U, 1U};
			array<uint64_t, 4U> anAnswer = {1U, 2U, 1U, 0U};

			array<uint64_t, 4U> anC = MultiWordIntegerMultiply(anA, anB);
			Assert::IsTrue(anC == anAnswer);
		}

		TEST_METHOD(MultiWordMultiply2)
		{
			array<uint64_t, 3U> anA = {1U, 0U, 5U};
			array<uint64_t, 3U> anB = {99U, 19U, 9U};
			array<uint64_t, 6U> anAnswer = {99U, 19U, 504U, 95U, 45U, 0U};

			array<uint64_t, 6U> anC = MultiWordIntegerMultiply(anA, anB);
			Assert::IsTrue(anC == anAnswer);
		}
		
		TEST_METHOD(MultiWordMultiply3)
		{
			array<uint64_t, 1U> anA = {0x1111111111111111ULL};
			array<uint64_t, 1U> anB = {0x1111111111111111ULL};
			array<uint64_t, 2U> anAnswer = {0x0fedcba987654321ULL, 0x123456789abcdf0ULL}; //0x123456789abcdf00fedcba987654321

			array<uint64_t, 2U> anC = MultiWordIntegerMultiply(anA, anB);
			Assert::IsTrue(anC == anAnswer);
		}

		TEST_METHOD(MultiWordMultiply4)
		{
			//0x7fe5cc27c0e7f6f339a9b461758f24e7c68a0ba50f23630679f611c966e3898b2fd3e1aacdb1e04ddeb53bbc4274a3aea4677419d1f832c72c4ec06155f59e12dfa52dccc535ead844311fbdfb6511fcfbad479648ec5847a5c9ca11572d1639c946e0d9d59e7089fb52746848cb90e284f142c8418a21051904ba31e735b17f
			array<uint64_t, 16U> anA = 
			{
				0x1904ba31e735b17fULL, 0x84f142c8418a2105ULL, 0xfb52746848cb90e2ULL, 0xc946e0d9d59e7089ULL, 
				0xa5c9ca11572d1639ULL, 0xfbad479648ec5847ULL, 0x44311fbdfb6511fcULL, 0xdfa52dccc535ead8ULL, 
				0x2c4ec06155f59e12ULL, 0xa4677419d1f832c7ULL, 0xdeb53bbc4274a3aeULL, 0x2fd3e1aacdb1e04dULL, 
				0x79f611c966e3898bULL, 0xc68a0ba50f236306ULL, 0x39a9b461758f24e7ULL, 0x7fe5cc27c0e7f6f3ULL
			};

			//0xa35553ab6aeb02b7e800bb5503c5a7c95c46ae4f6abed636a65f6ead2945b50f170e2be2a77385d5863a3f164b0533f491865af6826cc3f17ebb4959fc91c78ca4021545c1817d27484207eb4e9fad7a2ce9ddfead62cea83cc561e58f8fbaf99d5621a2512f88fa022d2e86f019904fbd5c37adfb7998d465e0152be13b8b1d
			array<uint64_t, 16U> anB = 
			{
				0x65e0152be13b8b1dULL, 0xbd5c37adfb7998d4ULL, 0x022d2e86f019904fULL, 0x9d5621a2512f88faULL, 
				0x3cc561e58f8fbaf9ULL, 0x2ce9ddfead62cea8ULL, 0x484207eb4e9fad7aULL, 0xa4021545c1817d27ULL, 
				0x7ebb4959fc91c78cULL, 0x91865af6826cc3f1ULL, 0x863a3f164b0533f4ULL, 0x170e2be2a77385d5ULL, 
				0xa65f6ead2945b50fULL, 0x5c46ae4f6abed636ULL, 0xe800bb5503c5a7c9ULL, 0xa35553ab6aeb02b7ULL
			};

			//0x5199f2173e21913177a00314f51560ff3e0a40cc6aeecf60b51b12501558a0e73498dfcd63aba4b3efe21b336ade66c1944d3e7cc44e5bc59d15688ba2d887066dd5ad2ed1f7c48b97c9ad76a67ac09fcdb8fec0072e8ddf5cd2693928b4a3f145825671e769111e2e4ec191adcb5b2797c66f29fb630926a0ffe590690c89504e4074c9b4521c8c9ca90e277216638de02daf1d175cc98de3867a63d6b42362668a149d5e203c9eaa9da17a8a2087f2e59126b9e789a6e5937deae7254d6f687a4a057497721e123b45527af8103038b173e7963c0271115d95415bc747919c90d4eff7bc92f3d0da8a9bf9576fe6d77d6582787a92af6dd40ce5e6dfba1063
			array<uint64_t, 32U> anAnswer = 
			{
				0xd40ce5e6dfba1063ULL, 0x7d6582787a92af6dULL, 0xda8a9bf9576fe6d7ULL, 0x90d4eff7bc92f3d0ULL, 
				0x5d95415bc747919cULL, 0xb173e7963c027111ULL, 0x3b45527af8103038ULL, 0x7a4a057497721e12ULL, 
				0x937deae7254d6f68ULL, 0xe59126b9e789a6e5ULL, 0xaa9da17a8a2087f2ULL, 0x668a149d5e203c9eULL, 
				0xe3867a63d6b42362ULL, 0xe02daf1d175cc98dULL, 0x9ca90e277216638dULL, 0x4e4074c9b4521c8cULL, 
				0xa0ffe590690c8950ULL, 0x97c66f29fb630926ULL, 0x2e4ec191adcb5b27ULL, 0x45825671e769111eULL, 
				0x5cd2693928b4a3f1ULL, 0xcdb8fec0072e8ddfULL, 0x97c9ad76a67ac09fULL, 0x6dd5ad2ed1f7c48bULL, 
				0x9d15688ba2d88706ULL, 0x944d3e7cc44e5bc5ULL, 0xefe21b336ade66c1ULL, 0x3498dfcd63aba4b3ULL, 
				0xb51b12501558a0e7ULL, 0x3e0a40cc6aeecf60ULL, 0x77a00314f51560ffULL, 0x5199f2173e219131ULL
			};

			array<uint64_t, 32U> anC = MultiWordIntegerMultiply(anA, anB); 
			Assert::IsTrue(anC == anAnswer);
		}

		TEST_METHOD(MultiWordModularDivision1)
		{
			array<uint64_t, 2U> anA = {0x00000000ffffffffULL, 0xffffffff00000000ULL}; //0xffffffff0000000000000000ffffffff
			array<uint64_t, 2U> anB = {0x0000000100000000ULL, 0x0000000000000000ULL}; //0x0000000100000000
			array<uint64_t, 2U> anQ = {0x0000000000000000ULL, 0x00000000ffffffffULL}; //0x00000000ffffffff0000000000000000
			array<uint64_t, 2U> anR = {0x00000000ffffffffULL, 0x0000000000000000ULL}; //0x00000000ffffffff
			
			tuple<array<uint64_t, 2U>, array<uint64_t, 2U>> anAnswer = MultiWordIntegerDivision(anA, anB);
			Assert::IsTrue(make_tuple(anQ, anR) == anAnswer);
		}

		TEST_METHOD(MultiWordModularDivision2)
		{
			array<uint64_t, 2U> anA = {0x9681fb13eac8839cULL, 0x7dc97ffd7e069bd2ULL}; //0x7dc97ffd7e069bd29681fb13eac8839c
			array<uint64_t, 2U> anB = {0x654327951065d244ULL, 0x0000000000000000ULL}; //0x654327951065d244
			array<uint64_t, 2U> anQ = {0x3e0034a388285fbcULL, 0x0000000000000001ULL}; //0x00000000000000013e0034a388285fbc
			array<uint64_t, 2U> anR = {0x55f53f9d1c5addacULL, 0x0000000000000000ULL}; //0x55f53f9d1c5addac
			
			tuple<array<uint64_t, 2U>, array<uint64_t, 2U>> anAnswer = MultiWordIntegerDivision(anA, anB);
			Assert::IsTrue(make_tuple(anQ, anR) == anAnswer);
		}

		TEST_METHOD(MultiWordModularDivision3)
		{ //zero quotient
			array<uint64_t, 2U> anA = {0x654327951065d244ULL, 0x0000000000000000ULL}; //0x654327951065d244
			array<uint64_t, 2U> anB = {0x9681fb13eac8839cULL, 0x7dc97ffd7e069bd2ULL}; //0x7dc97ffd7e069bd29681fb13eac8839c
			array<uint64_t, 2U> anQ = {0x0000000000000000ULL, 0x0000000000000000ULL}; //0x00000000000000013e0034a388285fbc
			array<uint64_t, 2U> anR = {0x654327951065d244ULL, 0x0000000000000000ULL}; //0x55f53f9d1c5addac
			
			tuple<array<uint64_t, 2U>, array<uint64_t, 2U>> anAnswer = MultiWordIntegerDivision(anA, anB);
			Assert::IsTrue(make_tuple(anQ, anR) == anAnswer);
		}

		TEST_METHOD(MultiWordModularDivision4)
		{
			array<uint64_t, 2U> anA = {0xc0ce7f086fbe831fULL, 0x0000000000000004ULL}; //0x0000000000000004c0ce7f086fbe831f
			array<uint64_t, 2U> anB = {0xadd45b4a591b374cULL, 0x0000000000000000ULL}; //0xadd45b4a591b374c
			array<uint64_t, 2U> anQ = {0x0000000000000007ULL, 0x0000000000000000ULL}; //0x7
			array<uint64_t, 2U> anR = {0x000000000000000bULL, 0x0000000000000000ULL}; //0xb
			
			tuple<array<uint64_t, 2U>, array<uint64_t, 2U>> anAnswer = MultiWordIntegerDivision(anA, anB);
			Assert::IsTrue(make_tuple(anQ, anR) == anAnswer);
		}

		TEST_METHOD(MultiWordModularDivision5)
		{
			//0xebd0f18634feec57e92742e1540e62e038b70497c6b62ade6be2b66efb5d064762dc764741c3ae0893a272b71668e0eb22c6b6e166be918122ca281bbb83825d
			array<uint64_t, 8U> anA = 
			{
				0x22ca281bbb83825dULL, 0x22c6b6e166be9181ULL, 0x93a272b71668e0ebULL, 0x62dc764741c3ae08ULL, 
				0x6be2b66efb5d0647ULL, 0x38b70497c6b62adeULL, 0xe92742e1540e62e0ULL, 0xebd0f18634feec57ULL
			};

			//0x90f6994efaba6059715c97b6c79c14ae499971db7d13ce0be8cb2e51e4478ab9
			array<uint64_t, 8U> anB = 
			{
				0xe8cb2e51e4478ab9ULL, 0x499971db7d13ce0bULL, 0x715c97b6c79c14aeULL, 0x90f6994efaba6059ULL,
				0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL
			};
			
			//0x60203bf7aba9bd7cc1520cb661487cb3c94ff33ef232cf7c6f6a4ef1063fc210
			array<uint64_t, 8U> anRemainderAnswer =
			{
				0x6f6a4ef1063fc210ULL, 0xc94ff33ef232cf7cULL, 0xc1520cb661487cb3ULL, 0x60203bf7aba9bd7cULL, 
				0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL
			};

			//0x1 a0716928b4a39e171602dc1df13b18ba25f546659109358f4fc1497776f04835
			array<uint64_t, 8U> anQuotientAnswer =
			{
				0x4fc1497776f04835, 0x25f546659109358f,	0x1602dc1df13b18ba,	0xa0716928b4a39e17,
				0x0000000000000001,	0x0000000000000000,	0x0000000000000000,	0x0000000000000000
			};
			
			tuple<array<uint64_t, 8U>, array<uint64_t, 8U>> anAnswer = MultiWordIntegerDivision(anA, anB);
			Assert::IsTrue(make_tuple(anQuotientAnswer, anRemainderAnswer) == anAnswer);
		}
	};
}