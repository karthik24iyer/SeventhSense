#pragma once

#include "Toggles.hpp"

extern bool aimHold;

class Thatsit {
	private:
		FeatureUtil util;
		int aimBone = 8;
		// Head - 8, Neck - 7, Chest - 6
		int aimBoneListGen[6] = {8, 8, 8, 8, 7, 7};
		int aimBoneListBurst[6] = { 8, 8, 7, 7, 7, 6 };
		// default values -
		float aimSmooth;  // old = 40
		float aimFov; // old = 4
		Toggles menu;
	
	public:

		void aimBot(uintptr_t localPlayer, uintptr_t entity[], int currentPlayers)
		{
			if (!aimHold) {
				return;
			}
			int activeWeaponId = util.getActiveWeaponId(util.getActiveWeapon(localPlayer));

			if (util.getIsGay(activeWeaponId)) {
				return;
			}
			aimBone = util.getIsBurstFWeapon(activeWeaponId)==1? aimBoneListBurst[rand() % 6] : aimBoneListGen[rand() % 6];
			//std::cout << "\n aimBone" << aimBone;

			aimSmooth = (float)(menu.getSmoothAimParam());
			aimFov = (float)(menu.getFovAimParam()/10);

			if (util.getIsDgle(activeWeaponId)) {
				aimBone = 8;
			}
			aimHold = util.aimAtPlayer(localPlayer, util.getClosestTarget(aimFov, entity, currentPlayers, aimBone), aimSmooth, aimBone);

		}
};