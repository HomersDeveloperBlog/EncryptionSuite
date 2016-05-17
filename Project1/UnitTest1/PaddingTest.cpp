#include "stdafx.h"
#include "CppUnitTest.h"

#include "Padding.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Padding_Test
{
	TEST_CLASS(Padding_Test)
	{
	public:
		TEST_METHOD(ZeroPaddingTest1)
		{ //Test of zero padding, padder should add zeroes
			const unsigned int nWordCount = 13U; //So less than the 16 in the following buffer
			const uint64_t anM[] = 
			{
				0x4599e65642762e15ULL, 0xb96b3351336dbf91ULL, 0x0f191f686f2aff7fULL, 0x5d211483ec2c4849ULL, 
				0xfac489d38f5a7df0ULL, 0x7d27883993ddda2dULL, 0xb04470eda206ac9aULL, 0x996dafeb4c9c2df5ULL, 
				0x98c4bba115472eeaULL, 0x723b79549c278485ULL, 0xd244429f381bff80ULL, 0x96715e864366735dULL, 
				0xa6fef5177f0c62efULL, 0x96715e864366735dULL, 0xec7E0c295250b385ULL, 0x640799d705866624ULL
			};
			
			vector<array<uint64_t, 4U>> anAnswer = 
			{
				{0x4599e65642762e15ULL, 0xb96b3351336dbf91ULL, 0x0f191f686f2aff7fULL, 0x5d211483ec2c4849ULL},
				{0xfac489d38f5a7df0ULL, 0x7d27883993ddda2dULL, 0xb04470eda206ac9aULL, 0x996dafeb4c9c2df5ULL},
				{0x98c4bba115472eeaULL, 0x723b79549c278485ULL, 0xd244429f381bff80ULL, 0x96715e864366735dULL},
				{0xa6fef5177f0c62efULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL}
			};

			vector<array<uint64_t, 4U>> vectPaddedM = PadMessageBuffer<4U>(
				reinterpret_cast<const char *>(static_cast<const uint64_t *>(anM)), 
				nWordCount * sizeof(uint64_t));

			Assert::IsTrue(equal(
				vectPaddedM.begin(),
				vectPaddedM.end(),
				anAnswer.begin(),
				anAnswer.end()));
		}

		TEST_METHOD(ZeroPaddingTest2)
		{ //Test of zero padding, padder should do nothing
			const unsigned int nWordCount = 16U;
			const uint64_t anM[] = 
			{
				0x4599e65642762e15ULL, 0xb96b3351336dbf91ULL, 0x0f191f686f2aff7fULL, 0x5d211483ec2c4849ULL, 
				0xfac489d38f5a7df0ULL, 0x7d27883993ddda2dULL, 0xb04470eda206ac9aULL, 0x996dafeb4c9c2df5ULL, 
				0x98c4bba115472eeaULL, 0x723b79549c278485ULL, 0xd244429f381bff80ULL, 0x96715e864366735dULL, 
				0xa6fef5177f0c62efULL, 0x96715e864366735dULL, 0xec7E0c295250b385ULL, 0x640799d705866624ULL
			};
			
			vector<array<uint64_t, 4U>> anAnswer = 
			{
				{0x4599e65642762e15ULL, 0xb96b3351336dbf91ULL, 0x0f191f686f2aff7fULL, 0x5d211483ec2c4849ULL},
				{0xfac489d38f5a7df0ULL, 0x7d27883993ddda2dULL, 0xb04470eda206ac9aULL, 0x996dafeb4c9c2df5ULL},
				{0x98c4bba115472eeaULL, 0x723b79549c278485ULL, 0xd244429f381bff80ULL, 0x96715e864366735dULL},
				{0xa6fef5177f0c62efULL, 0x96715e864366735dULL, 0xec7E0c295250b385ULL, 0x640799d705866624ULL}
			};

			vector<array<uint64_t, 4U>> vectPaddedM = PadMessageBuffer<4U>(
				reinterpret_cast<const char *>(static_cast<const uint64_t *>(anM)), 
				nWordCount * sizeof(uint64_t));

			Assert::IsTrue(equal(
				vectPaddedM.begin(),
				vectPaddedM.end(),
				anAnswer.begin(),
				anAnswer.end()));
		}

		TEST_METHOD(CiphertextStealingTest1)
		{ //Test of ciphertext stealing, padder should use data from previous block
			const unsigned int nWordCount = 13U; //So less than the 16 in the following buffer
			const uint64_t anM[] = 
			{
				0x4599e65642762e15ULL, 0xb96b3351336dbf91ULL, 0x0f191f686f2aff7fULL, 0x5d211483ec2c4849ULL, 
				0xfac489d38f5a7df0ULL, 0x7d27883993ddda2dULL, 0xb04470eda206ac9aULL, 0x996dafeb4c9c2df5ULL, 
				0x98c4bba115472eeaULL, 0x723b79549c278485ULL, 0xd244429f381bff80ULL, 0x96715e864366735dULL, 
				0xa6fef5177f0c62efULL, 0x96715e864366735dULL, 0xec7E0c295250b385ULL, 0x640799d705866624ULL
			};
			
			vector<array<uint64_t, 4U>> anAnswer = 
			{
				{0x4599e65642762e15ULL, 0xb96b3351336dbf91ULL, 0x0f191f686f2aff7fULL, 0x5d211483ec2c4849ULL},
				{0xfac489d38f5a7df0ULL, 0x7d27883993ddda2dULL, 0xb04470eda206ac9aULL, 0x996dafeb4c9c2df5ULL},
				{0x98c4bba115472eeaULL, 0x723b79549c278485ULL, 0xd244429f381bff80ULL, 0x96715e864366735dULL},
				{0x723b79549c278485ULL, 0xd244429f381bff80ULL, 0x96715e864366735dULL, 0xa6fef5177f0c62efULL}
			};

			vector<array<uint64_t, 4U>> vectPaddedM = PadMessageBuffer<4U>(
				reinterpret_cast<const char *>(static_cast<const uint64_t *>(anM)), 
				nWordCount * sizeof(uint64_t));

			Assert::IsTrue(equal(
				vectPaddedM.begin(),
				vectPaddedM.end(),
				anAnswer.begin(),
				anAnswer.end()));
		}

		TEST_METHOD(CiphertextStealingTest2)
		{ //Test of ciphertext stealing, padder should do nothing
			const unsigned int nWordCount = 16U;
			const uint64_t anM[] = 
			{
				0x4599e65642762e15ULL, 0xb96b3351336dbf91ULL, 0x0f191f686f2aff7fULL, 0x5d211483ec2c4849ULL, 
				0xfac489d38f5a7df0ULL, 0x7d27883993ddda2dULL, 0xb04470eda206ac9aULL, 0x996dafeb4c9c2df5ULL, 
				0x98c4bba115472eeaULL, 0x723b79549c278485ULL, 0xd244429f381bff80ULL, 0x96715e864366735dULL, 
				0xa6fef5177f0c62efULL, 0x96715e864366735dULL, 0xec7E0c295250b385ULL, 0x640799d705866624ULL
			};
			
			vector<array<uint64_t, 4U>> anAnswer = 
			{
				{0x4599e65642762e15ULL, 0xb96b3351336dbf91ULL, 0x0f191f686f2aff7fULL, 0x5d211483ec2c4849ULL},
				{0xfac489d38f5a7df0ULL, 0x7d27883993ddda2dULL, 0xb04470eda206ac9aULL, 0x996dafeb4c9c2df5ULL},
				{0x98c4bba115472eeaULL, 0x723b79549c278485ULL, 0xd244429f381bff80ULL, 0x96715e864366735dULL},
				{0xa6fef5177f0c62efULL, 0x96715e864366735dULL, 0xec7E0c295250b385ULL, 0x640799d705866624ULL}
			};

			vector<array<uint64_t, 4U>> vectPaddedM = PadMessageBuffer<4U>(
				reinterpret_cast<const char *>(static_cast<const uint64_t *>(anM)), 
				nWordCount * sizeof(uint64_t));

			Assert::IsTrue(equal(
				vectPaddedM.begin(),
				vectPaddedM.end(),
				anAnswer.begin(),
				anAnswer.end()));
		}
	};
}

