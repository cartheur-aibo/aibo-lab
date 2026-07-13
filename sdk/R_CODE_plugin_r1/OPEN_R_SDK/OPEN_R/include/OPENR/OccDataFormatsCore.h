//
// OccDataFormatsCore.h
//
// Copyright 2000,2001,2002 Sony Corporation
//

#ifndef _OccDataFormatsCore_h_DEFINED
#define _OccDataFormatsCore_h_DEFINED

#include <OPENR/OccTypes.h>
#include <OPENR/MWInformationTypes.h>
#include <OPENR/RobotSpeed.h>
#include <OPENR/TMatrix.h>

struct HeadInfo {                  // Total   = 48bytes
    longword    m_size_of_head;    //  4bytes : Size of Head Information
    longword    m_size_of_body;    //  4bytes : Size of Body Information
    longword    m_num_of_sensor;   //  4bytes : Number of SensorInfo
    OpenrTime   m_open_r_time;     //  4bytes : VR frame number 
    longword    m_posture_id;      //  4bytes : Posture ID written by CMAgent
    RobotSpeed  m_robot_speed;     // 28bytes : Moving Speed

    HeadInfo():
        m_size_of_head(0), m_size_of_body(0), m_num_of_sensor(0),
        m_open_r_time(0), m_posture_id(0) {}
    HeadInfo(longword h, longword b, longword s, OpenrTime t, longword i):
        m_size_of_head(h), m_size_of_body(b), m_num_of_sensor(s),
        m_open_r_time(t),m_posture_id(i) {}

    // Get
    longword   GetSizeOfHead()  { return m_size_of_head; }
    longword   GetSizeOfBody()  { return m_size_of_body; }
    longword   GetNumOfSensor() { return m_num_of_sensor; }
    OpenrTime  GetOpenrTime()   { return m_open_r_time; }
    longword   GetPostureID()   { return m_posture_id; }
    RobotSpeed& GetRobotSpeed() { return m_robot_speed; }

    // Set
    OStatus SetSizeOfHead(longword h) { m_size_of_head = h;  return oSUCCESS; }
    OStatus SetSizeOfBody(longword b) { m_size_of_body = b;  return oSUCCESS; }
    OStatus SetNumOfSensor(longword s){ m_num_of_sensor = s; return oSUCCESS; }
    OStatus SetOpenrTime(OpenrTime t) { m_open_r_time = t;   return oSUCCESS; }
    OStatus SetPostureID(longword i)  { m_posture_id = i;    return oSUCCESS; }
    OStatus SetRobotSpeed(const RobotSpeed& a) { 
        m_robot_speed = a;
        return oSUCCESS;
    }
};


struct SensorStaticInfo {    // Total = 20bytes
    longword m_offset;       // 4bytes :
    longword m_vertical;     // 4bytes :
    longword m_horizontal;   // 4bytes :
    longword m_skip;         // 4bytes :
    longword m_step;         // 4bytes :

    SensorStaticInfo():
        m_offset(0), m_vertical(0), m_horizontal(0), m_skip(0), m_step(0) {}

    SensorStaticInfo(longword o, longword v, longword h, longword s1, 
                     longword s2):
        m_offset(o), m_vertical(v), m_horizontal(h), m_skip(s1), m_step(s2) {}

    // Get
    longword GetOffset()     { return m_offset; }
    longword GetVertical()   { return m_vertical; }
    longword GetHorizontal() { return m_horizontal; }
    longword GetSkip()       { return m_skip; }
    longword GetStep()       { return m_step; }

    // Set
    OStatus SetOffset(longword o)     { m_offset = o;     return oSUCCESS; }
    OStatus SetVertical(longword v)   { m_vertical = v;   return oSUCCESS; }
    OStatus SetHorizontal(longword h) { m_horizontal = h; return oSUCCESS; }
    OStatus SetSkip(longword s)       { m_skip = s;       return oSUCCESS; }
    OStatus SetStep(longword s)       { m_step = s;       return oSUCCESS; }
};


/*
  struct SensorDynamicInfo
    OpenrTime     m_open_r_time : time after process (something like filter)
    longword      m_limit       : valid flag about movable range of sensor
    CoordinateID  m_matrix_flag : valid flag about ground contact of T_Matrix
    T_Matrix      m_t_matrix    : translation/rotation matrix
*/
struct SensorDynamicInfo {       // Total = 76bytes
    OpenrTime     m_open_r_time; //  4bytes
    longword      m_limit;       //  4bytes
    CoordinateID  m_matrix_flag; //  4bytes
    T_Matrix      m_t_matrix;    // 64bytes

    SensorDynamicInfo():
        m_open_r_time(0), m_limit(0), m_matrix_flag(cidINVALID) {}
    SensorDynamicInfo(OpenrTime o, longword l, CoordinateID m):
        m_open_r_time(o), m_limit(l), m_matrix_flag(m) {}

