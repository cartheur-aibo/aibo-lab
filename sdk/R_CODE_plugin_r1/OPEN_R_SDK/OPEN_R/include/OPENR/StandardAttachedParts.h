//
// StandardAttachedParts.h
//
// Copyright 2000 Sony Corporation
//

#ifndef _StandardAttachedParts_h_DEFINED
#define _StandardAttachedParts_h_DEFINED

#include <OPENR/MWInformationTypes.h>

/* 4 legs creature's metaphor  */
const AttachedParts apINVALID       = 0x00;
const AttachedParts apUNKNOWN       = 0x01;
const AttachedParts apDONT_CARE     = 0x02;
const AttachedParts apBODY      = 0x20;
const AttachedParts apHEAD      = 0x40;
const AttachedParts apTAIL      = 0x80;
const AttachedParts apLEGS      = 0xA0;
const AttachedParts apALL       = 0xFF;

#endif  //  _StandardAttachedParts_h_DEFINED
