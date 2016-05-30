//% a value to interpret as default / skip?

enum DirectionOption : size_t
{
	ENCRYPT = 0U, DECRYPT,
	DIRECTION_OPTION_COUNT
};

enum AlgorithmOption : size_t
{
	XOR_ENCRYPTION = 0U, RSA_ENCRYPTION,
	ALGORITHM_OPTION_COUNT
};

enum BlockSizeOption : size_t
{
	BLOCK_SIZE_64 = 0U, BLOCK_SIZE_128, BLOCK_SIZE_256, BLOCK_SIZE_512,
	BLOCK_SIZE_1024, BLOCK_SIZE_2048, BLOCK_SIZE_4096,
	BLOCK_SIZE_OPTION_COUNT
};

enum BlockModeOption : size_t
{
	ELECTRONIC_CODEBOOK = 0U, CIPHER_BLOCK_CHAINING,
	BLOCK_MODE_OPTION_COUNT
};

enum PaddingOption : size_t
{
	ZERO_PADDING = 0U, CIPHERTEXT_STEALING,
	PADDING_OPTION_COUNT
};

enum KeyGenerationOption : size_t
{
	INTERNALLY_GENERATED = 0U, EXTERNAL_FILE, //COMMAND_LINE_ARGUMENT
	KEY_GENERATION_OPTION_COUNT
};

const static DirectionOption eDefaultDirectionOption = ENCRYPT;
const static AlgorithmOption eDefaultAlgorithmOption = RSA_ENCRYPTION;
const static BlockSizeOption eDefaultBlockSizeOption = BLOCK_SIZE_1024;
const static BlockModeOption eDefaultBlockModeOption = CIPHER_BLOCK_CHAINING;
const static PaddingOption eDefaultPaddingOption = CIPHERTEXT_STEALING;
const static size_t nDefaultPaddingBytes = 0x0ULL;

//Should have magic numbers to check compatibility?
//Make into a header?
//This would be fine exception the decryption keys cannot be embedded in the file.

struct ProgramOptions
{
	DirectionOption m_eDirectionOption;
	AlgorithmOption m_eAlgorithmOption;
	BlockSizeOption m_eBlockSizeOption;
	BlockModeOption m_eBlockModeOption;
	PaddingOption m_ePaddingOption;
	size_t m_nPaddedBytes; //Unpadding key. //%Treat separate?
};

struct ProgramOptionsFile
{
	ProgramOptions m_oProgramOptions;
	char n_achDecryptionKey[512];
};

//To encrypt: supply source file name, direction as 0, and the 4 options.
//To decrypt: supply source file name, direction as 1, and the options file.
//Might make sense to parse the file in the same way as command line options
//Providing for default options and command-line key input would be nice.
//Could write the options file just like a command line argument, then send it through the same code.
//Note we have the option of forcing the arguments into all quotes.
//Easiest architecture is to write options struct like a command line string, then route it into the parser.
//The file is delimitted with zeroes, which is problematic. Should probably delimit with 'space' characters.
//Much of the benefit of using a rigid file structure is lost if we have to parse it. 
//We could do the alternative. We convert the command line options into an OptionsFile structure.
//One of the issues parsing the CL into a struct is that we would need to parse the long integers.
//We will have to do this if we want to have key input on CL
//So we need a component that translates a command line argument into a OptionsFile structure.
//Then something that can take the options file structure and unpack the options.

//Fix file handle resource leak when new throws exception.

#include <vector>
#include <string>

using namespace std;

int main(
	char ** astrArgs, 
	int nCount)
{
	//Initial pre-parse function
	//-Create vector of string for command line
	vector<string> vectCLArgs;

	//Create of program options struct
	//-Check number of arguments makes sense.
	//-Create default valued structure.
	ProgramOptions oProgramOptions;

	//-For each argument, pass substring to specific parsing function.
	try
	{
		size_t nParsedValue = stoull(vectCLArgs[0], nullptr, 10); //%exception on failure. out of range or invalid argument.
		if(nParsedValue < DIRECTION_OPTION_COUNT)
		{	//-Set struct with value returned.
			oProgramOptions.m_eDirectionOption = static_cast<DirectionOption>(nParsedValue);
			return true;
		}
		else
		{
			return false;
		}
	}
	catch()
	{
		return false;
	}


	if(oProgramOptions.m_eDirectionOption == ENCRYPT)
	{ //expect 4 more enums, then 1 target file name.

	}

	//-Return program options struct
	return oProgramOptions;

	//%filenames are also given as CL arguments, but there is no place for them in the program options.
	//%issue is how to work in files and decryption keys.
	//%Key (if present), to be included in struct.
	//%Files preloaded into buffers, provided as char *.
	//%So theres actually no way to avoid parsing command line arguments.
	//%Even if we are providing an options file, we have to parse that file name, among other things.
	//%We can't rely on the user to provide their key as part of our proprietary struct, written to a file.
	//%We might be able to ask that they write their key to a file in binary.
	//%This means it might be more flexible to have the options and key file separate.
	//%It facilitates more realistic use cases.
	//%But decryption requires unpadding information, and this is not included in the key.
	//%It would cause the options file to behave like a key.
	//%fuck it, put it in the options file

	//Load source file into buffer.
	//Call program using program options struct and loaded buffers.

	//Write return buffer to target file.

	return 0;
}