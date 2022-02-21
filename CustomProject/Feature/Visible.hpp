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

class Visible {
	
	private:
		FeatureUtil util;
		AimUtil aimUtil;
		DrawTxt txt;
		float originX = d2X;
		float originY = d2Y;
		float posX=0;
		float posY=0;
		float posZ=0;
		int span = d2Span;
		int size = 10;
		char color = 'g';
	
		void showEntity(uintptr_t localPlayer, uintptr_t entity, int offset) {
			//txt.drawRect((int)posX, (int)posY, size, color);
			util.setLastPlace(entity);
			const char* entityLastPlace = util.entityLastPlace;
			if (entityLastPlace != NULL) {
				//std::cout << "\n entityLastPlace=" << entityLastPlace;
				txt.drawMyText(450 + offset, 200, entityLastPlace);
			}
		}
	public:

		void makeEntityVisible(uintptr_t localPlayer, uintptr_t entity[], int entityTeam[], int maxPlayers) {
			int localTeam = util.getIntOffset(localPlayer + m_iTeamNum);
			int offset = 0;
			for (int i = 0; i < maxPlayers; i++) {
				if (localTeam != entityTeam[i] || (localTeam == entityTeam[i] && toggleFriendlyFire)) {
					showEntity(localPlayer, entity[i], offset);
					offset = offset + 100;
				}
			}
		}

		/* //Uses WPM, Deprecated...
		void makeEntityVisible(uintptr_t localPlayer, uintptr_t entity[], int entityTeam[], int maxPlayers) {

			//if (!util.isMapLoaded()) {
			//	return;
			//}

			int localTeam = util.getIntOffset(localPlayer + m_iTeamNum);

			uintptr_t dwGlowManager = util.getPtrOffset(moduleBase + dwGlowObjectManager);
			for (int i = 1; i < maxPlayers; i++) {
				int iGlowIndx = util.getIntOffset(entity[i] + m_iGlowIndex);
				// std::cout << "DEBUG: LocalTeam=" << localTeam << ", iGlowIndx=" << iGlowIndx << ", entityTeam[i]=" << entityTeam[i]<<"\n\n";
				//std::cout << "LocalTeam=" << localTeam << ", iGlowIndx=" << iGlowIndx << ", entityHealth=" << entityHealth << ", isDormant=" << isDormant << ", entityTeam[i]=" << entityTeam[i] << "\n";

				if (entityTeam[i] == 3) {
					glowStruct.setup('b'); // blue
				}
				else if (entityTeam[i] == 2) {
					glowStruct.setup('r'); // red
				}

				if ((localTeam != entityTeam[i]) || (localTeam == entityTeam[i] && toggleFriendlyFire))
				{
					if ((!util.isVisibleByDef(localPlayer, entity[i]) || !util.isVisibleByDef(entity[i], localPlayer))) {
					//if (!util.isVisibleByParser(localPlayer, entity[i]) && !util.isVisibleByParser(entity[i], localPlayer)) {
						glowStruct.setup('p'); // pink
					}
					if (util.isEntityDefusing(entity[i])) {
						glowStruct.setup('y'); //yellow
					}
					//if (util.isEntityScoped(entity[i])) {
					//	glowStruct.setup('g'); //green
					//}
				}
				if (util.isEnityHoldingC4(entity[i])) {
					glowStruct.setup('y'); //yellow
				}
				changeShowCondition(util.getEntityHealth(entity[i]));
				setGlowOffset(dwGlowManager, iGlowIndx);
			}
		}

		void showEntityOnRadar(uintptr_t entity[], int maxPlayers) {
			for (int i = 1; i < maxPlayers; i++) {
				//std::cout << "isSpotted" << util.isEntitySpotted(entity[i]) << "\n";
				if (!util.isEntitySpotted(entity[i])) {
					util.setEntitySpotted(entity[i]);
				}
			}
		}

		void stopFlash(uintptr_t localPlayer) {
			mem.WPM<float>(localPlayer + m_flFlashMaxAlpha, 0.f);
		}

		void resetFlash(uintptr_t localPlayer) {
			mem.WPM<float>(localPlayer + m_flFlashMaxAlpha, 255.f);
		}

		void setGlowOffset(uintptr_t offset, int glowIndex) {
			mem.WPM<GlowStruct>(offset + (glowIndex * 0x38) + 0x8, glowStruct);
		}

		void changeShowCondition(int entityHealth) {
			if (entityHealth < 30) {
				glowStruct.setAlpha(0.8f); // Near full
			}
			if (entityHealth < 10) {
				glowStruct.setAlpha(1.f); // full
			}

		}
		*/
};