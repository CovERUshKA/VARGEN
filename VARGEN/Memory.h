#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <comdef.h>
#include <Psapi.h>
#include "SDK/GlobalVars.h"
#include "SDK/Input.h"

struct PModule
{
	DWORD dwBase;
	DWORD dwSize;
};

class Memory final {
public:
	Memory() noexcept;

	uintptr_t present;
	uintptr_t reset;

	class ClientMode* clientMode;
	GlobalVars* globalVars;
	Input* input;
	int* smokeCount;
	std::add_pointer_t<bool __cdecl(Vector, Vector, short) noexcept> lineGoesThroughSmoke;
	std::add_pointer_t<bool __stdcall(const char*) noexcept> acceptMatch;
	std::add_pointer_t<void __fastcall(const char*, const char*) noexcept> setClanTag;

	bool Attach(const char* ProcessName, DWORD rights)
	{
		HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(entry);

		do
			if (!strcmp(_bstr_t(entry.szExeFile), ProcessName)) {
				pID = entry.th32ProcessID;
				CloseHandle(handle);
				_process = OpenProcess(rights, false, pID);
				return true;
			}
		while (Process32Next(handle, &entry));
		return false;
	}

	bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
	{
		for (; *szMask; ++szMask, ++Mask, ++Data)
		{
			if (*szMask == 'x' && *Mask != *Data)
			{
				return false;
			}
		}
		return (*szMask) == 0;
	}

	void CloseProcess(const char* ProcessName)
	{

		HANDLE hProcess;
		DWORD pid;
		pid = GetPIDbyName(ProcessName);
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		TerminateProcess(hProcess, -1);
		CloseHandle(hProcess);
	}

	PModule GetModule(const char* moduleName)
	{
		HANDLE module = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
		MODULEENTRY32 mEntry;
		mEntry.dwSize = sizeof(mEntry);

		do {
			if (!strcmp(_bstr_t(mEntry.szModule), moduleName)) {
				CloseHandle(module);

				PModule mod = { reinterpret_cast<DWORD>(mEntry.hModule), mEntry.modBaseSize };
				return mod;
			}
		} while (Module32Next(module, &mEntry));

		PModule mod = { static_cast<DWORD>(false), static_cast<DWORD>(false) };
		return mod;
	}

	template <class T>
	T Read(const DWORD addr) {
		T _read;
		ReadProcessMemory(_process, reinterpret_cast<LPVOID>(addr), &_read, sizeof(T), NULL);
		return _read;
	}

	void Read(const LPCVOID addr, LPVOID buf, DWORD size) {
		ReadProcessMemory(_process, addr, buf, size, NULL);
		return;
	}

	template <class T>
	void Write(const DWORD addr, T val) {
		WriteProcessMemory(_process, reinterpret_cast<LPVOID>(addr), &val, sizeof(T), NULL);
	}

	DWORD FindPattern(const DWORD start, const DWORD size, const char* sig, const char* mask) {
		BYTE* data = new BYTE[size];

		unsigned long bytesRead;
		if (!ReadProcessMemory(_process, reinterpret_cast<LPVOID>(start), data, size, &bytesRead)) {
			return NULL;
		}

		for (DWORD i = 0; i < size; i++) {
			if (DataCompare(static_cast<const BYTE*>(data + i), reinterpret_cast<const BYTE*>(sig), mask)) {
				return start + i;
			}
		}
		return NULL;
	}
	DWORD FindPatternArray(const DWORD start, const DWORD size, const char* mask, int count, ...) {
		char* sig = new char[count + 1];
		va_list ap;
		va_start(ap, count);
		for (int i = 0; i < count; i++) {
			char read = va_arg(ap, char);
			sig[i] = read;
		}
		va_end(ap);
		sig[count] = '\0';
		return FindPattern(start, size, sig, mask);
	}

	DWORD FindTextPattern(std::string moduleName, char* string)
	{
		DWORD Address = GetModule(moduleName.c_str()).dwBase;
		MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
		DWORD Length = ModInfo.SizeOfImage;

		int len = strlen(string);
		char* szMask = new char[len + 1];
		for (int i = 0; i < len; i++)
		{
			szMask[i] = 'x';
		}
		szMask[len] = '\0';

		for (DWORD c = 0; c < Length; c += 1)
		{
			if (bCompare((BYTE*)(Address + c), (BYTE*)string, szMask))
			{
				return (DWORD)(Address + c);
			}
		}
		return 0;
	}

	void Exit()
	{
		CloseHandle(_process);
	}
private:
	template <typename T = uintptr_t>
	static auto findPattern(const char* module, std::string pattern, size_t offset = 0) noexcept
	{
		MODULEINFO moduleInfo;

		if (GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(module), &moduleInfo, sizeof(moduleInfo))) {
			char* begin = static_cast<char*>(moduleInfo.lpBaseOfDll);
			char* end = begin + moduleInfo.SizeOfImage - pattern.length() + 1;

			for (char* c = begin; c != end; c++) {
				bool matched = true;
				auto it = c;

				if (*(c + pattern.length() - 1) != pattern.back())
					continue;

				for (auto a : pattern) {
					if (a != '?' && *it != a) {
						matched = false;
						break;
					}
					it++;
				}
				if (matched)
					return reinterpret_cast<T>(c + offset);
			}
		}
		MessageBoxW(NULL, L"Failed to find pattern", L"Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	bool DataCompare(const BYTE * pData, const BYTE * pMask, const char* pszMask) {
		for (; *pszMask; ++pszMask, ++pData, ++pMask) {
			if (*pszMask == 'x' && *pData != *pMask) {
				return false;
			}
		}
		return (*pszMask == NULL);
	}
	DWORD GetPIDbyName(const char* szProcessName) {
		HANDLE hSnapshot;
		PROCESSENTRY32 pe = { sizeof(pe) };
		hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot == INVALID_HANDLE_VALUE)
			return 0;
		if (!Process32First(hSnapshot, &pe))
			return 0;
		do {
			if (!lstrcmpiA(pe.szExeFile, szProcessName))
				return pe.th32ProcessID;
		} while (Process32Next(hSnapshot, &pe));
		return 0;
	}
	HANDLE _process;
	DWORD pID;
};

extern Memory mem;