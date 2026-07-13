//
// OccTypes.h
//
// Copyright 2000,2001,2002 Sony Corporation
//

#ifndef _OccTypes_h_DEFINED
#define _OccTypes_h_DEFINED

#include <Types.h>
#include <OPENR/OStatus.h>
#include <OPENR/OUnits.h>

//   OPEN-R Time 
typedef longword OpenrTime;

//   Target Identification 
typedef longword AppID;
const AppID INVALID_APPID = 0x0;

typedef byte Enable;
const Enable    enable  = 1;
const Enable    disable = 0;

// Expansion / Compatible flag
typedef byte ExpansionFlag;
const ExpansionFlag efCOMPATIBLE  = 0;
const ExpansionFlag efEXPANSION   = 1;
const ExpansionFlag efINVALID     = 0xff;

//   T_Matrix Type 
const longword  cidINVALID          = 0xffffff;
const longword  cidMASK_MATRIX      = 0x00000f;
const longword  cidGROUND           = 0x000000;
const longword  cidBODY             = 0x000001;

const longword  cidMASK_SITUATION   = 0x0000f0;
const longword  cidSTILL            = 0x000000;
const longword  cidMOVING           = 0x000010;
const longword  cidWALKING          = 0x000020;
const longword  cidCHANGE_POS       = 0x000030;
const longword  cidCHANGE_BODYC     = 0x000040;

struct CoordinateID {     // Total 4 Byte
public:
    longword m_flag;      // 4 Byte

public:
    CoordinateID (longword flag = cidINVALID)
        {
            m_flag = flag;
        }
    CoordinateID (longword matrix_flag, longword situation_flag)
        {
            m_flag = matrix_flag | situation_flag;
        }

    // Is
    bool IsFlag(longword flag = cidINVALID, longword mask = 0xffffffff) {
        return (m_flag & mask) == flag;
    }

    // Get
    longword GetMatrixFlag(void)    { return m_flag & cidMASK_MATRIX; }
    longword GetSituationFlag(void) { return m_flag & cidMASK_SITUATION; }

    // Set
    OStatus SetMatrixFlag(longword flag = cidINVALID) {
        if(flag == cidINVALID || m_flag == cidINVALID) {
            m_flag = flag;
        }else{
            m_flag = (m_flag&(~cidMASK_MATRIX)) | flag;
        }
        return oSUCCESS;
    }
    OStatus SetSituationFlag(longword flag) {
        if(flag == cidINVALID || m_flag == cidINVALID) {
            m_flag = flag;
        }else{
            m_flag = (m_flag&(~cidMASK_SITUATION)) | flag;
        }
        return oSUCCESS;
    }

    // Operator
    bool operator==(const CoordinateID& cid) { return m_flag == cid.m_flag; }
    bool operator!=(const CoordinateID& cid) { return m_flag != cid.m_flag; }
};

#endif  //  _OccTypes_h_DEFINED
