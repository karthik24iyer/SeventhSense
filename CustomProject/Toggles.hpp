#pragma once

#include <Windows.h>

extern bool hideMenu;
extern bool toggleMain;
extern bool toggleVisible;
extern bool toggleRadar;
//extern bool toggleFlash;
//extern bool toggleChams;
//extern bool resetChams;
extern bool toggleTrigger;
//extern bool toggleDelay;
//extern bool toggleNoRecoil;
extern bool toggleAim;
extern bool toggleFriendlyFire;

extern float cursorPosX;
extern float cursorPosY;
extern const float ScreenCenterX;
extern const float ScreenCenterY;
/*
const short MAX_AIM_SMOOTH = 30;
const short MIN_AIM_SMOOTH = 10;
const short MAX_AIM_FOV = 24;
const short MIN_AIM_FOV = 8;
const short STEP_AIM_SMOOTH = 5;
const short STEP_AIM_FOV = 4;
*/

const short DEF_AIM_SMOOTH = 20;
const short DEF_AIM_FOV = 4;
short aimSmooth = DEF_AIM_SMOOTH;
short aimFov = DEF_AIM_FOV;

bool aimHold = 1;

int originOffsetX = 0;
int originOffsetY = 0;
int originOffsetZ = 0;
int spanOffset = 0;
bool showPlayerRefForRadar = 0;
int upOffsetZ = 0;
int downOffsetZ = 0;

float timeDelay = 1.4f;

extern HANDLE hConsole;

class Toggles {

public:
	bool isToggle = 0;
	bool aim0DisplayOnce = 1;
	bool aim1DisplayOnce = 1;

	void showOTM() {
		char k;
		cin >> k;
		if (k != '0') exit(0);
	}

