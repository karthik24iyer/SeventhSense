#pragma once

#include "../Toggles.hpp"
#include "../Util/AimUtil.hpp"

extern bool aimHold;

class Aim {
	private:
		AimUtil util;
		int aimBone = 8;
		// Head - 8, Neck - 7, Chest - 6
		int aimBoneList[6] = {8, 8, 8, 8, 7, 7};
		// default values -
		float aimSmooth;  // old = 40
		float aimFov; // old = 4
		Toggles menu;
	
	public:

		void aimBot(uintptr_t localPlayer, uintptr_t entityList[], int currentPlayers)
		{
			if (!aimHold) {
				return;
			}

			//aimBone = aimBoneList[rand() % 6];
			//std::cout << "\n inside Aim";
			aimBone = 8;
			aimSmooth = (float)(menu.getSmoothAimParam())/5;
			aimFov = (float)(menu.getFovAimParam());
			//aimSmooth = 1;
			aimHold = util.aimAtPlayer(aimFov, aimSmooth, aimBone, localPlayer, util.getClosestTarget(aimFov, entityList, currentPlayers, aimBone, localPlayer));

		}
};