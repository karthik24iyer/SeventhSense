#pragma once

#include <Windows.h>

extern bool toggleMain;
extern bool toggleVisible;
extern bool toggleRadar;
extern bool toggleFlash;
extern bool toggleChams;
extern bool resetChams;
extern bool toggleTrigger;
extern bool toggleDelay;
extern bool toggleNoRecoil;
extern bool toggleAim;
extern bool toggleFriendlyFire;

const int MAX_AIM_SMOOTH = 40;
const int MIN_AIM_SMOOTH = 20;
const int MAX_AIM_FOV = 16;
const int MIN_AIM_FOV = 8;
const int DEF_AIM_SMOOTH = 30;
const int DEF_AIM_FOV = 10;
const int STEP_AIM_SMOOTH = 5;
const int STEP_AIM_FOV = 2;


int aimSmooth = DEF_AIM_SMOOTH;
int aimFov = DEF_AIM_FOV;

extern HANDLE hConsole;

class Toggles {

	public:
		bool isToggle = 0;
		bool aim0DisplayOnce = 1;
		bool aim1DisplayOnce = 1;

		void fetchInput() {
			if (GetAsyncKeyState(VK_HOME) & 1) {
				toggleMain = !toggleMain;
				showConsoleMsg();
			}

			else if (GetAsyncKeyState(VK_F1) & 1) {
				toggleVisible = !toggleVisible;
				showConsoleMsg();
			}
			//std::cout << "\nstatus: "<< isToggle;

			else if (GetAsyncKeyState(VK_F3) & 1) {
				toggleRadar = !toggleRadar;
				showConsoleMsg();
			}

			else if (GetAsyncKeyState(VK_CAPITAL) & 1) {
				toggleTrigger = !toggleTrigger;
				showConsoleMsg();
			}

			else if (GetAsyncKeyState(VK_NUMLOCK) & 1) {
				toggleDelay = !toggleDelay;
				toggleNoRecoil = !toggleNoRecoil;
				showConsoleMsg();
			}

			else if (GetAsyncKeyState(VK_F6) & 1) {
				toggleFlash = !toggleFlash;
				showConsoleMsg();
			}

			else if (GetAsyncKeyState(VK_SCROLL) & 1) {
				toggleChams = !toggleChams;
				if (!toggleChams) resetChams = true;
				showConsoleMsg();
			}

			else if (GetAsyncKeyState(VK_RSHIFT) & 1) {
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

			else if (GetAsyncKeyState(VK_F9) & 1) {
				toggleFriendlyFire = !toggleFriendlyFire;
				showConsoleMsg();
			}

			if (GetAsyncKeyState(VK_F2)) {
				toggleAim = 1;
				//std::cout << "\ntoggleAimIf=" << toggleAim;
				if (aim1DisplayOnce) {
					aim1DisplayOnce = 0;
					aim0DisplayOnce = 1;
					showConsoleMsg();
				}					
			}
			else {
				toggleAim = 0;
				//std::cout << "\ntoggleAimElse=" << toggleAim;
				if (aim0DisplayOnce) {
					aim0DisplayOnce = 0;
					aim1DisplayOnce = 1;
					showConsoleMsg();
				}
			}

		}

		int getFovAimParam() {			
			return aimFov;
		}

		int getSmoothAimParam() {
			return aimSmooth;
		}

		void showConsoleMsg() {
			system("cls");
			SetConsoleTextAttribute(hConsole, 7);
			std::cout << "\n\tHello World :P ... Working fine after PGL major 14th Nov 21 \n";
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
			std::cout << "\n\n\tVis: " << toggleVisible << " (1)";

			//SetConsoleTextAttribute(hConsole, 7);
			//printLine();

			if (toggleRadar) {
				SetConsoleTextAttribute(hConsole, 10);
			}
			else {
				SetConsoleTextAttribute(hConsole, 12);
			}
			std::cout << "\tRdr: " << toggleRadar << " (1)";

			if (toggleFlash) {
				SetConsoleTextAttribute(hConsole, 10);
			}
			else {
				SetConsoleTextAttribute(hConsole, 12);
			}
			std::cout << "\tFlh: " << toggleFlash << " (1)";

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

			if (toggleTrigger) {
				SetConsoleTextAttribute(hConsole, 10);
			}
			else {
				SetConsoleTextAttribute(hConsole, 12);
			}
			std::cout << "\n\n\tTrgr: " << toggleTrigger << " (0)";

			if (toggleDelay) {
				SetConsoleTextAttribute(hConsole, 10);
			}
			else {
				SetConsoleTextAttribute(hConsole, 12);
			}
			std::cout << "\tDl/Nr: " << toggleDelay << " (0)";

			SetConsoleTextAttribute(hConsole, 7);
			printLine();

			if (toggleAim) {
				SetConsoleTextAttribute(hConsole, 10);
			}
			else {
				SetConsoleTextAttribute(hConsole, 12);
			}
			std::cout << "\n\n\tAm: " << toggleAim << " (0)";
			
			setAimBars();
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

		void setAimBars() {
			int bar=0, activeBar=0;
			SetConsoleTextAttribute(hConsole, 3);
			std::cout << "\tSmth: ";
			bar = (MAX_AIM_SMOOTH - MIN_AIM_SMOOTH) / STEP_AIM_SMOOTH;
			activeBar = (int)(1 + (aimSmooth - MIN_AIM_SMOOTH) / STEP_AIM_SMOOTH);
			SetConsoleTextAttribute(hConsole, 6);
			for (int i = 0;i < activeBar;i++) {
				std::cout << "|";
			}
			SetConsoleTextAttribute(hConsole, 7);
			for (int i = 0;i <= bar - activeBar;i++) {
				std::cout << "|";
			}

			SetConsoleTextAttribute(hConsole, 3);
			std::cout << "\tFov: ";
			bar = (MAX_AIM_FOV - MIN_AIM_FOV) / STEP_AIM_FOV;
			activeBar = (1 + (aimFov - MIN_AIM_FOV) / STEP_AIM_FOV);
			SetConsoleTextAttribute(hConsole, 5);
			for (int i = 0;i < activeBar;i++) {
				std::cout << "|";
			}
			SetConsoleTextAttribute(hConsole, 7);
			for (int i = 0;i <= bar - activeBar;i++) {
				std::cout << "|";
			}
		}

		void setToggles() {
			toggleMain = true;
			toggleVisible = true;
			toggleRadar = true;
			toggleFlash = true;
			toggleChams = false;
			toggleTrigger = false;
			toggleDelay = false;
			toggleNoRecoil = false;
			toggleAim = false;
			toggleFriendlyFire = false;
		}

		void printLine() {
			std::cout << "\n\t---------------------------------------------";
		}
};