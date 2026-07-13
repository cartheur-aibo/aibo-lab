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

#ifndef _RCodeDefine_h_DEFINED
#define _RCodeDefine_h_DEFINED


const short TRACE_1        = 0x0001;
const short TRACE_2        = 0x0002;
const short TRACE_LINE     = 0x0007;
const short TRACE_CMD      = 0x000B;
const short TRACE_EVT      = 0x0012;
const short TRACE_SYS      = 0x0022;
const short TRACE_POW      = 0x0042;
const short TRACE_EV2      = 0x0082;
const short TRACE_SENSOR   = 0x0100;
const short TRACE_TARGET   = 0x0200;
const short TRACE_AUDIO    = 0x0400;
const short TRACE_ABLE     = 0x0800;
const short TRACE_PLUGIN   = 0x1000;
const short TRACE_SENDMW   = 0x2000;
const short TRACE_SEM      = 0x4000;
const short TRACE_SEMALL   = 0x8000;


const short SENS_VALIDSUB  = 0x0001;
const short SENS_SUBINFO1  = 0x1000;
const short SENS_SUBINFO2  = 0x2000;
const short SENS_SUBINFO3  = 0x4000;
const short SENS_SUBINFO4  = 0x8000;
const short SENS_SUB_MASK  = 0xF000;

const short MAX_OF_SUBINFO = 4;
const short INDEX_SUBINFO1 = 0;
const short INDEX_SUBINFO2 = 1;
const short INDEX_SUBINFO3 = 2;
const short INDEX_SUBINFO4 = 3;


const short ARG_NONE     =  0;
const short ARG_VALUE    =  1;
const short ARG_CONST    =  2;
const short ARG_STRING   =  3;
const short ARG_ACTION   =  4;
const short ARG_APPTAG   =  5; /* String */
const short ARG_DIMVAR   =  6; /* A variable for array */
const short ARG_UNDEF    =  7; /* Undefine */
const short ARG_SYSVAR   =  8; /* System variable */
const short ARG_USRVAR   =  9; /* User variable */
const short ARG_LABEL    = 10; /* Label */
const short ARG_CONSTS   = 11;
const short ARG_LOWORD   = 12;
const short ARG_HIWORD   = 13;
const short ARG_EXTVAR   = 14; /* PlugIn variable */
const short ARG_ARRAY    = 15;
const short ARG_EXPR     = 16;


/*
 * The first word of the instruction
 */

// Command
const short mNONE     = 0;
const short mPLAY     = 1;
const short mWAIT     = 2;
const short mSTOP     = 3;
const short mQUIT     = 4;
const short mSYNC     = 5;
const short mDUMP     = 6;
const short mTEST     = 7;
const short mSEND     = 8;

// Statement
const short mGO       = 10;
const short mIF       = 11;
const short mTHEN     = 12;
const short mELSE     = 13;
const short mENDIF    = 14;

const short mSET      = 16;
const short mGET      = 17;
const short mLET      = 18;
const short mCLR      = 19;
const short mADD      = 20;
const short mSUB      = 21;
const short mMUL      = 22;
const short mDIV      = 23;
const short mMOD      = 24;
const short mAND      = 25;
const short mIOR      = 26;
const short mXOR      = 27;
const short mNOT      = 28;
const short mRND      = 29;

const short mDIM      = 30;
const short mDGET     = 31;
const short mDSET     = 32;
const short mDSAVE    = 33;
const short mDLOAD    = 34;
const short mDFIND    = 35;

const short mCALL     = 40;
const short mRETURN   = 41;
const short mRET      = 42;
const short mONCALL   = 43;
const short mRESUME   = 44;

const short mSWITCH   = 50;
const short mCSET     = 51;
const short mCASE     = 52;
const short mIF_1ST   = 53;
const short mIF_AND   = 54;
const short mIF_OR    = 55;

const short mJT       = 56;
const short mJF       = 57;

