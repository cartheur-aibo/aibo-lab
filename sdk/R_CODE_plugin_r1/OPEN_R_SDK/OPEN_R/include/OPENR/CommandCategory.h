//
// CommandCategory.h
//
// Copyright 2000 Sony Corporation
//

#ifndef _CommandCategory_h_DEFINED
#define _CommandCategory_h_DEFINED

#include <OPENR/OccTypes.h>
#include <OPENR/CommandCategoryTypes.h>

struct CommandCategory {    // 4 bytes
    ExpansionFlag    m_expansion;    // 1:expansion/compatible flag
    CCategoryL       m_CategoryL;    // 1:category large
    CCategoryM       m_CategoryM;    // 1:category middle
    CCategoryS       m_CategoryS;    // 1:category small

    // constructor
    CommandCategory():m_expansion(0),m_CategoryL(cclINVALID),
                      m_CategoryM(ccmINVALID),m_CategoryS(ccsINVALID) {}

    // operator
    bool operator == (const CommandCategory& ic1) {
        if ((ic1.m_expansion == this->m_expansion) &&
            (ic1.m_CategoryL == this->m_CategoryL) &&
            (ic1.m_CategoryM == this->m_CategoryM) &&
            (ic1.m_CategoryS == this->m_CategoryS)) {
            return true;
        } else {
            return false;
        }
    }

    // Get
    ExpansionFlag    GetExpansionFlag() { return m_expansion; }
    CCategoryL       GetCategoryL() { return m_CategoryL; }
    CCategoryM       GetCategoryM() { return m_CategoryM; }
    CCategoryS       GetCategoryS() { return m_CategoryS; }

    // Is
    bool IsExpansion() { return (m_expansion == efEXPANSION ? true : false); }
};


#endif    // _CommandCategory_h_DEFINED

