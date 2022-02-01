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
		int aimBoneList[6] = { 8, 8, 8, 7, 7, 6 };
	public:

		void callTrigger(uintptr_t localPlayer) {
			//uintptr_t localPlayer = util.getLocalPlayer();
			uintptr_t enemyOnCH = mem.RPM<uintptr_t>(moduleBase + dwEntityList + ((util.getLocalCrossID(localPlayer) - 1) * 0x10));
			if(!util.isValidEntity(enemyOnCH)) return;
			boolean isFriendlyFire = toggleFriendlyFire && (util.getEntityTeam(localPlayer) == util.getEntityTeam(enemyOnCH));
			if ((util.getEntityTeam(localPlayer) != util.getEntityTeam(enemyOnCH)) || (isFriendlyFire))
			{
				forceAttack(localPlayer, enemyOnCH);
			}
		}
		void forceAttack(uintptr_t localPlayer, uintptr_t enemyOnCH)
		{
			moduleBase = util.getModuleBase();
			//DWORD activeWeapon = util.getActiveWeapon(localPlayer);
			int activeWeaponId = util.getActiveWeaponId(util.getActiveWeapon(localPlayer));
			if (util.getIsBurstFWeapon(activeWeaponId)
				|| util.getIsSpam(activeWeaponId)) return;
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
			else if (util.getIsGay(activeWeaponId)) {
				isGay = true;
			}
			else if (util.getIsSemiFWeapon(activeWeaponId)) {
				isSemiFWeapon = true;
			}

			if (isSniper && util.isScoped(localPlayer) && !util.isEntityMoving(localPlayer)) {
				if (toggleDelay) {
					Sleep(delay);
				}
				else {
					Sleep(delay % 5);
				}
				util.startFire();
				Sleep(20);
				util.stopFire();
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
						util.startFire();
						clipCount--;
						Sleep(15);
					}
					util.stopFire();
				}
				else if (isSemiFWeapon) {
					if (toggleDelay) {
						Sleep(delay % 50);
					}
					else {
						Sleep(delay % 5);
					}
					int clipCount = 7;
					while (clipCount > 0) {
						util.startFire();					
						clipCount--;
						Sleep(25);
					}
					util.stopFire();
				}*/
				
				if (isSingleFWeapon || isSemiFWeapon) {
					if (toggleDelay) {
						Sleep(delay % 50);
					}
					else {
						Sleep(delay % 7);
					}
					int clipCount = 1;
					while (clipCount > 0) {
						util.startFire();
						clipCount--;
						Sleep(15);
					}
					util.stopFire();
				}

				else if (isDgle) {
					//Sleep(50);
					int clipCount = 1;
					while (clipCount > 0) {
						float delta = util.getEntityBoneDelta(util.getLocalEyePos(), util.getLocalViewAngles(), 
							enemyOnCH, aimBoneList[rand() % 6]);
						//std::cout << "\n delta: " << delta;

						if (delta > 1.f) break;

						util.startFire();
						//std::cout << "\n shot ";
						clipCount--;
						Sleep(15);
					}
					util.stopFire();
				}
				else if (isGay) {
					int clipCount = 5;
					if (toggleDelay) {
						Sleep(delay);
					}
					else {
						Sleep(delay % 7);
					}
					while (clipCount > 0) {
						if (util.isScoped(localPlayer) && !util.isEntityMoving(localPlayer)) {
							util.startFire();					
						}
						clipCount--;
						Sleep(15);
					}
					util.stopFire();
				}
				/*else if (isSpam) {
					Sleep(delay % 25);
					int clipCount = 50;
					while (clipCount > 0) {
						util.startFire();
						clipCount--;
						Sleep(15);
					}
					util.stopFire();
				}*/
			}
		}

		
};