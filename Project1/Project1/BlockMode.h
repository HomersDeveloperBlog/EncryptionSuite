#pragma once

//Need cipher block chaining modification of the key
//Take the previous cipher text and XOR it with the current plain text
template<unsigned int NLength>
class BlockModeManager
{
	virtual /*ModeOfOperation_e*/ unsigned int GetModeEnum() = 0;

private:
	virtual void InitializeIV(
		char * o_achInitializationVector) = 0;

	virtual void PreBlockRoutine(
		const char * i_pPreviousPlainText,
		const char * i_pCurrentPlainText,
		const char * i_pPreviousCipherText,
		char * io_pCurrentCipherText) = 0;
	
	virtual void PostBlockRoutine(
		const char * i_pPreviousPlainText,
		const char * i_pCurrentPlainText,
		const char * i_pPreviousCipherText,
		char * io_pCurrentCipherText) = 0;
}