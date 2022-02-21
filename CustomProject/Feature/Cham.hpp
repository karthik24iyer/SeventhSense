#pragma once

#include "../Util/StructUtil.hpp"

class Cham {

	private:
		FeatureUtil util;

	public:
		void enableChams(uintptr_t entity[], int entityTeam[], bool &chamCount, int maxPlayers) {
			for (int i = 1; i < maxPlayers; i++) {
				//std::cout << "\nentityList["<<i<<"]= "<<entityList[i];
				if ((entityTeam[i] == 3 || entityTeam[i] == 2)) {
					if (!chamCount) {
						saveDefaultChams(entity[i]);
						chamCount = true;
					}
					if (entityTeam[i] == 3)
					{
						if (util.isEntityDefusing(entity[i])) {
							writeCham(entity[i], 'y');
						}
						else {
							writeCham(entity[i], 'b');
						}
					}
					else if (entityTeam[i] == 2)
					{
						if (util.isEnityHoldingC4(entity[i])) {
							writeCham(entity[i], 'y');
						}
						else {
							writeCham(entity[i], 'r');
						}
					}
				}
			}
		}

		void disableChams(uintptr_t entity[], int entityTeam[], int maxPlayers) {
			for (int i = 1; i < maxPlayers; i++) {
				if ((entityTeam[i] == 3 || entityTeam[i] == 2)) {
					mem.WPM<BYTE>(entity[i] + m_clrRender, defChamStruct.r);
					mem.WPM<BYTE>(entity[i] + m_clrRender + 0x01, defChamStruct.g);
					mem.WPM<BYTE>(entity[i] + m_clrRender + 0x02, defChamStruct.b);
				}
			}
		}

		void glowCham(uintptr_t engineBase, float brightness) {
			DWORD thisPtr = (int)(engineBase + model_ambient_min - 0x2c);
			DWORD xored = *(DWORD*)&brightness ^ thisPtr;
			mem.WPM<int>(engineBase + model_ambient_min, xored);
		}

		void saveDefaultChams(uintptr_t entity) {
			defChamStruct.setup(util.getByteOffset(entity + m_clrRender),
				util.getByteOffset(entity + m_clrRender + 0x01),
				util.getByteOffset(entity + m_clrRender + 0x02));
		}
		void writeCham(uintptr_t entity, char c) {

			switch (c) {
				case 'r':
					mem.WPM<BYTE>(entity + m_clrRender, BYTE(255));
					mem.WPM<BYTE>(entity + m_clrRender + 0x01, BYTE(50));
					mem.WPM<BYTE>(entity + m_clrRender + 0x02, BYTE(50));
					break;

				case 'g':
					mem.WPM<BYTE>(entity + m_clrRender, BYTE(50));
					mem.WPM<BYTE>(entity + m_clrRender + 0x01, BYTE(255));
					mem.WPM<BYTE>(entity + m_clrRender + 0x02, BYTE(50));
					break;

				case 'b':
					mem.WPM<BYTE>(entity + m_clrRender, BYTE(50));
					mem.WPM<BYTE>(entity + m_clrRender + 0x01, BYTE(50));
					mem.WPM<BYTE>(entity + m_clrRender + 0x02, BYTE(255));
					break;

				case 'y':
					mem.WPM<BYTE>(entity + m_clrRender, BYTE(255));
					mem.WPM<BYTE>(entity + m_clrRender + 0x01, BYTE(255));
					mem.WPM<BYTE>(entity + m_clrRender + 0x02, BYTE(50));
					break;
			}
		}

};
