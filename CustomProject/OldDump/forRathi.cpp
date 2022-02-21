#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <ctime>
#include <conio.h>

using namespace std;

#define dwLocalPlayer 0xDA545C
#define dwGlowObjectManager 0x5307C30
#define dwEntityList 0x4DBF73C
#define m_iGlowIndex 0x10488
#define m_iTeamNum 0xF4
#define m_iHealth 0x100
#define m_bDormant 0xED
#define m_dwBoneMatrix 0x26A8
#define m_vecOrigin 0x138
#define dwClientState_MapDirectory 0x188
#define dwClientState 0x588FEC
#define dwGameDir 0x627700
#define m_bSpotted 0x93D
#define m_bSpottedByMask 0x980

uintptr_t moduleBase;
DWORD procId;
HWND hwnd;
uintptr_t engineaddr;
HANDLE hProcess;

struct GlowStruct {
	float red, green, blue, alpha;

	uint8_t buffer1[8];
	float unknown = 1.f;
	uint8_t buffer2[4];
	BYTE renderOccluded = true;
	BYTE renderUnoccluded = false;
	BYTE fullBloom = false;

	void setup(float red, float green, float blue) {
		this->red = red;
		this->green = green;
		this->blue = blue;
		this->alpha = 0.6f;
	}

	void setAlpha(float alpha) {
		this->alpha = alpha;
	}

}glowStruct;
string titleGen(long num) {
	string titleName;
	for (int i = 0;i < num;i++) {
		titleName += rand() % 100 + 30;
	}
	return titleName;
};

