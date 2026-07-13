//
// CommandCategoryTypes.h
//
// Copyright 2000 Sony Corporation
//

#ifndef _CommandCategoryTypes_h_DEFINED
#define _CommandCategoryTypes_h_DEFINED

#include <Types.h>

typedef    byte    CCategoryL;        // Command Category Large
typedef    byte    CCategoryM;        // Command Category Middle
typedef    byte    CCategoryS;        // Command Category Small


/* this header file contains compatible command category only */

/* category L */
/* special command group */
const CCategoryL cclSYSTEM              = 0x00;
const CCategoryL cclSOFTMESSAGE         = 0x01;

/* basic command group */
const CCategoryL cclMOVE                = 0x08;
const CCategoryL cclSEARCH              = 0x09;
const CCategoryL cclFOLLOW              = 0x0a;
const CCategoryL cclCHANGEPOSTURE       = 0x0b;
const CCategoryL cclFALLDOWN            = 0x0c;

/* play command group */
const CCategoryL cclCONTACT             = 0x10;
const CCategoryL cclPLAYHUMAN           = 0x11;
const CCategoryL cclPLAYROBOT           = 0x12;
const CCategoryL cclPLAYANIMAL          = 0x13;     // NOT Implemented for 210
const CCategoryL cclPLAYOBJECT          = 0x14;
const CCategoryL cclPLAYALONE           = 0x15;

/* reflection & physiological command group */
const CCategoryL cclTACTILEACTION       = 0x18;
const CCategoryL cclREFLEXACTION        = 0x19;
const CCategoryL cclPHYSIOLOGICAL       = 0x1a;

/* emotion command group */
const CCategoryL cclEMOTIONDISPLAY      = 0x20;
const CCategoryL cclFEELINGDISPLAY      = 0x21;

/* intention command group */
const CCategoryL cclINTENTION           = 0x28;
const CCategoryL cclREQUEST             = 0x29;
const CCategoryL cclWARNING             = 0x2a;

/* fighting command group */
const CCategoryL cclGUARDPAL            = 0x30;
const CCategoryL cclGUARDAREA           = 0x31;     // NOT Implemented for 210
const CCategoryL cclGURADOBJECT         = 0x32;     // NOT Implemented for 210
const CCategoryL cclOFFENSIVE           = 0x33;

/* reproduction command group */
const CCategoryL cclNESTING             = 0x38;     // NOT Implemented for 210
const CCategoryL cclMATING              = 0x39;     // NOT Implemented for 210
const CCategoryL cclRAISE               = 0x3a;     // NOT Implemented for 210


const CCategoryL cclINVALID             = 0xff;


/* category M */
/* for CCategoryL cclSYSTEM              = 0x00 */
const CCategoryM ccmNORMALSTOP          = 0x00;
const CCategoryM ccmACTGAINON           = 0x02;
const CCategoryM ccmACTGAINOFF          = 0x03;
const CCategoryM ccmAIRTARGET           = 0x04;

/* for CCategoryL cclSOFTMESSAGE         = 0x01 */
const CCategoryM ccmMODE                = 0x00;
const CCategoryM ccmSOFTWARNING         = 0x01;
const CCategoryM ccmNOTICE              = 0x02;
const CCategoryM ccmERROR               = 0x03;


/* for CCategoryL cclMOVE                = 0x08 */
const CCategoryM ccmHEAD                = 0x00;
const CCategoryM ccmTURN                = 0x01;
const CCategoryM ccmMOVE                = 0x02;
const CCategoryM ccmSTOP                = 0x03;

/* for CCategoryL cclCHANGEPOSTURE       = 0x0b */
const CCategoryM ccmSTANDUP             = 0x00;
const CCategoryM ccmSITDOWN             = 0x01;
const CCategoryM ccmLIEDOWN             = 0x02;
const CCategoryM ccmSTATION             = 0x03;
const CCategoryM ccmSTATIONTRANSFER     = 0x04;
const CCategoryM ccmCHARGEPOSTURE       = 0x05;
const CCategoryM ccmGUARDPOSTURE        = 0x06;
const CCategoryM ccmMOVEHEAD            = 0x07;
const CCategoryM ccmWALK                = 0x08;

