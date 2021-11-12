#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <ctime>
#include <conio.h>
#include <vector>
#include <stdlib.h>
#include "Polymorph.hpp"
#include "MemoryRW.hpp"
#include "StructUtil.hpp"
#include "MyOffsets.hpp"
#include "FeatureUtil.hpp"

using namespace std;
using namespace offsets;

uintptr_t moduleBase;
uintptr_t engineBase;
DWORD procId;
HWND hwnd;
Polymorph pm;
MemoryRW mem;
FeatureUtil util;
uintptr_t engineaddr;
const int maxPlayers = 32;
const float chamsBrightness = 10.f;
float chamsBrightnessReset = 0.f;


bool toggleMain;
bool toggleVisible;
bool toggleRadar;
bool toggleFlash;
bool toggleChams;
bool toggleTrigger;
bool toggleDelay;
bool toggleFriendlyFire;

void setToggles() {
	toggleMain = true;
	toggleVisible = true;
	toggleRadar = true;
	toggleFlash = true;
	toggleChams = false;
	toggleTrigger = false;
	toggleDelay = false;
	toggleFriendlyFire = false;
}
string titleGen(long num) {
	string titleName;
	for (int i = 0;i < num;i++) {
		titleName += rand() % 100 + 30;
	}
	return titleName;
};

int getIntOffset(uintptr_t offset) {
	return mem.RPM<int>(offset);
}

uintptr_t getPtrOffset(uintptr_t offset) {
	return mem.RPM<uintptr_t>(offset);
}

BYTE getByteOffset(uintptr_t offset) {
	return mem.RPM<BYTE>(offset);
}

void setGlowOffset(uintptr_t offset, int glowIndex) {
	mem.WPM<GlowStruct>(offset + (glowIndex * 0x38) + 0x8, glowStruct);
}

int isEntitySpotted(uintptr_t entity) {
	return getIntOffset(entity + m_bSpotted);
}

void setEntitySpotted(uintptr_t entity) {
	mem.WPM<bool>(entity + m_bSpotted, true);
}

int isScoped(uintptr_t entity) {
	return getIntOffset(entity + m_bIsScoped);
}

int getIsDormant(uintptr_t entity) {
	return getIntOffset(entity + m_bDormant);
}

int	getEntityTeam(uintptr_t entity)
{
	return mem.RPM<int>(entity + m_iTeamNum);
}

int	getEntityHealth(uintptr_t entity)
{
	return mem.RPM<int>(entity + m_iHealth);
}

int	isEntityAlive(uintptr_t entity)
{
	return getEntityHealth(entity)>0?true:false;
}

int getEntityLifeState(uintptr_t entity)
{
	return mem.RPM<int>(entity + m_lifeState);
}


bool isValidEntity(uintptr_t entity)
{
	if ((isEntityAlive(entity) && getEntityTeam(entity) != 0) && !getIsDormant(entity) && getEntityLifeState(entity) == 0)
		return true;
	return false;
}

int getLocalCrossID(uintptr_t localPlayer)
{
	return mem.RPM<int>(localPlayer + m_iCrosshairId);
}

void changeShowCondition(int entityHealth) {
	if (entityHealth < 30) {
		glowStruct.setAlpha(0.8f); // Near full
	}
	if (entityHealth < 10) {
		glowStruct.setAlpha(1.f); // full
	}

}

bool isVisible(uintptr_t player1, uintptr_t player2) {
	int index = getIntOffset(player1 + 0x64);
	int byMask = getIntOffset(player2 + m_bSpottedByMask);
	return (bool)(byMask && (1 << (index - 1)));
}

void writeCham(uintptr_t entity, char c) {

	switch (c) {
	case 'r':
		mem.WPM<BYTE>(entity + m_clrRender, BYTE(255));
		mem.WPM<BYTE>(entity + m_clrRender + 0x01, BYTE(50));
		mem.WPM<BYTE>(entity + m_clrRender + 0x02, BYTE(50));
		break;

	case 'g':
		mem.WPM<BYTE>(entity + m_clrRender, BYTE(50));
		mem.WPM<BYTE>(entity + m_clrRender + 0x01, BYTE(255));
		mem.WPM<BYTE>(entity + m_clrRender + 0x02, BYTE(50));
		break;

	case 'b':
		mem.WPM<BYTE>(entity + m_clrRender, BYTE(50));
		mem.WPM<BYTE>(entity + m_clrRender + 0x01, BYTE(50));
		mem.WPM<BYTE>(entity + m_clrRender + 0x02, BYTE(255));
		break;

	case 'y':
		mem.WPM<BYTE>(entity + m_clrRender, BYTE(255));
		mem.WPM<BYTE>(entity + m_clrRender + 0x01, BYTE(255));
		mem.WPM<BYTE>(entity + m_clrRender + 0x02, BYTE(50));
		break;
	}
}

