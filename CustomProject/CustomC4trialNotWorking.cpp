#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <ctime>
#include <conio.h>
#include <vector>
#include <stdlib.h>
//#include "Util.hpp"
#include "Polymorph.hpp"
#include "MemoryRW.hpp"
//#include "Vector.hpp"
#include "StructUtil.hpp"
#include "MyOffsets.hpp"
//#include "FeatureUtil.hpp"

using namespace std;
using namespace offsets;

uintptr_t moduleBase;
uintptr_t engineBase;
DWORD procId;
HWND hwnd;
Polymorph pm;
MemoryRW mem;
uintptr_t engineaddr;
//static rn::bsp_parser* bspParser = new rn::bsp_parser;
const int maxPlayers = 32;
const float chamsBrightness = 10.f;
float chamsBrightnessReset = 0.f;


bool toggleMain = false;
bool toggleVisible = true;
bool toogleRadar = true;
bool toggleFlash = true;
bool toggleChams = false;
bool resetChams = false;
bool chamCount = false;



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

int	getEntClassID(int entity)
{
	int one = mem.RPM<int>(entity + 0x8);
	int two = mem.RPM<int>(one + 2 * 0x4);
	int three = mem.RPM<int>(two + 0x1);
	return mem.RPM<int>(three + 0x14);
}

void setGlowOffset(uintptr_t offset, int glowIndex) {
	mem.WPM<GlowStruct>(offset + (glowIndex * 0x38) + 0x8, glowStruct);
}


void setSpotted(uintptr_t entity) {
	mem.WPM<bool>(entity + m_bSpotted, true);
}

void changeShowCondition(int entityHealth) {
	if (entityHealth < 30) {
		glowStruct.setAlpha(0.8f); // Near full
	}
	if (entityHealth < 10) {
		glowStruct.setAlpha(1.f); // Near full
	}

}

bool isVisible(uintptr_t player1, uintptr_t player2) {
	int index = getIntOffset(player1 + 0x64);
	int byMask = getIntOffset(player2 + m_bSpottedByMask);
	return (bool)(byMask && (1 << (index - 1)));
}

void getc4entity()
{
	uintptr_t dwGlowManager = getPtrOffset(moduleBase + dwGlowObjectManager);
	int iGlowIndx = 0;
	for (int i = 1; i < 1337;i++)
	{
		uintptr_t glowObj = getPtrOffset(moduleBase + dwEntityList + i * 0x10);
		int isDormant = getIntOffset(glowObj + m_bDormant);
		if (isDormant) {
			continue;
		}
		int classId = getEntClassID((int)glowObj);
		/*if (classId==35) {
			iGlowIndx = getIntOffset(glowObj + i);
			cout << "foundC4, classId="<< classId <<"\n";
			//iGlowIndx = getIntOffset(glowObj + m_iGlowIndex);
			cout << "iGlowIndx: "<< iGlowIndx<<"\n";
			break;
		}*/
		if (classId == 129) {
			iGlowIndx = i;
			cout << "foundC4Planted, classId=" << classId << "\n";
			//iGlowIndx = getIntOffset(glowObj + m_iGlowIndex);
			//cout << "iGlowIndx: " << iGlowIndx << "\n";
			break;
		}
	}
	glowStruct.setup('g');
	mem.WPM<GlowStruct>(dwGlowManager + (iGlowIndx * 0x38) + 0x8, glowStruct);
}

