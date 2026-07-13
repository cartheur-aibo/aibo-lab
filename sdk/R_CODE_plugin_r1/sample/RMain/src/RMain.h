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

#ifndef _RMain_h_DEFINED
#define _RMain_h_DEFINED

#include <OPENR/OPENR.h>
#include <OPENR/OPENRAPI.h>
#include <OSystem/OPENRAPI2.h>
#include <OPENR/OObject.h>
#include <OPENR/OSubject.h>
#include <OPENR/OObserverVector.h>
#include <OPENR/OList.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <OPENR/OccMWCommand.h>
#include <OPENR/OccDataFormats.h>
#include <OPENR/OccSysEvent.h>
#include <OPENR/OccPowerEvent.h>
#include <OPENR/OccTargetInfo.h>
#include <OPENR/SensorId.h>
#include <OPENR/StandardSensorStruct.h>

#include <APP/OcaCCCommand.h>
#include <APP/OcaMWCommand.h>
#include <APP/InputSemanticsList.h>
#include <APP/OcaTimeInformation.h>

#include "PlugIn.h"
#include "RCode.h"
#include "RMainDefine.h"
#include "def.h"


class CCCommandQueu {
private:
    OcaCCCommandData   m_ccc;
public:
    CCCommandQueu():m_ccc() {}
    CCCommandQueu( const ObserverID& observerID )
        { m_ccc.SetObserverID( observerID ); }
    CCCommandQueu( const CCCCommandID commandid, const ObserverID& observerID ) {
        m_ccc.SetCommandID( commandid ); m_ccc.SetObserverID( observerID );
        m_ccc.SetTarget( ccctMW );
        m_ccc.SetSelectFlag( cccsfCOMMAND_FIX );
        m_ccc.SetRawInfinityFlag( cccifONESHOT );
        m_ccc.SetArbitrationFlag( cccafCHANGE0 );
        m_ccc.SetAppID( issemINVALID );
        m_ccc.SetLevel( ccclANY );
    }
    OcaCCCommandData& GetCCCommandData()
        { return m_ccc; }
    CCCSerial GetSerial() { return m_ccc.GetSerial(); }
    CCCCommand GetCommand() { return m_ccc.GetCommand(); }
    CCCCommandID GetCommandID() { return m_ccc.GetCommandID(); }
    CCCInfinityFlag GetRawInfinityFlag() { return m_ccc.GetRawInfinityFlag(); }
    CCCArbitrationFlag GetArbitrationFlag() { return m_ccc.GetArbitrationFlag(); }

    OStatus SetTargetPos( const TargetPos& target_pos )
        { m_ccc.SetTargetPos( target_pos ); return oSUCCESS; }
    OStatus SetCommandID( const CCCCommandID& cccid )
        { m_ccc.SetCommandID( cccid ); return oSUCCESS; }
    OStatus SetTarget( const CCCTarget& target )
        { m_ccc.SetTarget( target ); return oSUCCESS; }
    OStatus SetArbitrationFlag( const CCCArbitrationFlag& arbitrationflag )
        { m_ccc.SetArbitrationFlag( arbitrationflag ); return oSUCCESS; }
    OStatus SetSubInfo( const APPCSubInfo& subinfo )
        { m_ccc.SetSubInfo( subinfo ); return oSUCCESS; }
    OStatus SetObserverID( const ObserverID& observerID )
        { m_ccc.SetObserverID( observerID ); return oSUCCESS; }
    OStatus SetSubjectID( const SubjectID& subjectid )
        { m_ccc.SetSubjectID( subjectid ); return oSUCCESS; }
    OStatus SetAppID( const AppID& appid )
        { m_ccc.SetAppID( appid ); return oSUCCESS; }
    OStatus SetSerial( const CCCSerial serial )
        { m_ccc.SetSerial( serial ); return oSUCCESS; }
    OStatus SetCategoryL(const CCategoryL categoryl)
        { m_ccc.SetCategoryL( categoryl ); return oSUCCESS ; }
    OStatus SetCategoryM(const CCategoryM categorym)
        { m_ccc.SetCategoryM( categorym ); return oSUCCESS ; }
    OStatus SetCategoryS(const CCategoryS categorys)
        { m_ccc.SetCategoryS( categorys ); return oSUCCESS ; }
    OStatus SetSelectFlag(const CCCSelectFlag selectflag)
        { m_ccc.SetSelectFlag( selectflag ); return oSUCCESS; }

};