	void fetchInput() {

		if (GetAsyncKeyState(VK_F3) & 1) {
			hideMenu = !hideMenu;
			showConsoleMsg();
		}

		if (GetAsyncKeyState(VK_HOME) & 1) {
			toggleMain = !toggleMain;
			showConsoleMsg();
		}

		else if (GetAsyncKeyState(VK_SCROLL) & 1) {
			toggleVisible = !toggleVisible;
			showConsoleMsg();
		}

		else if (GetAsyncKeyState(VK_NUMLOCK) & 1) {
			toggleRadar = !toggleRadar;
			showConsoleMsg();
		}

		else if ((GetAsyncKeyState(VK_RMENU) & 1) && toggleRadar) {
			showPlayerRefForRadar = !showPlayerRefForRadar;
			showConsoleMsg();
		}

		else if ((GetAsyncKeyState(VK_RSHIFT) & 1) && showPlayerRefForRadar) {
			spanOffset = spanOffset - 2;
			showConsoleMsg();
		}

		else if ((GetAsyncKeyState(VK_RCONTROL) & 1) && showPlayerRefForRadar) {
			spanOffset = spanOffset + 2;
			showConsoleMsg();
		}

		else if ((GetAsyncKeyState(VK_UP) & 1) && showPlayerRefForRadar) {
			originOffsetY = originOffsetY - 2;
			showConsoleMsg();
		}

		else if ((GetAsyncKeyState(VK_DOWN) & 1) && showPlayerRefForRadar) {
			originOffsetY = originOffsetY + 2;
			showConsoleMsg();
		}

		else if ((GetAsyncKeyState(VK_RIGHT) & 1) && showPlayerRefForRadar) {
			originOffsetX = originOffsetX + 2;
			showConsoleMsg();
		}

		else if ((GetAsyncKeyState(VK_LEFT) & 1) && showPlayerRefForRadar) {
			originOffsetX = originOffsetX - 2;
			showConsoleMsg();
		}
		
		else if ((GetAsyncKeyState(VK_NUMPAD7) & 1) && showPlayerRefForRadar) {
			downOffsetZ = downOffsetZ - 50;
			showConsoleMsg();
		}

		else if ((GetAsyncKeyState(VK_NUMPAD4) & 1) && showPlayerRefForRadar) {
			downOffsetZ = downOffsetZ + 50;
			showConsoleMsg();
		}

		else if ((GetAsyncKeyState(VK_NUMPAD9) & 1) && showPlayerRefForRadar) {
			upOffsetZ = upOffsetZ + 10;
			showConsoleMsg();
		}

		else if ((GetAsyncKeyState(VK_NUMPAD6) & 1) && showPlayerRefForRadar) {
			upOffsetZ = upOffsetZ - 10;
			showConsoleMsg();
		}

		else if ((GetAsyncKeyState(VK_NUMPAD1) & 1)) {
			timeDelay = timeDelay + 0.1f;
			if (timeDelay >= 2.f) {
				timeDelay = 2.f;
			}
			showConsoleMsg();
		}

		else if ((GetAsyncKeyState(VK_NUMPAD0) & 1)) {
			timeDelay = timeDelay - 0.1f;
			if (timeDelay <= 0.1f) {
				timeDelay = 0.1f;
			}
			showConsoleMsg();
		}

		else if (GetAsyncKeyState(VK_CAPITAL) & 1) {
			//cout << "\ntoggle";
			toggleTrigger = !toggleTrigger;
			showConsoleMsg();

		}

		/*else if (GetAsyncKeyState(VK_F2) & 1) {
			toggleDelay = !toggleDelay;
			toggleNoRecoil = !toggleNoRecoil;
			showConsoleMsg();
			if (toggleTrigger && toggleDelay) {
				pressKey(VK_CAPITAL);
			}
		}

		else if (GetAsyncKeyState(VK_F6) & 1) {
			toggleFlash = !toggleFlash;
			showConsoleMsg();
		}

		/*else if (GetAsyncKeyState(VK_SCROLL) & 1) {
			toggleChams = !toggleChams;
			if (!toggleChams) resetChams = true;
			showConsoleMsg();
		}
		//------------------------------------------------------
		*/
		/*else if ((GetAsyncKeyState(VK_RSHIFT) & 1) && toggleAim) {
			aimSmooth = DEF_AIM_SMOOTH;
			aimFov = DEF_AIM_FOV;
			showConsoleMsg();
		}
		else if ((GetAsyncKeyState(VK_UP) & 1) && toggleAim) {
			if (aimFov < MAX_AIM_FOV) {
				aimFov += STEP_AIM_FOV;
				showConsoleMsg();
			}
		}

		else if ((GetAsyncKeyState(VK_DOWN) & 1) && toggleAim) {
			if (aimFov > MIN_AIM_FOV) {
				aimFov -= STEP_AIM_FOV;
				showConsoleMsg();
			}
		}

		else if ((GetAsyncKeyState(VK_RIGHT) & 1) && toggleAim) {
			if (aimSmooth < MAX_AIM_SMOOTH) {
				aimSmooth += STEP_AIM_SMOOTH;
				showConsoleMsg();
			}
		}

		else if ((GetAsyncKeyState(VK_LEFT) & 1) && toggleAim) {
			if (aimSmooth > MIN_AIM_SMOOTH) {
				aimSmooth -= STEP_AIM_SMOOTH;
				showConsoleMsg();
			}
		}
		*/
		else if (GetAsyncKeyState(VK_F1) & 1) {
			toggleFriendlyFire = !toggleFriendlyFire;
			showConsoleMsg();
		}
		
		if (GetAsyncKeyState(VK_F9)) {
			toggleAim = 1;
			//std::cout << "\ntoggleAimIf=" << toggleAim;
			if (aim1DisplayOnce) {
				aimHold = 1;
				aim1DisplayOnce = 0;
				aim0DisplayOnce = 1;
				showConsoleMsg();
			}
		}
		else {
			toggleAim = 0;
			//std::cout << "\ntoggleAimElse=" << toggleAim;
			if (aim0DisplayOnce) {
				//std::cout << "\n --------------------------------------------------------------------------\n";
				aim0DisplayOnce = 0;
				aim1DisplayOnce = 1;
				cursorPosX = ScreenCenterX;
				cursorPosY = ScreenCenterY;
				showConsoleMsg();
			}
		}

	}

	short getFovAimParam() {
		return aimFov;
	}

	short getSmoothAimParam() {
		return aimSmooth;
	}

