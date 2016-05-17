//Input sizes have to come from somewhere.
//I can imagine templating the functions, instantiating some common sizes
//Then having a trampoline function select the template.

//NLength -> NBlockWordCount
//need typedef TCryptoWord uintmax_t;
//const unsigned int NCryptoWordSize = sizeof(TCryptoWord);

//I'm thinking this function makes more sense as a member of the block mode
//Essentially we will contract the block mode into the generic encryptor
//Padding is part of the block loader.

//will need exceptions (inside) / error codes (outside) for bad template sizes
//uint_least8_t guaranteed to exist but uint8_t is not.
//Either way sizes could be distorted.
//For now I think we can require that the block size be a multiple of the machine uintmax_t
//Either way we have to convert a requested bit length to a machine specific word count that fits in unsigned int.

template<unsigned int NLength> //%template parameters have to get worked in here somehow.
void EncryptBuffer(
	char * io_achPlainText, //The blocks are already temporary, lets do them in place?
	const size_t i_nBufferByteCount,
	const BlockModeManager<NLength> * pBlockMode,
	const BlockCipher<NLength> * pBlockCipher) 
{
	const size_t nBlockByteCount = sizeof(uint64_t) * NLength;
	const size_t nBufferBlockCount = i_nBufferSizeInBytes / nBlockSizeInBytes;

	//We should perform appropriate padding procedure before sending it here. 
	//We could wrap in cipher text stealing, or zero padding.
	//With both, we need to record the original size so we can trim the padding.

	char achInitializationVector[NLength * sizeof(uint64_t)];
	pBlockMode->InitializeIV(achInitializionVector);
	//Probably this would have to be stored somewhere or generated randomly.

	//Initialization vector
	char * pPreviousPlainTextBlock = achInitializationVector; //%this procedure may vary based on MOO
	char * pCurrentPlainTextBlock = io_achMessage; 
	for(size_t nBlock = 0U;
		nBlock < nBufferBlockCount;
		++nBlock)
	{
		pBlockMode->PreBlockRoutine(
			pPreviousPlainText,
			pCurrentPlainText,
			pPreviousCipherText,
			pCurrentCipherText); //Here we prepare a new plaintext

		pBlockCipher->EncipherBlock(
			pCurrentCipherText);

		pBlockMode->PostBlockRoutine(
			pPreviousPlainText,
			pCurrentPlainText,
			pPreviousCipherText,
			pCurrentCipherText);

		pPreviousBlock = pCurrentBlock;
		pCurrentBlock += nBlockByteCount;
	}

	//Last block.
	const size_t nBufferRemainderByteCount = 
		i_nBufferSizeInBytes % nBlockSizeInBytes;
	if(nBufferRemainderByteCount)
	{
		//Last block. Ciphertext stealing?
	}
}