//
// DB record
//
struct CommandData
{
    longword    m_cid;
    byte        m_exp;
    byte        m_catL;
    byte        m_catM;
    byte        m_catS;
    byte        m_stage;
    byte        m_loop;
    byte        m_independent;
    byte        m_level;
    byte        m_posCange;
    byte        pad1;
    byte        pad2;
    byte        pad3;
    longword    m_startPos;
    longword    m_endPos;
    longword    m_resource;
    SensorId    m_sensorId;
};

struct InformationData
{
    longword    m_iid;
    byte        m_exp;
    byte        m_catL;
    byte        m_catM;
    byte        m_catS;
    longword    m_min;
    longword    m_max;
    longword    m_resolution;
    longword    m_unit;
    byte        m_exp1;
    byte        m_type;
    byte        pad1;
    byte        pad2;
    byte        m_exp2;
    byte        pad3;
    byte        m_part;
    byte        m_number;
};

//
// RReplay
//
#define cMTN_INIT       1
#define cMTN_DATA       2
#define cLED_INIT       3
#define cLED_DATA       4

//==============================================================
//    RMain
//==============================================================
class RMain : public OObject
{

public:
    RMain();
    virtual ~RMain();

    //
    //  OObserver & OSubject
    //
    OSubject*        subject[numOfSubject];
    OObserverVector* observer[numOfObserver];

    //
    //  OPEN-R Method
    //
    virtual OStatus DoInit    ( const OSystemEvent& event );
    virtual OStatus DoStart   ( const OSystemEvent& event );
    virtual OStatus DoStop    ( const OSystemEvent& event );
    virtual OStatus DoDestroy ( const OSystemEvent& event );

    //
    // Inter Object Communication Method
    //
    void ReadyMessage            ( const OReadyEvent& event );
    void ReadyTimeRequest        ( const OReadyEvent& event );
    void ReadyPeriodicTimeRequest( const OReadyEvent& event );
    void ReadyCCCommand          ( const OReadyEvent& event );
    void PlugInCommand           ( const OReadyEvent& event );
    void MWSensor                ( const OReadyEvent& event );
    // for RReplay
    void Effector                ( const OReadyEvent& event );
    void DefReplay               ( const OReadyEvent& event );
    void MtnCommand              ( const OReadyEvent& event );

    void NotifyCommand           ( const ONotifyEvent& event );
    void NotifyComplete          ( const ONotifyEvent& event );
    void NotifyTimeReply         ( const ONotifyEvent& event );
    void NotifyPeriodicTimeReply ( const ONotifyEvent& event );
    void NotifyAbleInfo          ( const ONotifyEvent& event );
    void NotifyInputSemantics    ( const ONotifyEvent& event );
    void NotifyMWCommand         ( const ONotifyEvent& event );
    void PlugInMessage           ( const ONotifyEvent& event );

    void MtnMessage              ( const ONotifyEvent& event );
    void MWCommand2              ( const ONotifyEvent& event );

    // Legacy Support connection
    void NotifyImage             ( const ONotifyEvent& event );
    void NotifyTargetInfo        ( const ONotifyEvent& event );
    void NotifySensor            ( const ONotifyEvent& event );
    void NotifyEvent             ( const ONotifyEvent& event );

#ifdef _RCLOCK_MODE_
    void Clock                   ( const ONotifyEvent& event );
#endif // _RCLOCK_MODE_
    void Clock2                  ( const ONotifyEvent& event );


private:

    /* Connection flag */
    short m_sensorConnectionFlag;
    short m_imageConnectionFlag;
    short m_targetInfoConnectionFlag;
    short m_plugInMessageConnectionFlag;
    short m_plugInCommandConnectionFlag;
    short m_mwsensorConnectionFlag;
#ifdef _RCLOCK_MODE_
    short m_rclockConnectionFlag;
#endif // _RCLOCK_mode_

