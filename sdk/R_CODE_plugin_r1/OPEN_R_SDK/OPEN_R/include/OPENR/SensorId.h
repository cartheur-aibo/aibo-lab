//
// SensorId.h
//
// Copyright 2000 Sony Corporation
//

#ifndef    _SensorId_h_DEFINED
#define    _SensorId_h_DEFINED

#include <OPENR/OStatus.h>
#include <OPENR/OccTypes.h>
#include <OPENR/MWInformationTypes.h>
#include <OPENR/StandardAttachedParts.h>
#include <OPENR/StandardSensorTypes.h>

struct SensorId {                       // 8 bytes
    ExpansionFlag    m_sensorExpansion; // 1 byte
    SensorType       m_sensor;          // 1 byte
    byte             _padding1;         // 1 byte
    byte             _padding2;         // 1 byte
    ExpansionFlag    m_partsExpansion;  // 1 byte
    byte             _padding3;         // 1 byte
    AttachedParts    m_parts;           // 1 byte
    AttachedPosition m_position;        // 1 byte

    // constructor
    SensorId(void) :
        m_sensorExpansion(efCOMPATIBLE),    
        m_sensor(stUNDEF),
        m_partsExpansion(efCOMPATIBLE),
        m_parts(apUNKNOWN),
        m_position(0),
        _padding1(0),
        _padding2(0),
        _padding3(0) {}

    SensorId(ExpansionFlag    expansionFlagS,
             SensorType       sensorType,
             ExpansionFlag    expansionFlagP,
             AttachedParts    attachedParts,
             AttachedPosition attachedPosition) :
        m_sensorExpansion(expansionFlagS),    
        m_sensor(sensorType),
        m_partsExpansion(expansionFlagP),    
        m_parts(attachedParts),
        m_position(attachedPosition),
        _padding1(0),
        _padding2(0),
        _padding3(0) {}

    // operator
    bool operator==(const SensorId& id1)
        {
            if ((id1.m_sensorExpansion == this->m_sensorExpansion) &&
                (id1.m_sensor          == this->m_sensor)          &&
                (id1.m_partsExpansion  == this->m_partsExpansion)  &&
                (id1.m_parts           == this->m_parts)           &&
                (id1.m_position        == this->m_position)) {
                return true;
            } else {
                return false;
            }
        }

    SensorId& operator=(const SensorId& a) {
        if (this != &a) {
            m_sensorExpansion = a.m_sensorExpansion;
            m_sensor          = a.m_sensor;
            m_partsExpansion  = a.m_partsExpansion;
            m_position        = a.m_position;
            m_parts           = a.m_parts;
        }
        return *this;
    }

    /* accessor */

    // Get
    ExpansionFlag    GetSensorExpansionFlag() { return m_sensorExpansion; }
    SensorType       GetSensor()              { return m_sensor; }
    ExpansionFlag    GetPartsExpansionFlag()  { return m_partsExpansion; }
    AttachedParts    GetParts()               { return m_parts; }
    AttachedPosition GetPosition()            { return m_position; }

    // Is
    bool IsSensorExpansion() {
        return (m_sensorExpansion == efEXPANSION ? true : false);
    }
    bool IsPartsExpansion() {
        return (m_partsExpansion == efEXPANSION ? true : false);
    }

    // Set
    OStatus SetSensorExpansionFlag(const ExpansionFlag flag) { 
        m_sensorExpansion = flag;
        return oSUCCESS;
    }
    OStatus SetPartsExpansionFlag(const ExpansionFlag flag) { 
        m_partsExpansion = flag;
        return oSUCCESS;
    }
    OStatus SetSensor(const SensorType sensor) {
        m_sensor = sensor;
        return oSUCCESS;
    }
    OStatus SetParts(const AttachedParts parts) {
        m_parts = parts;
        return oSUCCESS;
    }
    OStatus SetPosition(const AttachedPosition position) {
        m_position = position;
        return oSUCCESS;
    }
};

#endif // _SensorId_h_DEFINED
