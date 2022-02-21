using namespace std;

//extern MODULEENTRY32 client;
//extern MODULEENTRY32 engine;

namespace signatures {

	string dwClientState_pattern = "A1 ? ? ? ? 33 D2 6A 00 6A 00 33 C9 89 B0";
	
	string dwClientState_MapDirectory_pattern = "B8 ? ? ? ? C3 05 ? ? ? ? C3";

	string dwClientState_State_pattern = "83 B8 ? ? ? ? ? 0F 94 C0 C3";

	string dwClientState_ViewAngles_pattern = "F3 0F 11 86 ? ? ? ? F3 0F 10 44 24 ? F3 0F 11 86";

	string dwEntityList_pattern = "BB ? ? ? ? 83 FF 01 0F 8C ? ? ? ? 3B F8";

	string dwForceAttack_pattern = "89 0D ? ? ? ? 8B 0D ? ? ? ? 8B F2 8B C1 83 CE 04";

	string dwGameDir_pattern = "68 ? ? ? ? 8D 85 ? ? ? ? 50 68 ? ? ? ? 68";

	string dwGlowObjectManager_pattern = "A1 ? ? ? ? A8 01 75 4B";

	string dwLocalPlayer_pattern = "8D 34 85 ? ? ? ? 89 15 ? ? ? ? 8B 41 08 8B 48 04 83 F9 FF";

	string m_bDormant_pattern = "8A 81 ? ? ? ? C3 32 C0";

	string model_ambient_min_pattern = "F3 0F 10 0D ? ? ? ? F3 0F 11 4C 24 ? 8B 44 24 20 35 ? ? ? ? 89 44 24 0C";

	string is_c4_owner_pattern = "56 8B F1 85 F6 74 31";

	string dwViewMatrix_pattern = "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9";

	string dwGetAllClasses_pattern = "A1 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC CC A1 ? ? ? ? B9";
}
/*class Pattern {
	public:
		string name;
		string pattern;
		MODULEENTRY32 mod;
		int offset;
		int extra;
		bool isRelative;
		map<string, Pattern> sig;

		Pattern() {}

		Pattern(string pattern, MODULEENTRY32 mod, int offset, int extra, bool isRelative) {
			this->pattern = pattern;
			this->mod = mod;
			this->offset = offset;
			this->extra = extra;
			this->isRelative = isRelative;
		}

		map<string, Pattern> setPatterns() {
			sig.insert(make_pair("dwClientState", Pattern(dwClientState_pattern, engine, 1, 0, true)));
			sig.insert(make_pair("dwClientState_MapDirectory", Pattern(dwClientState_MapDirectory_pattern, engine, 7, 0, false)));
			sig.insert(make_pair("dwClientState_State", Pattern(dwClientState_State_pattern, engine, 2, 0, false)));
			sig.insert(make_pair("dwEntityList", Pattern(dwEntityList_pattern, client, 1, 0, true)));
			sig.insert(make_pair("dwForceAttack", Pattern(dwForceAttack_pattern, client, 2, 0, true)));
			sig.insert(make_pair("dwGameDir", Pattern(dwGameDir_pattern, engine, 1, 0, true)));
			sig.insert(make_pair("dwGlowObjectManager", Pattern(dwGlowObjectManager_pattern, client, 1, 4, true)));
			sig.insert(make_pair("dwLocalPlayer", Pattern(dwClientState_pattern, client, 3, 4, true)));
			sig.insert(make_pair("m_bDormant", Pattern(m_bDormant_pattern, client, 2, 8, false)));
			sig.insert(make_pair("model_ambient_min", Pattern(model_ambient_min_pattern, engine, 4, 0, true)));
			//std::cout << "\n dwClientState: 0x" << std::uppercase << std::hex << sig->second. << "\n";
			return sig;
		}

		ptrdiff_t get(string offset) {
			return (ptrdiff_t) ("0x" + uppercase(hex(sig.find(offset)->second)));
		}
};*/
