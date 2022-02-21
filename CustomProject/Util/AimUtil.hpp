#pragma once

#include "FireUtil.hpp"
#include <cstdlib>

extern float cursorPosX;
extern float cursorPosY;
extern const float ScreenCenterX;
extern const float ScreenCenterY;

extern float timeDelay;

class AimUtil {
	
	private:
		bool debug = 0;
		//MyVector vec;
		FeatureUtil util;
		FireUtil fireUtil;
		//int aimBoneList[6] = { 8, 8, 8, 7, 7, 6 };
		float slope;

	public:

		bool aimAtPlayer(float aimFov, float smooth, int bone, uintptr_t localPlayer, uintptr_t entity) {

			float nextX = cursorPosX;
			float nextY = cursorPosY;
			bool isFriendlyEnabled = toggleFriendlyFire && util.getEntityTeam(entity) == util.getEntityTeam(localPlayer);
			//if (entity != NULL && (util.isEntitySpotted(entity) || isFriendlyEnabled)) {
			if (entity != NULL || isFriendlyEnabled) {
				D3DXVECTOR3 bonePos = WorldToScreen(util.getEntBonePos(entity, bone));
				double relDist = getPlayerEntityDelta(bonePos.x, bonePos.y);

				if (bonePos.x != 0 || bonePos.y != 0)
				{
					if (relDist <= aimFov*5)
					{
						//int speed = util.getMouseSpeed();
						//std::cout << "\n speed=" << speed;
						//int speedCopy = speed;
						/*util.setMouseSpeed(1);
						setMousePosition(bonePos.x, bonePos.y);
						util.setMouseSpeed(10);*/

						float delta = util.getEntityBoneDelta(util.getLocalEyePos(), util.getLocalViewAngles(), entity, 8);
						//slope = getPlayerEntitySlope(bonePos.x, bonePos.y);
						//std::cout << "\t slope = " << slope;
						nextX = getNextAxisCoOrd((float)smooth, (float)relDist, nextX, bonePos.x);
						nextY = getNextAxisCoOrd((float)smooth, (float)relDist, nextY, bonePos.y);
						//nextY = getNextYAxisCoOrd((float)slope, (float)nextX, bonePos.x, bonePos.y);
						cursorPosX = nextX;
						cursorPosY = nextY;

						if (debug) {
							std::cout << "\n\n " << nextX << "\t --> \t" << bonePos.x;
							std::cout << "\t relDist = " << relDist;
							std::cout << "\t delta = " << delta;
							std::cout << "\n " << nextY << "\t --> \t" << bonePos.y;
						}
						if (relDist < 1.5f && delta < 0.4f) {
						//if (getPlayerEntityDelta(bonePos.x, bonePos.y) <1.5f ) {
							if (debug) {
								std::cout << "\n ITS DONE..";
							}

							fireUtil.fireWeapon(localPlayer, entity, 1);
							util.setMouseSpeed(10);
							return 0;
						}
						Sleep(timeDelay);
						setMousePosition(cursorPosX, cursorPosY);
					}
				}
			}
			//util.setMouseSpeed(10);
			return 1;
		}
		
		uintptr_t getClosestTarget(float fov, uintptr_t entityList[], int maxPlayers, int aimBone, uintptr_t localPlayer)
		{
			D3DXVECTOR3 viewAngles = util.getLocalViewAngles();
			D3DXVECTOR3 localEyePos = util.getLocalEyePos();
			uintptr_t localTeam = util.getEntityTeam(localPlayer);

			uintptr_t bestEntity = NULL;

			for (int i = 0; i < maxPlayers; i++)
			{
				uintptr_t entity = entityList[i];
				bool isVisible = util.isEntitySpotted(entity);
				//std::cout << "\n entity=" << entity;
				bool isFriendlyEnabled = toggleFriendlyFire && util.getEntityTeam(entity) == localTeam;
				//std::cout << "\nisFriendlyEnabled= " << isFriendlyEnabled;
				bool isEnemy = util.getEntityTeam(entity) != localTeam;
				if ((isEnemy || isFriendlyEnabled) && isVisible)
				{
					//D3DXVECTOR3 angle = vec.CalcAngle(localEyePos, getEntBonePos(entity, aimBone));
					//D3DXVECTOR3 cAngles = vec.ClampAngles(angle - viewAngles);
					//float delta = sqrt(cAngles.x * cAngles.x + cAngles.y * cAngles.y);

					float delta = util.getEntityBoneDelta(localEyePos, viewAngles, entity, aimBone);

					if (delta < fov)
					{
						//std::cout << "\ndelta= " << delta;
						fov = delta;
						bestEntity = entity;
						//cout << "\n target found...";
						break;
					}
					//if(delta<0.1)
						//Sleep(200);
				}
			}
			return bestEntity;
		}

