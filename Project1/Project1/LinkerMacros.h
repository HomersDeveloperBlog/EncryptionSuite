#pragma once

#ifdef JTH_COMPILAND_IS_RSA_EXE
	#define JTH_DLL_EXPORT _declspec(dllexport)
	#ifdef _DEBUG
		#define JTH_DLL_TEST_EXPORT _declspec(dllexport)
	#else
		#define JTH_DLL_TEST_EXPORT
	#endif //DEBUG
#else
	#define JTH_DLL_EXPORT _declspec(dllimport)
	#ifdef _DEBUG
		#define JTH_DLL_TEST_EXPORT _declspec(dllimport)
	#else
		#define JTH_DLL_TEST_EXPORT
	#endif //DEBUG
#endif //COMPILAND_IS_RSA_EXE

enum {SUCCESS = 0, FAILURE};

#define INLINE inline