#ifndef ZONEMGR_H
#define ZONEMGR_H

#include <map>
#include <vector>
#include "defines.h"
#include "Error.h"

class ZoneMgr {
	private:
		struct VarData {
			char type;
			int index;
		};
		typedef std::map<std::string, VarData> varTable;
		
		int inUse, saved;
		std::vector<varTable> zones;
		varTable zone;
	public:
		int boolCount, intCount, stringCount;
		ZoneMgr();
		void createZone();
		void saveZone();
		void loadZone();
		void addVar(std::string id, std::string type);
		char getVarType(std::string id);
		int getVarIndex(std::string id);
};

#endif
