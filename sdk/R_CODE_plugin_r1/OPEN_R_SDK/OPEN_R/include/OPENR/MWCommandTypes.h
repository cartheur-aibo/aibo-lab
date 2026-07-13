//
// MWCommandTypes.h
//
// Copyright 2000 Sony Corporation
//

#ifndef _MWCommandTypes_h_DEFINED_
#define _MWCommandTypes_h_DEFINED_

#include <Types.h>

//
// OccMWCommandData
//
typedef longword MWCSerial;
const MWCSerial   mwcUNDEFINE_SERIAL   = 0xFFFFFFFF;

typedef longword MWCID;

const MWCID      mwcNORMAL_STOP       = 0xFFFFFFFE; 
const MWCID      mwcUNDEFINE_ID       = 0xFFFF0000;

//
// OccMWCommandResultData
//
typedef longword MWCRType;
typedef longword MWCStatus;
typedef longword MWCResource;
typedef longword MWCPosture;
typedef longword MWCEffect;

const MWCRType   mwcFIRST             = 0x00000001;
const MWCRType   mwcSECOND            = 0x00000002;
const MWCRType   mwcLAST              = 0x00000003;
const MWCRType   mwcUNDEFINE_TYPE     = 0xFFFFFFFF;

const MWCStatus  mwcINVALID           = 0x00000001;
const MWCStatus  mwcACCEPTED          = 0x00000002;
const MWCStatus  mwcTOBEFINISHED      = 0x00000100;
const MWCStatus  mwcCONTINUOUS        = 0x00000200;
const MWCStatus  mwcCOMPLETE          = 0x00010000;
const MWCStatus  mwcINCOMPLETE        = 0x00020000;
const MWCStatus  mwcUNDEFINE_STATUS   = 0xFFFFFFFF;

const MWCEffect  mwcFAIL              = 0x00000000;
const MWCEffect  mwcSUCCESS           = 0x00000001;
const MWCEffect  mwcUNKNOWN_EFFECT    = 0xFFFFFFFF;

const MWCResource mwcMOTION_RESOURCE  = 0x0000FFFF;
const MWCResource mwcSOUND_RESOURCE   = 0x00FF0000;
const MWCResource mwcSPECIAL_RESOURCE = 0xFF000000;
const MWCResource mwcNONE             = 0x00000000;

#endif  /* #ifndef _MWCommandTypes_h_DEFINED_ */