    // Flag
    bool m_state_periodic_time_request; // for periodic time request
    bool m_state_receive_ready;

/* m_wlanCard */
static const short UNMOUNT_WLANCARD   = 0;  /* A WLAN card isn't attached. */
static const short MOUNT_WLANCARD     = 1;  /* A WLAN card is attached.    */

/* m_wlanFlag */
static const short NOT_AVAILABLE_WLANFLAG   = 0;  /* A WLAN card isn't available. */
static const short AVAILABLE_WLANFLAG       = 1;  /* A WLAN card is available.    */

/* m_wlanWait */
static const short NO_WAIT_DATA_WLAN        = 0;  /* There is no data that is waiting for transmission. */
static const short WAITING_DATA_WLAN        = 1;  /* There is data that is waiting for transmission.    */
static const short EXECUTE_DATA_WLAN        = 2;  /* There is data that is waiting for transmission.    */


    int m_wlanCard;   /* WLAN card in AIBO is on:1 | off:0 */
    int m_wlanFlag;   /* WLAN card is available:1 | not available:0 */
    int m_wlanWait;   /* When there is data that is waiting for transmission, 1 | usually 0 */

    int m_msStatus;
    int m_noholdup;

    int m_aiboId;     /* Individual ID R-CODE variable AiboId */
    int m_myaiboId;
    int m_aiboType;   /* Model information R-CODE variable AiboType */
    int m_bootType;   /* Boot classification R-CODE variable BootType */

    struct Action           /* ACTION.CFG The data structure of the reading buffer */
    {
        char  name[32];     /* Action name */
        short null;         /* padding     */
        short type;         /* Argument classification */
        CCCCommandID cccid;
    };
    Action*     m_action;   /* Action list */
    int         m_action_n; /* Action number */

    struct JointVar         /* JOINT.CFG The data structure of the reading buffer */
    {
        char                name[32];   /* Joint name */
        short               null;       /* padding    */
        short               type;       /* Classification */
        OPrimitiveID        pid;        /* PrimitiveID */
        int                 value;      /* Variable area (The angle of the joint is stored.) */
        int                 index;      /* A joint Index to send data to RReplay             */
        unsigned long       mask;       /* Mask value (Bit flag) to send data to RReplay     */

        MemoryRegionID      cmdVecID;   /* A data buffer (MemoryRegionID) to send DirectMsg */
        OCommandVectorData* cmdVec;     /* A data buffer (CommandVector) to send DirectMsg  */
    };
    JointVar*       m_jointVar;         /* Joint list   */
    int             m_jointVar_n;       /* Joint number */

    enum LegacyFlag {
        LEGACY_UNKNOWN     = 0,      /* Initial value                  */
        LEGACY_SEM_CONFIG  = 1,      /* A receipt by the semantics     */
        LEGACY_CONFIG      = 2       /* Legacy config                  */
    };

    struct SensVar        /* SENSOR.CFG The data structure of the reading buffer */
    {
        char         name[32];     /* Sensor variable name              */
        short        null;         /* padding                           */
        short        type;         /* Classification */
        longword     semOrmwiid;   /* SEMANTICS or MWIID */
        int          value;        /* Variable area (Sensor value is stored.) */
        short        validSubInfo;
        ISSubInfo    subinfo;
        LegacyFlag   modeID;       /* 0:UNKNOWN | 1:SEMANTICS | 2:MWCID(Legacy) */
    };
    SensVar*        m_sensVar;    /* Sensor list */
    int             m_sensVar_n;  /* Sensor number */

    struct AppTagVar
    {
        char            name[32];
        short           null;
        APPCCommandID   appcomid;
        APPCSubInfo     subinfo;
        ISSemanticsID   sem_id;
    };
    AppTagVar*      m_apptag;
    int             m_apptag_n;

    struct LMSComVar
    {
        char            l_name[32];
        char            m_name[32];
        char            s_name[32];
        CCCCommandID    cccid;
    };
    LMSComVar*      m_lmscom;
    int             m_lmscom_n;


    enum InitFlag {
        RCODE_NOT_INIT    = 0,   /* Not initialized         */
        RCODE_BEING_INIT  = 1,   /* It is being initialized */
        RCODE_FINISH_INIT = 2    /* It has been initialized */
    };

    enum AutoExec {
        NOT_AUTO  = 0,   /* not auto      */
        RUN_AUTO  = 1,   /* Exec R-Code.R */
        WLAN_AUTO = 2    /* Header display when WLAN is connected */
    };

    enum ExecFlag {
        WAIT_EXEC   = 0, /* Wait command */
        RUNING_EXEC = 1, /* Running       */
        PAUSE_EXEC  = 2  /* Pause (Resume it with Clock.) */
    };