/* for CCategoryL cclFALLDOWN            = 0x0c */
const CCategoryM ccmFALLDOWNSTRUGGLE    = 0x00;
const CCategoryM ccmAFTERSTANDUP        = 0x10;

/* for CCategoryL cclCONTACT             = 0x10 */
const CCategoryM ccmRIGHT               = 0x00;
const CCategoryM ccmFRONTRIGHT          = 0x01;
const CCategoryM ccmFRONT               = 0x02;
const CCategoryM ccmFRONTLEFT           = 0x03;
const CCategoryM ccmLEFT                = 0x04;
const CCategoryM ccmRIGHTLOW            = 0x10;
const CCategoryM ccmFRONTRIGHTLOW       = 0x11;
const CCategoryM ccmFRONTLOW            = 0x12;
const CCategoryM ccmFRONTLEFTLOW        = 0x13;
const CCategoryM ccmLEFTLOW             = 0x14;
const CCategoryM ccmRIGHTHIGH           = 0x20;
const CCategoryM ccmFRONTRIGHTHIGH      = 0x21;
const CCategoryM ccmFRONTHIGH           = 0x22;
const CCategoryM ccmFRONTLEFTHIGH       = 0x23;
const CCategoryM ccmLEFTHIGH            = 0x24;
const CCategoryM ccmFARRIGHT            = 0x30;
const CCategoryM ccmFARFRONTRIGHT       = 0x31;
const CCategoryM ccmFARFRONT            = 0x32;
const CCategoryM ccmFARFRONTLEFT        = 0x33;
const CCategoryM ccmFARLEFT             = 0x34;
const CCategoryM ccmFARRIGHTLOW         = 0x40;
const CCategoryM ccmFARFRONTRIGHTLOW    = 0x41;
const CCategoryM ccmFARFRONTLOW         = 0x42;
const CCategoryM ccmFARFRONTLEFTLOW     = 0x43;
const CCategoryM ccmFARLEFTLOW          = 0x44;
const CCategoryM ccmFARRIGHTHIGH        = 0x50;
const CCategoryM ccmFARFRONTRIGHTHIGH   = 0x51;
const CCategoryM ccmFARFRONTHIGH        = 0x52;
const CCategoryM ccmFARFRONTLEFTHIGH    = 0x53;
const CCategoryM ccmFARLEFTHIGH         = 0x54;

/* for CCategoryL cclPLAYHUMAN           = 0x11 */
const CCategoryM ccmGREET               = 0x00;
const CCategoryM ccmTALK                = 0x01;
const CCategoryM ccmHEAR                = 0x02;
const CCategoryM ccmPLAY                = 0x03;
const CCategoryM ccmTRICK               = 0x04;
const CCategoryM ccmCHARM               = 0x05;
const CCategoryM ccmAPPEAL              = 0x10;
const CCategoryM ccmCOWER               = 0x12;
const CCategoryM ccmFAWN                = 0x13;

/* for CCategoryL cclPLAYROBOT           = 0x12 */
const CCategoryM ccmDOMINANCE           = 0x10;
const CCategoryM ccmSUBMIT              = 0x11;

/* for CCategoryL cclPLAYOBJECT          = 0x14 */
const CCategoryM ccmINTEREST            = 0x00;
const CCategoryM ccmSUSPECT             = 0x01;
const CCategoryM ccmTOUCH               = 0x03;

