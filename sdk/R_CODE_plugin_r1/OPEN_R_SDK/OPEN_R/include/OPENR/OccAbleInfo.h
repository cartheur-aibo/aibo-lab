//
// OccAbleInfo.h
//
// Copyright 2000 Sony Corporation
//

#ifndef _OccAbleInfo_h_DEFINED
#define _OccAbleInfo_h_DEFINED

#include <Types.h>
#include <OPENR/OccTypes.h>
#include <OPENR/SensorId.h>
#include <OPENR/TargetPos.h>
#include <OPENR/CommandCategory.h>

typedef longword AbleStatus;
static const AbleStatus asFar      = 1;
static const AbleStatus asNear     = 2;
static const AbleStatus asJust     = 3;
static const AbleStatus asTooClose = 4;

struct OccAbleInfo{              // 56 Byte 
    OpenrTime       m_openrtime; //  4 Byte
    AppID           m_aid;       //  4 Byte
    SensorId        m_sid;       //  8 Byte
    CommandCategory m_group;     //  4 Byte
    TargetPos       m_tpos;      // 32 Byte
    AbleStatus      m_astatus;   //  4 Byte

    OccAbleInfo()
        :m_openrtime(0), m_aid(INVALID_APPID), m_sid()
    {
    }

    OccAbleInfo(OpenrTime openrtime, AppID aid, SensorId sid)
        :m_openrtime(openrtime), m_aid(aid), m_sid(sid)
    {
    }

    // Get
    OpenrTime&       GetOpenrTime      (void){ return m_openrtime; }
    AppID&           GetAppID          (void){ return m_aid; }
    SensorId&        GetSensorID       (void){ return m_sid; }
    CommandCategory& GetCommandCategory(void){ return m_group; }
    TargetPos&       GetTargetPos      (void){ return m_tpos; }
    AbleStatus&      GetAbleStatus     (void){ return m_astatus; }

    // Set
    OStatus SetOpenrTime(const OpenrTime& openrtime)
    {
        m_openrtime = openrtime; return oSUCCESS;
    }

    OStatus SetAppID(const AppID& aid)
    {
        m_aid = aid; return oSUCCESS;
    }

    OStatus SetSensorID(const SensorId& sid)
    {
        m_sid = sid; return oSUCCESS;
    }

    OStatus SetCommandCategory(const CommandCategory& group)
    {
        m_group = group; return oSUCCESS;
    }

    OStatus SetTargetPos(const TargetPos& tpos)
    {
        m_tpos = tpos; return oSUCCESS;
    }

    OStatus SetAbleStatus(const AbleStatus& astatus)
    {
        m_astatus = astatus; return oSUCCESS;
    }
};

#endif  // _OccAbleInfo_h_DEFINED
