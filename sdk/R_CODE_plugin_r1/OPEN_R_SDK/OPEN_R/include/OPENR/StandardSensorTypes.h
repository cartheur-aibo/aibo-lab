//
// StandardSensorTypes.h
//
// Copyright 2000 Sony Corporation
//

#ifndef _StandardSensorTypes_h_DEFINED
#define _StandardSensorTypes_h_DEFINED

#include <OPENR/MWInformationTypes.h>

/* Element of SensorID */
const SensorType stUNDEF              = 0x00;
const SensorType stCAMERA             = 0x01;
const SensorType stMIC                = 0x02;
const SensorType stDISTANCE_SENSOR    = 0x03;
const SensorType stBATTERY            = 0x04;
const SensorType stTHERMOMETER        = 0x05;
const SensorType stENERGY             = 0x06;
const SensorType stCOUNTER            = 0x07;
const SensorType stSWITCH             = 0x08;
const SensorType stTOUCH_SENSOR       = 0x09;
const SensorType stACCELERATION_METER = 0x0A;

const SensorType stDONT_CARE          = 0xff;

#endif  //  _StandardSensorTypes_h_DEFINED
