#pragma once

#include "../Misc/SrcSDK.hpp"

extern MODULEENTRY32 client;

class NetvarScan {
	public:
		HMODULE loadClientDLL()
		{
			std::filesystem::path p(client.szExePath);

			p = p.parent_path().parent_path().parent_path() / "bin";
			AddDllDirectory(p.wstring().c_str());

			return LoadLibraryEx(client.szExePath, NULL, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
		}

		intptr_t getOffset(RecvTable* table, const char* tableName, const char* netvarName)
		{
			for (int i = 0; i < table->m_nProps; i++)
			{
				RecvProp prop = table->m_pProps[i];

				if (!_stricmp(prop.m_pVarName, netvarName))
				{
					//std::cout << "\n" << prop.m_pVarName << ": 0x" << std::uppercase << std::hex << prop.m_Offset;
					return prop.m_Offset;
				}

				if (prop.m_pDataTable)
				{
					intptr_t offset = getOffset(prop.m_pDataTable, tableName, netvarName);
					if (offset)
					{
						//std::cout << "\n" << prop.m_pVarName << ": 0x" << std::uppercase << std::hex << offset + prop.m_Offset;
						return offset + prop.m_Offset;
					}
				}
			}
			return 0;
		}

		intptr_t getNetVarOffset(const char* tableName, const char* netvarName, ClientClass* clientClass)
		{
			ClientClass* currNode = clientClass;

			for (auto currNode = clientClass; currNode; currNode = currNode->m_pNext)
			{
				if (!_stricmp(tableName, currNode->m_pRecvTable->m_pNetTableName))
				{
					//std::cout << "\ntable: " << currNode->m_pRecvTable;
					return getOffset(currNode->m_pRecvTable, tableName, netvarName);
				}
			}

			return 0;
		}
};

class Netvar {
	private:
		HMODULE hMod;
		NetvarScan ns;
	public:
		Netvar() {
			hMod = ns.loadClientDLL();
		}

		intptr_t getNetvar(string netvar, string table, int offset) {
			ClientClass* dwGetallClassesAddr = (ClientClass*)((intptr_t)hMod + dwGetAllClasses);

			intptr_t result = ns.getNetVarOffset(table.c_str(), netvar.c_str(), dwGetallClassesAddr);
			if (offset != 0)
			{
				result += offset;
			}
			//std::cout << "\n " << netvar << ": 0x" << std::uppercase << std::hex << result;
			return result;
		}
};
