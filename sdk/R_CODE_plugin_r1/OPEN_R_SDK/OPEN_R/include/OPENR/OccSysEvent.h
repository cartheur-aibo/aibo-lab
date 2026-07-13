//
// OccSysEvent.h
//
// Copyright 2000 Sony Corporation
//

#ifndef _OccSysEvent_h_DEFINED
#define _OccSysEvent_h_DEFINED

#include <OPENR/SystemCategory.h>
#include <OPENR/MWCommandTypes.h>

const int PPM_MAXCOMMAND = 8;

struct OccSysEvent {
    OpenrTime       m_opnrtime;
    SystemCategory  m_category;
    longword        m_num;
    MWCID           m_mwcid[PPM_MAXCOMMAND];
    
    OccSysEvent(): m_opnrtime(0),  m_category(), m_num(0) {}

    // Get
    OpenrTime GetOpenrTime()    { return m_opnrtime; }
    longword  GetCommandNum()   { return m_num; }
    MWCID     GetMWCID(int num) { return m_mwcid[num]; }

    // Set
    OStatus SetOpenrTime(const OpenrTime opnrtime) {
        m_opnrtime = opnrtime;
        return oSUCCESS;
    }
    OStatus SetCommandNum(const longword num) {
        m_num  = num;
        return oSUCCESS;
    }
    OStatus SetMWCID(const int num, const MWCID mwcid) {
        m_mwcid[num]  = mwcid;
        return oSUCCESS;
    }
};

#endif // _OccSysEvent_h_DEFINED
