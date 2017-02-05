#pragma once
#include <Windows.h>

#ifndef DWORDPTR
#ifdef _WIN64
#define DWORDPTR DWORD64
#else
#define DWORDPTR DWORD
#endif
#endif

#if defined(ASMUTILS_LIBRARY)
#  define ASMUTILSSHARED_EXPORT __declspec( dllexport )
#else
#  define ASMUTILSSHARED_EXPORT __declspec( dllimport )
#endif

class ASMUtils
{
public:
	static ASMUTILSSHARED_EXPORT void reverseAddressx64(DWORD64 address, BYTE* reversedArrayAddress);
	static ASMUTILSSHARED_EXPORT void reverseAddressx86(DWORD address, BYTE* reversedArrayAddress);
	static ASMUTILSSHARED_EXPORT PVOID dynamicallyExecute(BYTE* code, unsigned int memCodeSize, bool argsPresent, PVOID arg);
	static ASMUTILSSHARED_EXPORT PVOID writeAssembly(BYTE* code, unsigned int size);
	static ASMUTILSSHARED_EXPORT PVOID writeAssembly(HANDLE pHandle, BYTE* code, unsigned int size);
	static ASMUTILSSHARED_EXPORT void printCode(BYTE* code, unsigned int size);
	~ASMUtils();

private:
	ASMUtils();
	
};

