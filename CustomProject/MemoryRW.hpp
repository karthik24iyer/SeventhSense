#pragma once

#include<iostream>

class MemoryRW {
	public:
		HANDLE hProcess;

		MODULEENTRY32 getModule(const char* modName, DWORD procId) {
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
			HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
			if (hSnap != INVALID_HANDLE_VALUE) {
				MODULEENTRY32 modEntry;
				modEntry.dwSize = sizeof(modEntry);
				if (Module32First(hSnap, &modEntry)) {
					do {
						if (!strcmp(modEntry.szModule, modName)) {
							CloseHandle(hSnap);
							return modEntry;
						}
					} while (Module32Next(hSnap, &modEntry));
				}
			}
			return MODULEENTRY32();// module = { -1 };
			//return module;
		}

		uintptr_t getModuleDll(uintptr_t procID, const char* modName) {
			HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
			MODULEENTRY32 mEntry;
			mEntry.dwSize = sizeof(mEntry);

			do {
				if (!strcmp(mEntry.szModule, modName)) {
					CloseHandle(hModule);
					return (uintptr_t)mEntry.hModule;
				}
			} while (Module32Next(hModule, &mEntry));

			if (GetLastError() == ERROR_NO_MORE_FILES)
				return NULL;

			return NULL;
		}

		template<typename T> T RPM(SIZE_T address) {
			T buffer;
			ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(T), NULL);
			return buffer;
		}

		template<typename T> void WPM(SIZE_T address, T buffer) {
			WriteProcessMemory(hProcess, (LPVOID)address, &buffer, sizeof(buffer), NULL);
		}

		void closeHandle() {
			CloseHandle(hProcess);
		}
};