const short mGLOBAL   = 60;
const short mLOCAL    = 61;
const short mARRAY    = 63;
const short mSCOPEIN  = 64;
const short mSCOPEOUT = 65;
const short mPUSH     = 66;
const short mPOP      = 67;
const short mARG      = 68;
const short mDUP      = 69;

const short mEQ       = 70;
const short mNE       = 71;
const short mLT       = 72;
const short mLE       = 73;
const short mGT       = 74;
const short mGE       = 75;
const short mLAND     = 76;
const short mLIOR     = 77;
const short mLNOT     = 78;

// MemoryStick
const short mVSAVE    = 80;
const short mVLOAD    = 81;
const short mVDUMP    = 82;

// Control
const short mINIT     = 90;
const short mEDIT     = 91;
const short mEND      = 92;
const short mRUN      = 93;
const short mBP       = 94;
const short mSTEP     = 95;
const short mEXIT     = 96;
const short mLOAD     = 97;
const short mSAVE     = 98;
const short mHALT     = 99;

// Debug
const short mPRINT    = 100;
const short mDEBUG    = 101;

// Structuralization
const short mDO       = 110;
const short mLOOP     = 111;
const short mWHILE    = 112;
const short mUNTIL    = 113;
const short mFOR      = 114;
const short mNEXT     = 115;
const short mREPEAT   = 116;
const short mWEND     = 117;
const short mBREAK    = 118;

// Sensor update
const short mFETCH    = 120;

// Pose/Motion
const short mDEFP     = 123;
const short mDEFM     = 124;


/*
 * The second word of the instruction
 */

// Command
const short tNONE     = 0;     // None
const short tACTION   = 1<<0;  // ACTION
const short tACTIONX  = 1<<1;  // ACTIONX
const short tMWCID    = 1<<2;  // MWCID
const short tCOMMAND  = 1<<3;  // Command
const short tMTN      = 1<<4;  // RReplay
const short tSND      = 1<<5;  // RReplay
const short tLED      = 1<<6;  // RReplay
const short tIMAGE    = 1<<7;
const short tSOUND    = 1<<5;  // same as tSND
const short tAPPID    = 1<<8;  // APPID
const short tAPPTAG   = 1<<9;  // APPTAG
const short tLMS      = 1<<10; // LMS
const short tLMSID    = 1<<11; // LMSID

const short tANY      = 0xFF;  // any

// Statement
const short OpEQ      = mEQ;    // =
const short OpNE      = mNE;    // <>
const short OpLT      = mLT;    // <
const short OpLE      = mLE;    // <=  and =<
const short OpGT      = mGT;    // >
const short OpGE      = mGE;    // >=  and =>
const short OpAND     = mAND;   // &
const short OpIOR     = mIOR;   // |
const short OpXOR     = mXOR;   // ^
const short OpLAND    = mLAND;  // &&
const short OpLIOR    = mLIOR;  // ||


