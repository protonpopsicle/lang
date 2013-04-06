#include "ZoneMgr.h"

ZoneMgr::ZoneMgr() {
    inUse       = 0;
    saved       = 0;
    boolCount   = 0;
    intCount    = 0;
    stringCount = 0;
    
    zones.push_back(zone);
}

void ZoneMgr::createZone() {
    zones.push_back(zone);
    inUse++;
}

void ZoneMgr::saveZone() {
    saved = inUse;
}

void ZoneMgr::loadZone() {
    inUse = saved;
}

void ZoneMgr::addVar(std::string id, std::string type) {
    varTable::iterator iter = zones[inUse].find(id);
    VarData varData;
    
    if (iter == zones[inUse].end()) {
        if (type == "bool") {
            varData.type = BOOL;
            varData.index = boolCount;
            boolCount++;
        }
        else if (type == "int") {
            varData.type = INT;
            varData.index = intCount;
            intCount++;
        }
        else if (type == "string") {
            varData.type = STRING;
            varData.index = stringCount;
            stringCount++;
        }
        zones[inUse].insert(std::make_pair(id, varData));
    }
    else {
        throw Error("'" + id + "' already declared in this scope");
    }
}

char ZoneMgr::getVarType(std::string id) {
    for (int i = inUse; i != 0; i--) {
        varTable::iterator iter = zones[i].find(id);
        if (iter != zones[i].end()) {
            return iter->second.type; // a variable's type
        }
    }
    throw Error("'" + id + "' wasn't declared in this scope");
}

int ZoneMgr::getVarIndex(std::string id) {
    for (int i = inUse; i != 0; i--) {
        varTable::iterator iter = zones[i].find(id);
        if (iter != zones[i].end()) {
            return iter->second.index; // a variable's index
        }
    }
    throw Error("'" + id + "' wasn't declared in this scope");
}