    short       m_pc; /* Program Counter (index of statement) */
    int         m_ct; /* Context  */
    short       m_ip; /* InterruptPointer  (index of on_call) */
    short       m_lb; /* LocalBase         (index of stack)   */

    InitFlag m_initFlag;  /* Initialization flag */
    AutoExec m_autoExec;  /* Autoexec flag       */
    ExecFlag m_execFlag;  /* Exec flag           */

    enum EditFlag {
        WAIT_SCRIPT    = 0, /* Default state */
        LOADING_SCRIPT = 1  /* During statement reading (EDIT-END) */
    };

    EditFlag m_editFlag;    /* A state of statement */

    short   m_execStop;
    int     m_syncFlag;   /* It is waiting synchronously by synchronous wait flat 1: SYNC command.
                             (restart by SYNC input) */
    short   m_sendFlag;   /* R-Code.R writing flag (with SEND-END 1)*/
    int     m_sendFp;

    int     m_trace;
    int     m_debug;

    short   m_waiting;    /* Is it in WAIT? (Value shows the part doing WAIT.) */
    int     m_wait;       /* WAIT number */
    ulong   m_waitPart;   /* The part flag of partial WAIT (wait_mask[] is set up) */

    short   m_waitName[32];  /* The part name  of partial WAIT (for the Trace message indication)    */
    ulong   m_waitMask[32];  /* The part flag list of partial WAIT (the [WAIT] block of R-CODE.DIC) */
    int     m_waitMask_n;    /* The number of part flags of partial WAIT */

    int         m_fdCount;
    int         m_sysEvent;    /* During the SysEvent processing, flag (0/1) */
    MWCSerial   m_sysSerial;   /* The serial number of the SysEvent final command */
                               /* (It has Complete of this command, and is considered the SysEvent end.) */
    int         m_sysStatus;   /* A state of return    0:usually Boot, 1:a return from SysEvent */
    int         m_sysEvent2Down; /* 1 when shutting down after SysEvent is processed */
    int         m_categoryL;   /* Last SysEvent Category L */
    int         m_categoryM;   /* Last SysEvent Category M */
    int         m_categoryS;   /* Last SysEvent Category S */
    int         m_spos;        /* The substitution of category L, M, and S / Last MWCommand */

    short  m_lineNum;   /* Line num */

    MWCSerial   m_waitSysSerial;
    int         m_waitSysCommand;

    static const short CONTINUOUS_DELIMITER      = 0;
    static const short NONE_CONTINUOUS_DELIMITER = 1;
    short  m_delsMode;   /* The continuous delimiter mode */

    short  m_startLog;   /* The LOG output before R-CODE becomes a ready condition (0:OFF, 1:ON) */
    short  m_legacyMode; /* R-CODE Ver1.2 Legacy Support Mode (0:Not Legacy Mode. 1:Legacy Mode) */

    int    m_waitMode;   /* Wait mode (0:Automatic wait, 1:Not automatic wait[high-speed executive mode]) */
    int    m_autoReset;  /* AutoReset flag (0:none, 1:When it begins to read a sensor variable, 0 clear)  */
    int    m_nofalldown; /* Ignore fall down recovery (1:ignore, 0:normal) */

    short  m_lastCond;   /* Final condition judgment value    (Use on the compound IF condition.) */
    short  m_csetFlag;   /* CSET it is carried out flag. */
    short  m_caseFlag;   /* CASE it is carried out flag. */
    short  m_interrupt;  /* Interrupt number / Permit only 1 at present.  */
    short  m_errorFlag;  /* Error during statement reading */
    short  m_argc;       /* Counter to take out an argument with ARG */
    short  m_stepMode;   /* STEP mode */

    int    m_power;      /* Power variable Legacy support (0:motor power off, 1:motor power on) */
    int    m_clock;      /* Clock variable (16ms)   */
    int    m_clock2;     /* Clock2 variable (500ms) */

    short  m_clockDown;  /* WAIT:<ms> When running, <ms>/32 is set, and it subtracts 1 every 32ms. */
    short  m_countDown;  /* Time to Shutdown (32ms) */
    short  m_delayFlag;  /* The flag that takes a return posture at the end of the fall down recovery*/
    MWCID  m_delayMwcid; /* The above return posture MWCID (initial/recover varies on SysEvent.) */
    int    m_intClock;   /* +1 is done by Clock(). (non-fixed period) */