ConstTable const_table[] =
{
    { "NONE",       mNONE      },
    { "PLAY",       mPLAY      },
    { "WAIT",       mWAIT      },
    { "STOP",       mSTOP      },
    { "QUIT",       mQUIT      },
    { "SYNC",       mSYNC      },
    { "DUMP",       mDUMP      },
    { "TEST",       mTEST      },
    { "SEND",       mSEND      },

    { "EDIT",       mEDIT      },
    { "END",        mEND       },
    { "RUN",        mRUN       },
    { "SAVE",       mSAVE      },
    { "LOAD",       mLOAD      },
    { "INIT",       mINIT      },
    { "BP",         mBP        },
    { "STEP",       mSTEP      },
    { "HALT",       mHALT      },

    { "VSAVE",      mVSAVE     },
    { "VLOAD",      mVLOAD     },
    { "VDUMP",      mVDUMP     },

    { "IF",         mIF        },
    { "GO",         mGO        },
    { "SET",        mSET       },
    { "GET",        mGET       },
    { "LET",        mLET       },
    { "CLR",        mCLR       },
    { "ADD",        mADD       },
    { "SUB",        mSUB       },
    { "MUL",        mMUL       },
    { "DIV",        mDIV       },
    { "MOD",        mMOD       },
    { "AND",        mAND       },
    { "IOR",        mIOR       },
    { "XOR",        mXOR       },
    { "NOT",        mNOT       },
    { "RND",        mRND       },

    { "DIM",        mDIM       },
    { "DGET",       mDGET      },
    { "DSET",       mDSET      },
    { "DSAVE",      mDSAVE     },
    { "DLOAD",      mDLOAD     },
    { "DFIND",      mDFIND     },

    { "CALL",       mCALL      },
    { "RETURN",     mRETURN    },
    { "RET",        mRET       },
    { "ONCALL",     mONCALL    },
    { "RESUME",     mRESUME    },
    { "EXIT",       mEXIT      },

    { "SWITCH",     mSWITCH    },
    { "CSET",       mCSET      },
    { "CASE",       mCASE      },
    { "IF_1ST",     mIF_1ST    },
    { "IF_AND",     mIF_AND    },
    { "IF_OR",      mIF_OR     },
    { "THEN",       mTHEN      },
    { "ELSE",       mELSE      },
    { "ENDIF",      mENDIF     },

    { "JT",         mJT        },
    { "JF",         mJF        },

    { "GLOBAL",     mGLOBAL    },
    { "LOCAL",      mLOCAL     },
    { "ARRAY",      mARRAY     },
    { "SCOPEIN",    mSCOPEIN   },
    { "SCOPEOUT",   mSCOPEOUT  },
    { "PUSH",       mPUSH      },
    { "POP",        mPOP       },
    { "ARG",        mARG       },
    { "DUP",        mDUP       },

    { "EQ",         mEQ        },
    { "NE",         mNE        },
    { "LT",         mLT        },
    { "LE",         mLE        },
    { "GT",         mGT        },
    { "GE",         mGE        },
    { "LAND",       mLAND      },
    { "LIOR",       mLIOR      },
    { "LNOT",       mLNOT      },

    { "ACTION",     tACTION    },
    { "ACTION+",    tACTION    },
    { "ACTIONX",    tACTIONX   },
    { "MWCID",      tMWCID     },
    { "COMMAND",    tCOMMAND   },
    { "LMS",        tLMS       },
    { "LMSID",      tLMSID     },
    { "MTN",        tMTN       },
    { "LED",        tLED       },
    { "SND",        tSND       },
    { "IMAGE",      tIMAGE     },
    { "SOUND",      tSOUND     },
    { "APPID",      tAPPID     },
    { "APPTAG",     tAPPTAG    },

    { "=",          OpEQ       },
    { "==",         OpEQ       },
    { "<>",         OpNE       },
    { "!=",         OpNE       },
    { "<",          OpLT       },
    { "<=",         OpLE       },
    { "=<",         OpLE       },
    { ">",          OpGT       },
    { ">=",         OpGE       },
    { "=>",         OpGE       },
    { "&",          OpAND      },
    { "|",          OpIOR      },
    { "^",          OpXOR      },
    { "&&",         OpLAND     },
    { "||",         OpLIOR     },

    { "PRINT",      mPRINT     },
    { "DEBUG",      mDEBUG     },

    { "DO",         mDO        },
    { "LOOP",       mLOOP      },
    { "WHILE",      mWHILE     },
    { "UNTIL",      mUNTIL     },
    { "FOR",        mFOR       },
    { "NEXT",       mNEXT      },
    { "REPEAT",     mREPEAT    },
    { "WEND",       mWEND      },
    { "BREAK",      mBREAK     },

    { "FETCH",      mFETCH     },

    { NULL,         0          }  // end of const_table
};

#endif // _RCodeDefine_h_DEFINED

