//
// InformationCategory.h
//
// Copyright 2000 Sony Corporation
//

#ifndef _InformationCategory_h_DEFINED
#define _InformationCategory_h_DEFINED

#include <OPENR/OccTypes.h>
#include <OPENR/InformationCategoryTypes.h>

struct InformationCategory {   // 4 bytes
    ExpansionFlag m_expansion; // 1:expansion/compatible flag
    ICategoryL    m_CategoryL; // 1:category large
    ICategoryM    m_CategoryM; // 1:category middle
    ICategoryS    m_CategoryS; // 1:category small

    // operator
    bool operator == (const InformationCategory& ic1) {
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
    ExpansionFlag GetExpansionFlag() { return m_expansion; }
    ICategoryL    GetCategoryL() { return m_CategoryL; }
    ICategoryM    GetCategoryM() { return m_CategoryM; }
    ICategoryS    GetCategoryS() { return m_CategoryS; }

    // Is
    bool IsExpansion() { return (m_expansion == efEXPANSION ? true : false); }
};

#endif    // _InformationCategory_h_DEFINED
