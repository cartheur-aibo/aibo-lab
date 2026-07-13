//
// InformationCategoryTypes.h
//
// Copyright 2000 Sony Corporation
//

#ifndef _InformationCategoryTypes_h_DEFINED
#define _InformationCategoryTypes_h_DEFINED

#include <Types.h>

typedef    byte    ICategoryL;        // Information Category Large
typedef    byte    ICategoryM;        // Information Category Middle
typedef    byte    ICategoryS;        // Information Category Small

/****************************************/
/* compatible information category L    */
/****************************************/
const ICategoryL iclUNKNOWN             = 0x00;
const ICategoryL iclDONT_CARE           = 0xFF;

const ICategoryL iclLENGTH              = 0x10;
const ICategoryL iclMASS                = 0x11;
const ICategoryL iclTIME                = 0x12;

const ICategoryL iclTEMPERATURE_K       = 0x14;
const ICategoryL iclAMOUNTOFSUBSTNACE   = 0x15;
const ICategoryL iclLUMINOSITIY         = 0x16;

const ICategoryL iclANGLE               = 0x1A;

const ICategoryL iclTEMPERATURE_C       = 0x20;
const ICategoryL iclTEMPERATURE         = 0x20;    // same as iclTEMPERATURE_C

const ICategoryL iclVELOCITY            = 0x21;
const ICategoryL iclACCELERATION        = 0x22;    // acceleration of inertia
const ICategoryL iclANGULARVELOCITY     = 0x23;
const ICategoryL iclANGULARACCELERATION = 0x24;
const ICategoryL iclFREQUENCY           = 0x25;
const ICategoryL iclFORCE               = 0x26;
const ICategoryL iclPRESSURE            = 0x27;
const ICategoryL iclAREA                = 0x28;
const ICategoryL iclVOLUME              = 0x29;
const ICategoryL iclMOMENT              = 0x2A;
const ICategoryL iclDENSITY             = 0x2B;
const ICategoryL iclENERGY              = 0x2C;
const ICategoryL iclPOWER               = 0x2D;

const ICategoryL iclLAYER_H             = 0x60;
const ICategoryL iclLAYER_M             = 0x61;
const ICategoryL iclLAYER_L             = 0x62;
const ICategoryL iclLAYER_C             = 0x63;

const ICategoryL iclSOUND               = 0x70;



const ICategoryL iclPERCENTAGE          = 0x80;
const ICategoryL iclTORQUE              = 0x81;
const ICategoryL iclTEMPERATURE_F       = 0x82;
const ICategoryL iclDISTANCE            = 0x84;
const ICategoryL iclTOUCH               = 0x88;
const ICategoryL iclSWITCH              = 0x89;
const ICategoryL iclMODE                = 0x8A;
const ICategoryL iclACTION              = 0x8B;

/****************************************/
/* compatible information category M    */
/****************************************/
const ICategoryM icmUNKNOWN             = 0x00;
const ICategoryM icmDONT_CARE           = 0xFF;

// Image:LayerH/M/L/C
const ICategoryM icmIMAGE_Y             = 0x01;
const ICategoryM icmIMAGE_CR            = 0x02;
const ICategoryM icmIMAGE_CB            = 0x03;
const ICategoryM icmIMAGE_CDT           = 0x04;
const ICategoryM icmIMAGE_Y_LH          = 0x05;
const ICategoryM icmIMAGE_Y_HL          = 0x06;
const ICategoryM icmIMAGE_Y_HH          = 0x07;

// SOUND
const ICategoryM icmSTEREO              = 0x01;

// DISTANCE
//    direction
const ICategoryM icmFORWARD             = 0xA0;
const ICategoryM icmBACKWARD            = 0xA1;

// TEMPERATURE
//    Position
const ICategoryM icmONPARTS             = 0x81;
const ICategoryM icmBATTERY             = 0x82;
const ICategoryM icmENVIRONMENT         = 0x83;

// L:ACTION
const ICategoryM icmRESTRAINT           = 0x81;
const ICategoryM icmRESTART             = 0x82;


// ENERGY
const ICategoryM icmMAIN_BATTERY        = 0x84;

// TOUCH
//    touch
const ICategoryM icmHIT                 = 0x93;
const ICategoryM icmPAT                 = 0x94;
const ICategoryM icmTOUCH               = 0x95;

// L:SWITCH
const ICategoryM icmON                  = 0x91;
const ICategoryM icmOFF                 = 0x92;
const ICategoryM icmINTERACTION         = 0x93;
const ICategoryM icmFUNCTION1           = 0x94;
const ICategoryM icmFUNCTION2           = 0x95;
const ICategoryM icmFUNCTION3           = 0x96;
const ICategoryM icmFUNCTION4           = 0x97;
const ICategoryM icmFUNCTION5           = 0x98;
const ICategoryM icmFUNCTION6           = 0x99;
const ICategoryM icmFUNCTION7           = 0x9A;
const ICategoryM icmFUNCTION8           = 0x9B;
const ICategoryM icmFUNCTION9           = 0x9C;
const ICategoryM icmFUNCTION10          = 0x9D;
const ICategoryM icmFUNCTION11          = 0x9E;
const ICategoryM icmFUNCTION12          = 0x9F;

// L:MODE
const ICategoryM icmMODE1               = 0xA0;
const ICategoryM icmMODE2               = 0xA1;
const ICategoryM icmMODE3               = 0xA2;
const ICategoryM icmMODE4               = 0xA3;
const ICategoryM icmMODE5               = 0xA4;
const ICategoryM icmMODE6               = 0xA5;

/****************************************/
/* compatible information category S    */
/****************************************/
const ICategoryS icsUNKNOWN                 = 0x00;
const ICategoryS icsDEFAULT                 = 0x00;
const ICategoryS icsSENSOR_INFORMATION_MASK = 0x80;
const ICategoryS icsDONT_CARE               = 0xFF;

/* Audio Information */
const ICategoryS icsOccAudioData            = 0x01;

/* Image Information */
const ICategoryS icsOccImageData            = 0x02;

/* Sensor Event */
const ICategoryS icsEVENT_NULL                 = 0x03;
const ICategoryS icsEVENT_FORCE                = 0x04;
const ICategoryS icsEVENT_TIME                 = 0x05;

/* Sensor Information */
const ICategoryS icsSENSOR_BatteryEnergy       = 0x81;
const ICategoryS icsSENSOR_StandardBatteryRest = 0x82;
const ICategoryS icsSENSOR_StandardDistance    = 0x83;
const ICategoryS icsSENSOR_StandardTemperature = 0x84;


#endif    // _InformationCategory_h_DEFINED
