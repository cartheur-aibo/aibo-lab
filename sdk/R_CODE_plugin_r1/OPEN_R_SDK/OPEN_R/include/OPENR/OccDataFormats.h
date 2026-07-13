//
// OccDataFormats.h
//
// Copyright 2000 Sony Corporation
// 

#ifndef _OccDataFormats_h_DEFINED
#define _OccDataFormats_h_DEFINED

#include <OPENR/ODataFormats.h>
#include <OPENR/OccDataFormatsCore.h>

struct OccImageData{
    OccSensorInformation* m_p1;
    byte*                 m_p2;

    OccImageData():m_p1(NULL),m_p2(NULL){}
    OccImageData(const void* head, const void* image) {
        m_p1 = (OccSensorInformation*)head;
        m_p2 = (byte*)image;
    }

    // Get
    OccSensorInformation* GetSensorInformation() { return m_p1; }
    void* GetValue(MWIID id) { return (void*)(m_p2 + m_p1->GetOffset(id)); }

};


struct OccSensorData{
    OccSensorInformation* m_p1;
    byte*                 m_p2;

    OccSensorData():m_p1(NULL),m_p2(NULL){}
    OccSensorData(const void* head, const void* sensor) {
        m_p1 = (OccSensorInformation*)head;
        m_p2 = (byte*)sensor;
    }

    // Get
    OccSensorInformation* GetSensorInformation() { return m_p1; } 
    void* GetValue(MWIID id) { return (void*)(m_p2 + m_p1->GetOffset(id)); }
};


struct OccEventData{
    OpenrTime m_open_r_time;
    MWIID     m_event_id;
    longword  m_posture_id;
    longword  m_value;

    OccEventData():
        m_open_r_time(0), m_event_id(0), m_posture_id(0), m_value(0) {}
    OccEventData(OpenrTime o, MWIID e, longword p, longword v):
        m_open_r_time(o), m_event_id(e), m_posture_id(p), m_value(v) {}

    // Get
    OpenrTime GetOpenrTime() { return m_open_r_time; }
    MWIID     GetEventID()   { return m_event_id; }
    longword  GetPostureID() { return m_posture_id; }
    longword  GetValue()     { return m_value; }

    // Set
    OStatus SetOpenrTime(OpenrTime o) { m_open_r_time = o; return oSUCCESS; }
    OStatus SetEventID(MWIID e)       { m_event_id = e;    return oSUCCESS; }
    OStatus SetPostureID(longword p)  { m_posture_id = p;  return oSUCCESS; }
    OStatus SetValue(longword v)      { m_value = v;       return oSUCCESS; }
};


struct OccAudioData{
    OccSensorInformation* m_p1;
    byte*                 m_p2;

    OccAudioData():m_p1(NULL),m_p2(NULL){}
    OccAudioData(const void* head, const void* audio) {
        m_p1 = (OccSensorInformation*)head;
        m_p2 = (byte*)audio;
    }

    // Get
    OccSensorInformation* GetSensorInformation() { return m_p1; }
    void* GetValue()                             { return (void*)m_p2; }
};


#endif  //  _OccDataFormats_h_DEFINED
