//
// SystemCategoryTypes.h
//
// Copyright 2000 Sony Corporation
//

#ifndef _SystemCategoryTypes_h_DEFINED
#define _SystemCategoryTypes_h_DEFINED

#include <Types.h>

typedef byte    SCategoryL;     // System Category Large
typedef byte    SCategoryM;     // System Category Middle
typedef byte    SCategoryS;     // System Category Small


// compatible System category L
const SCategoryL    sclUNKNOWN      = 0xff;

const SCategoryL    sclSTATE        = 0x01;
const SCategoryL    sclPOWER        = 0x02;

// compatible System category M
const SCategoryM    scmUNKNOWN      = 0xff;
// State
const SCategoryM    scmPOSTURE      = 0x01;
const SCategoryM    scmTIMEZONE     = 0x02;
const SCategoryM    scmRESTART      = 0x03;

// Power
const SCategoryM    scmSTART        = 0x01;
const SCategoryM    scmSTOP         = 0x02;
// DISTANCE

// compatible System category S
const SCategoryS    scsUNKNOWN      = 0xff;
// Posture
const SCategoryS    scsON           = 0x00;
const SCategoryS    scsOFF          = 0x80;
const SCategoryS    scsSTATION_ON   = 0x01;
const SCategoryS    scsSTATION_OFF  = 0x81;
const SCategoryS    scsHOLD_UP      = 0x02;
const SCategoryS    scsHOLD_DOWN    = 0x82;
const SCategoryS    scsJAM          = 0x03;
const SCategoryS    scsJAMQUIT      = 0x83;
const SCategoryS    scsFALL_DOWN    = 0x04;

const SCategoryS    scsFALL_DOWN_BACK       = 0x14;
const SCategoryS    scsFALL_DOWN_HEAD       = 0x24;
const SCategoryS    scsFALL_DOWN_TAIL       = 0x34;
const SCategoryS    scsFALL_DOWN_RIGHT      = 0x44;
const SCategoryS    scsFALL_DOWN_LEFT       = 0x54;

const SCategoryS    scsGET_UP       = 0x84;
const SCategoryS    scsMECH_LOAD_H  = 0x05;
const SCategoryS    scsMECH_LOAD_L  = 0x85;
const SCategoryS    scsSTAY_ON      = 0x07;
const SCategoryS    scsSTAY_OFF     = 0x87;
const SCategoryS    scsLONGPAT      = 0x06;
const SCategoryS    scsMASK         = 0x80;

// Time Zone
const SCategoryS    scsTIME_ZONE_CHANGE = 0x01;

// Restart
const SCategoryS    scsAPPRESET     = 0x01;
const SCategoryS    scsALLRESET     = 0x02;

// Start
const SCategoryS    scsNORMAL       = 0x01;
const SCategoryS    scsFALL_DOWN_S  = 0x02;

const SCategoryS    scsFALL_DOWN_BACK_S       = 0x12;
const SCategoryS    scsFALL_DOWN_HEAD_S       = 0x22;
const SCategoryS    scsFALL_DOWN_TAIL_S       = 0x32;
const SCategoryS    scsFALL_DOWN_RIGHT_S      = 0x42;
const SCategoryS    scsFALL_DOWN_LEFT_S       = 0x52;

const SCategoryS    scsAC           = 0x03;
const SCategoryS    scsSTATION      = 0x04;

// Stop
const SCategoryS    scsSTOP         = 0x01;

#endif  //  _SystemCategoryTypes_h_DEFINED