//We are also developing an interface here for the volume encryptor
//I'm thinking iterator to block would be the easiest.
//But the volume is not blockified, its a buffer, not in ram, that is streamed in.
//The block format of array<uint64_t> is specific to RSA.

//How to handle 0 padding and ciphertext stealing. Writing the last block differently makes sense.
//But if we copy from file, our buffer won't be long enough to accommodate padding.
//We would have to copy it (again) into another buffer. O(n) overhead for O(1) change.
//But its awkward to combine the loader and the padding code.
//Fuck it. Just do it with 3 copies.

//We could force the block loader to serve up array<char, N> blocks.
//The data does not necessarily come from a file.
//Compatibility with the c++ stream might be a good idea
//Streaming the file in from the ram may not even be desirable. Probably for collossal files only.
//Merging the padder with a block parser that serves up blocks as char arrays might be good.
//Probably messy to have the padding be done by detecting the final block and serving it up differently.
//But this would allow us to separate file loading/source identification from
//the padding requirements, without creating a new copy.
//But we should allow for the possibility that the blocks are expanded 
//or compressed(unlikely) and modified arbitrarily by the block loader.
//I think providing a block iterator is a reasonable expectation.
//If that iterator has to be a transform iterator, and/or load blocks from file during ++/*, is its own business.
//The iterator must be constructible from a char *, length.
//This makes 0 padding or ciphertext stealing difficult if the iterator does not know it is the last.
//Actually the file io forces a ++ on read. So a ++, test, -- is not out of the question.
//Iterator stores a char * pPostion only. On ++, we pPosition += NBlockByteCount chars forward.
//On *, we must return an array<char, NBlockByteCount>. Copy NBlockByteCount bytes starting at pPosition.
//At end, if we did ++ normally on last, we could be slightly past the end.
//We could modify the construction of the buffer iterator to serve up the 1 block past the end.
//But this would violate C++ language rule against forming invalid iterators.
//Better make ++ equivalent to N lower level ++ operators?
//This would mean we would execute read N times (byte by byte). Only seeking.
//Note this format is prime for inlining a file read(current, N) instead of a memcpy.
//We are cascading, read from file, copy from buffer into array<char, N>, possible modifier (padding) into array<char, M>

//Side note: can we cascade transform iterators together? Say XformIt<Old<>,Func>::make_xform_pair
//++ would call ++ of the previous link. * would call func(*previous). != calls != on previous
//This allows us to generate an input stream for a function would creating pass over the elements first.
//If a function consists of independent function applications over a range,
//maybe it shouldn't have been stored as a function of iterators in the first place.
//Not necessarily true, inlining decryption, coordinate transformation, conversions etc is valuable.
//A kind of dual to the transform iterator is the right transform iterator,
//where output values are first modified in some way before being stored.
//The * operator returns a value that modifies the asisgnment operator ?