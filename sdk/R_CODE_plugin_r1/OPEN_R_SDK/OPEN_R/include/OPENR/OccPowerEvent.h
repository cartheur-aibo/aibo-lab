//
// OccPowerEvent.h
//
// Copyright 2000 Sony Corporation
//

#ifndef _OccPowerEvent_h_DEFINED
#define _OccPowerEvent_h_DEFINED

#include <OPENR/SystemCategory.h>
#include <OPENR/OccBootCondition.h>
#include <OPENR/OccSysEvent.h>
#include <OPENR/MWCommandTypes.h>


typedef longword PEEmergency;
typedef OccBC PEBootCause;

const PEEmergency KEY_SW                         = 0x00000001;
const PEEmergency CONNECT_TO_EX_POWER            = 0x00000002;
const PEEmergency DISCONNECT_TO_EX_POWER         = 0x00000004;
const PEEmergency CLK_GEN_ERR                    = 0x00000008;
const PEEmergency BATTERY_CAPACITY_LOW           = 0x00000010;
const PEEmergency BATTERY_OVER_CURRENT           = 0x00000020;
const PEEmergency BATTERY_OVER_TEMP_DISCHARGING  = 0x00000040;
const PEEmergency BATTERY_OVER_TEMP_CHARGING     = 0x00000080;
const PEEmergency BATTERY_ERROR_OF_CHARGING      = 0x00000100;
const PEEmergency ERROR_OF_PLUNGER               = 0x00000200;
const PEEmergency ERROR_OF_FAN                   = 0x00000400;
const PEEmergency RTC_ERROR                      = 0x00000800;
const PEEmergency OPEN_R_POWER_NG                = 0x00001000;
const PEEmergency BREAK_WIRE                     = 0x00002000;
const PEEmergency CONNECT_TO_ANYTHING            = 0x00004000;


struct OccPowerEvent {
    OpenrTime           m_opnrtime;
    SystemCategory      m_category;
    PEEmergency         m_emergency;
    PEBootCause         m_cause;
    OccBootCondition    m_recomendCondition;
    longword            m_num;
    MWCID               m_mwcid[PPM_MAXCOMMAND];
   
    OccPowerEvent(): m_opnrtime(0),  m_category(), m_emergency(0), m_cause(0),
                     m_recomendCondition(), m_num(0) {}
    
    // Get
    OpenrTime   GetOpenrTime()    { return m_opnrtime; }
    PEEmergency GetRobotStatus()  { return m_emergency; }
    PEBootCause GetBootCause()    { return m_cause; }
    longword    GetCommandNum()   { return m_num; }
    MWCID       GetMWCID(int num) { return m_mwcid[num]; }

    // Set
    OStatus SetOpenrTime(const OpenrTime opnrtime) {
        m_opnrtime = opnrtime;
        return oSUCCESS;
    }
    OStatus SetRobotStatus(const PEEmergency status) {
        m_emergency = status;
        return oSUCCESS;
    }
    OStatus SetBootCause(const PEBootCause bootcause) {
        m_cause = bootcause;
        return oSUCCESS;
    }
    OStatus SetCommandNum(const longword num) {
        m_num  = num;
        return oSUCCESS;
    }
    OStatus SetMWCID(const longword num, const MWCID mwcid) {
        m_mwcid[num]  = mwcid;
        return oSUCCESS;
    }    
};


#endif // _OccPowerEvent_h_DEFINED
