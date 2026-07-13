//
// OccBootCondition.h
//
// Copyright 2000 Sony Corporation
//

#ifndef _OccBootCondition_h_DEFINED
#define _OccBootCondition_h_DEFINED

#include <time.h>

#include <OPENR/OTime.h>
#include <OPENR/OPENR.h>
#include <OPENR/OPower.h>

typedef word      OccBC;
typedef time_t    BCTime;
typedef longword  BCTimeFlag;
typedef int       BCVibration;

const OccBC occbcNULL                      = 0x0000;
const OccBC occbcBOOT_TIMER                = 0x0001;
const OccBC occbcVIBRATION_DETECTED        = 0x0002;
const OccBC occbcKEY_SW                    = 0x0004;
const OccBC occbcSTATION_CONNECTED         = 0x0008;
const OccBC occbcSTATION_DISCONNECTED      = 0x0010;
const OccBC occbcBATTERY_CAPACITY_FULL     = 0x0020;
const OccBC occbcREQ_FROM_STATION          = 0x0040;
const OccBC occbcALL                       = 0x007F;

const BCTimeFlag RELATIVE                  = 0x00000000;   // Relative Time
const BCTimeFlag ABSOLUTE                  = 0x80000000;   // Absolute Time


struct OccBootCondition{
    OccBC         m_mask;
    OccBC         m_condition;
    BCTimeFlag    m_tflag;
    time_t        m_time;
    BCVibration   m_level;

    OccBootCondition():m_mask(occbcALL), m_condition(occbcALL), 
                       m_tflag(RELATIVE),m_time(0x10), m_level(0x04) {}

    // Get
    OccBC         GetMask()        { return m_mask; }
    OccBC         GetBCCondition() { return m_condition; }
    BCTimeFlag    GetBCTimeFlag()  { return m_tflag; }
    BCTime        GetTime()        { return m_time; }
    BCVibration   GetVibration()   { return m_level; }

    // Set
    OStatus SetMask(const OccBC mask) {
        m_mask = mask;
        return oSUCCESS;
    }
    OStatus SetBCCondition(const OccBC condition) {
        m_condition = condition;
        return oSUCCESS;
    }
    OStatus SetBCTimeFlag(const BCTimeFlag t_flag) {
        m_tflag = t_flag;
        return oSUCCESS;
    }
    OStatus SetTime(const BCTime time) {
        m_time = time;
        return oSUCCESS;
    }
    OStatus SetBCVibration(const BCVibration level) {
        m_level = level;
        return oSUCCESS;
    }
    
};


#endif // _OccBootCondition_h_DEFINED