    MWCID  m_initialPose;   /* Initial pose MWCID */
    MWCID  m_recoverPose;   /* Recover pose MWCID */
    MWCID  m_chagePose;     /* Charging pose MWCID */
    MWCID  m_baseMWCID;
    ulong   m_baseID;       /* Base of AppId of Target */
    SubjectID       m_TRsbjID;      /* SubjectID of TargetInfo of TR.BIN */

    int             m_tmYear;
    int             m_tmMonth;
    int             m_tmDay;
    int             m_tmHour;
    int             m_tmMin;
    int             m_tmSec;
    int             m_tmDow;

    int             m_seed;         /* Seed variable */

    short           m_battRest;     /* Battery remaining */
    short           m_battRestMin;  /* Battery remaining minimum value */

    static const short SHUTDOWN_API      = 0;
    static const short SHUTDOWN_EXCPTION = 1;
    static const short SHUTDOWN_APPCMD   = 2;
    short           m_shutdownMode;  /* Shutdown mode */

    short           m_clockMode;     /* 0:FullSpeed, 1:Necessary minimum */

    MWCPosture      m_mwcPos;        /* Present posture information */

    int             m_posture1;      /* Posture1 variable (Upper 16bit of mwc_pos) */
    int             m_posture2;      /* Posture2 variable (Lower 16bit of mwc_pos) */

    MWIID           m_tempHi;        /* MWIID of the high temperature alarm (Turn off power supply when this occurs.) */

    MWIID           m_imageLY;       /* Brightness ID of the image for calculation */

    int             m_brightness;    /* Brightness variable */
    char            m_newline[8];    /* Line feed character code */
    int             m_control;       /* Control code transmission */
                                     /* (Cope with '\000' - '\017' more than lower bit.) */

    int             m_resultNo;      /* CC command result SerialNO (PLAY, SEND) */
    int             m_resultType;    /* CC command result Type */

    void            Error ( char *fmt, ... );
    void            ClearError ( void );
    void            ShowErrors ( void );
    void            PrintInputSemantics( OcaInputSemantics &sem );

    int             LineCount  ( const char *path );
    void            ReadActSub ( const char *path );
    void            ReadAction ( void );
    void            ReadJoint  ( void );
    void            ReadSensor ( void );
    void            ReadAppTag ( void );
    void            ReadLMSCom ( void );

    void            InitRMain  ( void );
    void            ShowHeader ( void );

    void            SETX ( RStatement *st );


    void            Run     ( void );
    void            Execute ( int run );
    void            StopRun ( int ack );
    void            HaltRun ( int ack );
    void            LoadProgram ( const char *filename );

    void            CommandSEND ( RMessage* msg );
    void            CommandPLAY ( RMessage* msg, int pose       );
    void            CommandSTOP ( RMessage* msg, int emergency );
    void            StopAIBO ( int emergency );
    void            SHUTDOWN ( void );
    void            EmergencyExec( OcaInputSemantics *data );
    void            SetPeriodicTimeRequest( void );

    /* CCCommand Queue (->CC) */
    CCCSerial       m_cccSerial; /* Serial number */
    int             m_cccBusy;   /* Flag during the command transmission */
                                 /* (After Result of 2ND is returned, the next command can be sent.) */
    struct CCCMsg
    {
        CCCMsg *next;            /* List structure */
        CCCommandQueu ccc;
        int cmd;                 /* R-Code Command (The command to deal with SHUTDOWN on the R-Code side) */
    };
    CCCMsg* m_cccMsgHead;        /* A pointer to the head of MwcMsg */

    void PutCCCMsg  ( CCCommandQueu* ccc, int cmd );
    void ClrCCCMsg  ( void );
    void SendCCCMsg ( int mode );

    /* Message Queue (->RComm/SComm) */
    struct SysMsg
    {
        SysMsg *next;       /* List structure */
        char    msg[200];   /* Message strings */
    };
    SysMsg *m_sysMsgHead;   /* The command to deal with SHUTDOWN on the R-Code side */

    void PutSysMsg  ( char *msg );
    void ClrSysMsg  ( void );
    void SendSysMsg ( void );
    void Ack        ( void );
    void Nak        ( void );
    void Syn        ( void );
    void Printf     ( char *fmt, ... );

    void InitMWC    ( void );
    void InitCDB    ( void );
    void DumpCDB    ( void );
    void DumpIDB    ( void );
    void DumpDIC    ( void );