    // Get
    OpenrTime     GetOpenrTime()  { return m_open_r_time; }
    longword      GetLimit()      { return m_limit; }
    CoordinateID  GetMatrixFlag() { return m_matrix_flag; }
    T_Matrix&     GetT_Matrix()   { return m_t_matrix; }

    // Set
    OStatus SetOpenrTime(OpenrTime o) { 
        m_open_r_time = o;
        return oSUCCESS;
    }
    OStatus SetLimit(longword l) {
        m_limit = l;
        return oSUCCESS;
    }
    OStatus SetMatrixFlag(CoordinateID m) {
        m_matrix_flag = m;
        return oSUCCESS;
    }
    OStatus SetT_Matrix(const T_Matrix& t) { 
        m_t_matrix = t;
        return oSUCCESS;
    }
};


struct SensorInfo {              // Total   = 12bytes
    MWIID    m_info_id;          //  4bytes : InformationID
    longword m_ss_info_offset;   //  4bytes : offset to SnsorStaticInfo
    longword m_sd_info_offset;   //  4bytes : offset to SnsorDynamicInfo

    SensorInfo(): m_info_id(0), m_ss_info_offset(0), m_sd_info_offset(0) {}
    SensorInfo(MWIID id, longword ss, longword sd):
        m_info_id(id), m_ss_info_offset(ss), m_sd_info_offset(sd) {}

    // Get
    MWIID    GetInformation()             { return m_info_id; } 
    longword GetSensorStaticInfoOffset()  { return m_ss_info_offset; }  
    longword GetSensorDynamicInfoOffset() { return m_sd_info_offset; } 

    // Set
    OStatus SetInformation(MWIID id) {
        m_info_id = id;
        return oSUCCESS;
    }
    OStatus SetSensorStaticInfoOffset(longword ss) { 
        m_ss_info_offset = ss;
        return oSUCCESS;
    }
    OStatus SetSensorDynamicInfoOffset(longword sd) {
        m_sd_info_offset = sd;
        return oSUCCESS;
    }
};


struct OccSensorInformation {   // Total   = 60bytes
    HeadInfo   m_h_info;        // 48bytes : Head Information
    SensorInfo m_s_info[1];     // 12bytes : 1st element of SensorInfo

    // Get
    longword    GetSizeOfHead()  { return GetHeadInfo()->GetSizeOfHead(); }
    longword    GetSizeOfBody()  { return GetHeadInfo()->GetSizeOfBody(); }
    longword    GetNumOfSensor() { return GetHeadInfo()->GetNumOfSensor(); }
    OpenrTime   GetOpenrTime()   { return GetHeadInfo()->GetOpenrTime(); }
    longword    GetPostureID()   { return GetHeadInfo()->GetPostureID(); }
    RobotSpeed& GetRobotSpeed()  { return GetHeadInfo()->GetRobotSpeed(); }

    longword    GetOffset(MWIID id) {
        return GetSensorStaticInfo(id)->GetOffset();
    }
    longword    GetVertical(MWIID id) {
        return GetSensorStaticInfo(id)->GetVertical();
    }
    longword    GetHorizontal(MWIID id) {
        return GetSensorStaticInfo(id)->GetHorizontal();
    }
    longword    GetSkip(MWIID id) {
        return GetSensorStaticInfo(id)->GetSkip();
    }
    longword    GetStep(MWIID id) {
        return GetSensorStaticInfo(id)->GetStep();
    }
    OpenrTime   GetOpenrTime(MWIID id) {
        return GetSensorDynamicInfo(id)->GetOpenrTime();
    }
    longword    GetLimit(MWIID id) {
        return GetSensorDynamicInfo(id)->GetLimit();
    }
    CoordinateID  GetMatrixFlag(MWIID id) {
        return GetSensorDynamicInfo(id)->GetMatrixFlag();
    }
    T_Matrix&   GetT_Matrix(MWIID id) {
        return GetSensorDynamicInfo(id)->GetT_Matrix();
    }

private:
    MWIID GetInformation(longword index) {
        return m_s_info[index].GetInformation();
    }

    HeadInfo*    GetHeadInfo() { return &m_h_info; }

    SensorInfo*  GetSensorInfo(longword i) { return &m_s_info[i]; }
    SensorStaticInfo*  GetSensorStaticInfo(MWIID id) {
        return (SensorStaticInfo*)
            ((byte*)&m_h_info + 
             m_s_info[GetIndex(id)].GetSensorStaticInfoOffset());
    }
    SensorDynamicInfo* GetSensorDynamicInfo(MWIID id) {
        return (SensorDynamicInfo*)
            ((byte*)&m_h_info + 
             m_s_info[GetIndex(id)].GetSensorDynamicInfoOffset());
    }
    slongword GetIndex(MWIID id) {
        slongword index = -1;
        for(int i = 0; i < GetNumOfSensor(); i++)
        {
            if(GetInformation(i)==id)
            {
                index = i;
                break;
            }
        }
        return index;
    }
};

#endif  //  _OccDataFormatsCore_h_DEFINED