	void showConsoleMsg() {

		system("cls");
		if (hideMenu) return;

		SetConsoleTextAttribute(hConsole, 7);
		std::cout << "\n\tHello World :P ... Last Updated 16th Feb 22 \n";
		if (toggleMain) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		std::cout << "\n\n\t\t\tMain: " << toggleMain << " (1)";

		SetConsoleTextAttribute(hConsole, 7);
		printLine();
		printLine();

		if (toggleVisible) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		std::cout << "\n\n\tVis: " << toggleVisible << " (0)";

		SetConsoleTextAttribute(hConsole, 7);
		printLine();

		if (toggleRadar) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		//Old -- std::cout << "\tRdr: " << toggleRadar << " (1)";
		std::cout << "\n\n\tRdr: " << toggleRadar << " (0)";

		/*if (toggleFlash) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		std::cout << "\tFlh: " << toggleFlash << " (0)";

		SetConsoleTextAttribute(hConsole, 7);
		printLine();

		if (toggleChams) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		std::cout << "\n\n\tCham: " << toggleChams << " (0)";
		
		SetConsoleTextAttribute(hConsole, 7);
		printLine();
		*/
		if (toggleTrigger) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		std::cout << "\tTrgr: " << toggleTrigger << " (0)";

		/*if (toggleDelay) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "\tDl/Nr: " << toggleDelay << " (0)";
		*/
		SetConsoleTextAttribute(hConsole, 7);
		printLine();
		
		if (toggleAim) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		std::cout << "\n\n\tAm: " << toggleAim << " (0)";

		SetConsoleTextAttribute(hConsole, 5);
		std::cout << "\tSmth: " << timeDelay;

		//setAimBars();
		/*
		if (toggleNoRecoil) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		std::cout << "\t\tnrc: " << toggleNoRecoil << " (0)";
		*/
		SetConsoleTextAttribute(hConsole, 7);
		printLine();
		if (toggleFriendlyFire) {
			SetConsoleTextAttribute(hConsole, 10);
		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
		}
		std::cout << "\n\n\tFrfr: " << toggleFriendlyFire << " (0)";

		SetConsoleTextAttribute(hConsole, 7);
		printLine();
		std::cout << "\n\n\n";
	}

	/*void setAimBars() {
		short bar = 0, activeBar = 0;
		SetConsoleTextAttribute(hConsole, 3);
		std::cout << "\tSmth: ";
		bar = (MAX_AIM_SMOOTH - MIN_AIM_SMOOTH) / STEP_AIM_SMOOTH;
		activeBar = (short)(1 + (aimSmooth - MIN_AIM_SMOOTH) / STEP_AIM_SMOOTH);
		SetConsoleTextAttribute(hConsole, 6);
		for (short i = 0;i < activeBar;i++) {
			std::cout << "|";
		}
		SetConsoleTextAttribute(hConsole, 7);
		for (short i = 0;i <= bar - activeBar;i++) {
			std::cout << "|";
		}

		SetConsoleTextAttribute(hConsole, 3);
		std::cout << "\tFov: ";
		bar = (MAX_AIM_FOV - MIN_AIM_FOV) / STEP_AIM_FOV;
		activeBar = (1 + (aimFov - MIN_AIM_FOV) / STEP_AIM_FOV);
		SetConsoleTextAttribute(hConsole, 5);
		for (short i = 0;i < activeBar;i++) {
			std::cout << "|";
		}
		SetConsoleTextAttribute(hConsole, 7);
		for (short i = 0;i <= bar - activeBar;i++) {
			std::cout << "|";
		}
	}*/

	void setToggles() {
		hideMenu = 0;
		toggleMain = 1;
		toggleVisible = 0;
		toggleRadar = 0;
		//toggleFlash = 0;
		//toggleChams = 0;
		toggleTrigger = 0;
		//toggleDelay = 0;
		//toggleNoRecoil = 0;
		toggleAim = 0;
		toggleFriendlyFire = 0;

		setButtons();
	}

	void setButtons() {
		//bool shouldRestart=0;
		if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0) {
			//pressKey(VK_CAPITAL);
			toggleTrigger = 1;
			//Sleep(50);
		}
		if ((GetKeyState(VK_NUMLOCK) & 0x0001) != 0) {
			//pressKey(VK_NUMLOCK);
			toggleRadar = 1;
			//Sleep(50);
		}
		if ((GetKeyState(VK_SCROLL) & 0x0001) != 0) {
			//pressKey(VK_SCROLL);
			toggleVisible = 1;
			//Sleep(50);
		}
	}

	void printLine() {
		std::cout << "\n\t---------------------------------------------";
	}

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

	/*void pressKey(WORD key) {
		INPUT input;
		WORD vkey = key;
		input.type = INPUT_KEYBOARD;
		input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;
		input.ki.wVk = vkey;
		input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
		SendInput(1, &input, sizeof(INPUT));
		Sleep(2);
		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));
		Sleep(2);
	}*/

	bool isMyHwId() {

		const char* hid = "{ced62aa7-f371-11ea-b873-806e6f6e6963}";
		HW_PROFILE_INFO hwProfileInfo;
		if (GetCurrentHwProfile(&hwProfileInfo)) {
			//printf("HWID: %s\n", hwProfileInfo.szHwProfileGuid);
			const char* newHid = hwProfileInfo.szHwProfileGuid;
			if (strcmp(hid, newHid) == 0) {
				return 1;
			}
		}
		return 0;
	}
};