int	getEntClassID(int entity)
{
	int one = mem.RPM<int>(entity + 0x8);
	int two = mem.RPM<int>(one + 2 * 0x4);
	int three = mem.RPM<int>(two + 0x1);
	return mem.RPM<int>(three + 0x14);
}

DWORD getActiveWeapon(uintptr_t entity)
{
	DWORD weaponIndex = mem.RPM<DWORD>(entity + m_hActiveWeapon) & 0xFFF;
	return mem.RPM<DWORD>((moduleBase + dwEntityList + weaponIndex * 0x10) - 0x10);
}

long getActiveWeaponId(DWORD activeWeapon) {
	//std::cout<<"\weaponId="<< mem.RPM<DWORD>(activeWeapon + m_iItemDefinitionIndex);
	return mem.RPM<DWORD>(activeWeapon + m_iItemDefinitionIndex);
}

int isEntityDefusing(uintptr_t entity) {
	return getIntOffset(entity + m_bIsDefusing);
}

boolean isEnityHoldingC4(uintptr_t entity) {
	if (getActiveWeaponId(getActiveWeapon(entity)) == c4) {
		return true;
	}
	return false;
}
int getClipsInActiveWeapon(uintptr_t entity) {
	DWORD activeWeapon = getActiveWeapon(entity);
	//std::cout<<"\nclips= "<< mem.RPM<DWORD>(activeWeapon + m_iClip1);
	return mem.RPM<DWORD>(activeWeapon + m_iClip1);
}

boolean isEntityMoving(uintptr_t entity) {
	int* velocity = mem.RPM<int*>(entity + m_vecVelocity);
	if (velocity == NULL) {
		//std::cout << "\n is still";
		//std::cout << "\nmoving with vel="<<velocity;
		return false;
	}
	else {
		return true;
	}
}
void startFire() {
	mem.WPM<int>(moduleBase + dwForceAttack, 5);
}

void stopFire() {
	mem.WPM<int>(moduleBase + dwForceAttack, 4);
}

void forceAttack(uintptr_t localPlayer)
{	
	DWORD activeWeapon = getActiveWeapon(localPlayer);
	long activeWeaponId = getActiveWeaponId(activeWeapon);
	//std::cout << "\nfound id: "<< activeWeaponId;
	srand((unsigned int)time(0));
	int delay = 100 + rand() % 100 + 1;

	boolean isSniper = false;
	boolean isBurstFWeapon = false;
	boolean isSingleFWeapon = false;
	boolean isSemiFWeapon = false;
	boolean isGay = false;
	boolean isSpam = false;
	
	if (util.getIsSniper(activeWeaponId)) {
		isSniper = true;
	}
	else if (util.getIsBurstFWeapon(activeWeaponId)) {
		isBurstFWeapon = true;
	}
	else if (util.getIsSingleFWeapon(activeWeaponId)) {
		isSingleFWeapon = true;
	}
	else if (util.getIsSemiFWeapon(activeWeaponId)) {
		isSemiFWeapon = true;
	}
	else if (util.getIsGay(activeWeaponId)) {
		isGay = true;
	}
	else if (util.getIsSpam(activeWeaponId)) {
		isSpam = true;
	}

	if (isSniper && isScoped(localPlayer) && !isEntityMoving(localPlayer)) {
		if (toggleDelay) {
			Sleep(delay);
		}
		else {
			Sleep(delay % 5);
		}
		startFire();
		Sleep(20);
		stopFire();
	}
	else if(!isSniper) {
		
		if (isBurstFWeapon) {
			if (toggleDelay) {
				Sleep(delay%100);
			}
			else {
				Sleep(delay % 5);
			}
			int clipCount = 10;
			while(clipCount > 0) {
				startFire();
				clipCount--;
				Sleep(15);
			}
			stopFire();
		}
		else if (isSemiFWeapon) {
			if (toggleDelay) {
				Sleep(delay % 75);
			}
			else {
				Sleep(delay % 5);
			}
			int clipCount = 7;
			while (clipCount > 0) {
				startFire();
				clipCount--;
				Sleep(25);
			}
			stopFire();
		}
		else if (isSingleFWeapon) {
			if (toggleDelay) {
				Sleep(delay % 50);
			}
			else {
				Sleep(delay % 5);
			}
			int clipCount = 1;
			while (clipCount > 0) {
				startFire();
				clipCount--;
				Sleep(15);
			}
			stopFire();
		}
		else if (isGay) {
			int clipCount = 5;
			if (toggleDelay) {
				Sleep(delay);
			}
			else {
				Sleep(delay % 25);
			}
			while (clipCount > 0) {
				if (isScoped(localPlayer) && !isEntityMoving(localPlayer)) {
					startFire();
				}
				clipCount--;
				Sleep(25);
			}
			stopFire();
		}
		else if (isSpam) {
			Sleep(delay % 25);
			int clipCount = 50;
			while (clipCount > 0) {
				startFire();
				clipCount--;
				Sleep(15);
			}
			stopFire();
		}
	}
}