void polymorphic() {
	srand(time(0));
	int count = 0;
	for (count = 0;count < 10;count++) {
		int index = rand() % (6 - 0 + 1) + 0;
		switch (index) {

		case 0:
			__asm __volatile{
				sub eax, 3
				add eax, 1
				add eax, 2
			}

		case 1:
			__asm __volatile{
				push eax
				pop eax
			}

		case 2:
			__asm __volatile{
				inc eax
				dec eax
			}

		case 3:
			__asm __volatile{
				dec eax
				add eax, 1
			}

		case 4:
			__asm __volatile{
				pop eax
				push eax
			}

		case 5:
			__asm __volatile{
				mov eax, eax
				sub eax, 1
				add eax, 1
			}

		case 6:
			__asm __volatile{
				xor eax, eax
				mov eax, eax
			}
		}
	}
}

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
int main() {

	polymorphic();
	SetConsoleTitle(titleGen(rand() % 100 + (time(0) % 100)).c_str());

	cout << "Hello World :P ... Working fine post PGL major update 22nd Oct 21 \n\n";
	//cout << "'F1' to toggleMain, 'F2' to toggle isVisible and 'END' to exit.\n\n\n";
	cout << "Enjoy kro Rathi brothers. Bs blatant mt ho jana.. ni to Allah bi bacha ni skte !\n\n";
	cout << "F1 se on/off hota hai, F2 se 'behind wall color change' on/off hai \n\n";
	cout << "Enjoy kro new Radar visibility with 'F3'\n\n";
	cout << "Also dont forget to press 'END' jb bnd krna ho. Direct window mt bnd kr dena. VERY IMP !!";

	hwnd = FindWindowA(NULL, "Counter-Strike: Global Offensive");

	GetWindowThreadProcessId(hwnd, &procId);

	auto client = getModule("client.dll", procId);
	moduleBase = (uintptr_t)client.modBaseAddr;

	engineaddr = getModuleDll(procId, "engine.dll");
	bool toggleMain = true;
	bool toogleVisible = true;
	bool toogleRadar = true;
	/*
	auto bytes = new uint8_t[client.modBaseSize]; //making a variable size of the module
	DWORD bytes_read;
	ReadProcessMemory(hProcess, client.modBaseAddr, bytes, client.modBaseSize, &bytes_read); //reading the module and storing as bytes_read
	//if (bytes_read != client.modBaseSize) throw; //checking that the size of bytes read is = to size of bytes in the module

	//example of scanning for LocalPlayer and EntityList
	uintptr_t LocalPlayerPtr = find_pattern(client, bytes, "8D 34 85 ? ? ? ? 89 15 ? ? ? ? 8B 41 08 8B 48 04 83 F9 FF", 0x3, 0x4); //0x3 is the offset, 0x4 is the extra
	uintptr_t EntityListPtr = find_pattern(client, bytes, "BB ? ? ? ? 83 FF 01 0F 8C ? ? ? ? 3B F8", 0x1, 0x0); //0x1 is the offset, there is no extra
	delete[] bytes;
	printf("[+] Found dwLocalPlayer @ 0x%X\n", LocalPlayerPtr - (uintptr_t)client.modBaseAddr);
	printf("[+] Found dwEntityList @ 0x%X\n", EntityListPtr - (uintptr_t)client.modBaseAddr);
	cout << "bytes_read=" << bytes_read<<", client.modBaseSize="<< client.modBaseSize<<"\n";
	//optional example
	uintptr_t localPlayer = LocalPlayerPtr - (uintptr_t)client.modBaseAddr; //getting rid of the base is optional, you will just have to add it back when you RPM
	uintptr_t entityList = EntityListPtr - (uintptr_t)client.modBaseAddr;

	//cout << "localPlayer="<< localPlayer<<"\t address="<< &localPlayer;
	//uintptr_t localPlayer = RPM<uintptr_t>(moduleBase + dwLocalPlayer);

*/
	while (!GetAsyncKeyState(VK_END))
	{
		if (GetAsyncKeyState(VK_F1) & 1) {
			toggleMain = !toggleMain;
			toogleVisible = !toogleVisible;
		}
		if (GetAsyncKeyState(VK_F2) & 1) {
			toogleVisible = !toogleVisible;
			if (toogleVisible) {
			}
		}
		if (GetAsyncKeyState(VK_F3) & 1) {
			toogleRadar = !toogleRadar;
		}

		if (toggleMain || toogleRadar) {
			polymorphic();
			uintptr_t localPlayer = RPM<uintptr_t>(moduleBase + dwLocalPlayer);
			uintptr_t dwGlowManager = RPM<uintptr_t>(moduleBase + dwGlowObjectManager);
			int LocalTeam = RPM<int>(localPlayer + m_iTeamNum);

			for (int i = 1; i < 32; i++) {
				uintptr_t dwEntity = RPM<uintptr_t>(moduleBase + dwEntityList + i * 0x10);

				int iGlowIndx = RPM<int>(dwEntity + m_iGlowIndex);
				int EnmHealth = RPM<int>(dwEntity + m_iHealth); if (EnmHealth < 1 || EnmHealth > 100) continue;
				int Dormant = RPM<int>(dwEntity + m_bDormant); if (Dormant) continue;
				int EntityTeam = RPM<int>(dwEntity + m_iTeamNum);
				int isSpottedByMask = RPM<int>(dwEntity + m_bSpottedByMask);
				int isSpotted = RPM<int>(dwEntity + m_bSpotted);
				// glowStruct format 1/0 - (r, g, b)
				if (LocalTeam == EntityTeam) {
					glowStruct.setup(0, 0, 1); // blue
				}
				else if (LocalTeam != EntityTeam)
				{
					if (toogleRadar && isSpotted == false) {
						WPM<bool>(dwEntity + m_bSpotted, true);
					}
					if (toogleVisible) {
						if (isSpottedByMask) {
							glowStruct.setup(1, 0, 1); // pink
						}
						else {
							glowStruct.setup(1, 0, 1); // pink
						}
					}
					else {
						glowStruct.setup(1, 0, 0); // red

					}
				}
				if (EnmHealth < 30) {
					glowStruct.setAlpha(1); // full
				}

				if (toggleMain) {
					WPM<GlowStruct>(dwGlowManager + (iGlowIndx * 0x38) + 0x8, glowStruct);
				}
			}
		}
	}
	closeHandle();
	getch();
}