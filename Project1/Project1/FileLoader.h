#pragma once

#include <fstream>
#include <tuple>

using namespace std;

//number of chars read on success, zero on failure.
//non-null pointer to char buffer on success, null on failure
tuple<shared_ptr<char>, size_t> LoadFileIntoBuffer(
	const char * i_strFilePath
	const char ** i_pstrBuffer)
{
	ifstream oInputFileStream(
		i_strFilePath, 
		ifstream::in | ifstream::binary);

	if(oInputFileStream)
	{
		//Get file length
		oInputFileStream.seekg(0, oInputFileStream.end);
		size_t nLength = oInputFileStream.tellg(); //%could be 0
		char * pBuffer = new char[nLength]; //%catch

		oInputFileStream.seekg(0, oInputFileStream.beg); //%check return values for errors
		oInputFileStream.read(pBuffer, nLength);
		oInputFileStream.close();

		return make_tuple<>(pBuffer, nLength);
	}

	return make_tuple(nullptr, 0x0ULL);
}