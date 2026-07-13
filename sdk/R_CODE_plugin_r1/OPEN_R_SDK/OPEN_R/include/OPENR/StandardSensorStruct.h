//
// StandardSensorStruct.h
//
// Copyright 2000,2003 Sony Corporation
//

#ifndef _StandardSensorStruct_h_DEFINED
#define _StandardSensorStruct_h_DEFINED

#include <Types.h>
#include <OPENR/OUnits.h>
#include <OPENR/ODataFormats.h>

struct StandardTemperature { // 4bytes
    Temperature m_temperature;

    // Get
    Temperature& GetTemperature() { return m_temperature; }
};


struct StandardBatteryRest{ // 4bytes
    Percentage  m_rest;

    // Get
    Percentage&  GetBatteryRest() { return m_rest; }
};
  

struct StandardDistance{ // 4bytes
    Length m_distance;

    // Get
    Length& GetDistance() { return m_distance; }

    // Is
    bool IsValid() { return ((m_distance==opsdINVALID_VALUE) ? false : true); }
};

#endif  //  _StandardSensorStruct_h_DEFINED
