#include "stdafx.h"

#define ASMUTILS_LIBRARY

#include <cstdio>
#include "ASMUtils.h"

PVOID ASMUtils::dynamicallyExecute(BYTE* code, unsigned int memCodeSize, bool argsPresent, PVOID arg) {
	SYSTEM_INFO sysInfos;
	GetSystemInfo(&sysInfos);

	typedef PVOID(*DYNAMIC_FUNC_EX)(PVOID);
	typedef PVOID(*DYNAMIC_FUNC_EX_NO_ARG)(void);

	unsigned int AlignedRangeSize = ((memCodeSize / sysInfos.dwPageSize) *sysInfos.dwPageSize) + sysInfos.dwPageSize;
	DYNAMIC_FUNC_EX dynamicCode = (DYNAMIC_FUNC_EX)VirtualAlloc(NULL, AlignedRangeSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	DYNAMIC_FUNC_EX_NO_ARG dynamicCodeNoArg = (DYNAMIC_FUNC_EX_NO_ARG)dynamicCode;
	memcpy(dynamicCode, code, memCodeSize);
	PVOID ret = 0;
	if (argsPresent) {
		ret = (dynamicCode)(arg);
	} else {
		ret = (dynamicCodeNoArg)();
	}

	VirtualFree(dynamicCode, NULL, MEM_RELEASE);
	return ret;
}

void ASMUtils::reverseAddressx64(DWORD64 address, BYTE* reversedArrayAddress) {
	DWORD64 mask = 0xFF00000000000000;
	int octalAddressLength = sizeof(mask);

	for (int i = 0; i < octalAddressLength; i++) {
		int bitsRightShift = (i * 8);
		int reverseBitsRightShift = (octalAddressLength - 1 - i) * 8;

		unsigned char newByte = (unsigned char)((address & (mask >> bitsRightShift)) >> reverseBitsRightShift);

		reversedArrayAddress[octalAddressLength - 1 - i] = newByte;
	}
}

void ASMUtils::reverseAddressx86(DWORD address, BYTE* reversedArrayAddress) {
	DWORD mask = 0xFF000000;
	int octalAddressLength = sizeof(mask);

	for (int i = 0; i < octalAddressLength; i++) {
		int bitsRightShift = (i * 8);
		int reverseBitsRightShift = (octalAddressLength - 1 - i) * 8;

		unsigned char newByte = (unsigned char)((address & (mask >> bitsRightShift)) >> reverseBitsRightShift);

		reversedArrayAddress[octalAddressLength - 1 - i] = newByte;
	}
}

void ASMUtils::printCode(BYTE* code, unsigned int size) {
	for (unsigned int i = 0; i < size; i++) {
		BYTE b = code[i];
		printf("{0x%x}, ", b);
	}
}

PVOID ASMUtils::writeAssembly(BYTE* code, unsigned int size) {
	SYSTEM_INFO sysInfos;
	GetSystemInfo(&sysInfos);

	DWORDPTR AlignedRangeSize = ((size / sysInfos.dwPageSize) * sysInfos.dwPageSize) + sysInfos.dwPageSize;
	PVOID dynamicCode = VirtualAlloc(NULL, AlignedRangeSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(dynamicCode, code, size);
	
	return dynamicCode;
}

PVOID ASMUtils::writeAssembly(HANDLE pHandle, BYTE* code, unsigned int size) {
	SYSTEM_INFO sysInfos;
	GetSystemInfo(&sysInfos);

	DWORDPTR AlignedRangeSize = ((size / sysInfos.dwPageSize) * sysInfos.dwPageSize) + sysInfos.dwPageSize;
	PVOID dynamicCode = VirtualAllocEx(pHandle, NULL, AlignedRangeSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(pHandle, dynamicCode, code, size, NULL);

	return dynamicCode;
}


ASMUtils::ASMUtils()
{
}


ASMUtils::~ASMUtils()
{
}
