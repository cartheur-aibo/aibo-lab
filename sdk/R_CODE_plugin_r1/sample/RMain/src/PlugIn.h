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

#ifndef _PlugIn_h_DEFINED
#define _PlugIn_h_DEFINED

struct PlugIn
{
    char        name[32];   /* PlugIn name */
    char        cfg[32];    /* *.cfg name */
    SubjectID   sbjID;      /* SubjectID of PlugIn.Message */
    ObserverID  obsID;      /* ObserverID of PlugIn.Command */
    int         var_n;      /* The number of variables which PlugIn provides */
    char**      var;        /* The layout of the variable name */
    int*        val;        /* The array of the variable values */
    int         cmd_n;      /* The number of commands which PlugIn provides */
    char**      cmd;        /* The array of the command pattern */
    short*      len;        /* The array of character string length for identification */

    longword    notify;     /* NOTIFY type */

    PlugIn():
        var_n(0),var(NULL),val(NULL),cmd_n(0),cmd(NULL),len(NULL),notify(0)
        {
            strcpy(name, "");
            strcpy(cfg,  "");
        };
};

#define NOTIFY_INIT (1<<0)  /* Inform at the time of INIT. */
#define NOTIFY_STOP (1<<1)  /* Inform at the time of STOP. */
#define NOTIFY_QUIT (1<<2)  /* Inform at the time of QIUT. */
#define NOTIFY_VAR  (1<<3)  /* Inform at the time of SET: <variable> */

#define PLUGIN_INI 0        /* Initial registration */
#define PLUGIN_MSG 1        /* String */
#define PLUGIN_VAR 2        /* Variable renewal */

#define MSG_STRING 0        /* String */
#define MSG_R_CODE 1        /* R-Code command string */

typedef struct              /* A substitute for union
                               (Because SubjectID/ObserverID are classes, union isn't defined.) */
{
    short       type;       /* PLUGIN_INI or _MSG _VAR */
    short       mode;       /* dummy */
    char        msg[128];   /* dummy */
}
PlugInData;

typedef struct              /* A structure for the message transmission */
{
    short       type;       /* PLUGIN_MSG */
    short       mode;       /* MSG_STRING/MSG_R_CODE */
    char        msg[128];   /* String */
}
PlugInMsgData;

typedef struct              /* A structure for the initial registration */
{
    short       type;       /* PLUGIN_INI */
    char        name[32];   /* PlugIn name */
    SubjectID   sbjID;      /* SubjectID of PlugIn.Message */
    ObserverID  obsID;      /* ObserverID of PlugIn.Command */
    longword    notify;     /* NOTIFY type */
}
PlugInIniData;

typedef struct              /* A structure for the variable value renewal */
{
    short       type;       /* PLUGIN_VAR */
    short       n;          /* The number of the variables */
    short       index[30];  /* The index of the variables (The turn that it appears in *.cfg) */
    short       value[30];  /* Variable values */
}
PlugInVarData;

#endif // _PlugIn_h_DEFINED
