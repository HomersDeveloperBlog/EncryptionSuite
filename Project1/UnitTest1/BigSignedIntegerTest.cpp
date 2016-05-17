#include "stdafx.h"
#include "CppUnitTest.h"

#include "BigSignedInteger.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

//All of these tests are messed up. tuple of arrays absolutely not working.
namespace BigSignedInteger_Test
{		
	TEST_CLASS(BigSignedInteger_Test)
	{
	public:
		TEST_METHOD(MultiWordAdd1)
		{
			bool bSignA = POSITIVE_SIGN_BIT;
			array<uint64_t, 2U> anUIntA = {0x20a80e29360074e9ULL, 0x93934a6e3d1a6020ULL}; //0x93934a6e3d1a602020a80e29360074e9
			tuple<bool, array<uint64_t, 2U>> anA = make_tuple(bSignA, anUIntA); //This is kinda ugly. No forwarding solution?

			bool bSignB = POSITIVE_SIGN_BIT;
			array<uint64_t, 2U> anB = {0xcd05843d8b35ff66ULL, 0x6825e061d5d0fd1bULL}; //0x6825e061d5d0fd1bcd05843d8b35ff66
			
			bool bSignAnswer = POSITIVE_SIGN_BIT;
			array<uint64_t, 2U> anAnswer = {0xedad9266c136744fULL, 0xfbb92ad012eb5d3bULL}; //0xfbb92ad012eb5d3bedad9266c136744f
			
			uint64_t nCarry = SignedMultiWordIntegerAddWithCarryInPlace(anA, make_tuple(bSignB, anB));
			Assert::IsTrue(nCarry == 0ULL);
			Assert::IsTrue(anA == make_tuple(bSignAnswer, anAnswer));
		}
		
		TEST_METHOD(MultiWordAdd2)
		{
			bool bSignA = POSITIVE_SIGN_BIT;
			array<uint64_t, 2U> anUIntA = {0x20a80e29360074e9ULL, 0x93934a6e3d1a6020ULL}; //0x93934a6e3d1a602020a80e29360074e9
			tuple<bool, array<uint64_t, 2U>> anA = make_tuple(bSignA, anUIntA);

			bool bSignB = NEGATIVE_SIGN_BIT;
			array<uint64_t, 2U> anB = {0xcd05843d8b35ff66ULL, 0x6825e061d5d0fd1bULL}; //0x6825e061d5d0fd1bcd05843d8b35ff66
			
			bool bSignAnswer = POSITIVE_SIGN_BIT;
			array<uint64_t, 2U> anAnswer = {0x53a289ebaaca7583ULL, 0x2b6d6a0c67496304ULL}; //0x2b6d6a0c6749630453a289ebaaca7583
			
			uint64_t nCarry = SignedMultiWordIntegerAddWithCarryInPlace(anA, make_tuple(bSignB, anB));
			Assert::IsTrue(nCarry == 0ULL);
			Assert::IsTrue(anA == make_tuple(bSignAnswer, anAnswer));
		}
	
		TEST_METHOD(MultiWordAdd3)
		{
			bool bSignA = NEGATIVE_SIGN_BIT;
			array<uint64_t, 2U> anUIntA = {0x20a80e29360074e9ULL, 0x93934a6e3d1a6020ULL}; //0x93934a6e3d1a602020a80e29360074e9
			tuple<bool, array<uint64_t, 2U>> anA = make_tuple(bSignA, anUIntA);

			bool bSignB = POSITIVE_SIGN_BIT;
			array<uint64_t, 2U> anB = {0xcd05843d8b35ff66ULL, 0x6825e061d5d0fd1bULL}; //0x6825e061d5d0fd1bcd05843d8b35ff66
			
			bool bSignAnswer = NEGATIVE_SIGN_BIT;
			array<uint64_t, 2U> anAnswer = {0x53a289ebaaca7583ULL, 0x2b6d6a0c67496304ULL}; //-0x2b6d6a0c6749630453a289ebaaca7583
			
			uint64_t nCarry = SignedMultiWordIntegerAddWithCarryInPlace(anA, make_tuple(bSignB, anB));
			Assert::IsTrue(nCarry == 0ULL);
			Assert::IsTrue(anA == make_tuple(bSignAnswer, anAnswer));
		}
		
		TEST_METHOD(MultiWordAdd4)
		{
			bool bSignA = NEGATIVE_SIGN_BIT;
			array<uint64_t, 2U> anUIntA = {0x20a80e29360074e9ULL, 0x93934a6e3d1a6020ULL}; //0x93934a6e3d1a602020a80e29360074e9
			tuple<bool, array<uint64_t, 2U>> anA(bSignA, anUIntA);
			get<1>(anA) = anUIntA;
			pair<bool, array<uint64_t, 2U>> anAA(bSignA, anUIntA);

			bool bSignB = NEGATIVE_SIGN_BIT;
			array<uint64_t, 2U> anB = {0xcd05843d8b35ff66ULL, 0x6825e061d5d0fd1bULL}; //0x6825e061d5d0fd1bcd05843d8b35ff66
			
			bool bSignAnswer = NEGATIVE_SIGN_BIT;
			array<uint64_t, 2U> anAnswer = {0xedad9266c136744fULL, 0xfbb92ad012eb5d3bULL}; //-0xfbb92ad012eb5d3bedad9266c136744f
			
			uint64_t nCarry = SignedMultiWordIntegerAddWithCarryInPlace(anA, make_tuple(bSignB, anB));
			Assert::IsTrue(nCarry == 0ULL);
			Assert::IsTrue(anA == make_tuple(bSignAnswer, anAnswer));
		}

		//TEST_METHOD(MultiWordAdd5)
		//{
		//	//0xebd0f18634feec57e92742e1540e62e038b70497c6b62ade6be2b66efb5d064762dc764741c3ae0893a272b71668e0eb22c6b6e166be918122ca281bbb83825d
		//	array<uint64_t, 8U> anA = 
		//	{
		//		0x22ca281bbb83825dULL, 0x22c6b6e166be9181ULL, 0x93a272b71668e0ebULL, 0x62dc764741c3ae08ULL, 
		//		0x6be2b66efb5d0647ULL, 0x38b70497c6b62adeULL, 0xe92742e1540e62e0ULL, 0xebd0f18634feec57ULL
		//	};

		//	//0x291e9f62f6e3c8a174facf657cf49fdefe7edeee39276aa82a9bd18f7657a0c50728469d5fc4ffc5ef7e2d77254381eccd2f67c14de1e12782bcf3db2dc8c237
		//	array<uint64_t, 8U> anB = 
		//	{
		//		0x82bcf3db2dc8c237ULL, 0xcd2f67c14de1e127ULL, 0xef7e2d77254381ecULL, 0x0728469d5fc4ffc5ULL, 
		//		0x2a9bd18f7657a0c5ULL, 0xfe7edeee39276aa8ULL, 0x74facf657cf49fdeULL, 0x291e9f62f6e3c8a1ULL
		//	};

		//	//0x114ef90e92be2b4f95e221246d10302bf3735e385ffdd9586967e87fe71b4a70c6a04bce4a188adce8320a02e3bac62d7eff61ea2b4a072a8a5871bf6e94c4494
		//	array<uint64_t, 8U> anAnswer = 
		//	{
		//		0xa5871bf6e94c4494ULL, 0xeff61ea2b4a072a8ULL, 0x8320a02e3bac62d7ULL, 0x6a04bce4a188adceULL, 
		//		0x967e87fe71b4a70cULL, 0x3735e385ffdd9586ULL, 0x5e221246d10302bfULL, 0x14ef90e92be2b4f9ULL
		//	};

		//	uint64_t nCarry = MultiWordIntegerAddInPlace(anA, anB);
		//	Assert::IsTrue(nCarry == 1ULL);
		//	Assert::IsTrue(anA == anAnswer);
		//}
		
		TEST_METHOD(MultiWordSubtract1)
		{
			//0xebd0f18634feec57e92742e1540e62e038b70497c6b62ade6be2b66efb5d064762dc764741c3ae0893a272b71668e0eb22c6b6e166be918122ca281bbb83825d
			bool bSignA = POSITIVE_SIGN_BIT;
			array<uint64_t, 8U> anUIntA = 
			{
				0x22ca281bbb83825dULL, 0x22c6b6e166be9181ULL, 0x93a272b71668e0ebULL, 0x62dc764741c3ae08ULL, 
				0x6be2b66efb5d0647ULL, 0x38b70497c6b62adeULL, 0xe92742e1540e62e0ULL, 0xebd0f18634feec57ULL
			};
			tuple<bool, array<uint64_t, 8U>> anA(bSignA, anUIntA);

			//0x291e9f62f6e3c8a174facf657cf49fdefe7edeee39276aa82a9bd18f7657a0c50728469d5fc4ffc5ef7e2d77254381eccd2f67c14de1e12782bcf3db2dc8c237
			bool bSignB = NEGATIVE_SIGN_BIT;
			array<uint64_t, 8U> anB = 
			{
				0x82bcf3db2dc8c237ULL, 0xcd2f67c14de1e127ULL, 0xef7e2d77254381ecULL, 0x0728469d5fc4ffc5ULL, 
				0x2a9bd18f7657a0c5ULL, 0xfe7edeee39276aa8ULL, 0x74facf657cf49fdeULL, 0x291e9f62f6e3c8a1ULL
			};

			//0x114ef90e92be2b4f95e221246d10302bf3735e385ffdd9586967e87fe71b4a70c6a04bce4a188adce8320a02e3bac62d7eff61ea2b4a072a8a5871bf6e94c4494
			bool bSignAnswer = POSITIVE_SIGN_BIT;
			array<uint64_t, 8U> anAnswer =
			{
				0xa5871bf6e94c4494ULL, 0xeff61ea2b4a072a8ULL, 0x8320a02e3bac62d7ULL, 0x6a04bce4a188adceULL, 
				0x967e87fe71b4a70cULL, 0x3735e385ffdd9586ULL, 0x5e221246d10302bfULL, 0x14ef90e92be2b4f9ULL
			};

			uint64_t nCarry = SignedMultiWordIntegerSubtractWithCarryInPlace(anA, make_tuple(bSignB, anB));
			Assert::IsTrue(nCarry == 1ULL);
			Assert::IsTrue(anA == make_tuple(bSignAnswer, anAnswer));
		}
	};
}