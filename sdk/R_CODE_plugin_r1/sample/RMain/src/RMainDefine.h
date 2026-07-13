//
// Copyright 2006 Sony Corporation
//
// Permission to use, copy, modify, and redistribute this software for
// non-commercial use is hereby granted.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//


#ifndef _RMainDefine_h_DEFINED
#define _RMainDefine_h_DEFINED

#include <APP/OcaInputSemantics.h>
#include <APP/OcaCCCommand.h>

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned long  ulong;
typedef unsigned short uword;


// for periodic time request
static const char*     PERIODIC_TIME_FILE_PATH = "/MS/PTIME";   // for debug
static const longword  PERIODIC_TIME_INTERVAL  = 500;           // msec
static const longword  CLOCK_COUNT_VALUE       = 16;            // 500/32 = 15.63

static const char*  AMS_FILE_PATH             = "/MS/OPEN-R/APP/PC/AMS/";
static const char*  AIBO_ID_FILE              = "/MS/OPEN-R/SYSTEM/DATA/P/Aibo-id";

static const char*  SEM_LABEL_FILE  = "SEMLABEL.BIN";
static const char*  SUB_LABEL_FILE  = "SUBLABEL.BIN";

static const char*  RCODE_SENSOR_FILE     = "SENSOR.CFG";
static const char*  RCODE_APPTAG_FILE     = "APPTAG.BIN";
static const char*  RCODE_LMSCOMMAND_FILE = "LMSCOM.CFG";
static const char*  RCODE_SYSTEM_FILE     = "SYSTEM.CFG";
static const char*  RCODE_JOINT_FILE      = "JOINT.CFG";
static const char*  RCODE_PLUGIN_FILE     = "PLUGIN.CFG";
static const char*  RCODE_ACTION_AMS_FILE = "ACTION.AMS";
static const char*  RCODE_ACTION_CFG_FILE = "ACTION.CFG";
static const char*  RCODE_ACTION_EXT_FILE = "ACTION.EXT";
static const char*  RCODE_USER_VAR_FILE   = "UsrVar.CFG";
static const char*  RCODE_DICTIONARY_FILE = "R-CODE.dic";
static const char*  RCODE_RREPLAY_FILE    = "RReplay.cfg";

static const char*  RCODE_CNTVERTION_FILE = "/MS/OPEN-R/APP/CONF/CNTVER.OIF";

static const char*  RCODE = "/MS/OPEN-R/APP/PC/AMS/R-CODE.R";

static const char*  ERROR_LOG = "/MS/OPEN-R/APP/PC/AMS/ERROR.log";
static const char*  EVENT_LOG = "/MS/OPEN-R/APP/PC/AMS/EVENT.log";

static const word   MAX_PATH_LEN         = 256;
static const word   MAX_STRING_BUFF      = 256;

static const short  SYSTEM_STACK = 5;

static const ISSemanticsID SEM_RISE_POSE    = 0x00000C02;
static const CCCCommandID  CCCC_SHUTDOWN_ID = 0x00010003;
static const CCCCommandID  CCCC_JUMP_TO_NT  = 0xFFF00000;

static const ISSubInfo SPOS_ON         = 0x00000100;
static const ISSubInfo SPOS_STATION_ON = 0x00000101;
static const ISSubInfo SPOS_FALL_DOWN  = 0x00000104;
static const ISSubInfo SPOS_GET_UP     = 0x00000204;           
static const ISSubInfo EMERGENCY_MASK  = 0x0000ffff;
static const ISSubInfo MASK_BOOT_STAT  = 0x00000f00;
static const ISSubInfo BOOT_STATION    = 0x00000800;

// constant value
static const int    SEM_STATE_UNIT_SIZE = 16;

static const short  MAX_RESERVED_VARIABLE_NUM = 256;

static const char*  DEFAULT_NEWLINE = "\r\n";
static const char*  UNIX_NEWLINE    = "\n";
static const char*  MAC_NEWLINE     = "\r";

static const short  BATT_REST_MIN  = 20;  /* 20% Default value */
static const short  BATT_REST_INIT = 100; /* Battery Initialization value */

#endif // _RMainDefine_h_DEFINED