		float getPlayerEntityDelta(float x, float y)
		{
			if (debug) {
				//std::cout << "\n x=" << x << "\t\t y=" << y;
				//std::cout << "\n ScreenCenterX=" << ScreenCenterX << "\t ScreenCenterX=" << ScreenCenterY;
			}
			float ydist = (y - ScreenCenterY);
			float xdist = (x - ScreenCenterX);
			return (float) sqrt(pow(ydist, 2) + pow(xdist, 2));
		}

		void setMousePosition(float x, float y) {
			bool smooth = false;
			//int width = GetSystemMetrics(SM_CXSCREEN);
			//int height = GetSystemMetrics(SM_CYSCREEN);
			//ScreenCenterY = height * 0.5f;
			//ScreenCenterX = width * 0.5f;

			float AimSpeed = 2;
			float TargetX = 0;
			float TargetY = 0;

			//X Axis
			if (x != 0)
			{
				if (x > ScreenCenterX)
				{
					TargetX = -(ScreenCenterX - x);
					TargetX /= AimSpeed;
					if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
				}

				if (x < ScreenCenterX)
				{
					TargetX = x - ScreenCenterX;
					TargetX /= AimSpeed;
					if (TargetX + ScreenCenterX < 0) TargetX = 0;
				}
			}

			//Y Axis

			if (y != 0)
			{
				if (y > ScreenCenterY)
				{
					TargetY = -(ScreenCenterY - y);
					TargetY /= AimSpeed;
					if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
				}

				if (y < ScreenCenterY)
				{
					TargetY = y - ScreenCenterY;
					TargetY /= AimSpeed;
					if (TargetY + ScreenCenterY < 0) TargetY = 0;
				}
			}

			if (!smooth)
			{
				mouse_event(MOUSEEVENTF_MOVE, (DWORD)(TargetX), (DWORD)(TargetY), NULL, NULL);
				return;
			}

			/*TargetX /= 5;
			TargetY /= 5;

			//Mouse even't will round to 0 a lot of the time, so we can add this, to make it more accurrate on slow speeds.
			if (fabs(TargetX) < 1)
			{
				if (TargetX > 0)
				{
					TargetX = 1;
				}
				if (TargetX < 0)
				{
					TargetX = -1;
				}
			}
			if (fabs(TargetY) < 1)
			{
				if (TargetY > 0)
				{
					TargetY = 1;
				}
				if (TargetY < 0)
				{
					TargetY = -1;
				}
			}
			mouse_event(MOUSEEVENTF_MOVE, (DWORD)(TargetX), (DWORD)(TargetY), NULL, NULL);*/
		}

		float getMatrixIndice(int i) {
			return mem.RPM<float>(moduleBase + dwViewMatrix + sizeof(float) * i);
		}