/* for CCategoryL cclPLAYALONE           = 0x15 */
const CCategoryM ccmEXERCISE            = 0x00;
const CCategoryM ccmGROOM               = 0x01;
const CCategoryM ccmIDLEACTION          = 0x02;
const CCategoryM ccmDANCE               = 0x03;
const CCategoryM ccmSING                = 0x04;
const CCategoryM ccmDANCETOMUSIC        = 0x05;
const CCategoryM ccmAUTOMATIC           = 0x06;
const CCategoryM ccmSPEAKONESELF        = 0x07;

/* for CCategoryL cclTACTILEACTION       = 0x18 */
const CCategoryM ccmPLEASANT            = 0x00;
const CCategoryM ccmTICKLISH            = 0x01;
const CCategoryM ccmHATE                = 0x02;
const CCategoryM ccmPRESSINTERACT       = 0x03;

/* for CCategoryL cclREFLEXACTION        = 0x19 */
const CCategoryM ccmACHE                = 0x10;
const CCategoryM ccmNOISY               = 0x04;
const CCategoryM ccmPUTBACK             = 0x05;

/* for CCategoryL cclPHYSIOLOGICAL       = 0x1a */
const CCategoryM ccmEXCRETE             = 0x00;
const CCategoryM ccmSLEEPY              = 0x01;
const CCategoryM ccmNAP                 = 0x03;
const CCategoryM ccmWAKEUP              = 0x04;
const CCategoryM ccmHUNGRY              = 0x05;
const CCategoryM ccmBREATH              = 0x06;
const CCategoryM ccmHOT                 = 0x07;
const CCategoryM ccmCOLD                = 0x08;
const CCategoryM ccmITCH                = 0x09;

/* for CCategoryL cclEMOTIONDISPLAY      = 0x20 */
const CCategoryM ccmJOY                 = 0x00;
const CCategoryM ccmSAD                 = 0x01;
const CCategoryM ccmANGRY               = 0x02;
const CCategoryM ccmSURPRISE            = 0x03;
const CCategoryM ccmDISGUST             = 0x04;
const CCategoryM ccmFEAR                = 0x05;

/* for CCategoryL cclFEELINGDISPLAY      = 0x21 */
const CCategoryM ccmCHEERFUL            = 0x00;
const CCategoryM ccmDISPLEASURE         = 0x01;
const CCategoryM ccmDEPRESSED           = 0x02;
const CCategoryM ccmEMBARRASSED         = 0x03;
const CCategoryM ccmABASHED             = 0x04;
const CCategoryM ccmLOOKAROUND          = 0x05;
const CCategoryM ccmWONDER              = 0x06;
const CCategoryM ccmBORE                = 0x07;
const CCategoryM ccmLOSEINTEREST        = 0x08;

/* for CCategoryL cclINTENTION           = 0x28 */
const CCategoryM ccmVICTORY             = 0x00;
const CCategoryM ccmDEFEAT              = 0x01;
const CCategoryM ccmYES                 = 0x02;
const CCategoryM ccmNO                  = 0x03;
const CCategoryM ccmREJECT              = 0x04;

/* for CCategoryL cclREQUEST             = 0x29 */
const CCategoryM ccmTEACHME             = 0x00;
const CCategoryM ccmPLAYME              = 0x01;

/* for CCategoryL cclWARNING             = 0x2a */
const CCategoryM ccmWARNING             = 0x00;

/* for CCategoryL cclGUARDPAL            = 0x30 */
const CCategoryM ccmCOVER               = 0x00;
const CCategoryM ccmTHREAT              = 0x01;
const CCategoryM ccmATTACK              = 0x02;

/* for CCategoryL cclOFFENSIVE           = 0x33 */
const CCategoryM ccmPROTECT             = 0x00;
const CCategoryM ccmPROVOKE             = 0x03;
const CCategoryM ccmESCAPE              = 0x04;

const CCategoryM ccmINVALID                = 0xff;


/* category S */
const CCategoryS ccsINVALID                = 0xff;
const CCategoryS ccsWILDCARD               = 0xff;

#endif    // _CommandCategoryTypes_h_DEFINED

