#pragma once

#include "ClassId.hpp"
#include "Stable.hpp"

extern bool toggleDelay;
extern bool toggleFriendlyFire;
extern bool toggleNoRecoil;

class Trigger {
	
	private:
		FeatureUtil util;
		Stable nr;
	public:

		void callTrigger() {
			uintptr_t localPlayer = util.getLocalPlayer();
			DWORD enemyOnCH = mem.RPM<DWORD>(moduleBase + dwEntityList + ((util.getLocalCrossID(localPlayer) - 1) * 0x10));
			boolean isValid = util.isValidEntity(enemyOnCH);
			boolean isFriendlyFire = toggleFriendlyFire && (util.getEntityTeam(localPlayer) == util.getEntityTeam(enemyOnCH));
			if ((util.getEntityTeam(localPlayer) != util.getEntityTeam(enemyOnCH) && isValid) || (isFriendlyFire && isValid))
			{
				forceAttack(localPlayer);
			}
		}
		void forceAttack(uintptr_t localPlayer)
		{
			moduleBase = util.getModuleBase();
			DWORD activeWeapon = util.getActiveWeapon(localPlayer);
			long activeWeaponId = util.getActiveWeaponId(activeWeapon);
			//std::cout << "\nfound id: "<< activeWeaponId;
			srand((unsigned int)time(0));
			int delay = 100 + rand() % 100 + 1;

			boolean isSniper = false;
			//boolean isBurstFWeapon = false;
			boolean isSingleFWeapon = false;
			boolean isSemiFWeapon = false;
			boolean isGay = false;
			//boolean isSpam = false;
			boolean isDgle = false;

			if (util.getIsSniper(activeWeaponId)) {
				isSniper = true;
			}
			/*else if (util.getIsBurstFWeapon(activeWeaponId)) {
				isBurstFWeapon = true;
			}
			else if (util.getIsSpam(activeWeaponId)) {
				isSpam = true;
			}*/
			else if (util.getIsSingleFWeapon(activeWeaponId)) {
				isSingleFWeapon = true;
			}
			else if (util.getIsDgle(activeWeaponId)) {
				isDgle = true;
			}
			else if (util.getIsSemiFWeapon(activeWeaponId)) {
				isSemiFWeapon = true;
			}
			else if (util.getIsGay(activeWeaponId)) {
				isGay = true;
			}

			if (isSniper && util.isScoped(localPlayer) && !util.isEntityMoving(localPlayer)) {
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
			else if (!isSniper) {

				/*if (isBurstFWeapon) {
					if (toggleDelay) {
						Sleep(delay % 30);
					}
					else {
						Sleep(delay % 5);
					}
					int clipCount = 10;
					while (clipCount > 0) {
						startFire();
						clipCount--;
						Sleep(15);
					}
					stopFire();
				}
				else */if (isSemiFWeapon) {
					if (toggleDelay) {
						Sleep(delay % 50);
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

				if (isSingleFWeapon) {
					if (toggleDelay) {
						Sleep(delay % 50);
					}
					else {
						Sleep(delay % 7);
					}
					int clipCount = 1;
					while (clipCount > 0) {
						startFire();
						clipCount--;
						Sleep(15);
					}
					stopFire();
				}

				else if (isDgle) {
					Sleep(50);
					int clipCount = 1;
					while (clipCount > 0) {
						startFire();
						clipCount--;
						Sleep(50);
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
						if (util.isScoped(localPlayer) && !util.isEntityMoving(localPlayer)) {
							startFire();					
						}
						clipCount--;
						Sleep(25);
					}
					stopFire();
				}
				/*else if (isSpam) {
					Sleep(delay % 25);
					int clipCount = 50;
					while (clipCount > 0) {
						startFire();
						clipCount--;
						Sleep(15);
					}
					stopFire();
				}*/
			}
		}

		void startFire() {
			mem.WPM<int>(moduleBase + dwForceAttack, 5);
		}

		void stopFire() {
			mem.WPM<int>(moduleBase + dwForceAttack, 4);
		}
};