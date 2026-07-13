//
// StandardPosture.h
//
// Copyright 2000 Sony Corporation
//

#ifndef _StandardPosture_h_DEFINED_
#define _StandardPosture_h_DEFINED_

#include <OPENR/OPENR.h>
#include <OPENR/MWCommandTypes.h>

//
// Standard Postures Mask
//
const MWCPosture mwcSTANDARD_MASK     = 0xFFFF0000;

//
// Standard Postures
//
const MWCPosture mwcSTANDING          = 0x00010000;
const MWCPosture mwcSITTING           = 0x00020000;
const MWCPosture mwcSLEEPING          = 0x00030000;
const MWCPosture mwcWALKING           = 0x00040000;
const MWCPosture mwcSTATION           = 0x00050000;
const MWCPosture mwcPSTATION          = 0x00060000;
const MWCPosture mwcLIFT              = 0x00070000;
const MWCPosture mwcNEUTRAL           = 0x01000000;

//
// Fall down Postures
//
const MWCPosture mwcFALLDOWN_HEAD     = 0x10010000;
const MWCPosture mwcFALLDOWN_LEFT     = 0x10020000;
const MWCPosture mwcFALLDOWN_RIGHT    = 0x10030000;
const MWCPosture mwcFALLDOWN_TAIL     = 0x10040000;
const MWCPosture mwcFALLDOWN_BACK     = 0x10050000;

const MWCPosture mwcFALLDOWN_NT       = 0x11000000;

//
// Unknown Posture
//
const MWCPosture mwcUNKNOWN_POSTURE   = 0xFF000000;

//
// Temporary Posture
//
const MWCPosture mwcTEMPORARY_POSTURE = 0x00FF0000;

//
// Undefined Posture
//
const MWCPosture mwcUNDEFINE_POSTURE  = 0xFFFFFFFF;

#endif  /* _StandardPosture_h_DEFINED_ */
