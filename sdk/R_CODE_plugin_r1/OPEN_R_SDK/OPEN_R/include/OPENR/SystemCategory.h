//
// SystemCategory.h
//
// Copyright 2000 Sony Corporation
//
//  SystemCategory  : expansion/L/M/S
//                for SystemCategory

#ifndef _SystemCategory_h_DEFINED
#define _SystemCategory_h_DEFINED

#include <OPENR/OStatus.h>
#include <OPENR/OccTypes.h>
#include <OPENR/SystemCategoryTypes.h>

struct SystemCategory { // 4 bytes
    ExpansionFlag   m_expansion;    // 1:expansion/compatible flag
    SCategoryL      m_CategoryL;    // 1:category large
    SCategoryM      m_CategoryM;    // 1:category middle
    SCategoryS      m_CategoryS;    // 1:category small

    // constructor
    SystemCategory():
        m_expansion(efCOMPATIBLE), m_CategoryL(0), m_CategoryM(0),
        m_CategoryS(0) {}

    // operator
    bool operator == (const SystemCategory& sc1)
        {
            if ((sc1.m_expansion == this->m_expansion) &&
                (sc1.m_CategoryL == this->m_CategoryL) &&
                (sc1.m_CategoryM == this->m_CategoryM) &&
                (sc1.m_CategoryS == this->m_CategoryS)) {
                return true;
            } else {
                return false;
            }
        }

    // Get
    ExpansionFlag GetExpansionFlag() { return m_expansion; }
    SCategoryL    GetCategoryL()     { return m_CategoryL; }
    SCategoryM    GetCategoryM()     { return m_CategoryM; }
    SCategoryS    GetCategoryS()     { return m_CategoryS; }

    // Set
    OStatus SetCategoryL(const SCategoryL category) {
        m_CategoryL = category;
        return oSUCCESS;
    }
    OStatus SetCategoryM(const SCategoryM category) {
        m_CategoryM = category;
        return oSUCCESS;
    }
    OStatus SetCategoryS(const SCategoryL category) {
        m_CategoryS = category;
        return oSUCCESS;
    }

};


#endif  // _SystemCategory_h_DEFINED

