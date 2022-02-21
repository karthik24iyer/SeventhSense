#pragma once

//#include <cstring>
#include "../Util/StructUtil.hpp"
#include "../Util/DrawText.hpp"
#include "../Util/CoOrd.hpp"
#include "../Toggles.hpp"

extern bool toggleFriendlyFire;
extern int originOffsetX;
extern int originOffsetY;
//extern int originOffsetZ;
extern int spanOffset;
extern bool showPlayerRefForRadar;
//extern int upOffsetZ = 0;
//extern int downOffsetZ = 0;

class Radar {

	private:
		FeatureUtil util;
		DrawTxt txt;
		float originX = d2X;
		float originY = d2Y;
		float posX = 0;
		float posY = 0;
		float posZ = 0;
		int span = d2Span;
		int size = 10;
		char color = 'w';

		void setLocation(char map, uintptr_t entity) {
			posX = util.getEntityPos(entity).x;
			posY = util.getEntityPos(entity).y;
			posZ = util.getEntityPos(entity).z;

			if (map == 'd' || map == 'D') {
				originX = d2X; originY = d2Y; span = d2Span;
				setColor(d2B, d2T);
			}
			else if (map == 'i' || map == 'I') {
				originX = infX; originY = infY; span = infSpan;
				setColor(infB, infT);
			}
			else if (map == 'm' || map == 'M') {
				originX = mirX; originY = mirY; span = mirSpan;
				setColor(mirB, mirT);
			}
			else if (map == 'n' || map == 'N') {
				originX = nukeX; originY = nukeY; span = nukeSpan;
				setColor(nukeB, nukeT);
			}
			else if (map == 'o' || map == 'O') {
				originX = ovX; originY = ovY; span = ovSpan;
				setColor(ovB, ovT);
			}
			else if (map == 'v' || map == 'V') {
				originX = vtX; originY = vtY; span = vtSpan;
				setColor(vtB, vtT);
			}
			else if (map == 'a' || map == 'A') {
				originX = anX; originY = anY; span = anSpan;
				setColor(anB, anT);
			}
			else {
				return;
			}
			if (util.isEntityDefusing(entity)) {
				color = 'y';
			}
			posX = ((posX + 350) / (span + spanOffset)) + originX + originOffsetX;
			posY = ((1000 - posY) / (span + spanOffset)) + originY + originOffsetY;
			//txt.drawMyText(posX, posY);
			txt.drawRect((int)posX, (int)posY, size, color);
		}

		void setColor(float down, float up) {
			if (posZ < down + downOffsetZ) {
				color = 'b';
			}
			else if (posZ > up + upOffsetZ) {
				color = 'g';
			}
			else {
				color = 'w';
			}
		}

	public:

		void showRefLocation(char map, uintptr_t localPlayer) {
			setLocation(map, localPlayer);
		}

		void showEntityLocation(char map, uintptr_t localPlayer, uintptr_t entity[], int entityTeam[], int maxPlayers) {
			if (showPlayerRefForRadar) {
				showRefLocation(map, localPlayer);
			}
			else {
				int localTeam = util.getIntOffset(localPlayer + m_iTeamNum);
				for (int i = 0; i < maxPlayers; i++) {
					if (localTeam != entityTeam[i]) {
						setLocation(map, entity[i]);
					}
				}
			}
		}

};