#ifndef JTH_FILELOADER_H
#define JTH_FILELOADER_H

#include <fstream>
#include <tuple>
#include <memory>
#include "assert.h"

using namespace std;

//number of chars read on success, zero on failure.
//non-null pointer to char buffer on success, null on failure
//shared pointer necessary to delete buffer if return value is ignored
tuple<shared_ptr<char>, size_t> ReadFileIntoBuffer(
	const char * i_strFilePath)
{
	assert(i_strFilePath);

	if(i_strFilePath) //Handling of null filepaths implementation defined.
	{
		ifstream oInputFileStream(
			i_strFilePath, 
			ifstream::in | ifstream::binary);

		if(oInputFileStream)
		{
			//Get file size
			oInputFileStream.seekg(0, oInputFileStream.end);
			size_t nLength = oInputFileStream.tellg();
		
			//Create a suitable buffer
			shared_ptr<char> pBuffer = nullptr;
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
			}
		
			oInputFileStream.close();

			return make_tuple<>(pBuffer, nLength);
		}
	}

	return make_tuple(nullptr, 0x0ULL);
}

size_t WriteFileFromBuffer(
	const char * i_strFilePath,
	const shared_ptr<char> i_pContentBuffer,
	size_t i_nBufferByteCount)
{
	assert(i_strFilePath);
	assert(i_pContentBuffer && i_nBufferByteCount);

	if(i_strFilePath
		&& i_nBufferByteCount
		&& i_pContentBuffer)
	{
		ofstream oOutputFileStream(
			i_strFilePath, 
			ofstream::out | ofstream::binary);

		if(oOutputFileStream)
		{//Write to the file from the buffer
			oOutputFileStream.write(i_pContentBuffer.get(), i_nBufferByteCount);
			oOutputFileStream.close();
		}

		return i_nBufferByteCount;
	}

	return 0x0ULL;
}

#endif //JTH_FILELOADER_H