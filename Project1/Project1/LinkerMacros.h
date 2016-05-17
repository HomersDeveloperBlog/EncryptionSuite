#ifdef JTH_COMPILAND_IS_RSA_EXE
	#define JTH_RSA_EXPORT _declspec(dllexport)
	#ifdef _DEBUG
		#define JTH_RSA_TEST_EXPORT _declspec(dllexport)
	#else
		#define JTH_RSA_TEST_EXPORT
	#endif //DEBUG
#else
	#define JTH_RSA_EXPORT _declspec(dllimport)
	#ifdef _DEBUG
		#define JTH_RSA_TEST_EXPORT _declspec(dllimport)
	#else
		#define JTH_RSA_TEST_EXPORT
	#endif //DEBUG
#endif //COMPILAND_IS_RSA_EXE

#define INLINE inline