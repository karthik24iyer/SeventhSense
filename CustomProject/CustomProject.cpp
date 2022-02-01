#define NOMINMAX

#include "Gloabls.hpp"

using namespace std;

extern MODULEENTRY32 client;
extern MODULEENTRY32 engine;

const short int maxPlayers = 32;
const float chamsBrightness = 10.f;
float chamsBrightnessReset = 0.f;


int main() {
	bool areOffsetsLoaded = 0;
	pm.polymorphic();
	SetConsoleTitle(pm.titleGen(rand() % 100 + (time(0) % 1000)).c_str());
	menu.showOTM();

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

	if (!menu.isMyHwId()) {
		isWorking = 0;
		debug = 1;
		std::cout << "\n\tnot my predator\n\n\n";
	}
	else {
		menu.showConsoleMsg();
	}

	hwnd = FindWindowA(NULL, "Counter-Strike: Global Offensive - Direct3D 9");

	if (hwnd == NULL) {
		if (!debug) {
			isWorking = 0;
			std::cout << "\ttitle not found\n\n";
				
		}
	}
	else {

		GetWindowThreadProcessId(hwnd, &procId);
		client = mem.getModule("client.dll", procId);
		moduleBase = (uintptr_t)client.modBaseAddr;

		//engineaddr = mem.getModuleDll(procId, "engine.dll");
		engine = mem.getModule("engine.dll", procId);
		engineBase = (uintptr_t)engine.modBaseAddr;

		if (!areOffsetsLoaded) {
			sig.ScanAll(client, engine);
			areOffsetsLoaded = 1;
		}
	}


	while (!GetAsyncKeyState(VK_END) && isWorking && (util.isInGame()|| debug))
	{
		menu.fetchInput();
		
		if (!toggleMain) continue;

		pm.polymorphic();

		uintptr_t localPlayer = util.getLocalPlayer();

		int localTeam = util.getIntOffset(localPlayer + m_iTeamNum);
		if (localTeam < 0 && !debug) {
			isWorking = false;
			std::cout << "\tDEBUG: LocalTeam=" << localTeam << "\n\n";
			break;
		}

		if (!util.isEntityAlive(localPlayer)) continue;

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

		/*if (toggleFlash) {
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
		}*/

		if (toggleTrigger) {		
			trgr.callTrigger(localPlayer);
		}

		/*else if (toggleNoRecoil && !toggleTrigger) {
			nr.noRecoil(localPlayer);
		}*/	
	}
	mem.closeHandle();
	if (!isWorking) {
		std::cout << "\tnot working currently\n\n";
	}
	char k;
	std::cout << "\tRestart ?? y/* \t";
	menu.clearConsoleInputBuffer();
	//menu.pressKey(VK_RETURN);
	std::cin >> k;
	if (k == 'Y' || k == 'y') {
		std::cout << "\n\n";
		goto restart;
	}
}
