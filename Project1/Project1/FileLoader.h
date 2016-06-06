#ifndef JTH_FILELOADER_H
#define JTH_FILELOADER_H

#include <fstream>
#include <tuple>
#include <memory>
#include <string>
#include "assert.h"

using namespace std;

//number of chars read on success, zero on failure.
//non-null pointer to char buffer on success, null on failure
//shared pointer necessary to delete buffer if return value is ignored
//Should probably return a string. This return value is poorly conceived.
//Just make sure std::string has a .data() function or something.
tuple<shared_ptr<char>, size_t> ReadFileIntoBuffer(
	const string & i_strFilePath)
{
	assert(i_strFilePath.data());
	ifstream oInputFileStream(
		i_strFilePath.data(), 
		ifstream::in | ifstream::binary);
	
	size_t nLength = 0x0ULL;
	shared_ptr<char> pBuffer = nullptr;
	if(oInputFileStream)
	{
		//Get file size
		oInputFileStream.seekg(0, oInputFileStream.end);
		nLength = oInputFileStream.tellg();
		
		//Create a suitable buffer
		if(nLength)
		{
			pBuffer = shared_ptr<char>(
				new (nothrow) char[nLength], 
				default_delete<char[]>());

			if(pBuffer)
			{
				//Read from the file into the buffer
				oInputFileStream.seekg(0, oInputFileStream.beg);
				oInputFileStream.read(pBuffer.get(), nLength);
			}
			else
			{
				nLength = 0x0ULL;
			}
		}
		
		oInputFileStream.close();
	}

	return make_tuple(pBuffer, nLength);
}

size_t WriteFileFromBuffer(
	const string & i_strFilePath,
	const shared_ptr<char> i_pContentBuffer,
	size_t i_nBufferByteCount)
{
	assert(i_strFilePath.data());
	assert(i_pContentBuffer && i_nBufferByteCount);

	if(i_nBufferByteCount
		&& i_pContentBuffer)
	{
		ofstream oOutputFileStream(
			i_strFilePath.data(), 
			ofstream::out | ofstream::binary);

		if(oOutputFileStream)
		{ //Write to the file from the buffer
			oOutputFileStream.write(i_pContentBuffer.get(), i_nBufferByteCount);
			oOutputFileStream.close();
		}
	}

	return i_nBufferByteCount;
}

#endif //JTH_FILELOADER_H