		D3DXVECTOR3 WorldToScreen(D3DXVECTOR3 vIn)
		{
			D3DXVECTOR3 vOut;
			float vMatrix[16];
			for (int i = 0; i < 16; i++)
			{
				//vMatrix[i] = csgo->RPM<float>(csgo->client_base + hazedumper::signatures::dwViewMatrix + sizeof(float) * i);
				vMatrix[i] = getMatrixIndice(i);
			}

			vOut.x = vMatrix[0] * vIn.x + vMatrix[1] * vIn.y + vMatrix[2] * vIn.z + vMatrix[3];
			vOut.y = vMatrix[4] * vIn.x + vMatrix[5] * vIn.y + vMatrix[6] * vIn.z + vMatrix[7];

			float w = vMatrix[12] * vIn.x + vMatrix[13] * vIn.y + vMatrix[14] * vIn.z + vMatrix[15];

			if (w < 0.01f) {
				vOut.x = 0; vOut.y = 0; vOut.z = 0;
				return vOut;
				//return false;
			}

			float invw = 1.0f / w;

			vOut.x *= invw;
			vOut.y *= invw;

			//int width, height;

			//width = GetSystemMetrics(SM_CXSCREEN);
			//height = GetSystemMetrics(SM_CYSCREEN);

			//float x = (float)width / 2;
			//float y = (float)height / 2;
			float x = (float) ScreenCenterX;
			float y = (float) ScreenCenterY;

			x += 0.5f * vOut.x * x + 0.5f;
			y -= 0.5f * vOut.y * y + 0.5f;

			//x += 0.5 * vOut.x * (float)width + 0.5;
			//y -= 0.5 * vOut.y * (float)height + 0.5;

			vOut.x = x;
			vOut.y = y;

			//return true;
			return vOut;
		}

		
		/*float getPlayerEntityDeltaSlope(float x, float y) {
			return (y - ScreenCenterY) / (x - ScreenCenterX);
		}*/

		float getPlayerEntitySlope(float bonePosX, float bonePosY) {
			return (ScreenCenterY - bonePosY) / (ScreenCenterX - bonePosX) * (1);
		}
		
		float getNextYAxisCoOrd(float slope, float pos, float bonePosX, float bonePosY) {
			float Y = cursorPosY + ((1) * slope * (pos - cursorPosX));
			//float Y = (float) bonePosY + ((-1)*(slope) * (pos - (float) bonePosX));
			//std::cout << "\n inside funcY = "<<Y;
			return Y;
		}
		
		float getNextAxisCoOrd(float smooth, float relDist, float curPos, float bonePos) {

			float delta = relDist / smooth;
			float finalPos = curPos;
			float bandWidth = 0.0001f;
			if (debug) {
				//std::cout << "\n delta=" << delta;
			}
			
			if (abs(curPos - bonePos) < bandWidth) {
				finalPos = bonePos;
			}

			else if ((curPos > bonePos) && (abs(curPos - bonePos) > bandWidth)) {

				if ((curPos - delta) < bonePos) {
					finalPos = bonePos;
				}
				else {
					finalPos = (curPos - delta);
				}
			}

			else if ((curPos < bonePos) && (abs(curPos - bonePos) > bandWidth)) {
				
				if ((curPos + delta) > bonePos) {
					finalPos = bonePos;
				}
				else {
					finalPos = (curPos + delta);
				}
			}

			return finalPos;

		}
		
		/*float getSmoothTime() {
			if (relDist >= 2) {
				return 1;
			}
			/*else if (relDist < 3 && relDist > 1) {
				return (short) relDist;
			}
			
			//return 0.5f;
			return timeDelay;
		}

		//Deprecated. Uses WPM .. 
		bool aimAtPlayerOld(uintptr_t localPlayer, DWORD player, float smooth, int bone)
		{
			bool isFriendlyEnabled = toggleFriendlyFire && getEntityTeam(player) == getEntityTeam(getLocalPlayer());
			if (player != NULL && (isEntitySpotted(player)|| isFriendlyEnabled))
			{
				D3DXVECTOR3 aimAngles = getEntBonePos(player, bone) - getLocalEyePos();
				vec.VectorAngles(aimAngles, aimAngles);
				D3DXVECTOR3 delta = aimAngles - getLocalViewAngles();
				float relDist = vec.getVecMagnitude(delta);
				relDist > 1.f ? 1.f : relDist;
				smooth < 1 ? 1 : smooth;
				if (relDist < 0.01f) {
					int activeWeaponId = getActiveWeaponId(getActiveWeapon(localPlayer));
					if (getIsDgle(activeWeaponId)) {
						startFire();
						Sleep(10);
						stopFire();
					}
					return 0;
				}
				//std::cout << "\nmag: " << relDist;
				D3DXVECTOR3 sAngles = vec.ClampAngles(getLocalViewAngles() + (delta / (smooth*(1+ relDist))));
				//std::cout << "\n sAnglesX = " << sAngles.x << "\t sAnglesY = " << sAngles.y;
				//std::cout << "\nangle: " << vec.getVecMagnitude(sAngles);
				//setLocalViewAngles(sAngles);
			}
			return 1;
		}*/


};