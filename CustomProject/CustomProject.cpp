#define NOMINMAX

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <ctime>
#include <conio.h>
#include <vector>
#include <stdlib.h>
#include "MyOffsets.hpp"
#include "Polymorph.hpp"
#include "MemoryRW.hpp"
#include "FeatureUtil.hpp"
#include "Trigger.hpp"
#include "Visible.hpp"
#include "Cham.hpp"
#include "Thatsit.hpp"
#include "Toggles.hpp"
#include <string>

using namespace std;
using namespace offsets;

bool toggleMain;
bool toggleVisible;
bool toggleRadar;
bool toggleFlash;
bool toggleChams;
bool resetChams;
bool toggleTrigger;
bool toggleDelay;
bool toggleNoRecoil;
bool toggleAim;
bool toggleFriendlyFire;

uintptr_t moduleBase;
uintptr_t engineBase;
DWORD procId;
HWND hwnd;
uintptr_t engineaddr;
HANDLE hConsole;

Polymorph pm;
MemoryRW mem;
FeatureUtil util;
Trigger trgr;
Visible vis;
Cham cham;
Stable nr;
Thatsit aim;
Toggles menu;

const int maxPlayers = 32;
const float chamsBrightness = 10.f;
float chamsBrightnessReset = 0.f;

void clearConsoleInputBuffer()
{
	/* keybd_event('S', 0, 0, 0);
	keybd_event('S', 0,KEYEVENTF_KEYUP, 0);
	keybd_event(VK_BACK, 0, 0, 0);
	keybd_event(VK_BACK, 0,KEYEVENTF_KEYUP, 0); */
	PINPUT_RECORD ClearingVar1 = new INPUT_RECORD[256];
	DWORD ClearingVar2;
	ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), ClearingVar1, 256, &ClearingVar2);
	delete[] ClearingVar1;
}

int main() {

restart:
	menu.setToggles();
	bool isWorking = 1;
	bool chamCount = 0;
	bool debug = 0;
	resetChams = 0;
	bool isEntityLoaded = 0;
	pm.polymorphic();
	SetConsoleTitle(pm.titleGen(rand() % 100 + (time(0) % 1000)).c_str());
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	menu.showConsoleMsg();

	hwnd = FindWindowA(NULL, "Counter-Strike: Global Offensive");
/*
	const char* hid = "{ced62aa7-f371-11ea-b873-806e6f6e6963}";
	HW_PROFILE_INFO hwProfileInfo;
	if (GetCurrentHwProfile(&hwProfileInfo)) {
		printf("HWID: %s\n", hwProfileInfo.szHwProfileGuid);
		const char* newHid = hwProfileInfo.szHwProfileGuid;
		if (strcmp(hid, newHid)==0) {
		}
	}
*/
	if (hwnd == NULL) {
		if (!debug) {
			isWorking = false;
			std::cout << "\ttitle not found\n\n";
		}
	}
	else {

		GetWindowThreadProcessId(hwnd, &procId);
		auto client = mem.getModule("client.dll", procId);
		moduleBase = (uintptr_t)client.modBaseAddr;

		//engineaddr = mem.getModuleDll(procId, "engine.dll");
		auto engine = mem.getModule("engine.dll", procId);
		engineBase = (uintptr_t)engine.modBaseAddr;
	}


	while (!GetAsyncKeyState(VK_END) && isWorking)
	{
		menu.fetchInput();
		
		if (toggleMain) {
			pm.polymorphic();

			uintptr_t localPlayer = util.getLocalPlayer();
			int localTeam = util.getIntOffset(localPlayer + m_iTeamNum);
			if (localTeam < 0 && !debug) {
				isWorking = false;
				std::cout << "\tDEBUG: LocalTeam=" << localTeam << "\n\n";
				break;
			}

			boolean debugId = false;
			if (debugId) {
				DWORD activeWeapon = util.getActiveWeapon(localPlayer);
				long activeWeaponId = util.getActiveWeaponId(activeWeapon);
				std::cout << "\nfound id: "<< activeWeaponId;
				break;
			}

			uintptr_t entity[maxPlayers];
			int entityTeam[maxPlayers];
			int currentPlayers = 1;

			for (int i = 1; i < maxPlayers; i++) {
				entity[currentPlayers] = util.getCurrentEntity(i);
				if (!util.isValidEntity(entity[currentPlayers])) {
					continue;
				}
				entityTeam[currentPlayers] = util.getEntityTeam(entity[currentPlayers]);
				currentPlayers++;
			}

			if (toggleAim) {
				aim.aimBot(localPlayer, entity, currentPlayers);
			}

			if (toggleVisible) {
				vis.makeEntityVisible(localPlayer, entity, entityTeam, currentPlayers);
			}

			if (toggleRadar) {
				vis.showEntityOnRadar(entity, currentPlayers);
			}

			if (toggleFlash) {
				vis.stopFlash(localPlayer);
			}
			else {
				vis.resetFlash(localPlayer);
			}

			if (toggleChams) {
				cham.enableChams(entity, entityTeam, chamCount, currentPlayers);
				cham.glowCham(engineBase, chamsBrightness);
			}

			if (resetChams) {
				cham.disableChams(entity, entityTeam, currentPlayers);
				cham.glowCham(engineBase, chamsBrightnessReset);
				resetChams = false;
			}

			if (toggleTrigger) {
				trgr.callTrigger();				
			}

			if (toggleNoRecoil) {
				nr.noRecoil();
			}	

		}
	}
	mem.closeHandle();
	if (!isWorking) {
		std::cout << "\tnot working currently\n\n";
	}
	char k;
	std::cout << "\tRestart ?? y/* \t";
	clearConsoleInputBuffer();
	std::cin >> k;
	if (k == 'Y' || k == 'y') {
		std::cout << "\n\n";
		goto restart;
	}
}