void setChams()
{
	float r = 255;
	float g = 0;
	float b = 0;
	float brightness = 10;

	for (int x = 1; x < 1337; x++)
	{
		int entity = getIntOffset(moduleBase + dwEntityList + x * 0x10);
		int classId = getEntClassID(entity);
		//cout << "classId" << classId<<"\n";

		if (classId != 44 && classId != 170) {
			continue;
		}
		if (classId == 44) {
			//Model Color
			mem.WPM<BYTE>(dwEntityList + 0x70, BYTE(r));
			mem.WPM<BYTE>(dwEntityList + 0x71, BYTE(g));
			mem.WPM<BYTE>(dwEntityList + 0x72, BYTE(b));
		}
		else {
			//Viewmodel Color
			BYTE NewNormalColor = (brightness == 0) ? 255 : (BYTE)(255.f / (brightness / 10.f));
			mem.WPM<BYTE>(dwEntityList + 0x70, BYTE(NewNormalColor));
			mem.WPM<BYTE>(dwEntityList + 0x71, BYTE(NewNormalColor));
			mem.WPM<BYTE>(dwEntityList + 0x72, BYTE(NewNormalColor));
		}
	}
	//Model Brightness
	/*
	DWORD thisPtr = (int)(engineBase + model_ambient_min - 0x2c);
	DWORD xored = *(DWORD*)&brightness ^ thisPtr;
	mem.WPM<int>(engineBase + model_ambient_min, xored);*/
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

/*
Vec3 getBonePos(uint32_t entity, int boneID) {
	uint32_t boneMatrix = getPtrOffset(entity + m_dwBoneMatrix);
	BonePos bonepos = mem.RPM<BonePos>(boneMatrix + 0x30 * boneID + 0xC);
	return { bonepos.x, bonepos.y, bonepos.z };
}

Vec3 getOrigin(uintptr_t entity) {
	return mem.RPM<Vec3>(entity + m_vecOrigin);
}

char* getGameDirectory() {
	static array<char, 0x120> gameDirectory = mem.RPM<array<char, 0x120>>(engineaddr + dwGameDir);
	return gameDirectory.data();
}

char* getMapDirectory() {
	uintptr_t engineptr = mem.RPM<uintptr_t>(engineaddr + dwClientState);
	static array<char, 0x120> mapDirectory = mem.RPM<array<char, 0x120>>(engineptr + dwClientState_MapDirectory);
	return mapDirectory.data();
}
void loadMapRayCast() {
	if (bspParser->load_map(getGameDirectory(), getMapDirectory())) {
		//cout << "map rays loaded!\n";
	}
}
*/

int main() {

restart:
	bool isWorking = true;
	pm.polymorphic();
	SetConsoleTitle(titleGen(rand() % 100 + (time(0) % 1000)).c_str());

	cout << "Hello World :P ... Working fine after PGL major update 22nd Oct 21 \n\n";
	//cout << "'F1' to toggleMain, 'F2' to toggle isVisible and 'END' to exit.\n\n\n";

	hwnd = FindWindowA(NULL, "Counter-Strike: Global Offensive");

	if (hwnd == NULL) {
		isWorking = false;
		cout << "title not found\n\n";
	}
	else {

		GetWindowThreadProcessId(hwnd, &procId);
		auto client = mem.getModule("client.dll", procId);
		moduleBase = (uintptr_t)client.modBaseAddr;

		//engineaddr = mem.getModuleDll(procId, "engine.dll");
		auto engine = mem.getModule("engine.dll", procId);
		engineBase = (uintptr_t)engine.modBaseAddr;
	}

	//loadMapRayCast();

	while (!GetAsyncKeyState(VK_END) && isWorking)
	{
		if (GetAsyncKeyState(VK_HOME) & 1) {
			toggleMain = !toggleMain;
		}
		if (GetAsyncKeyState(VK_F1) & 1) {
			toggleVisible = !toggleVisible;
		}

		if (GetAsyncKeyState(VK_F3) & 1) {
			toogleRadar = !toogleRadar;
		}

		if (GetAsyncKeyState(VK_F6) & 1) {
			toggleFlash = !toggleFlash;
		}

		if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
			toggleChams = !toggleChams;
		}

		if (GetAsyncKeyState(VK_NUMPAD0) & 1) {
			resetChams = !resetChams;
		}

		getc4entity();
		if (toggleMain) {
			pm.polymorphic();
			uintptr_t localPlayer = getPtrOffset(moduleBase + dwLocalPlayer);
			uintptr_t dwGlowManager = getPtrOffset(moduleBase + dwGlowObjectManager);
			int localTeam = getIntOffset(localPlayer + m_iTeamNum);
			if (localTeam < 0) {
				isWorking = false;
				cout << "DEBUG: LocalTeam=" << localTeam << "\n\n";
				break;
			}
			if (toggleFlash) {
				mem.WPM<float>(localPlayer + m_flFlashMaxAlpha, 0.f);
			}
			else {
				mem.WPM<float>(localPlayer + m_flFlashMaxAlpha, 255.f);
			}

			for (int i = 1; i < maxPlayers; i++) {
				uintptr_t entity = getPtrOffset(moduleBase + dwEntityList + i * 0x10);
				//uintptr_t playerResource = getPtrOffset(moduleBase + dwPlayerResource);
				int isDefusing = getIntOffset(entity + m_bIsDefusing);
				//cout << "getEntClassID(entity)=" << getEntClassID(entity) << "\n";
				//int isC4Owner = getIntOffset(entity + m_hOwner);
				//cout << "\nisC4Owner"<<isC4Owner;

				int iGlowIndx = getIntOffset(entity + m_iGlowIndex);
				int entityTeam = getIntOffset(entity + m_iTeamNum);

				// Check doesn't work. Sometimes negative integers are associated with their values
				/*if (iGlowIndx < 0 || entityTeam < 0) {
					isWorking = false;
					cout << "DEBUG: LocalTeam=" << localTeam << ", iGlowIndx=" << iGlowIndx << ", entityTeam=" << entityTeam<<"\n\n";
					break;
				}*/

				int entityHealth = getIntOffset(entity + m_iHealth);
				int isDormant = getIntOffset(entity + m_bDormant);
				if (entityHealth < 1 || entityHealth > 100 || isDormant) {
					continue;
				}

				//cout << "LocalTeam=" << localTeam << ", iGlowIndx=" << iGlowIndx << ", entityHealth=" << entityHealth << ", isDormant=" << isDormant << ", entityTeam=" << entityTeam << "\n";

				if (entityTeam == 3) {
					glowStruct.setup('b'); // blue
				}
				else if (entityTeam == 2) {
					glowStruct.setup('r'); // red
				}

				if (localTeam != entityTeam)
				{
					int isSpotted = getIntOffset(entity + m_bSpotted);
					//cout << "isSpotted" << isSpotted << "\n";
					if (toogleRadar && isSpotted == false) {
						if (isSpotted != 0 && isSpotted != 1) {
							isWorking = false;
							cout << "DEBUG: LocalTeam=" << localTeam << ", iGlowIndx=" << iGlowIndx << ", entityTeam=" << entityTeam << ", entityHealth=" << entityHealth;
							cout << "\nisDormant=" << isDormant << ", isSpotted=" << isSpotted << "\n\n";
							break;
						}
						else {
							setSpotted(entity);
						}
					}
					if ((!isVisible(localPlayer, entity) || !isVisible(entity, localPlayer))) {
						glowStruct.setup('p'); // pink
					/*
					Vec3 enemyHead = getBonePos(entity, 8); // 8 is head
					Vec3 localOrigin = getOrigin(localPlayer);
					if ((bspParser->is_visible(localOrigin, enemyHead)==false)) {
						glowStruct.setup(1, 0, 1); // pink
					}
					*/
					}
					if (isDefusing) {
						glowStruct.setup('y');
					}
				}
				changeShowCondition(entityHealth);
				if (toggleVisible) {
					setGlowOffset(dwGlowManager, iGlowIndx);
				}

				if (toggleChams && (entityTeam == 3 || entityTeam == 2)) {
					if (!chamCount) {
						defChamStruct.setup(getByteOffset(entity + m_clrRender),
							getByteOffset(entity + m_clrRender + 0x01),
							getByteOffset(entity + m_clrRender + 0x02));
						chamCount = true;
					}
					//setChams();

					if (entityTeam == 3)
					{
						if (isDefusing) {
							writeCham(entity, 'y');
						}
						else {
							writeCham(entity, 'b');
						}
					}
					else if (entityTeam == 2)
					{
						writeCham(entity, 'r');
					}

					DWORD thisPtr = (int)(engineBase + model_ambient_min - 0x2c);
					DWORD xored = *(DWORD*)&chamsBrightness ^ thisPtr;
					//cout << "readPtr=" << thisPtr <<"xored"<< xored<<"\n";
					//cout << "byte=" << mem.RPM<BYTE>(entity + 0x70) << "\n";
					mem.WPM<int>(engineBase + model_ambient_min, xored);
					resetChams = false;
				}

				if (resetChams && (entityTeam == 3 || entityTeam == 2)) {

					mem.WPM<BYTE>(entity + m_clrRender, defChamStruct.r);
					mem.WPM<BYTE>(entity + m_clrRender + 0x01, defChamStruct.g);
					mem.WPM<BYTE>(entity + m_clrRender + 0x02, defChamStruct.b);

					DWORD thisPtr = (int)(engineBase + model_ambient_min - 0x2c);
					DWORD xored = *(DWORD*)&chamsBrightnessReset ^ thisPtr;
					mem.WPM<int>(engineBase + model_ambient_min, xored);
					//resetChams = false;
					toggleChams = false;
				}
				//getc4entity();
			}

		}
	}
	mem.closeHandle();
	if (!isWorking) {
		cout << "not working currently\n\n";
	}
	char k;
	cout << "Restart ?? y/n \t";
	cin >> k;
	if (k == 'Y' || k == 'y') {
		cout << "\n\n";
		goto restart;
	}
}