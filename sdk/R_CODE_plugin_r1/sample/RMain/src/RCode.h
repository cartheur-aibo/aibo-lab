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

#ifndef _R_Code_h_DEFINED
#define _R_Code_h_DEFINED

#include "RMainDefine.h"

/******************************************************************************
 *  The structures used with R-Code
 *
 *      RStatement      First of all, the R-CODE character string (statement)
 *                      is analyzed by this structure.
 *
 *                      The kind of the specified parameter (value, constant,
 *                      variable, character string) is remembered, then it is
 *                      evaluated during execution, and it becomes the basic
 *                      information that makes RMessage.
 *
 *                      The R-Code script is stored in the statement member
 *                      variable of RMain, which is an array of this structure.
 *
 *                      A constant is developed at this level.
 *                      A variable is stored as a pointer to RVariable.
 *                      A string is registered in dictionary, and Offset
 *                      for that string buffer, string_buf, is stored in
 *                      RStatement.
 *
 *      RMessage        RMessage is derived from RStatement.  It is used to
 *                      to issue instructions to AIBO.
 *
 *                      The member variables of this structure contain the
 *                      values from the character string.
 *
 *                      It is used as an instruction message to RSensor
 *                      and as the instruction in the RMain message.
 *
 *      RSensorData     The group of Sensor data that is sent by RSensor.
 *
 *      RWord           The character string of the dictionary
 *                      (RMain.dictionary) is stored in a string buffer
 *                      (RMain.string_buf). (to save memory)
 *
 ******************************************************************************/

#define MKLONG(lo,hi)  ((ulong)(((uword)(lo))|((ulong)((uword)(hi)))<<16))
#define LOWORD(l)      ((uword)(l))
#define HIWORD(l)      ((uword)(((ulong)(l)>>16)&0xFFFF))


struct RWord
{
    char* word; // Strings     Pointer to RMain.str_buf
    short type; // Pattern information
    int   data; // ARG_NONE     0
                // ARG_VALUE    Integral value
                // ARG_CONST    Fixed numerical value
                // ARG_STRING   (Reserved)
                // ARG_UNDEF    undef
                // ARG_SYSVAR   stack index of sys_var
                // ARG_USRVAR   stack index of usr_var
                //    (Unused in the case of a local variable: index of Word is used as id.)
                // ARG_LABEL    index of statement

    RWord():word(NULL),type(0),data(0){};

};

union RVariable
{
    struct
    {
        int *value;
    }
    sys;

    struct
    {
        short  id;
        int  value;
    }
    usr;
};

struct RDimVariable
{
    short    id;
    short    index;
    int      value;
};

struct RStatement
{
    short   line_num;       // line number
    short   context;        // context
    short   plugin_info;    // PlugIn info

    int op1;            // The R-Code statement takes the following form:
    int op2;            //

    int arg0;           //   <op1>:<op2>:<arg0>:<arg1>:<arg2>:<arg3>:<arg4>
    int arg1;           //
    int arg2;           //
    int arg3;           //
    int arg4;           //

    struct
    {
        unsigned op1  : 4;  // op1  pattern information
        unsigned op2  : 4;  // op2  pattern information
        unsigned arg0 : 4;  // arg0 pattern information
        unsigned arg1 : 4;  // arg1 pattern information
        unsigned arg2 : 4;  // arg2 pattern information
        unsigned arg3 : 4;  // arg3 pattern information
        unsigned arg4 : 4;  // arg4 pattern information
        unsigned wait : 1;  // wait flag
        unsigned bp   : 1;  // BreakPoint flag
        unsigned ct   : 1;  // context flag
        unsigned ce   : 1;  // CASE:ELSE flag
    }
    type;
};

struct RMessage
{
    ushort  mode;           // op1 of RStatement
    ushort  type;           // op2 of RStatement

    int arg0;
    int arg1;
    int arg2;
    int arg3;
    int arg4;

    char    name[32+1];

    RMessage():mode(0),type(0),arg0(0),arg1(0),arg2(0),arg3(0),arg4(0){strcpy(name,"");};
};

struct RSnapInfo
{
    short pc;
    short ct;
    short ip;
    short sp;
    short lb;
    short lv;
};

struct RSensorData
{
    short   dummy;
};

struct ConstTable
{
    char *name;
    short value;
};


#endif // _R_Code_h_DEFINED