    void LoadMtn    ( char *filename );
    void LoadLED    ( char *filename );
    void LoadSND    ( char *filename, int n );
    void PlayMtn    ( void );
    void StopMtn    ( void );
    void InitMtn    ( void );
    void RReplayCmd ( char *cmd );

    int typo;
    int typ0;
    int typ1;
    int typ2;
    int typ3;
    int typ4;

    int m_soundWait;    /* Waiting for sound reproduction? (Used by the RReplay control)*/

    /* DirectCommand Queue (->VRComm) */
    struct DirectMsg
    {
        DirectMsg *next;    /* List structure */
        int index;          /* index in joint_var[] of the applicable joint */
        int value;          /* Value */
    };
    DirectMsg* m_directMsgHead; /* The queue to send a command to VRComm directly (DirectMsg) */
    void PutDirectMsg ( int index, int value );
    void SendDirectMsg ( void );

    //
    // WLAN info
    //
    uchar ip_addr[4];   /* WLAN IP address of the connection */

    //
    // Replay
    //
    typedef struct
    {
        short   cmd;    /* Command number 1:MTN_INIT 2:MTN_DATA 3:MTN_PLAY 4:LED_INIT 5:LED_DATA */

        unsigned long       time;           /* The time in [8ms] units at INIT of DATA */
        unsigned long       mtnMask;        /* mtnData Effective mask */
        short               mtnData[32];    /* Joint angle [degree] */

        unsigned long       ledMask;        /* An effective mask */
        short               ledData[32];    /* 0-4 Resolution: 64ms */
    }
    ReplayData;

    /* ReplayData Queue (->RReplay) */
    struct ReplayMsg
    {
        ReplayMsg  *next;
        ReplayData  data;   /* ReplayData */
    };
    ReplayMsg* m_replayMsgHead;    /* The queue to send ReplayData to RReplay */
    void PutReplayMsg ( ReplayData *data );
    void SendReplayMsg ( void );

    struct Pose
    {
        Pose       *next;
        short       name;   /* WordID */
        ReplayData  data;   /* RReplayData which has Pose information */
    };
    struct KeyFrame
    {
        KeyFrame   *next;
        short       time;   /* [8ms] */
        Pose       *pose;
    };
    struct Motion
    {
        Motion     *next;
        short       name;   /* WordID */
        KeyFrame   *kfrm;
    };

    Pose   *m_poseList;     /* not used */
    Motion *m_motionList;   /* not used */

    short m_defFlag;    /* not used:0 */

    MWCID       m_MtnMWCID[8*1024]; /* The list of MWCID which contains motions */
    char        m_MtnType[8*1024];  /* LastResult 0:no wait, 1:must wait */
    int         m_MtnN;             /* The effective head of MtnMWCID[]/MtnType[] */

    MWCSerial   m_wait2Serial;      /* MWCID that it is waiting for 2nd.Result */
    MWCSerial   m_waitLSerial;      /* MWCID that it is waiting for Last.Result */

    int         m_stationMode;      /* 0:Power supply off on station
                                       1:Ver1.1 original use
                                       2:Moves on station */

    PlugIn *m_plugin;
    int     m_plugin_n;
    void ReadPlugIn    ( void );
    void RegistPlugIn  ( void );
    void ReadPlugInCfg ( int i, int phase );

    /* PlugInData Queue (->PlugIn) */
    struct PlugInMsg
    {
        PlugInMsg  *next;
        int         i;      /* index in plugin[] of applicable PlugIn */
        PlugInData  data;
    };
    PlugInMsg* m_pluginMsgHead; /* The queue to send PlugInData to PlugIn */
    void PutPlugInMsg  ( int i, PlugInData *data );
    void ClrPlugInMsg  ( void );
    void SendPlugInMsg ( void );


    bool GetRMainSemanticsLabels(void);
    bool GetSemLabels(void);
    bool GetSubLabels(void);

    // InputSemantics for managing state
    longword           m_num_of_sem_state_unit;
    OcaInputSemantics* m_sem_state;
    OcaInputSemantics* GetSemanticsState(ISSemanticsID sem_id);


/*****************************************************************/