void showConsoleMsg() {
	system("cls");
	std::cout << "\n\tHello World :P ... Working fine after PGL major update 22nd Oct 21 \n";
	std::cout << "\n\n\tMain: " << toggleMain;
	std::cout << "\n\n\tVis: " << toggleVisible;
	std::cout << "\n\n\tRdr: " << toggleRadar;
	std::cout << "\n\n\tFlh: " << toggleFlash;
	std::cout << "\n\n\tCham: " << toggleChams;
	std::cout << "\n\n\ttrgr: " << toggleTrigger;
	std::cout << "\n\n\tdl: " << toggleDelay;
	std::cout << "\n\n\tfrfr: " << toggleFriendlyFire;
	std::cout << "\n\n\n";
}

int main() {

restart:
	setToggles();
	bool isWorking = true;
	bool chamCount = false;
	bool resetChams = false;
	bool isEntityLoaded = false;
	pm.polymorphic();
	SetConsoleTitle(titleGen(rand() % 100 + (time(0) % 1000)).c_str());
	showConsoleMsg();
	//std::cout << "'F1' to toggleMain, 'F2' to toggle isVisible and 'END' to exit.\n\n\n";

	hwnd = FindWindowA(NULL, "Counter-Strike: Global Offensive");

	if (hwnd == NULL) {
		isWorking = false;
		std::cout << "\ttitle not found\n\n";
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
		if (GetAsyncKeyState(VK_NUMLOCK) & 1) {
			toggleMain = !toggleMain;
			showConsoleMsg();
		}
		if (GetAsyncKeyState(VK_F1) & 1) {
			toggleVisible = !toggleVisible;
			showConsoleMsg();
		}

		if (GetAsyncKeyState(VK_F3) & 1) {
			toggleRadar = !toggleRadar;
			showConsoleMsg();
		}

		if (GetAsyncKeyState(VK_CAPITAL) & 1) {
			toggleTrigger = !toggleTrigger;
			showConsoleMsg();
		}

		if (GetAsyncKeyState(VK_SCROLL) & 1) {
			toggleDelay = !toggleDelay;
			showConsoleMsg();
		}

		if (GetAsyncKeyState(VK_F6) & 1) {
			toggleFlash = !toggleFlash;
			showConsoleMsg();
		}

		if (GetAsyncKeyState(VK_F2) & 1) {
			toggleChams = !toggleChams;
			if (!toggleChams) resetChams = true;
			showConsoleMsg();
		}
/*
		if (GetAsyncKeyState(VK_NUMPAD0) & 1) {
			resetChams = !resetChams;
		}
*/
		if (GetAsyncKeyState(VK_NUMPAD5) & 1) {
			toggleFriendlyFire = !toggleFriendlyFire;
			showConsoleMsg();
		}

		if (toggleMain) {
			pm.polymorphic();
			uintptr_t localPlayer = getPtrOffset(moduleBase + dwLocalPlayer);
			uintptr_t entity[maxPlayers];
			int entityTeam[maxPlayers];
			int currentPlayers=1;

			boolean debugId = false;
			if (debugId) {
				DWORD activeWeapon = getActiveWeapon(localPlayer);
				long activeWeaponId = getActiveWeaponId(activeWeapon);
				std::cout << "\nfound id: "<< activeWeaponId;
				break;
			}
			int localTeam = getIntOffset(localPlayer + m_iTeamNum);
			if (localTeam < 0) {
				isWorking = false;
				std::cout << "\tDEBUG: LocalTeam=" << localTeam << "\n\n";
				break;
			}
			if (toggleFlash) {
				mem.WPM<float>(localPlayer + m_flFlashMaxAlpha, 0.f);
			}
			else {
				mem.WPM<float>(localPlayer + m_flFlashMaxAlpha, 255.f);
			}

			for (int i = 1; i < maxPlayers; i++) {
				entity[currentPlayers] = getPtrOffset(moduleBase + dwEntityList + i * 0x10);
				if (!isValidEntity(entity[currentPlayers])) {
					continue;
				}
				entityTeam[currentPlayers] = getEntityTeam(entity[currentPlayers]);
				currentPlayers++;
			}

			if (toggleVisible) {
				for (int i = 1; i < currentPlayers; i++) {
					uintptr_t dwGlowManager = getPtrOffset(moduleBase + dwGlowObjectManager);
					int iGlowIndx = getIntOffset(entity[i] + m_iGlowIndex);
					// std::cout << "DEBUG: LocalTeam=" << localTeam << ", iGlowIndx=" << iGlowIndx << ", entityTeam=" << entityTeam<<"\n\n";
					//std::cout << "LocalTeam=" << localTeam << ", iGlowIndx=" << iGlowIndx << ", entityHealth=" << entityHealth << ", isDormant=" << isDormant << ", entityTeam=" << entityTeam << "\n";

					if (entityTeam[i] == 3) {
						glowStruct.setup('b'); // blue
					}
					else if (entityTeam[i] == 2) {
						glowStruct.setup('r'); // red
					}

					if (localTeam != entityTeam[i])
					{
						if ((!isVisible(localPlayer, entity[i]) || !isVisible(entity[i], localPlayer))) {
							glowStruct.setup('p'); // pink
						}
						if (isEntityDefusing(entity[i])) {
							glowStruct.setup('y'); //yellow
						}
					}
					if (isEnityHoldingC4(entity[i])) {
						glowStruct.setup('y'); //yellow
					}
					changeShowCondition(getEntityHealth(entity[i]));
					setGlowOffset(dwGlowManager, iGlowIndx);
				}
			}

			if (toggleRadar) {
				for (int i = 1; i < currentPlayers; i++) {
					int isSpotted = isEntitySpotted(entity[i]);
					//std::cout << "isSpotted" << isSpotted << "\n";
					if (toggleRadar && isSpotted == false) {
						if (isSpotted != 0 && isSpotted != 1) {
							isWorking = false;
							break;
						}
						else {
							setEntitySpotted(entity[i]);
						}
					}
				}
			}

			if (toggleChams) {
				for (int i = 1; i < currentPlayers; i++) {
					//std::cout << "\nentityList["<<i<<"]= "<<entityList[i];
					if ((entityTeam[i] == 3 || entityTeam[i] == 2)) {
						if (!chamCount) {
							defChamStruct.setup(getByteOffset(entity[i] + m_clrRender),
								getByteOffset(entity[i] + m_clrRender + 0x01),
								getByteOffset(entity[i] + m_clrRender + 0x02));
							chamCount = true;
						}

						if (entityTeam[i] == 3)
						{
							if (isEntityDefusing(entity[i])) {
								writeCham(entity[i], 'y');
							}
							else {
								writeCham(entity[i], 'b');
							}
						}
						else if (entityTeam[i] == 2)
						{
							if (isEnityHoldingC4(entity[i])) {
								writeCham(entity[i], 'y');
							}
							else {
								writeCham(entity[i], 'r');
							}
						}

						DWORD thisPtr = (int)(engineBase + model_ambient_min - 0x2c);
						DWORD xored = *(DWORD*)&chamsBrightness ^ thisPtr;
						mem.WPM<int>(engineBase + model_ambient_min, xored);
					}
				}
			}

			if (resetChams) {
				for (int i = 1; i < currentPlayers; i++) {
					if ((entityTeam[i] == 3 || entityTeam[i] == 2)) {
						mem.WPM<BYTE>(entity[i] + m_clrRender, defChamStruct.r);
						mem.WPM<BYTE>(entity[i] + m_clrRender + 0x01, defChamStruct.g);
						mem.WPM<BYTE>(entity[i] + m_clrRender + 0x02, defChamStruct.b);

						DWORD thisPtr = (int)(engineBase + model_ambient_min - 0x2c);
						DWORD xored = *(DWORD*)&chamsBrightnessReset ^ thisPtr;
						mem.WPM<int>(engineBase + model_ambient_min, xored);
					}
				}
				resetChams = false;
			}

			if (toggleTrigger) {
				DWORD enemyOnCH = mem.RPM<DWORD>(moduleBase + dwEntityList + ((getLocalCrossID(localPlayer) - 1) * 0x10));
				boolean isValid = isValidEntity(enemyOnCH);
				boolean isFriendlyFire = toggleFriendlyFire && (getEntityTeam(localPlayer) == getEntityTeam(enemyOnCH));
				if ((getEntityTeam(localPlayer) != getEntityTeam(enemyOnCH) && isValid) || (isFriendlyFire && isValid))
				{
					forceAttack(localPlayer);
				}
			}
		}
	}
	mem.closeHandle();
	if (!isWorking) {
		std::cout << "\tnot working currently\n\n";
	}
	char k;
	std::cout << "\tRestart ?? y/* \t";
	std::cin >> k;
	if (k == 'Y' || k == 'y') {
		std::cout << "\n\n";
		goto restart;
	}
}