    int             m_stringBufMax;
    char*           m_stringBuf;
    int             m_stringBuf_n;   /* stringBuf Use size */
    int             m_stringBuf_0;   /* Index value for resetting (Right after securing a System area) */
    int             m_stringBuf_1;   /* Index value for resetting (At the time of the EDIT end) */


    int             m_dictionaryMax;
    RWord*          m_dictionary;
    short           m_dictionary_n;  /* dictionary Use size */
    short           m_dictionary_0;  /* Index value for resetting (Right after securing a System area) */
    short           m_dictionary_1;  /* Index value for resetting (At the time of the EDIT end) */

    char*           GetDictionaryWord(int index) { return m_dictionary[index].word; };
    short           GetDictionaryType(int index) { return m_dictionary[index].type; };

    int             m_stackMax;
    RVariable*      m_stack;
    short           m_sp;            /* stack pointer */
    short           m_sp0;           /* Index value for resetting (Right after securing a System area) */
    short           m_sp1;           /* Index value for resetting (At the time of the EDIT end) */

    RDimVariable*   m_dimVariable;
    short           m_dimVariable_n;

    short           m_reservedVarID[MAX_RESERVED_VARIABLE_NUM]; /* The variable list kept at the time of the end */
    short           m_reservedVar_n;

    int             m_statementMax;
    RStatement*     m_statement;
    int             m_statement_n;   /* statement use size */

    short           m_callLevel;

    short           m_on_callMax;
    RSnapInfo*      m_on_call;
    short           m_on_call_i;     /* The index value of executed ONCALL */

#ifdef _MACRO_
    RStatement      m_macro[32];
    int             m_macro_i;
    int             m_macro_n;
    int             m_macroFlag;
    short           m_macroContext;
    short           m_macroTypeCt;
    short           m_macroTypeCe;
#endif // _MACRO_

    void            ClearDic ( void );
    void            ResetDic ( void );
    int             AddReservedVar ( int id );
    void            ReservedVariable ( void );

    short           RegIDraw ( char *word, short type, int data );
    int             RegistID ( char *word, short type, int data );
    int             GetToken ( char *s, int *arg, short type );

    char*           CopyWord ( char *word );
    short           SearchID ( char *word, short type, short *type_result );
    char*           IDtoWord ( short id );

    void            SaveVar ( short id );
    void            LoadVar ( short id );
    void            SaveDimVar ( short id );
    void            LoadDimVar ( short id );

    void            PushStack ( int data );
    int             PopStack ( void );
    void            DupStack ( void );
    void            ArgStack ( short id );
    void            OpStack1 ( short type );
    void            OpStack2 ( short type );

    int             RenewDim(int addArray_n, int varId);
    void            AllDelDim( void );

    short           SearchIF ( void );
    short           SearchLoop ( short level );
    void            ClrMacro ( RStatement *st );
    void            AddMacro ( char *fmt, ... );
    void            RunMacro ( void );
    void            ExpandStatementToMessage ( RStatement *st, RMessage *msg );

    void            UpdateTimeVariable( void );
    void            AddSystemVariable ( char *name, int *data );
    void            AddPlugInVariable ( char *name, int *data );
    void            AddGlobalVariable ( short id, int data );
    void            AddLocalVariable  ( short id, int data );
    void            AddDIMVariable ( short id, short data );

    void            RegistSysConstants ( void );
    void            RegistUsrConstants ( void );
    void            RegistSysVariables ( void );
    void            RegistUsrVariables ( void );


    int*            FindVar ( short id, int *reset );
    int             EvalVar ( short id );
    short           SetVar  ( short id, int var );

    void            RunCommand ( char *cmd );
    void            ParseCommand ( char *cmd );
    void            AddWord( char *cmd, short type, short x );
    short           ParseStatement ( char *line, RStatement *st );
    short           AutoWait( void );
    int             ProcessStatement ( RStatement *st, int st_idx );
    void            ProcessMessage ( RMessage* msg );

    void            JumpTo ( short type, short label );
    void            ScopeIn ( int data );
    short           ScopeOut ( void );
    void            CallFunction ( short cnt );
    void            ReturnFunction ( void );
    int             CheckInterrupt ( void );
    void            ResetEnv ( short i, int reset_ip );

    void            EvalIf ( RStatement *st, RMessage *msg, int force );
    int             EvalCond ( short op, int v1, int v2 );
    int             EvalCondLocal ( short op, int v1, int v2 );

};
#endif // _RMain_h_DEFINED



