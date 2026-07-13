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

#include <iostream.h>
#include <stdlib.h>
#include <math.h>
#include <MCOOP.h>

#include <ODrivers/WLANDriverProxy.h>

#include <APP/OcaInputSemanticsDef.h>
#include <APP/OcaInputSemantics.h>
#include <APP/OcaAppCommand.h>

#include <OPENR/core_macro.h>
#include <OPENR/ODebug.h>
#include <OPENR/OCalendarTime.h>
#include <OPENR/OccAbleInfo.h>

#include <Middlewares/zlib.h>

#include "RMain.h"
#include "RMainSemantics.h"
#include "RMainGlobalsExt.h"
#include "RCodeDefine.h"
#include "RMainFiles.h"
#include <OSystem/InheritanceData.h>
#include <OUtil/FileWithCheckSum.h>

/////////////////////////////////////////////////////////////////
//
// RMain
//
/////////////////////////////////////////////////////////////////

RMain::RMain ()
  :
    m_wlanCard( UNMOUNT_WLANCARD ),
    m_wlanFlag( NOT_AVAILABLE_WLANFLAG ),
    m_wlanWait( NO_WAIT_DATA_WLAN ),

    m_msStatus( 0 ),
    m_noholdup( 0 ),

    m_aiboId( 0 ),
    m_myaiboId( 0 ),
    m_aiboType( 0 ),
    m_bootType( 0 ),

    m_action_n( 0 ),
    m_jointVar_n( 0 ),
    m_sensVar_n( 0 ),
    m_apptag_n( 0 ),
    m_lmscom_n( 0 ),


#ifdef _MACRO_
    m_macro_i( 0 ),
    m_macro_n( 0 ),
    m_macroContext( 0 ),
    m_macroTypeCt( 0 ),
    m_macroTypeCe( 0 ),
#endif // _MACRO_

    m_pc( 0 ),
    m_ct( 0 ),
    m_ip( 0 ),
    m_lb( 0 ),

    m_initFlag( RCODE_NOT_INIT ),
    m_autoExec( NOT_AUTO ),
    m_editFlag( WAIT_SCRIPT ),
    m_execFlag( WAIT_EXEC ),
    m_execStop( 0 ),
    m_syncFlag( 0 ),
    m_sendFlag( 0 ),
    m_sendFp( 0 ),

    m_trace( 0 ),
    m_debug( 0 ),

    m_waiting( 0 ),
    m_wait( 0 ),
    m_waitPart( 0 ),
    m_waitMask_n( 0 ),

    m_fdCount( 0 ),
    m_sysEvent( 0 ),
    m_sysEvent2Down( 0 ),
    m_sysSerial( 0 ),
    m_waitSysSerial( 0 ),
    m_waitSysCommand( 0 ),
    m_sysStatus( 0 ),
    m_categoryL( 0 ),
    m_categoryM( 0 ),
    m_categoryS( 0 ),
    m_spos( 0 ),
    m_resultNo( 0 ),
    m_resultType( 0 ),

    m_delsMode( CONTINUOUS_DELIMITER ),
    m_startLog( 0 ),
    m_waitMode( 0 ),
    m_autoReset( 0 ),
    m_nofalldown( 0 ),
    m_csetFlag( 0 ),
    m_caseFlag( 0 ),
    m_interrupt( 0 ),
    m_stepMode( 0 ),

    m_power( 0 ),
    m_clock( 0 ),
    m_clock2( 0 ),
    m_clockDown( 0 ),
    m_countDown( 0 ),
    m_delayFlag( 0 ),
    m_delayMwcid( 0 ),
    m_intClock( 0 ),

    m_seed( 1 ),

    m_posture1( 0 ),
    m_posture2( 0 ),

    m_battRest( BATT_REST_INIT ),
    m_battRestMin( BATT_REST_MIN ),

    m_shutdownMode( 0 ),
    m_stationMode( 0 ),
    m_clockMode( 0 ),

    m_cccSerial( 0 ),
    m_cccBusy( 0 ),
    m_cccMsgHead( NULL ),

    m_sysMsgHead( NULL ),

    m_soundWait( 0 ),

    m_directMsgHead( NULL ),
    m_replayMsgHead( NULL ),

    m_defFlag( 0 ),
    m_poseList( NULL ),
    m_motionList( NULL ),

    m_plugin_n( 0 ),
    m_pluginMsgHead( NULL ),

    m_stringBuf_n( 0 ),
    m_dictionary_n( 0 ),
    m_dimVariable_n( 0 ),
    m_sp( 0 ),
    m_statement_n( 0 ),
    m_callLevel( 0 ),
    m_reservedVar_n( 0 ),

    m_sensorConnectionFlag(0),
    m_imageConnectionFlag(0),
    m_targetInfoConnectionFlag(0),
    m_plugInMessageConnectionFlag(0),
    m_plugInCommandConnectionFlag(0),
    m_mwsensorConnectionFlag(0),
#ifdef _RCLOCK_MODE_
    m_rclockConnectionFlag(0),
#endif // _RCOCK_MODE_
    m_legacyMode(0)
{
    NEW_ALL_SUBJECT_AND_OBSERVER;

    strcpy( m_newline, DEFAULT_NEWLINE );
    m_control = 0xFFFF;
    m_initialPose = 0;
    m_recoverPose = 0;
    m_chagePose   = 0;
    m_baseID = 0x60100;
    m_MtnN = 0;
    m_wait2Serial = 0;
    m_waitLSerial = 0;
}

RMain::~RMain()
{
    DELETE_ALL_SUBJECT_AND_OBSERVER;
}

/***************************************************/
/*              OPEN-R Method                      */
/***************************************************/

OStatus
RMain::DoInit ( const OSystemEvent& event )
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("RMain::DoInit()\n"));
#endif //_RMAIN_DEBUG_

    REGISTER_ALL_ENTRY;
    SET_ALL_READY_AND_NOTIFY_ENTRY;

    m_state_receive_ready         = false;
    m_state_periodic_time_request = false;
    m_num_of_sem_state_unit       = 0;


    /* Read RobotDesign */
    char buf[1024];
    OPENR::GetRobotDesign( buf );
    char *p = buf;
    while( *p && !isdigit(*p) ) p++;
    m_aiboType = atoi(p);


    /* Check if memory stick exists */
    OMemoryStickStatus  status;
    if ( OPENR::GetMemoryStickStatus(&status) == oSUCCESS ) {
        m_msStatus = status;
    }

    GetRMainSemanticsLabels();

    /*  Read *.cfg */
    InitRMain();    /* System.cfg & memory allocation */
    ReadAction();   /* Action.cfg -> m_action[]    */
    ReadJoint();    /* Joint.cfg  -> m_jointVar[]  */
    ReadSensor();   /* Sensor.cfg -> m_sensVar[]   */
    ReadPlugIn();   /* PlugIn.cfg -> m_PlugIn[]    */
    ReadAppTag();   /* Apptag.cfg -> m_apptagVar[] */
    ReadLMSCom();   /* LMSCom.cfg -> m_lmscomVar[] */

    /* Read Aibo-id */
    int productID = 0;
    int fd = open( AIBO_ID_FILE, O_RDONLY );
    if( fd >= 0 ) {
        read(fd, &productID, sizeof( productID ));
        close(fd);
        m_myaiboId = productID;
    }

    /* Clear Dictionary */
    ClearDic();

    return oSUCCESS;
}

byte *SeekCVER ( byte *p )
{
    size_t n;

    p += 8;
    while( 1 ) {
        if( p[0] != 'C' ) break;
        if( p[1] == 'O' ) {
            n = *((size_t*)(p+4));
            p = p + 8 + n;
        } else if( p[1] == 'V' ) {
            return p;
        }
    }
    return NULL;
}

int CheckCVER ( byte *p1, byte *p2 )
{
    if( p1 && p2 ) {
        if( (memcmp( p1+ 8, p2+ 8, 6 ) == 0) &&
            (memcmp( p1+16, p2+16, 6 ) == 0) ) {
            return 0; // OK
        }
    }
    return 1; // NG
}

OStatus
RMain::DoStart ( const OSystemEvent& event )
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("RMain::DoStart()\n"));
#endif //_RMAIN_DEBUG_

    m_state_receive_ready = true;

    /* Check CNTVER */
    ODesignDataID dataID = odesigndataID_UNDEF;
    OStatus  stat;
    byte    *data1;
    size_t   size1;
    byte     buf[1024];
    byte     tmp[1024];
    byte    *data2 = tmp;
    uLong   size2;
    int      fd;
    memset( buf, 0, 1024 );
    memset( tmp, 0, 1024 );
    stat = OPENR::FindDesignData( "EXPINFO", &dataID, &data1, &size1 );
    if( stat != oSUCCESS ) {
        OSYSLOG1((osyslogERROR, "can't find EXPINFO\n"));
        OPENR::Fatal( ofatalMEMORY_STICK );
        return oFAIL;
    }
    fd = open( RCODE_CNTVERTION_FILE, O_RDONLY );
    if( fd < 0 ) {
        OSYSLOG1((osyslogERROR, "/APP/CONF/CNTVER.OIF not found.\n" ));
        OPENR::Fatal( ofatalMEMORY_STICK );
        return oFAIL;
    } else {
        size_t n = read( fd, buf, 1024 );
        close( fd );
        if( n == 0 ) {
            OSYSLOG1((osyslogERROR, "CNTVER.OIF file size = 0.\n" ));
            OPENR::Fatal( ofatalMEMORY_STICK );
            return oFAIL;
        }

        int readBufSize;
        readBufSize = n + 8;
        memset( tmp, 0, 1024 );
        if( int tt = gunzip( tmp, (uLong*)(&size2), buf, readBufSize ) != 0 ) {
            OSYSLOG1((osyslogERROR, "gunzip failed.\n" ));
            for(int i=0;i<128;i++){
                DPRINTF(("buf %d:%X %X\n",i, buf[i], tmp[i]));
            }
            OPENR::Fatal( ofatalMEMORY_STICK );
            return oFAIL;
        }
        if( size2 > 0 ) {
            data1 = SeekCVER( data1 );
            data2 = SeekCVER( data2 );
        } else {
            OSYSLOG1((osyslogERROR, "CNTVER.OIF expanded data size = 0.\n" ));
            OPENR::Fatal( ofatalMEMORY_STICK );
            return oFAIL;
        }
        if( CheckCVER( data1, data2 ) )
        {
            OSYSLOG1((osyslogERROR, "unmatch contents !\n" ));
            OPENR::Fatal( ofatalMEMORY_STICK );
            return oFAIL;
        }
    }

    // WLAN Check
    WLANDriverProxy wlanDriver;
    EtherDriverGetWLANSettingsMsg settingsMsg;
    if( wlanDriver.GetSettings( 0, &settingsMsg ) == ETHER_OK ) {
        m_wlanCard = MOUNT_WLANCARD;
    }

    // Boot Condition Check
    OBootCondition bc;
    OPENR::GetBootCondition( &bc );
    m_bootType = bc.bitmap;

    InitMWC();

    // Set TIME Request
    if ( subject[sbjPeriodicTimeRequest]->IsReady() ) {
        SetPeriodicTimeRequest();
        subject[sbjPeriodicTimeRequest]->NotifyObservers();
        observer[obsPeriodicTimeReply]->AssertReady();
        m_state_periodic_time_request = true;
    }

    /*** Connection Check  ***/
    /* Observer*/
    /* Level 1 IF Sensor */
    if ( observer[obsSensor]->NumberOfSubjects() > 0) {
        DPRINTF(("RMain: Sensor Connection OK!\n"));
        m_sensorConnectionFlag = 1;
    }

    /* Level 1 IF Image */
    if ( observer[obsImage]->NumberOfSubjects() > 0) {
        DPRINTF(("RMain: Image Connection OK!\n"));
        m_imageConnectionFlag = 1;
    }

    /* TR.BIN TargetInfo */
    if ( observer[obsTarget]->NumberOfSubjects() > 0) {
        DPRINTF(("RMain: TargetInfo Connection OK!\n"));
        m_targetInfoConnectionFlag = 1;
    }

    /* PlugInMessage */
    if ( observer[obsPlugInMessage]->NumberOfSubjects() > 0) {
        DPRINTF(("RMain: PlugInMessage Connection OK!\n"));
        m_plugInMessageConnectionFlag = 1;
    }

    /* CC */
    if ( observer[obsComplete]->NumberOfSubjects() > 0) {
        DPRINTF(("RMain: Complete connection OK!\n"));
    }

#ifdef _RCLOCK_MODE_
    /* RClock */
    if ( observer[obsClock]->NumberOfSubjects() > 0) {
        DPRINTF(("RMain: RClock Connection OK!\n"));
        m_rclockConnectionFlag = 1;
    }
#endif // _RCLOCK_MODE_

    /* Subject */
    /* PlugInCommand */
    if ( subject[sbjPlugInCommand]->NumberOfObservers() > 0) {
        DPRINTF(("RMain: PlugInCommand Connection OK!\n"));
        m_plugInCommandConnectionFlag = 1;
    }

    /* MWSensor */
    if ( subject[sbjMWSensor]->NumberOfObservers() > 0) {
        DPRINTF(("RMain: MWSensor Connection OK!\n"));
        m_mwsensorConnectionFlag = 1;
    }

    /* CC */
    if ( subject[sbjCCCommand]->NumberOfObservers() > 0) {
        DPRINTF(("RMain: CCCommand Connection OK!\n"));
    }

    /* It is requested to MW at Legacy Mode. */
    if( m_legacyMode == 1 ) {
        for( int i=0; i<m_sensVar_n; i++ ) {
            if( (m_sensVar[i].type != '*')    &&
                (!islower(m_sensVar[i].type)) &&
                (m_sensVar[i].modeID == LEGACY_CONFIG ))  {
                DPRINTF(("RMain: RequireMWInfo:%d\n", m_sensVar[i].semOrmwiid ));
                if( OPENR::RequireMWInfo( m_sensVar[i].semOrmwiid ) != oSUCCESS ) {
                    OSYSLOG1((osyslogERROR,
                              "RequireMWInfo failed! (mwiid=%d)\n", m_sensVar[i].semOrmwiid ));
                }
            }
        }
        if( OPENR::RequireMWInfo( m_imageLY ) != oSUCCESS ) {
            OSYSLOG1((osyslogWARNING, "RequireMWInfo failed! (mwiid=%d)\n", m_imageLY ));
        }
    }

    for( int i=0; i<numOfObserver; i++ ) {
        if ((i == obsSensor ||
             i == obsImage  ||
             i == obsTarget ) &&
            ( m_legacyMode == 0 )) {
            continue;
        }

#ifdef _RCLOCK_MODE_
        /* AssertReady isn't taken out when it isn't connected to RCCLOCK. */
        if ( i == obsClock && m_rclockConnectionFlag == 0 ) continue;
#endif // _RCLOCK_MODE_

        if( observer[i]->AssertReady() != oSUCCESS ) {
            cout << "RMain::DoStart()" << endl;
            cout << "\tAssertReady() failed for observer[" << i << "]" << endl;
        }
    }

    return oSUCCESS;
}

OStatus
RMain::DoStop ( const OSystemEvent& event )
{
    m_state_receive_ready = false;

    /* DeassertReady */
    for( int i=0; i<numOfObserver; i++) {
        observer[i]->DeassertReady();
    }

    ReservedVariable();

    return oSUCCESS;
}

OStatus
RMain::DoDestroy ( const OSystemEvent& event )
{
    return oSUCCESS;
}



//////////////////////////////////////////////////////////////////////
//
// R-CODE Stuff
//
//////////////////////////////////////////////////////////////////////
/* IMM  RWord.data 		does data immediately exist? */
/* VAR  RStatement.op2 	a macro to access variable (Get/Set) */
/* OP2  RStatement.op2  a macro to evaluate (Get) */
/* ARG0 RStatement.arg0 a macro to evaluate (Get) */
/* ARG1 RStatement.arg1 a macro to evaluate (Get) */
/* ARG2 RStatement.arg2 a macro to evaluate (Get) */
/* ARG3 RStatement.arg3 a macro to evaluate (Get) */
/* ARG4 RStatement.arg4 a macro to evaluate (Get) */
#define IMM(type) (type < ARG_UNDEF)
#define VAR  *( FindVar(st->op2,NULL) )
#define OP2  ((IMM(st->type.op2 ))?( st->op2  ):( EvalVar( st->op2  ) ))
#define ARG0 ((IMM(st->type.arg0))?( st->arg0 ):( EvalVar( st->arg0 ) ))
#define ARG1 ((IMM(st->type.arg1))?( st->arg1 ):( EvalVar( st->arg1 ) ))
#define ARG2 ((IMM(st->type.arg2))?( st->arg2 ):( EvalVar( st->arg2 ) ))
#define ARG3 ((IMM(st->type.arg3))?( st->arg3 ):( EvalVar( st->arg3 ) ))
#define ARG4 ((IMM(st->type.arg4))?( st->arg4 ):( EvalVar( st->arg4 ) ))

/////////////////////////////////////////////////////////////////
//
// R-CODE Dictionary
//
/////////////////////////////////////////////////////////////////
/* Clear dictionary */
void
RMain::ClearDic ( void )
{
    m_dictionary_n = 0;
    m_stringBuf_n = 0;

    RegistSysVariables(); /* * Be sure to initialize SysVar first. -> SetVar */
    RegistSysConstants();

    RegistPlugIn();

    RegistUsrConstants();
    RegistUsrVariables();

    m_stringBuf_0 = m_stringBuf_n;
    m_dictionary_0 = m_dictionary_n;
    m_sp0 = m_sp;
}

/* Reset dictionary */
void
RMain::ResetDic ( void )
{
    m_stringBuf_n = m_stringBuf_0;
    m_dictionary_n = m_dictionary_0;
    m_sp = m_sp0;
    AllDelDim();
}

int
RMain::AddReservedVar ( int varid )
{
    if ( m_reservedVar_n >= MAX_RESERVED_VARIABLE_NUM ) {
        Error( "Limit Reserved Variable. num:%d", m_reservedVar_n );
        return false;
    }

    m_reservedVarID[m_reservedVar_n] = varid;
    m_reservedVar_n++;
    return true;
}

void
RMain::ReservedVariable ( void )
{
    int fv;
    for( int i=0; i< m_reservedVar_n; i++ ) {
        fv=0;
        for( int j=0; j< m_dimVariable_n; j++ ) {
            if ( m_dimVariable[j].id == m_reservedVarID[i] ) {
                /* for DIM */
                SaveDimVar( m_reservedVarID[i] );
                fv=1;
                break;
            }
        }
        /* for a normal variable */
        if ( fv==0 ) SaveVar( m_reservedVarID[i] );
    }
}

/////////////////////////////////////////////////////////////////
//
// Dictionary stuff
//
/////////////////////////////////////////////////////////////////
/* The function to register an ID into the dictionary */
short
RMain::RegIDraw ( char *word, short type, int data )
{
    short i = m_dictionary_n++;
    if( i >= m_dictionaryMax ) {
        Error( "dictionary over flow!" );
        return -1;
    }
    m_dictionary[i].word = word;
    m_dictionary[i].type = type;
    m_dictionary[i].data = data;
    return i;
}

/* The function that copies a string to the top of string_buf */
char*
RMain::CopyWord ( char *word )
{
    char *p = m_stringBuf + m_stringBuf_n;
    strcpy( p, word );
    m_stringBuf_n += ((strlen(word)/4 + 1) * 4);
    if( m_stringBuf_n >= m_stringBufMax ) {
        Error( "m_stringBuf over flow!" );
        return NULL;
    }
    return p;
}

/* The function that registers Word into the dictionary */
int
RMain::RegistID ( char *word, short type, int data )
{
    char *p = CopyWord( word );
    if( !p ) return -1;
    return RegIDraw( p, type, data );
}

/* The function that finds Word in the dictionary */
short
RMain::SearchID ( char *word, short type, short *type_result )
{
    for( short i=0; i<m_dictionary_n; i++ ) {
        if( strcmp( m_dictionary[i].word, word ) ) continue;

        switch( type ) {
        case ARG_STRING:
            if( m_dictionary[i].type != ARG_STRING ) continue;
            break;

        case ARG_LABEL:
            if( m_dictionary[i].type == ARG_CONST  ) break;
            if( m_dictionary[i].type != ARG_LABEL  ) continue;
            break;

        case -1:
            Error( "multiple defined [%s].", word );
            return -1;
        }

        if( m_dictionary[i].type == ARG_UNDEF ) {
            m_dictionary[i].type = type;
        }
        if( type_result ) *type_result = m_dictionary[i].type;
        return i;
    }

    if( type < 0 ) return 0;

    switch( type ) {
    case ARG_UNDEF:
        *type_result = ARG_UNDEF;
        return RegistID( word, ARG_UNDEF, -1 );

    case ARG_LABEL:
        *type_result = ARG_LABEL;
        return RegistID( word, ARG_LABEL, -1 );

    case ARG_STRING:
        *type_result = ARG_STRING;
        return RegistID( word, ARG_STRING, -1 );

    case ARG_CONST:
    case ARG_SYSVAR:
    case ARG_USRVAR:
    case ARG_EXTVAR:
        *type_result = 0;
        Error( "[%s] is not defined.", word );
        return 0;
    }

    return 0;
}

/* The function that changes ID into Word */
char*
RMain::IDtoWord ( short id )
{
    return m_dictionary[id].word;
}

/* A function for the command string analysis */
int
RMain::GetToken ( char *s, int *arg, short type )
{
    static ulong l;
    static char *q;
    char *p;
    if( s == NULL ) p = q;
    else            p = s;

    if( type == ARG_HIWORD ) {
        if( arg ) *arg = HIWORD(l);
        return ARG_VALUE;
    }

    if( *p ) {
        if( *p == '"' ) {
            p++;
            q = p;
            while( *q && (*q != '"') ) q++;
            if( *q ) *q++ = '\0';
        } else {
            q = p;
        }
        while( *q && !strchr( ": \t\n\r", *q ) ) q++;
        if( *q ) {
            if( m_delsMode == NONE_CONTINUOUS_DELIMITER ) {
                while( *q && strchr( ": \t\n\r", *q ) ) {
                    *q++ = '\0';
                }
            } else {
                *q++ = '\0';
            }
        }
    } else p = NULL;

    if( !arg ) {
        if( p == NULL ) return 0;
        else return AtoI(p);
    } else if( p )  {
        if( *p == '\0' ) {
            *arg = 0;
            return 0;
        } else if( type == ARG_ACTION ) {
            int i;
            for( i=0; i<m_action_n; i++ ) {
                if( !strcmp( p, m_action[i].name ) ) {
                    *arg = i;
                    return ARG_ACTION;
                }
            }
            Error( "[%s] is not defined.", p );
            *arg = 0;
            return 0;
        } else if( type == ARG_APPTAG ) {
            int i;
            for( i=0; i<m_apptag_n; i++ ) {
                if( !strcmp( p, m_apptag[i].name ) ) {
                    *arg = i;
                    return ARG_APPTAG;
                }
            }
            Error( "[%s] is not defined.", p );
            *arg = 0;
            return 0;
        } else if( (type != ARG_STRING) &&
                   (type != ARG_LABEL) &&
                   strchr( "0123456789+-", *p ) ) {
            if( type == ARG_LOWORD ) {
                l = AtoL(p);
                *arg = LOWORD(l);
            } else {
                *arg = AtoI(p);
            }
            return ARG_VALUE;
        } else {
            short type_result;
            if( type == ARG_CONSTS ) {
                short id;
                int x = 0;
                while( p < q ) {
                    char *r = p;
                    while( *r && (*r != '|') && (*r != '+') ) r++;
                    *r = '\0';
                    id = SearchID( p, ARG_CONST, &type_result );
                    if( type_result == ARG_CONST ) {
                        x += m_dictionary[id].data;
                    } else Error( "[%s] is not defined.", p );
                    p = r+1;
                }
                *arg = x;
                type_result = ARG_CONST;
            } else {
                *arg = SearchID( p, type, &type_result );
                if( type_result == ARG_CONST ) *arg = m_dictionary[*arg].data;
            }
            return type_result;
        }
    } else {
        *arg = 0;
        return ARG_NONE;
    }
}

/* The function that saves the variables to a file */
void
RMain::SaveVar ( short id )
{
    char *name = IDtoWord( id );
    char  path[MAX_PATH_LEN];
    char  line[MAX_STRING_BUFF];
    int   fp;
    char *p;

    if( m_battRest < m_battRestMin ) {
        return;
    }

    sprintf( path, "%s%s.sav", AMS_FILE_PATH, name );
    p = path;
    while( *p ) *p++ = toupper(*p);
    fp = Fopen( path, "w" );

    if( fp < 0 ) {
        Error( "can't open [%s].", path );
    } else {
        sprintf( line, "%d\n", EvalVar( id ) );
        Fputs( line, fp );
        Fclose( fp );
    }
}

/* The function that reads variables from a file */
void
RMain::LoadVar ( short id )
{
    char *name = IDtoWord( id );
    char  path[MAX_PATH_LEN];
    char  line[MAX_STRING_BUFF];
    int   fp;
    char *p;

    sprintf( path, "%s%s.sav", AMS_FILE_PATH, name );
    p = path;
    while( *p ) *p++ = toupper(*p);
    fp = Fopen( path, "r" );

    if( fp < 0 ) {
        SetVar( id, 0 );
    } else {
        Fgets( line, 255, fp );
        Fclose( fp );
        SetVar( id, AtoI(line) );
    }
}

void
RMain::SaveDimVar ( short id )
{
    char *name = IDtoWord( id );
    char  path[MAX_PATH_LEN];
    int   fp;
    char *p;

    if( m_battRest < m_battRestMin &&
        m_msStatus != omemorystickWRITABLE ) {
        return;
    }

    sprintf(path, "%s%s.gvr", AMS_FILE_PATH, name );
    p = path;
    while( *p ) *p++ = toupper(*p);
    fp = Fopen( path, "w" );

    if( fp < 0 ) {
        Error( "can't open [%s].", path );
    } else {
        int nCount=0, nfirst = -1;
        for( int i=0; i < m_dimVariable_n; i++ ) {
            DPRINTF(("m_dimVariable[i]:%x\n", id ));
            if( m_dimVariable[i].id == id ) {
                if( nfirst == -1 ) nfirst = i;
                nCount++;
            }
        }

        size_t size = 0;
        size += sizeof(int) * nCount;
        size_t bufsize = size;

        void* buf;
        if ( NewRegion(bufsize, &buf) != sSUCCESS ) {
            OSYSLOG1((osyslogERROR, "err! (cannot allocate memory region), WrDimGv"));
            return;
        }
        memset(buf, 0, bufsize);

        int* pDim = static_cast<int*>(buf);
        for( int i=nfirst; i < (nfirst + nCount); i++ ) {
            *pDim = m_dimVariable[i].value;
            DPRINTF(("DimSAVE:%x, nfirst:%d, nCount:%d\n", *pDim, nfirst, nCount ));
            pDim++;
        }

        long ret = write( fp, buf, bufsize );

        DPRINTF(("Write %d 0x%x %d\n",ret, &buf, bufsize ));
        Fclose( fp );
        DeleteRegion( buf );
    }
}

void
RMain::LoadDimVar ( short id )
{
    char *name = IDtoWord( id );
    char  path[MAX_PATH_LEN];
    int   fp;
    char *p;

    sprintf( path, "%s%s.gvr", AMS_FILE_PATH, name );
    p = path;
    while( *p ) *p++ = toupper(*p);

    fp = Fopen( path, "r" );
    if( fp < 0 ) {
        return;
    }

    struct stat file_stat;
    if( stat( path, &file_stat) != sSUCCESS) {
        return;
    }
    DPRINTF(("fsize:%d\n", file_stat.size ));

    void* buf;
    if ( NewRegion(file_stat.size, &buf) != sSUCCESS ) {
        OSYSLOG1((osyslogERROR, "err! (memory allocation error),"
                  " size:%d, file:%s DimVar", file_stat.size, path));
        return;
    }
    long nbyte;
    lseek(fp, 0, SEEK_SET);// return to the top of file
    if ( (nbyte = read(fp, buf, file_stat.size)) != file_stat.size ) {
        if ( nbyte < 0 ) {
            // read fail
            fprintf(stdout, "cannot read file\n");
            if ( DeleteRegion(buf) != sSUCCESS ) {
                OSYSLOG1((osyslogERROR, "err! (delete buffer error 1),"
                        " file:%s DimVar", path));
            }
            Fclose( fp );
            return;
        }
    }
    Fclose( fp );

    DPRINTF(("LoadDIM: %d:", file_stat.size));
    p = static_cast<char*>(buf);
    for( int i = 0; i< file_stat.size; i++ ) {
        DPRINTF(("%d", p[i] ));
    }
    DPRINTF(("\n"));
}

/////////////////////////////////////////////////////////////////
//
// Control structure
//
/////////////////////////////////////////////////////////////////
void
RMain::JumpTo ( short type, short label )
{
    if( type == ARG_LABEL ) {
        if( label == -1 ) {
            Error( "undefined label!" );
        } else {
            m_pc = label;
        }
    }
}

/* Process SCOPEIN (like { from the C language) */
void
RMain::ScopeIn ( int data )
{
    m_stack[m_sp].usr.id    = -2;
    m_stack[m_sp].usr.value = data;
    m_sp++;
}

/* Process SCOPEIN (like } from the C language) */
short
RMain::ScopeOut ( void )
{
    while( m_stack[--m_sp].usr.id != -2 );
    return m_stack[m_sp].usr.value;
}

/* Process a function call (CALL) */
void
RMain::CallFunction ( short cnt )
{
    if( m_stackMax < m_sp+SYSTEM_STACK ) {
        Error( "stack over-flow!" );
    } else {
        short i;

        m_sp += SYSTEM_STACK;

        for( i=m_sp-1; i>=m_sp-cnt; i-- ) {
            m_stack[i].usr.id      = m_stack[i-SYSTEM_STACK].usr.id;
            m_stack[i].usr.value = m_stack[i-SYSTEM_STACK].usr.value;
        }

        m_stack[i].usr.id      = -1;
        m_stack[i].usr.value = m_lb;
        i--;
        m_stack[i].usr.id      = -1;
        m_stack[i].usr.value = m_ip;
        i--;
        m_stack[i].usr.id      = -1;
        m_stack[i].usr.value = m_ct;
        i--;
        m_stack[i].usr.id      = -1;
        m_stack[i].usr.value = m_caseFlag;
        i--;
        m_stack[i].usr.id      = -1;
        m_stack[i].usr.value = m_pc;

        m_lb = m_sp-cnt;

        m_argc = 0;

        m_callLevel++;
    }
}

void
RMain::ReturnFunction ( void )
{
    if( m_sp-SYSTEM_STACK < 0 ) {
        Error( "stack under-flow!" );
    } else {
        for( short i=m_sp-1; i>=m_lb; i-- ) m_sp--;
        m_lb = m_stack[--m_sp].usr.value;
        m_ip = m_stack[--m_sp].usr.value;
        m_ct = m_stack[--m_sp].usr.value;
        m_caseFlag = m_stack[--m_sp].usr.value;
        m_pc = m_stack[--m_sp].usr.value;
        m_callLevel--;
    }
}

/* Interrupt management (ONCALL) */
int
RMain::CheckInterrupt ( void )
{
    if( m_interrupt == 0 ) {
        RMessage msg_buf;
        RMessage *msg = &msg_buf;
        RStatement *st;
        for( short i=m_ip-1; i>=0; i-- ) {
            st = m_statement + m_on_call[i].pc;
            ExpandStatementToMessage( st, msg );
            if( EvalCondLocal( msg->type, msg->arg0, msg->arg1 ) ) {
                if( m_interrupt == 0 ) m_on_call_i = i;
                m_interrupt++;
                CallFunction( 0 );
                JumpTo( st->type.arg2, msg->arg2 );
                return 1;
            }
        }
    }
    return 0;
}

/* Interruption return (RESUME) */
void
RMain::ResetEnv ( short i, int reset_ip )
{
    RSnapInfo *snap = NULL;
    if( i>=0 ) snap = m_on_call + i;

    if( snap ) {
        m_pc = snap->pc;
        m_ct = snap->ct;
        if( reset_ip ) m_ip = snap->ip;
        m_sp = snap->sp;
        m_lb = snap->lb;
    } else {
        m_pc = 0;
        m_ct = 0;
        if( reset_ip ) m_ip = 0;
        m_sp = m_sp1;
        m_lb = m_sp1;

        /* The System interrupt return at the ONCALL time of processing resets ONCALL. */
        m_interrupt=0;
        m_on_call_i=0;
    }

    if( !reset_ip ) {
        short lv = 0;
        if( snap )lv = snap->lv;
        for( i=0; (i<m_ip) && (m_on_call[i].lv<=lv); i++ );
        m_ip = i;
    }

    m_syncFlag  = 0;
    m_waiting   = 0;
    m_waitPart  = 0;
    m_clockDown = 0;
#ifdef _MACRO_
    m_macro_i    = 0;
    m_macro_n    = 0;
#endif // _MACRO_

}

/* The evaluation of an IF statement (Call EvalCond) */
void
RMain::EvalIf ( RStatement *st, RMessage *msg, int force )
{
    ExpandStatementToMessage( st, msg );
    if( st->type.arg2 == ARG_CONST ) {
        switch( st->arg2 ) {
        case mTHEN:
            if( !EvalCond( msg->type, msg->arg0, msg->arg1 ) ) {
                if( st->arg3 >= 0 ) {
                    m_pc = st->arg3 + 1;
                } else if( st->arg4 >= 0 ) {
                    m_pc = st->arg4 + 1;
                } else {
                    Error( "can't find ENDIF." );
                }
            }
            break;

        case mBREAK:
            if( EvalCond( msg->type, msg->arg0, msg->arg1 ) ) {
                if( st->arg4 >= 0 ) {
                    int ep = m_statement[st->arg4].arg4;
                    if( ep >= 0 ) m_pc = ep + 1;
                    else Error( "can't find LoopEnd." );
                }
                else Error( "can't find LoopTop." );
            }
            break;

        case mCALL:
            if( EvalCond( msg->type, msg->arg0, msg->arg1 ) ) {
                CallFunction( msg->arg4 );
                JumpTo( st->type.arg3, msg->arg3 );
            }
            break;

        default:
            Error( "Syntax error in IF statement." );
        }
    }
    else
    {
        if( force || EvalCond( msg->type, msg->arg0, msg->arg1 ) ) {
            JumpTo( st->type.arg2, msg->arg2 );
        } else {
            JumpTo( st->type.arg3, msg->arg3 );
        }
    }
}

/* The evaluation of a condition */
int
RMain::EvalCond ( short op, int v1, int v2 )
{
    switch( op ) {
    case OpEQ:   m_lastCond = (v1 == v2); break;
    case OpNE:   m_lastCond = (v1 != v2); break;
    case OpLT:   m_lastCond = (v1 <  v2); break;
    case OpLE:   m_lastCond = (v1 <= v2); break;
    case OpGT:   m_lastCond = (v1 >  v2); break;
    case OpGE:   m_lastCond = (v1 >= v2); break;
    case OpAND:  m_lastCond = (v1 &  v2); break;
    case OpIOR:  m_lastCond = (v1 |  v2); break;
    case OpXOR:  m_lastCond = (v1 ^  v2); break;
    case OpLAND: m_lastCond = (v1 && v2); break;
    case OpLIOR: m_lastCond = (v1 || v2); break;
    default: Error( "unknown operator [%d].", op );
    }
    return m_lastCond;
}

/* The evaluation a condition (The edition which isn't dependent on last_cond:
   for the interruption check) */
int
RMain::EvalCondLocal ( short op, int v1, int v2 )
{
    int cond = 0;
    switch( op ) {
    case OpEQ:   cond = (v1 == v2); break;
    case OpNE:   cond = (v1 != v2); break;
    case OpLT:   cond = (v1 <  v2); break;
    case OpLE:   cond = (v1 <= v2); break;
    case OpGT:   cond = (v1 >  v2); break;
    case OpGE:   cond = (v1 >= v2); break;
    case OpAND:  cond = (v1 &  v2); break;
    case OpIOR:  cond = (v1 |  v2); break;
    case OpXOR:  cond = (v1 ^  v2); break;
    case OpLAND: cond = (v1 && v2); break;
    case OpLIOR: cond = (v1 || v2); break;
    default: Error( "unknown operator [%d].", op );
    }
    return cond;
}

/////////////////////////////////////////////////////////////////
//
// Stack Operations
//
/////////////////////////////////////////////////////////////////

void
RMain::PushStack ( int data )
{
    m_stack[m_sp].usr.id    = -3;
    m_stack[m_sp].usr.value = data;
    m_sp++;
}

int
RMain::PopStack ( void )
{
    if( m_sp <= m_lb ) return 0;
    return m_stack[--m_sp].usr.value;
}

void
RMain::DupStack ( void )
{
    if( m_sp <= m_lb ) return;
    PushStack( m_stack[m_sp-1].usr.value );
}

void
RMain::ArgStack ( short id )
{
    if( m_sp <= m_lb+m_argc ) return;
    m_stack[m_lb+m_argc].usr.id = id;
    m_argc++;
}

/* Stack arithmetic operation */
void
RMain::OpStack1 ( short type )
{
    int x;

    x = PopStack();

    switch( type ) {
    case mNOT:  x = ~x; break;
    case mLNOT: x = !x; break;
    default: Error( "unknown operator [%d].", type ); return;
    }

    PushStack( x );
}

/* Stack arithmetic operation */
void
RMain::OpStack2 ( short type )
{
    int x, y, z;

    y = PopStack();
    x = PopStack();

    switch( type ) {
    case mADD:  z = (x +  y); break;
    case mSUB:  z = (x -  y); break;
    case mMUL:  z = (x *  y); break;
    case mDIV:  z = (x /  y); break;
    case mMOD:  z = (x %  y); break;
    case mAND:  z = (x &  y); break;
    case mIOR:  z = (x |  y); break;
    case mXOR:  z = (x ^  y); break;
    case mLAND: z = (x && y); break;
    case mLIOR: z = (x || y); break;
    case mEQ:   z = (x == y); break;
    case mNE:   z = (x != y); break;
    case mLT:   z = (x <  y); break;
    case mLE:   z = (x <= y); break;
    case mGT:   z = (x >  y); break;
    case mGE:   z = (x >= y); break;
    default: Error( "unknown operator [%d].", type ); return;
    }

    PushStack( z );
}

/////////////////////////////////////////////////////////////////
//
// Data array
//
/////////////////////////////////////////////////////////////////
int
RMain::RenewDim(int addArray_n, int varId)
{
    int nDelCount=0;
    if( addArray_n == 0 ) {
        for( int i=0; i < m_dimVariable_n; i++ ) {
            if( m_dimVariable[i].id == varId ){
                m_dimVariable[i].id = -1;
                nDelCount++;
            }
        }
        if( nDelCount == m_dimVariable_n ) {
            if ( DeleteRegion((void*)m_dimVariable) != sSUCCESS ) {
                OSYSLOG1((osyslogERROR, "Error DeleteRegion: m_dimVariable"));
                return 1;
            }
            m_dimVariable_n = 0;
            return 0;
        }
    }

    RDimVariable* tmp_dimVariable;
    int mem_size=0;
    int new_dimVariable_n = m_dimVariable_n - nDelCount + addArray_n;
    mem_size = sizeof(RDimVariable) * new_dimVariable_n;
    if ((NewRegion( mem_size, (void **)&tmp_dimVariable)) != sSUCCESS) {
        OSYSLOG1((osyslogERROR, "Error NewRegion: tmp_dimVariable"));
        return 1;
    }
    memset( tmp_dimVariable, 0, mem_size );

    int tmpIndex=0;
    for( int i=0;i < m_dimVariable_n; i++ ) {
        if ( m_dimVariable[i].id != -1 ) {
            tmp_dimVariable[tmpIndex].id    = m_dimVariable[i].id;
            tmp_dimVariable[tmpIndex].index = m_dimVariable[i].index;
            tmp_dimVariable[tmpIndex].value = m_dimVariable[i].value;
            tmpIndex++;
        }
    }

    if ( DeleteRegion((void*)m_dimVariable) != sSUCCESS ) {
        OSYSLOG1((osyslogERROR, "Error DeleteRegion: dimVariable"));
        return 1;
    }
    if ((NewRegion(mem_size, (void **)&m_dimVariable)) != sSUCCESS) {
        OSYSLOG1((osyslogERROR, "Error NewRegion: dimVariable"));
        return 1;
    }

    memset( m_dimVariable, 0, mem_size );
    memcpy( m_dimVariable, tmp_dimVariable, mem_size );

    if( DeleteRegion((void*)tmp_dimVariable) != sSUCCESS) {
        OSYSLOG1((osyslogERROR, "Error DeleteRegion: tmp_dimVariable"));
        return 1;
    }

    int index=0;
    for( int i=0;i < new_dimVariable_n; i++ ) {
        if ( m_dimVariable[i].id == 0 ) {
            m_dimVariable[i].id    = varId;
            m_dimVariable[i].index = index++;
        }
    }

    m_dimVariable_n = new_dimVariable_n;
    return 0;
}

void
RMain::AllDelDim( )
{
    if ( m_dimVariable_n ) {
        if ( DeleteRegion((void*)m_dimVariable) != sSUCCESS ) {
            OSYSLOG1((osyslogERROR, "Error DeleteRegion: dimVariable"));
            return ;
        }
        m_dimVariable_n = 0;
    }
    return;
}

/////////////////////////////////////////////////////////////////
//
// Parsing functions
//
/////////////////////////////////////////////////////////////////
short
RMain::SearchIF ( void )
{
    short idx = m_statement_n;
    short lev = 1;

    while( lev > 0 ) {
        idx--;
        if( idx < 0 ) {
            Error( "can't find IF." );
            return -1;
        }

        switch( m_statement[idx].op1 ) {
        case mIF:
        case mIF_1ST:
            if( m_statement[idx].arg2 == mTHEN ) {
                lev--;
            }
            break;

        case mENDIF:
            lev++;
            break;
        }
    }

    return idx;
}

short
RMain::SearchLoop ( short lev )
{
    short idx = m_statement_n;

    if( lev == 0 ) lev = 1;

    while( lev > 0 ) {
        idx--;
        if( idx < 0 ) {
            Error( "can't find LoopTop." );
            return -1;
        }

        switch( m_statement[idx].op1 ) {
        case mWHILE:
        case mREPEAT:
        case mDO:
        case mFOR:
            lev--;
            break;

        case mLOOP:
        case mNEXT:
        case mWEND:
        case mUNTIL:
            lev++;
            break;
        }
    }

    return idx;
}

#ifdef _MACRO_
void
RMain::ClrMacro ( RStatement *st )
{
    m_macro_i = 0;
    m_macro_n = 0;
    memset( m_macro, 0, 10*sizeof(RStatement) );
    m_macroContext = st->context;
    m_macroTypeCt = st->type.ct;
    m_macroTypeCe = st->type.ce;
}

void
RMain::AddMacro ( char *fmt, ... )
{
    va_list args;
    char buf[MAX_STRING_BUFF];

    va_start( args, fmt );
    vsprintf( buf, fmt, args );
    va_end( args );

    if( m_trace & TRACE_CMD ) Printf( "Macro Expand : %s", buf );

    ParseStatement( buf, &(m_macro[m_macro_i]) );

    m_macro[m_macro_i].context = m_macroContext;
    m_macro[m_macro_i].type.ct = m_macroTypeCt;
    m_macro[m_macro_i].type.ce = m_macroTypeCe;

    m_macro_i++;
    m_macro_n++;
}

void
RMain::RunMacro ( void )
{
    m_macro_i    = 0;
    m_macroFlag = 1;
}
#endif // _MACRO_

/* The function in which a statement is developed into in an RMessage */
void
RMain::ExpandStatementToMessage ( RStatement *st, RMessage *msg )
{
    msg->mode = st->op1;
    msg->type = st->op2;

    msg->arg0 = ARG0;
    msg->arg1 = ARG1;
    msg->arg2 = ARG2;
    msg->arg3 = ARG3;
    msg->arg4 = ARG4;

    switch( st->type.arg0 ) {
    case ARG_SYSVAR:
    case ARG_USRVAR:
    case ARG_EXTVAR:
    case ARG_STRING:
        strcpy( msg->name, IDtoWord(st->arg0) ); break;
    default:
        strcpy( msg->name, ""                 ); break;
    }
}


/////////////////////////////////////////////////////////////////
//
// Variable stuff
//
/////////////////////////////////////////////////////////////////
void
RMain::UpdateTimeVariable( void )
{
    OTime time;
    OStatus stat = OPENR::GetTime( &time );
    if( stat == oSUCCESS ) {
        OCalendarTime ct( time );
        m_tmYear  = ct.year_ + otimeSTART_YEAR;
        m_tmMonth = ct.month_;
        m_tmDay   = ct.day_;
        m_tmHour  = ct.hour_;
        m_tmMin   = ct.min_;
        m_tmSec   = ct.sec_;
        m_tmDow   = ct.dow_;
    } else DPRINTF(( "GetTime error! %d\n", stat ));
}

void
RMain::AddSystemVariable ( char *name, int *data )
{
    if( m_stackMax < m_sp+1 ) {
        Error( "stack over flow!" );
        return;
    }

    if( SearchID( name, -1, NULL ) ) return;

    m_stack[m_sp].sys.value = data;
    RegIDraw( name, ARG_SYSVAR, m_sp );

    m_sp++;
}


void
RMain::AddPlugInVariable ( char *name, int *data )
{
    if( m_stackMax < m_sp+1 ) {
        Error( "stack over flow!" );
        return;
    }

    if( SearchID( name, -1, NULL ) ) return;

    m_stack[m_sp].sys.value = data;
    RegIDraw( name, ARG_EXTVAR, m_sp );

    m_sp++;
}

void
RMain::AddGlobalVariable ( short id, int data )
{
    m_dictionary[id].type = ARG_USRVAR;
    m_dictionary[id].data = data;
}


void
RMain::AddDIMVariable ( short id, short data )
{
    m_dictionary[id].type = ARG_DIMVAR;
    m_dictionary[id].data = data;
}

void
RMain::AddLocalVariable ( short id, int data )
{
    if( m_stackMax < m_sp+1 ) {
        Error( "stack over flow!" );
        return;
    }

    m_stack[m_sp].usr.id    = id;
    m_stack[m_sp].usr.value = data;

    m_sp++;
}

void
RMain::RegistSysConstants ( void )
{
    ConstTable *sysConst = const_table;
    while( sysConst->name ) {
        if( !SearchID( sysConst->name, -1, NULL ) ) {
            RegIDraw( sysConst->name, ARG_CONST, sysConst->value );
        }
        sysConst++;
    }
}

void
RMain::RegistSysVariables ( void )
{
    int i;

    for( i=0; i<m_jointVar_n; i++ ) {
        AddSystemVariable( m_jointVar[i].name, &(m_jointVar[i].value) );
    }


    for( i=0; i<m_sensVar_n; i++ ) {
        AddSystemVariable( m_sensVar[i].name, &(m_sensVar[i].value) );
    }

    AddSystemVariable( "MsStatus"   , &(m_msStatus));
    AddSystemVariable( "NoHoldUp"   , &(m_noholdup));

    AddSystemVariable( "AiboId"     , &(m_aiboId)  );
    AddSystemVariable( "MyAiboId"   , &(m_myaiboId));
    AddSystemVariable( "AiboType"   , &(m_aiboType));
    AddSystemVariable( "BootType"   , &(m_bootType));

    AddSystemVariable( "WLANCard"   , &(m_wlanCard));
    AddSystemVariable( "WLANFlag"   , &(m_wlanFlag));

    AddSystemVariable( "Status"     , &(m_sysStatus));
    AddSystemVariable( "CategoryL"  , &(m_categoryL));
    AddSystemVariable( "CategoryM"  , &(m_categoryM));
    AddSystemVariable( "CategoryS"  , &(m_categoryS));
    AddSystemVariable( "SPos"       , &(m_spos)     );

    AddSystemVariable( "Power"      , &(m_power)    );
    AddSystemVariable( "Trace"      , &(m_trace)    );
    AddSystemVariable( "Debug"      , &(m_debug)    );
    AddSystemVariable( "Clock"      , &(m_clock)    );
    AddSystemVariable( "Clock2"     , &(m_clock2)   );
    AddSystemVariable( "Context"    , &(m_ct)       );

    AddSystemVariable( "Wait"       , &(m_wait)     );
    AddSystemVariable( "Sync"       , &(m_syncFlag) );
    AddSystemVariable( "WaitMode"   , &(m_waitMode) );
    AddSystemVariable( "IntClock"   , &(m_intClock) );

    AddSystemVariable( "Year"       , &(m_tmYear)   );
    AddSystemVariable( "Month"      , &(m_tmMonth)  );
    AddSystemVariable( "Day"        , &(m_tmDay)    );
    AddSystemVariable( "Hour"       , &(m_tmHour)   );
    AddSystemVariable( "Min"        , &(m_tmMin)    );
    AddSystemVariable( "Sec"        , &(m_tmSec)    );
    AddSystemVariable( "Dow"        , &(m_tmDow)    );

    AddSystemVariable( "Seed"       , &(m_seed)     );

    AddSystemVariable( "Posture1"   , &(m_posture1) );
    AddSystemVariable( "Posture2"   , &(m_posture2) );

    AddSystemVariable( "AutoReset"  , &(m_autoReset));

    AddSystemVariable( "Brightness" , &(m_brightness));

    AddSystemVariable( "NoFallDown" , &(m_nofalldown));

    /* Cast int  */
    int cccSerial = (int)m_cccSerial;
    AddSystemVariable( "ComNo"      , &(cccSerial));
    AddSystemVariable( "ResultNo"   , &(m_resultNo));
    AddSystemVariable( "ResultType" , &(m_resultType));
}

void
RMain::RegistUsrConstants ( void )
{
    char line[MAX_STRING_BUFF];
    char word[MAX_STRING_BUFF];
    char data[MAX_STRING_BUFF];
    int  mode = 1;
    RMainFiles rf;
    int fp = rf.Open( RCODE_DICTIONARY_FILE );
    if( fp < 0 ) return;

    m_waitMask_n = 0;

    while( rf.GetLine( line ) ) {
        char *p = IgnoreSpace(line);
        if( *p == '[' ) {
            mode = 0;
            if( !strncmp( p, "[CONST", 6 ) ) mode = 1;
            if( !strncmp( p, "[WAIT]", 6 ) ) mode = 2;
        }
        if( !isalnum(*p) ) continue;

        if( sscanf( p, "%s %s", word, data ) != 2 ) continue;

        switch( mode ) {
        case 1:
            if( !SearchID( word, -1, NULL ) ) {
                RegistID( word, ARG_CONST, AtoI(data) );
            }
            break;

        case 2:
            if( !SearchID( word, -1, NULL ) ) {
                short x = (short)(0x8000 + (unsigned short)m_waitMask_n);
                m_waitName[m_waitMask_n] = RegistID( word, ARG_CONST, x );
                m_waitMask[m_waitMask_n] = AtoL(data);
                m_waitMask_n++;
            }
            break;
        }
    }
    rf.Close();
}

void
RMain::RegistUsrVariables ( void )
{
    char line[MAX_STRING_BUFF];
    char word[MAX_STRING_BUFF];
    char data[MAX_STRING_BUFF];
    RMainFiles rf;
    int fp = rf.Open( RCODE_USER_VAR_FILE );
    if( fp < 0 ) return;

    while( rf.GetLine( line ) ) {
        char *p = IgnoreSpace(line);
        if( !isalnum(*p) ) continue;

        if( sscanf( p, "%s %s", word, data ) != 2 ) continue;

        short type_result;
        short id = SearchID( word, ARG_UNDEF, &type_result );
        if( type_result == ARG_UNDEF ) {
            AddGlobalVariable( id, AtoI(data) );
        }
    }
    rf.Close();
}


/////////////////////////////////////////////////////////////////
//
// R-CODE Dictionary
//
/////////////////////////////////////////////////////////////////

int*
RMain::FindVar ( short id, int *reset )
{
    static int dummy;
    for( short i=m_sp-1; i>=m_lb; i-- ) {
        if( m_stack[i].usr.id == id ) {
            return &(m_stack[i].usr.value);
        }
    }

    if( m_dictionary[id].type == ARG_UNDEF ) AddGlobalVariable( id, 0 );

    switch( m_dictionary[id].type ) {
    case ARG_SYSVAR:

        if( id < m_jointVar_n ) {
            OJointValue val;
            OPENR::GetJointValue( m_jointVar[id].pid, &val );
            switch( m_jointVar[id].type ) {
            case 'r':
            case 'R': m_jointVar[id].value = (short)degrees( val.value / 1000000.0 ); break;
            case 'V': m_jointVar[id].value = val.value;             break;
            case 'H': m_jointVar[id].value = val.value    >> 16;    break;
            case 'L': m_jointVar[id].value = val.value    & 0xFFFF; break;
            case 'S': m_jointVar[id].value = val.signal;            break;
            case 'P': m_jointVar[id].value = val.pwmDuty;           break;
            case 'v': m_jointVar[id].value = val.refValue;          break;
            case 'h': m_jointVar[id].value = val.refValue >> 16;    break;
            case 'l': m_jointVar[id].value = val.refValue & 0xFFFF; break;
            case 's': m_jointVar[id].value = val.refSignal;         break;
            }
        } else if( m_autoReset && reset ) {
            int i = id - m_jointVar_n;
            if( i < m_sensVar_n ) {
                switch( m_sensVar[i].type ) {
                case 'N':
                case 'L':
                case 'P':
                case 'H':
                case '0':
                case '1':
                case 'a':
                    *reset = 1;
                    break;
                }
            }
        }

        return m_stack[m_dictionary[id].data].sys.value;

    case ARG_USRVAR:
        return &(m_dictionary[id].data);

    case ARG_EXTVAR:
        return m_stack[m_dictionary[id].data].sys.value;

    case ARG_LABEL:
        return &(m_dictionary[id].data);

    case ARG_DIMVAR:
        return &(m_dictionary[id].data);

    default:
        Error( "[%s] is not defined.", m_dictionary[id].word );
        break;
    }

    return &dummy;
}

int
RMain::EvalVar ( short id )
{
    int reset = 0;
    int *p = FindVar( id, &reset );
    int  v = *p;
    if( reset ) *p = 0;
    return v;
}

short
RMain::SetVar ( short id, int var )
{
    switch( m_dictionary[id].type ) {
    case ARG_SYSVAR:
        {
            short idx = m_dictionary[id].data;

            *(FindVar(id,NULL)) = var;

            if( m_stack[idx].sys.value == &m_seed ) {
                srand( (unsigned int)m_seed );
            }
        }
        break;

    case ARG_EXTVAR:
        {
            int i,j;
            int *val = FindVar(id,NULL);
            *val = var;

            for( i=0; i<m_plugin_n; i++ ) {
                if( m_plugin[i].notify & NOTIFY_VAR ) {
                    for( j=0; j<m_plugin[i].var_n; j++ ) {
                        if( val == &(m_plugin[i].val[j]) ) {
                            PlugInVarData data;
                            data.type = PLUGIN_VAR;
                            data.n    = 1;
                            data.index[0] = j;
                            data.value[0] = var;
                            PutPlugInMsg( i, (PlugInData*)(&data) );

                            i = m_plugin_n;
                            break;
                        }
                    }
                }
            }
        }
        break;

    default:
        *(FindVar(id,NULL)) = var;
        break;
    }

    if( m_debug ) { if( m_debug == 2 || !m_execFlag ) Printf( "<SET> %d", var ); }

    return 0;
}

void
RMain::RunCommand ( char *cmd )
{
    char tmp[MAX_STRING_BUFF];
    strcpy( tmp, cmd );
    ParseCommand( tmp );
}

/* Command string analysis function (direct processing of the execution command) */
void
RMain::ParseCommand ( char *cmd )
{
    RStatement  st_buf;
    RStatement *st  = &st_buf;
    short label;

    memset( st, 0, sizeof(RStatement) );
    st->line_num = ++m_lineNum;

    while(1) {
        char *p = strstr( cmd, "\201\100" );
        if( !p ) break;
        *p++ = ' ';
        *p   = ' ';
    }

    cmd = IgnoreSpace( cmd );
    if( !isalnum(cmd[0]) && (cmd[0]!=':') ) return;

    label = ParseStatement( cmd, st );

    if( label ) {
        if( m_dictionary[label].data == -1 ) {
            m_dictionary[label].data = m_statement_n;
        } else {
            Error( "multiple label [%s].", m_dictionary[label].word );
        }
        return;
    }

    /* Process the command which is not a statement here. (EDIT/END/RUN) */
    switch( st->op1 ) {
    case mEDIT:
        ResetDic();
        ClearError();
        m_lineNum     = 0;
        m_editFlag    = LOADING_SCRIPT;
        m_errorFlag   = 0;
        m_statement_n = 0;
        m_callLevel   = 0;
        m_ip          = 0;
        if( m_debug ) Printf( "<EDIT>" );
        break;

    case mEND:
        if( m_editFlag == LOADING_SCRIPT ) {
            m_editFlag    = WAIT_SCRIPT;
            m_stringBuf_1 = m_stringBuf_n;
            m_dictionary_1 = m_dictionary_n;
            m_sp1 = m_sp;
            if( m_errorFlag ) {
                Printf( "***** ERROR *****" );
                m_statement_n = 0;
                ShowErrors();
            }
            if( m_debug ) Printf( "<END>" );
            Ack();
        }
        if( m_execFlag )    StopRun(1);
        break;

    case mRUN:
        if( m_editFlag == WAIT_SCRIPT ) Run();
        break;

    default:
        /* Store it in statement for EDIT mode. */
        if( m_editFlag == LOADING_SCRIPT ) {
            short idx;
            m_statement[m_statement_n] = st_buf;

            /* The analysis of the IF/LOOP structure */
            /* It stores so the previous index can be referred directly,
            	and processing is sped up. */
            switch( st->op1 ) {
            case mIF:
            case mIF_1ST:
                if( (st->type.arg2 == ARG_CONST) && (st->arg2 == mTHEN) ) {
                    m_statement[m_statement_n].type.arg3 = 0;
                    m_statement[m_statement_n].arg3 = -1; /* index of ELSE */
                    m_statement[m_statement_n].type.arg4 = 0;
                    m_statement[m_statement_n].arg4 = -1; /* index of ENDIF */
                }
                if( (st->type.arg2 == ARG_CONST) && (st->arg2 == mBREAK) )
                {
                    idx = SearchLoop( st->arg3 );
                    if( idx >= 0 ) {
                        /* Register index of LoopTop for IF:BREAK  */
                        m_statement[m_statement_n].arg4 = idx;
                    }
                }
                break;

            case mELSE:
                idx = SearchIF();
                if( idx >= 0 ) {
                    /* Register index of ELSE IF */
                    m_statement[idx].arg3 = m_statement_n;
                }
                break;

            case mENDIF:
                idx = SearchIF();
                if( idx >= 0 ) {
                    /* Register index of ENDIF IF */
                    m_statement[idx].arg4 = m_statement_n;
                    /* Register index of ENDIF, if there is an ELSE. */
                    if( m_statement[idx].arg3 >= 0 ) {
                        m_statement[m_statement[idx].arg3].arg4 = m_statement_n;
                    }
                }
                break;

            case mWEND:
            case mUNTIL:
            case mLOOP:
            case mNEXT:
                idx = SearchLoop(0);
                if( idx >= 0 ) {
                    /* Register index of LoopTop for LoopEnd. */
                    m_statement[m_statement_n].arg4 = idx;
                    /* Register index of LoopEnd for LoopTop. */
                    m_statement[idx].arg4 = m_statement_n;
                }
                break;

            case mBREAK:
                idx = SearchLoop( st->op2 );
                if( idx >= 0 ) {
                    /* Register index of LoopTop for BREAK. */
                    m_statement[m_statement_n].arg4 = idx;
                }
                break;
            }
            m_statement_n++;
        }
        else /* For execution mode */
        {
            ProcessStatement( st, 0 );
        }
        break;
    }
}

/* An agreement decision function with the command pattern (for PlugIn) */
/*   p command                 */
/*   q Command pattern         */
/*   n The effective command pattern  length */
int CodeComp ( char *p, char *q, int n )
{
    while( n-- ) {
        if( strchr( ": \t\n\r", *p ) ) {
            if( *q != ':' ) return 0;
            while( *p && strchr( ": \t\n\r", *p ) ) p++;
            q++;
        } else if( *p != *q ) {
            return 0;
        } else {
            p++;
            q++;
        }
    }

    return 1;
}

short
RMain::ParseStatement ( char *line, RStatement *st )
{
    int rc1;
    int rc2;

    typo = ARG_UNDEF;
    typ0 = ARG_UNDEF;
    typ1 = ARG_UNDEF;
    typ2 = ARG_UNDEF;
    typ3 = ARG_UNDEF;
    typ4 = ARG_UNDEF;

    line = IgnoreSpace( line );
    char *x = strstr( line, "//" );
    if( x ) *x = '\0';

    /* Is it a PlugIn command? */
    for( int i=0; i<m_plugin_n; i++ )
    for( int j=0; j<m_plugin[i].cmd_n; j++ ) {
        if( CodeComp( line, m_plugin[i].cmd[j], m_plugin[i].len[j] ) ) {
            char *p = m_plugin[i].cmd[j];
            char *s = p;
            int   o = 0;

            while(1) {
                if( (*p == ':') || (*p == '\0') ) {
                    char tmp[32];
                    char *q = tmp;
                    while( s < p ) *q++ = *s++;
                    *q = '\0';
                    s++;
                    if( tmp[0] == '%' ) {
                        if( tmp[1] == 's' ) {
                            switch( o ) {
                            case 1: typo = ARG_STRING;
                            case 2: typ0 = ARG_STRING;
                            case 3: typ1 = ARG_STRING;
                            case 4: typ2 = ARG_STRING;
                            case 5: typ3 = ARG_STRING;
                            case 6: typ4 = ARG_STRING;
                            }
                        }
                    }
                    o++;
                }
                if( *p == '\0' ) break;
                p++;
            }

            st->plugin_info = 0x8000 | (i<<8) | j;

            i = m_plugin_n;
            break;
        }
    }

    if( CodeComp( line, "BP:", 3 ) ) {
        st->type.bp = 1;
        line += 3;
    }

    if( !CodeComp( line, "CASE:", 5 ) ) {
        st->context = -1;
        st->type.ct = 0;
        st->type.ce = 0;
    } else {
        int tmp;
        rc1 = GetToken( line, &tmp, ARG_CONST );
        rc1 = GetToken( NULL, &tmp, ARG_UNDEF );

        if( (rc1 == ARG_CONST) && (tmp == mELSE) ) {
            st->type.ct = 0;
            st->type.ce = 1;
        } else {
            st->context = tmp;
            st->type.ct = 1;
            st->type.ce = 0;
        }
        line = NULL;
    }

    rc1 = GetToken( line, &(st->op1), ARG_CONST );

    if( rc1 == ARG_NONE ) typo = ARG_LABEL;

    if( !m_defFlag ) {
        switch( st->op1 ) {
        case mSEND:
            typ0 = ARG_STRING;
            break;

        case mPLAY:
            typ0 = ARG_STRING;
            break;

        case mPRINT:
        case mDEFP:
        case mDEFM:
            typo = ARG_STRING;
            break;

        case mGO:
        case mCALL:
        case mJT:
        case mJF:
            typo = ARG_LABEL;
            break;

        case mIF:
        case mIF_1ST:
        case mIF_AND:
        case mIF_OR:
            typ2 = typ3 = ARG_LABEL;
            break;

        case mONCALL:
            typ2 = typ4 = ARG_LABEL;
            break;

        }
    }

    rc2 = GetToken( NULL, &(st->op2), typo );

    st->type.op1 = rc1;
    st->type.op2 = rc2;

    if( (rc1 == ARG_NONE) && (rc2 != ARG_NONE) ) return st->op2; // label

    if( !m_defFlag ) {
        switch( st->op1 ) {
        case mPLAY:
            switch( st->op2 ) {
            case tACTION:
            case tACTIONX:
                typ0 = ARG_ACTION;
                break;

            case tCOMMAND:
                typ1 = typ2 = ARG_STRING;
                break;

            case tMWCID:
                typ0 = ARG_LOWORD;
                typ1 = ARG_HIWORD;
                break;

            case tLMS:
                typ0 = typ1 = typ2 = ARG_STRING;
                break;

            case tLMSID:
                typ0 = typ1 = typ2 = ARG_LOWORD;
                break;
            }
            break;

        case mSET:
            if( st->type.arg4 ) {
                typ1 = typ2 = typ3 = typ4 = ARG_STRING;
            }
            break;

        case mSEND:
            switch( st->op2 ) {
            case tAPPID:
                typ0 = ARG_LOWORD;
                typ1 = ARG_HIWORD;
                break;
            case tAPPTAG:
                typ0 = ARG_APPTAG;
                break;
            }
            break;
        }
    }

    st->type.arg0 = GetToken( NULL, &(st->arg0), typ0 );
    st->type.arg1 = GetToken( NULL, &(st->arg1), typ1 );
    st->type.arg2 = GetToken( NULL, &(st->arg2), typ2 );

    if( !m_defFlag ) {
        switch( st->op1 ) {
        case mIF:
        case mIF_1ST:
            if( (st->type.arg2 == ARG_CONST) && (st->arg2 == mBREAK) ) {
                typ3 = ARG_UNDEF;
            }
            break;
        }
    }

    st->type.arg3 = GetToken( NULL, &(st->arg3), typ3 );
    st->type.arg4 = GetToken( NULL, &(st->arg4), typ4 );

    /* arg5(Wait flag) (not used) */
    st->type.wait = ((GetToken( NULL, NULL, 0 ))?(1):(0));

    /* Region merge of WAIT/STOP/QUIT */
    /* Adjust the conditional expression arrangement ( <v1><op><v2> -> <op><v1><v2> ) */
    if( !m_defFlag ) {
        switch( st->op1 ) {
        case mWAIT:
        case mSTOP:
        case mQUIT:
            if( st->type.arg0 == ARG_CONST ) st->op2 |= st->arg0;
            if( st->type.arg1 == ARG_CONST ) st->op2 |= st->arg1;
            if( st->type.arg2 == ARG_CONST ) st->op2 |= st->arg2;
            if( st->type.arg3 == ARG_CONST ) st->op2 |= st->arg3;
            if( st->type.arg4 == ARG_CONST ) st->op2 |= st->arg4;
            break;

        case mIF:
        case mIF_1ST:
        case mIF_AND:
        case mIF_OR:
        case mCSET:
        case mWHILE:
        case mUNTIL:
        case mONCALL:
            if( (st->type.arg0 != ARG_NONE) && (st->type.op2 != ARG_CONST) ) {
                short type    = st->type.op2;
                int data      = st->op2;
                st->type.op2  = st->type.arg0;
                st->op2       = st->arg0;
                st->type.arg0 = type;
                st->arg0      = data;
            }
            break;

        case mDO:
        case mLOOP:
            if( st->type.arg0 != ARG_CONST ) {
                short type    = st->type.arg0;
                int data      = st->arg0;
                st->type.arg0 = st->type.arg1;
                st->arg0      = st->arg1;
                st->type.arg1 = type;
                st->arg1      = data;
            }
            break;
        }
    }

    return 0;
}

/* The function to make a Command string to send to PlugIn */
void
RMain::AddWord ( char *cmd, short type, short x )
{
    if( type == ARG_NONE ) return;
    if( *cmd ) strcat( cmd, ":" );
    if( type == ARG_STRING ) {
        strcat( cmd, IDtoWord(x) );
    } else {
        char tmp[32];
        sprintf( tmp, "%d", ((IMM(type))?(x):(EvalVar(x))) );
        strcat( cmd, tmp );
    }
}

short
RMain::AutoWait( void )
{
    int ret = 0;
    if( m_errorFlag ) return ret;

    if( (( m_execFlag ) && ( m_waitMode == 0 )) ||
        ( m_wlanWait != NO_WAIT_DATA_WLAN )   ) {
#ifdef _RMAIN_DEBUG_
        DPRINTF(("AutoWait:%d\n", m_execFlag ));
#endif // _RMAIN_DEBUG_
        m_execFlag = PAUSE_EXEC;
        ret = 1;
    }
    return ret;
}

/* The statement execution function  */
int
RMain::ProcessStatement ( RStatement *st, int st_idx )
{
    RMessage msg_buf;
    RMessage *msg = &msg_buf;
    int op2;

#ifdef _MACRO_
    m_macroFlag = 0;
#endif // _MACRO_

    if( st->type.ct ) {
        if( st->context != m_ct ) return 0;
        m_caseFlag = 1;
    } else if( st->type.ce ) {
        if( m_caseFlag ) return 0;
    } else {
        m_caseFlag = 0;
    }

    if( st->op1 == mCSET ) {
        if( m_csetFlag == 1 ) return 0;
    } else {
        m_csetFlag = 0;
    }

    /* Time Variable Update */
    UpdateTimeVariable();

    if( (m_trace & TRACE_LINE) && m_execFlag && st->line_num ) {
        Printf( "#%05d", st->line_num );
    }

    if( st->plugin_info ) {
        int i = (st->plugin_info & 0x7F00)>>8;
        int j = (st->plugin_info & 0x00FF);
        char cmd[MAX_STRING_BUFF];
        char *p = m_plugin[i].cmd[j];
        char *q = cmd;
        int   o = 0;

        while( *p && (*p != '%') ) {
            if( *p == ':' ) o++;
            *q++ = *p++;
        }
        if( *p == '%' ) q--;
        *q = '\0';

        switch( o ) {
        case 0: AddWord( cmd, st->type.op1,  st->op1  );
        case 1: AddWord( cmd, st->type.op2,  st->op2  );
        case 2: AddWord( cmd, st->type.arg0, st->arg0 );
        case 3: AddWord( cmd, st->type.arg1, st->arg1 );
        case 4: AddWord( cmd, st->type.arg2, st->arg2 );
        case 5: AddWord( cmd, st->type.arg3, st->arg3 );
        case 6: AddWord( cmd, st->type.arg4, st->arg4 );
        }

        PlugInMsgData data;
        data.type = PLUGIN_MSG;
        data.mode = MSG_R_CODE;
        strcpy( data.msg, cmd );
        PutPlugInMsg( i, (PlugInData*)(&data) );

        AutoWait();
    } else

    switch( st->op1 ) {
    case mINIT:
        if( (st->type.op2 == ARG_CONST) && (st->op2 == tMTN) ) {
            InitMtn();
        } else {
            switch( (op2=OP2) ) {
            case 0: // Normal Boot
                if( Fstat( RCODE ) == 0 ) {
                    Printf( "Loading R-CODE.R ..." );
                    LoadProgram( RCODE );
                    Printf( "Done." );
                    m_pc = 0;
                    m_ct = 0;
                    m_sp = m_sp0;
                    m_lb = m_sp0;
                    m_syncFlag  = 0;
                    m_waiting   = 0;
                    m_waitPart  = 0;
                    m_interrupt = 0;
                    m_autoExec  = RUN_AUTO;
                    m_stepMode  = 0;
                    m_execStop  = 0;
                    Printf( "AutoStart" );
                }
                break;

            case 3: // WLAN mode
                m_aiboId = st->arg0;
                ip_addr[0] = st->arg1;
                ip_addr[1] = st->arg2;
                ip_addr[2] = st->arg3;
                ip_addr[3] = st->arg4;
                if( m_execFlag ) {
#ifdef _MACRO_
                    m_macro_i      = 0;
                    m_macro_n      = 0;
#endif // _MACRO_
                    m_execFlag  = WAIT_EXEC;
                    m_execStop  = 0;
                    m_waiting   = 0;
                    m_waitPart  = 0;
                    m_clockDown = 0;
                    m_trace     = 0;
                    m_debug     = 0;
                    StopAIBO(2);
                }
                m_autoExec = WLAN_AUTO;
                m_wlanFlag = AVAILABLE_WLANFLAG;
                m_stepMode = 0;
                m_execStop = 0;
                break;

            case 4: // WLAN disconnect
                m_wlanFlag = NOT_AVAILABLE_WLANFLAG;
                m_stepMode = 0;
                m_execStop = 0;
                ClrSysMsg();
                break;

            case 1: // R-CODE Pro mode
                m_trace = 0;
                m_debug = 1;
                break;

            case 2: // Clear Dictionary
                if( ( m_editFlag == WAIT_SCRIPT ) &&
                    ( m_execFlag == WAIT_EXEC   )) {
                    ClearDic();
                    if( m_debug ) Printf( "<INIT>" );
                }
                break;

            case 9: // Reboot
                OPENR::Shutdown( OBootCondition( obcbBOOT_TIMER, 1, obcbttRELATIVE ) );
                break;

            case 901: // JUMP_TO_NT
                if( (m_mwcPos != mwcSTATION) &&
                    (m_mwcPos != mwcPSTATION) &&
                    (m_mwcPos != mwcLIFT) &&
                    (m_mwcPos != mwcTEMPORARY_POSTURE) )
                {
                    CCCommandQueu ccc( CCCC_JUMP_TO_NT, observer[obsComplete]->GetID());
                    PutCCCMsg( &ccc, 0 );
                }
                break;
            }

            for( int i=0; i<m_plugin_n; i++ ) {
                if( m_plugin[i].notify & NOTIFY_INIT ) {
                    PlugInMsgData data;
                    data.type = PLUGIN_MSG;
                    data.mode = MSG_R_CODE;
                    sprintf( data.msg, "INIT:%d", op2 );
                    PutPlugInMsg( i, (PlugInData*)(&data) );
                }
            }
        }
        break;

    case mDUMP:
        switch( (op2=OP2) ) {
        case 1: DumpCDB(); break;
        case 2: DumpIDB(); break;
        case 3: DumpDIC(); break;
        }
        break;

    case mSTEP:
        m_stepMode = OP2;
        if( m_debug ) Printf( "<STEP> %d", m_stepMode );
        break;

    case mLOAD:
        if( st->type.op2 == ARG_CONST ) {
                switch( st->op2 ) {
                case tMTN: LoadMtn( IDtoWord(st->arg0)           ); break;
                case tLED: LoadLED( IDtoWord(st->arg0)           ); break;
                case tSND: LoadSND( IDtoWord(st->arg0), st->arg1 ); break;
                }
        } else
            Error( "LOAD command is UNDER CONSTRUCTION !\n" );
        break;

    case mSAVE:
        Error( "SAVE command is UNDER CONSTRUCTION !\n" );
        break;

    case mWAIT:
#ifdef _RMAIN_DEBUG_
    DPRINTF(("    WAIT:%d,%d,%d,%d,%d,%d,%d,%d\n", m_execFlag, m_waitMode, m_wlanWait,
             m_cccBusy, m_waiting, m_wait, m_waitPart, m_waitMode ));
#endif // _RMAIN_DEBUG_

        if( (op2=OP2) ) {
            if( op2 > 0 ) {
                short x = op2 / 32;
                if( m_trace & TRACE_CMD ) Printf( "Wait %d[ms]...", op2 );
                if( x == 0 ) x = 1;
                m_clockDown = x;
                return 1;
            } else {
                int i = op2 & 0x7FFF;
                if( (m_wait > 0) && (i < m_waitMask_n) ) {
                    if( m_trace & TRACE_CMD ) {
                        Printf( "Wait %s...", IDtoWord(m_waitName[i]) );
                    }
                    m_waitPart = m_waitMask[i];
                    m_waiting   = 1; //tANY;
                    return 1;
                }
            }
        } else if( m_wait > 0 ) {
            if( m_trace & TRACE_CMD ) Printf( "Wait..." );
            m_waitPart = 0;
            m_waiting  = 1; //tANY;
            return 1;
        }
        break;

    case mSYNC:
        m_syncFlag = 1;
        Syn();
        return 1;

    case mGO:
        JumpTo( st->type.op2, OP2 );
        break;

    case mIF:     EvalIf( st, msg, 0 ); AutoWait();  break;
    case mIF_1ST: EvalIf( st, msg, 0 );              break;
    case mIF_AND:
        if(  m_lastCond ) EvalIf( st, msg, 0 );
        break;

    case mIF_OR:
        if( !m_lastCond ) EvalIf( st, msg, 0 );
        else              EvalIf( st, msg, 1 );
        break;

    case mELSE:
        if( st->arg4 ) {
            m_pc = st->arg4 + 1;
        } else {
            Error( "can't find ENDIF." );
            return 1;
        }
        break;

    case mENDIF:  break;
    case mGLOBAL:
        AddGlobalVariable( st->op2, ARG0 );
        break;

    case mLOCAL:
        AddLocalVariable( st->op2, ARG0 );
        break;

    case mSCOPEIN:  ScopeIn(0); break;
    case mSCOPEOUT: ScopeOut(); break;

    case mCALL:
        CallFunction( st->arg0 );
        JumpTo( st->type.op2, OP2 );
        break;

    case mRETURN:
        {
            int x = OP2;
            ReturnFunction();
            if( st->type.op2 ) PushStack( x );
        }
        break;

    case mRET:
        {
            int x = OP2;
            ReturnFunction();
            if( x ) m_ct = x;
        }
        break;

    case mONCALL:
        // st->op2 > 0 )
        if( st->type.arg0 != ARG_NONE ) {
            m_on_call[m_ip].pc = st_idx;
            m_on_call[m_ip].ct = m_ct;
            m_on_call[m_ip].ip = m_ip;
            m_on_call[m_ip].sp = m_sp;
            m_on_call[m_ip].lb = m_lb;
            m_on_call[m_ip].lv = m_callLevel;
            m_ip++;
        } else {
            m_ip += st->op2;
        }
        break;

    case mRESUME:
        ReturnFunction();
        m_interrupt--;
        if( m_interrupt == 0 ) {
            RSnapInfo *snap = m_on_call + m_on_call_i;
            RStatement *st = m_statement + snap->pc;
            switch( ARG3 ) {
            case 0:
                break;
            case 1:
                JumpTo( st->type.arg4, ARG4 );
                break;
            case 2:
                ResetEnv( m_on_call_i, 1 );
                break;
            case 3:
                ResetEnv( m_on_call_i, 0 );
                JumpTo( st->type.arg4, ARG4 );
                break;
            case 4:
                ResetEnv( -1,          1 );
                break;
            case 5:
                ResetEnv( -1,          0 );
                JumpTo( st->type.arg4, ARG4 );
                break;
            }
        }
        break;

    case mEXIT: StopRun(1); return 1;
    case mHALT: HaltRun(1); return 1;

    case mPUSH:
        PushStack( OP2 );
        break;

    case mPOP:
        {
        int r = PopStack();
        if( st->type.op2 ) {
            if( SetVar( st->op2, r ) ) return 1;
        }
        }
        break;

    case mDUP:
        DupStack();
        break;

    case mARG:
        ArgStack( st->op2 );
        break;

    case mJT:
        {
            int r = PopStack();
            if( r != 0 ) JumpTo( st->type.op2, OP2 );
        }
        break;
    case mJF:
        {
            int r = PopStack();
            if( r == 0 ) JumpTo( st->type.op2, OP2 );
        }
        break;

    case mFETCH:
        m_execFlag = PAUSE_EXEC;
        return 1;

    case mSET:
        if( st->type.op2 == ARG_CONST ) {
        } else if( (m_dictionary[st->op2].type == ARG_SYSVAR) &&
                   (m_dictionary[st->op2].data < m_jointVar_n) ) {
            SETX( st );
        } else if( SetVar( st->op2, ARG0 ) ) return 1;
        break;

    case mDIM:
        if( !m_dimVariable_n ) {
            DPRINTF(("NEWDIM:st-op2:%s:arg0:%d\n", IDtoWord(st->op2), st->arg0 ));
            m_dimVariable_n = st->arg0;
            int mem_size = 0;
            mem_size = sizeof(RDimVariable) * m_dimVariable_n;
            if ((NewRegion(mem_size, (void **)&m_dimVariable)) != sSUCCESS) {
                OSYSLOG1((osyslogERROR, "Error NewRegion: m_dimVariable"));
                return 1;
            }
            memset( m_dimVariable, 0, mem_size );
            for(int i=0; i < m_dimVariable_n; i++ ) {
                m_dimVariable[i].id = st->op2;
                m_dimVariable[i].index = i;
            }
            AddDIMVariable( st->op2, ARG0 );
        } else {
            int ret = RenewDim( st->arg0, st->op2 );
            if( ret ) return 1;
            VAR = ARG0;
        }
        break;

    case mDGET:
        for( int i = 0;i < m_dimVariable_n; i++ ) {
            if( m_dimVariable[i].id    == st->arg0 &&
                m_dimVariable[i].index == st->arg1  ) {
                SetVar( st->op2, m_dimVariable[i].value );
            }
        }
        break;

    case mDSET:
        if( st->arg0 <= m_dictionary[st->op2].data ) {
                for( int i = 0; i< m_dimVariable_n; i++ ) {
                    if ( m_dimVariable[i].id    == st->op2 &&
                         m_dimVariable[i].index == st->arg0 ) {
                        m_dimVariable[i].value = st->arg1;
                    }
                }
        } else {
            Error( "index out of range." );
        }
        break;

    case mDFIND:
        SetVar( st->op2, -1 );
        for( int i = 0; i < m_dimVariable_n; i++ ) {
            if ( m_dimVariable[i].id    == st->arg0 &&
                 m_dimVariable[i].value == st->arg1  ) {
                SetVar( st->op2, m_dimVariable[i].index );
            }
        }
        break;

    case mLET:
        VAR = ARG0;
        break;

    case mGET:
        if( m_debug ) {
            if( m_debug == 2 || !m_execFlag ) Printf( "<GET> %d", OP2 );
        } else {
            Printf( "%d", OP2 );
        }
        break;

    case mCLR:
        if( st->type.op2 == ARG_CONST ) {
            switch( OP2 ) {
            case 0: // SENSORS
                for( int i=0; i<m_sensVar_n; i++ ) {
                    switch( m_sensVar[i].type ) {
                    case 'N':
                    case 'L':
                    case 'P':
                    case 'H':
                    case '0':
                    case '1':
                    case 'a':
                        m_sensVar[i].value = 0;
                        break;
                    }
                }
                break;
            }
        }
        break;

    case mADD:  if( st->type.op2 ) VAR += ARG0;         else OpStack2( st->op1 ); break;
    case mSUB:  if( st->type.op2 ) VAR -= ARG0;         else OpStack2( st->op1 ); break;
    case mMUL:  if( st->type.op2 ) VAR *= ARG0;         else OpStack2( st->op1 ); break;
    case mDIV:  if( st->type.op2 ) VAR /= ARG0;         else OpStack2( st->op1 ); break;
    case mMOD:  if( st->type.op2 ) VAR %= ARG0;         else OpStack2( st->op1 ); break;
    case mAND:  if( st->type.op2 ) VAR &= ARG0;         else OpStack2( st->op1 ); break;
    case mIOR:  if( st->type.op2 ) VAR |= ARG0;         else OpStack2( st->op1 ); break;
    case mXOR:  if( st->type.op2 ) VAR ^= ARG0;         else OpStack2( st->op1 ); break;
    case mNOT:  if( st->type.op2 ) VAR  = ~OP2;         else OpStack1( st->op1 ); break;
    case mLAND: if( st->type.op2 ) VAR  = OP2 && ARG0;  else OpStack2( st->op1 ); break;
    case mLIOR: if( st->type.op2 ) VAR  = OP2 || ARG0;  else OpStack2( st->op1 ); break;
    case mLNOT: if( st->type.op2 ) VAR  = !OP2;         else OpStack1( st->op1 ); break;

    case mEQ:
    case mNE:
    case mLT:
    case mLE:
    case mGT:
    case mGE:
        OpStack2( st->op1 );
        break;

    case mRND:
        {
            int r   = rand();
            if( st->type.arg1 ) {
                int from = ARG0;
                int to   = ARG1;
                int s      = from + r * (to - from + 1) / RAND_MAX;
                if( s > to ) s = to;
                VAR = s;
            } else if( st->type.arg0 ) {
                int from = OP2;
                int to   = ARG0;
                int s      = from + r * (to - from + 1) / RAND_MAX;
                if( s > to ) s = to;
                PushStack( s );
            } else {
                int from = 0;
                int to   = OP2;
                int s      = from + r * (to - from + 1) / RAND_MAX;
                if( s > to ) s = to;
                PushStack( s );
            }
        }
        break;

    case mSWITCH:
        m_ct = OP2;
        break;

    case mCSET:
        ExpandStatementToMessage( st, msg );
        {
            if( EvalCond( msg->type, msg->arg0, msg->arg1 ) ) {
                m_ct = msg->arg2;
                m_csetFlag = 1;
            }
        }
        break;

    case mWHILE:
        ExpandStatementToMessage( st, msg );
        if( !EvalCond( msg->type, msg->arg0, msg->arg1 ) )
        {
            if( st->arg4 >= 0 ) m_pc = st->arg4 + 1;
            else Error( "can't find WEND." );
        }
        AutoWait();
        break;

    case mWEND:
        if( st->arg4 >= 0 ) m_pc = st->arg4;
        else Error( "can't find WHILE." );
        break;

    case mREPEAT:
        break;

    case mUNTIL:
        ExpandStatementToMessage( st, msg );
        if( !EvalCond( msg->type, msg->arg0, msg->arg1 ) ) {
            if( st->arg4 >= 0 ) m_pc = st->arg4;
            else Error( "can't find REPEAT." );
        }
        AutoWait();
        break;

    case mDO:
        {
            int flag = 1;
            ExpandStatementToMessage( st, msg );
            switch( msg->type ) {
            case mUNTIL:
                flag = 0;
                /* FALLTHRU */
            case mWHILE:
                if( !((EvalCond( msg->arg0, msg->arg1, msg->arg2 ))?(flag):(!flag)) ) {
                    if( st->arg4 >= 0 ) m_pc = st->arg4 + 1;
                    else Error( "can't find LOOP." );
                }
            default:     AutoWait();
                break;
            }
        }
        break;

    case mLOOP:
        {
            int flag = 1;
            ExpandStatementToMessage( st, msg );
            switch( msg->type ) {
            case mUNTIL:
                flag = 0;
                /* FALLTHRU */
            case mWHILE:
                if( ((EvalCond( msg->arg0, msg->arg1, msg->arg2 ))?(flag):(!flag)) ) {
                    if( st->arg4 >= 0 ) m_pc = st->arg4;
                    else Error( "can't find DO." );
                }
                AutoWait();
                break;
            default:
                if( st->arg4 >= 0 ) m_pc = st->arg4;
                else Error( "can't find DO." );
                break;
            }
        }
        break;

    case mFOR:
        if( st->type.arg2 == ARG_NONE ) st->arg2 = 1;
        SetVar( st->op2, ARG0 );
        ExpandStatementToMessage( st, msg );
        if( !EvalCond( ((msg->arg2 > 0)?(OpLE):(OpGE)), OP2, msg->arg1 ) ) {
            if( st->arg4 >= 0 ) m_pc = st->arg4 + 1;
            else Error( "can't find NEXT." );
        }
        AutoWait();
        break;
    case mNEXT:
        {
            int x = st->arg4;
            if( x >= 0 ) {
                st = &(m_statement[x]);
                ExpandStatementToMessage( st, msg );
                VAR += msg->arg2;
                if( EvalCond( ((msg->arg2 > 0)?(OpLE):(OpGE)), OP2, msg->arg1 ) ) {
                    m_pc = x + 1;
                }
            } else Error( "can't find FOR." );
            AutoWait();
        }
        break;

    case mBREAK:
        if( st->arg4 >= 0 ) {
            int ep = m_statement[st->arg4].arg4;
            if( ep >= 0 ) m_pc = ep + 1;
            else Error( "can't find LoopEnd." );
        }
        else Error( "can't find LoopTop." );
        break;

    case mPRINT:
        {
            char fmt[1024];
            strcpy( fmt, IDtoWord( st->op2 ) );
            Printf( fmt, ARG0, ARG1, ARG2, ARG3, ARG4 );
        }
        break;

    case mDEBUG:
        {
        }
        break;

    case mVSAVE:
        if ( st->arg0 == 1 ) {
            Printf("Reserved Variable:%s", IDtoWord(st->arg0));
            AddReservedVar ( st->op2 );
        }
        else SaveVar( st->op2 );
        break;

    case mVLOAD:
        LoadVar( st->op2 );
        break;

    case mDSAVE:
        if ( st->arg0 == 1 ) {
            Printf("Reserved Variable:%s", IDtoWord(st->arg0));
            AddReservedVar( st->op2 );
        } else SaveDimVar( st->op2 );
        break;

    case mDLOAD:
        LoadDimVar( st->op2 );
        break;

    case mVDUMP:
        {
            if( m_dictionary[st->op2].type == ARG_DIMVAR ) {
                int ind=0;
                for( int i=0; i<m_dimVariable_n; i++ ) {
                    if( m_dimVariable[i].id == st->op2 ) {
                        Printf("DIM ID:%s INDEX:%d Value:%d",
                               IDtoWord(m_dimVariable[i].id),
                               m_dimVariable[i].index,
                               m_dimVariable[i].value );
                        ind++;
                    }
                }
            if ( !ind )
                Printf("DIM ID:%s nothing variable",
                       IDtoWord(m_dimVariable[st->op2].id));
            } else {
                /* Invalid auto_reser */
                int *p = FindVar( st->op2, NULL );
                int  v = ((p)?(*p):(0));
                Printf( "%s = %d", IDtoWord( st->op2 ), v );
            }
        }
        break;

    default:
#ifdef _MACRO_
        if( m_macroFlag ) break;
#endif // _MACRO_

        ExpandStatementToMessage( st, msg );
        ProcessMessage( msg );

        switch( msg->mode ) {
        case mPLAY:
            if( m_execFlag ) {
                AutoWait();
            }
            break;
        }
        break;
    }

#ifdef _MACRO_
    while( m_macro_i < m_macro_n ) {
        if( ProcessStatement( &(m_macro[m_macro_i++]), 0 ) ) return 1; //break;
    }
#endif // _MACRO_

    return 0;
}

void
RMain::ProcessMessage ( RMessage *msg )
{
    switch( msg->type ) {
    default:
        switch( msg->mode ) {
        case mNONE: /* do nothing */       break;
        case mPLAY: CommandPLAY( msg, 0 ); break;
        case mSTOP: CommandSTOP( msg, 0 ); break;
        case mQUIT: CommandSTOP( msg, 1 ); break;
        case mSEND: CommandSEND( msg    ); break;
        default: Error( "unknown mode [%d].", msg->mode );
        }
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
//
// Config stuff
//
/////////////////////////////////////////////////////////////////
int
RMain::LineCount ( const char *path )
{
    int n = 0;
    RMainFiles rf;
    char line[MAX_STRING_BUFF];
    int fp = rf.Open( path );
    if( fp < 0 ) return 0;

    while( rf.GetLine( line ) ) {
        char *p = IgnoreSpace(line);
        if( !isalnum(*p) ) continue;
        n++;
    }
    rf.Close();
    return n;
}

void
RMain::ReadActSub ( const char *path )
{
    char line[MAX_STRING_BUFF];
    char buf1[64];
    char buf2[16];
    char buf3[16];

    RMainFiles rf;
    int fp = rf.Open( path );
    if( fp < 0 ) return;

    while( rf.GetLine( line ) ) {
        char *p = IgnoreSpace(line);
        if( !isalnum(*p) ) continue;

        sscanf( p, "%s %s %s", buf1, buf2, buf3 );

        strcpy( m_action[m_action_n].name, buf1 );
        m_action[m_action_n].cccid = AtoL(buf2);
        m_action[m_action_n].type  = AtoI(buf3);
        m_action_n++;
    }

    rf.Close();
}

void
RMain::ReadAction ( void )
{
    int n = 0;
    n += LineCount( RCODE_ACTION_AMS_FILE );
    n += LineCount( RCODE_ACTION_CFG_FILE );
    n += LineCount( RCODE_ACTION_EXT_FILE );
    m_action = new Action[n];
    if( !m_action ) {
        Error( "can't alloc action!" );
        return;
    }
    ReadActSub( RCODE_ACTION_AMS_FILE );
    ReadActSub( RCODE_ACTION_CFG_FILE );
    ReadActSub( RCODE_ACTION_EXT_FILE );
}

typedef struct
{
    char          PRM[32];
    int           index;
    unsigned long mask;
}
PRM;

void
RMain::ReadJoint ( void )
{
    char line[MAX_STRING_BUFF];
    char buf1[64];
    char buf2[64];
    char buf3[16];
    OPrimitiveID pid;
    OJointValue  val;
    int   i;

    PRM prm[64];
    int prm_n = 0;
    int index = 0;
    int flag  = 0;
    RMainFiles rf;

    int fx = rf.Open(  RCODE_RREPLAY_FILE );
    if( fx < 0 ) return;

    while( rf.GetLine( line ) ) {
        char *p = IgnoreSpace(line);
        if( *p == '[' ) {
            if( !strncmp( p, "[JOINT]", 7 ) ) { flag = 1; index = 0; continue; }
            else if( !strncmp( p, "[EAR]",   5 ) ) { flag = 1;            continue; }
            else if( !strncmp( p, "[LED]",   5 ) ) { flag = 1; index = 0; continue; }
            else {
                flag = 0;
            }
        }
        if( (flag == 0) || (!isalnum(*p)) ) continue;

        sscanf( p, "%s", prm[prm_n].PRM );
        prm[prm_n].index = index;
        prm[prm_n].mask  = (1 << index);
        prm_n++;
        index++;
    }
    rf.Close();


    int n = LineCount( RCODE_JOINT_FILE );
    m_jointVar = new JointVar[n];
    if( !m_jointVar ) {
        Error( "can't alloc m_jointVar!" );
        return;
    }

    RMainFiles rfjoin;
    int fp = rfjoin.Open( RCODE_JOINT_FILE );
    if( fp < 0 ) return;

    while( rfjoin.GetLine( line ) ) {
        char *p = IgnoreSpace(line);
        if( !isalnum(*p) ) continue;

        sscanf( p, "%s %s %s", buf1, buf2, buf3 );

        if( OPENR::OpenPrimitive( buf2, &pid ) == oSUCCESS ) {
            strcpy( m_jointVar[m_jointVar_n].name, buf1 );

            m_jointVar[m_jointVar_n].index = 0;
            m_jointVar[m_jointVar_n].mask  = 0;
            for( i=0; i<prm_n; i++ ) {
                if( !strcmp( buf2, prm[i].PRM ) ) {
                    m_jointVar[m_jointVar_n].index = prm[i].index;
                    m_jointVar[m_jointVar_n].mask  = prm[i].mask;
                    break;
                }
            }
            m_jointVar[m_jointVar_n].pid   = pid;
            m_jointVar[m_jointVar_n].type  = buf3[0];
            m_jointVar[m_jointVar_n].value = 0;

            OPENR::NewCommandVectorData( 1,
                                         &(m_jointVar[m_jointVar_n].cmdVecID),
                                         &(m_jointVar[m_jointVar_n].cmdVec) );
            m_jointVar[m_jointVar_n].cmdVec->SetNumData(1);
            OCommandInfo* info = m_jointVar[m_jointVar_n].cmdVec->GetInfo(0);
            switch( buf3[0] ) {
            case '3': info->Set( odataJOINT_COMMAND3, pid, 1 ); break;
            case '2': info->Set( odataLED_COMMAND2,   pid, 1 ); break;
            default : info->Set( odataJOINT_COMMAND2, pid, 1 ); break;
            }

            m_jointVar_n++;
        } else {
            OSYSLOG1((osyslogWARNING, "can't open primitive [%s].\n", buf2 ));
        }
    }
    rfjoin.Close();
}


void
RMain::ReadSensor ( void )
{
    char line[MAX_STRING_BUFF];
    char buf1[64];
    char buf2[64];
    char buf3[64];
    char buf4a[64];
    char buf4b[64];
    short validSubInfo;
    int flag=0;
    RMainFiles rf;

    int n = LineCount( RCODE_SENSOR_FILE );
    m_sensVar = new SensVar[n];
    if( !m_sensVar ) {
        Error( "can't alloc m_sensVar!" );
        return;
    }

    int fp = rf.Open( RCODE_SENSOR_FILE );
    if( fp < 0 ) return;

    while( rf.GetLine( line ) ) {
        char *p = IgnoreSpace(line);

        if( *p == '[' ) {
            if ( !strncmp( p, "[SENSOR]", 8 ) ) { flag = 1; }
            else { flag = 0; }
        }

        if( !isalnum(*p) ) continue;

        sscanf( p, "%s %s %s", buf1, buf2, buf3 );

        if ( strstr( buf2, ":") == NULL && flag == 1 ) {
            Error( "Old Sensor config file!" );
            rf.Close();
            return;
        }

        validSubInfo = 0;
        m_sensVar[m_sensVar_n].value = 0;
        m_sensVar[m_sensVar_n].subinfo = 0;
        m_sensVar[m_sensVar_n].validSubInfo = 0;
        m_sensVar[m_sensVar_n].modeID = LEGACY_UNKNOWN;

        if ( flag == 1 ) {
            /* S: Semantics ID / I: MWIID */
            if( !strncmp( buf2, "S:", 2)) { m_sensVar[m_sensVar_n].modeID = LEGACY_SEM_CONFIG; }
            else                          { m_sensVar[m_sensVar_n].modeID = LEGACY_CONFIG; }
            memcpy( buf2, &buf2[2], sizeof(buf2)-2 );
        }

        char* pdest;
        int result;
        pdest = strstr( buf2, "." );
        result = pdest - buf2 + 1;
        if( pdest == NULL ) {
            if( isalpha(buf2[0]) ) {
                m_sensVar[m_sensVar_n].semOrmwiid = g_labels.GetSemanticsID(buf2);
            } else {
                m_sensVar[m_sensVar_n].semOrmwiid = AtoL(buf2);
            }
        } else {
            memset(buf4a,0,sizeof(buf4a));
            memset(buf4b,0,sizeof(buf4b));
            memcpy(buf4a, buf2, result-1 );
            memcpy(buf4b, &buf2[result], sizeof(buf2)-result);

            if( isalpha(buf4a[0]) ) {
                m_sensVar[m_sensVar_n].semOrmwiid = g_labels.GetSemanticsID(buf4a);
            } else {
                m_sensVar[m_sensVar_n].semOrmwiid = AtoL(buf4a);
            }

            if( isalpha(buf4b[0]) ) {
                if     ( strcmp( buf4b, "SUBINFO1" ) == 0 ) validSubInfo |= SENS_SUBINFO1;
                else if( strcmp( buf4b, "SUBINFO2" ) == 0 ) validSubInfo |= SENS_SUBINFO2;
                else if( strcmp( buf4b, "SUBINFO3" ) == 0 ) validSubInfo |= SENS_SUBINFO3;
                else if( strcmp( buf4b, "SUBINFO4" ) == 0 ) validSubInfo |= SENS_SUBINFO4;
                else m_sensVar[m_sensVar_n].subinfo = g_labels.GetSubInfoID(buf4b);
            } else {
                m_sensVar[m_sensVar_n].subinfo = AtoL(buf4b);
            }

            validSubInfo |= SENS_VALIDSUB;
            m_sensVar[m_sensVar_n].validSubInfo = validSubInfo;
        }

        strcpy( m_sensVar[m_sensVar_n].name, buf1 );
        m_sensVar[m_sensVar_n].type  = buf3[0];
        m_sensVar_n++;
    }

    rf.Close();
}

void
RMain::ReadAppTag ( void )
{
    char line[MAX_STRING_BUFF];
    char buf1[64];
    char buf2[32];
    char buf3[32];
    char buf4[32];
    char buf5[32];
    char buf6[32];
    char buf7[32];
    RMainFiles rf;

    int n = LineCount( RCODE_APPTAG_FILE );
    m_apptag = new AppTagVar[n];
    if( !m_apptag ) {
        Error( "can't alloc m_apptag_var!" );
        return;
    }

    int fp = rf.Open( RCODE_APPTAG_FILE );
    if( fp < 0 ) return;

    while( rf.GetLine( line ) ) {
        char *p = IgnoreSpace(line);
        if( !isalnum(*p) ) continue;

        char *x = strstr( line, "//" );
        if( x ) *x = '\0';

        buf1[0]=buf2[0]=buf3[0]=buf4[0]=buf5[0]=buf6[0]=buf7[0]='\0';
        sscanf( p, "%s %s %s %s %s %s %s", buf1, buf2, buf3, buf4, buf5, buf6, buf7 );

        if( !strncmp( p, "[Version]", 9 ) ) {
            OSYSLOG1((osyslogWARNING, "AppTag.cfg Version:%s, %s ", buf2, buf3));
            continue;
        }

        strcpy( m_apptag[m_apptag_n].name, buf1 );
        m_apptag[m_apptag_n].appcomid = AtoI(buf2);
        m_apptag[m_apptag_n].subinfo.SetMember(0, AtoI(buf3));
        m_apptag[m_apptag_n].subinfo.SetMember(1, AtoI(buf4));
        m_apptag[m_apptag_n].subinfo.SetMember(2, AtoI(buf5));
        m_apptag[m_apptag_n].subinfo.SetMember(3, AtoI(buf6));
        m_apptag[m_apptag_n].sem_id = g_labels.GetSemanticsID(buf7);
        m_apptag_n++;
    }

    rf.Close();
}

void
RMain::ReadLMSCom ( void )
{
    char line[MAX_STRING_BUFF];
    char buf1[32+1];
    char buf2[32+1];
    char buf3[32+1];
    char buf4[32];
    RMainFiles rf;

    int n = LineCount( RCODE_LMSCOMMAND_FILE );
    if( !n ) return;

    m_lmscom = new LMSComVar[n];
    if( !m_apptag ) {
        Error( "can't alloc lmscom_var!" );
        return;
    }

    int fp = rf.Open( RCODE_LMSCOMMAND_FILE );
    if( fp < 0 ) return;

    while( rf.GetLine( line ) ) {
        char *p = IgnoreSpace(line);
        if( !isalnum(*p) ) continue;

        char *x = strstr( line, "//" );
        if( x ) *x = '\0';

        buf1[0]=buf2[0]=buf3[0]=buf4[0]='\0';
        sscanf( p, "%s %s %s %s", buf1, buf2, buf3, buf4 );

        if( !strncmp( p, "[Version]", 9 ) ) {
            OSYSLOG1((osyslogWARNING, "LMSCom.cfg Version:%s, %s ", buf2, buf3));
            continue;
        }

        strcpy( m_lmscom[m_lmscom_n].l_name, buf1 );
        strcpy( m_lmscom[m_lmscom_n].m_name, buf2 );
        strcpy( m_lmscom[m_lmscom_n].s_name, buf3 );
        m_lmscom[m_lmscom_n].cccid = AtoI(buf4);
        m_lmscom_n++;
    }

    rf.Close();
}

void
RMain::InitRMain ( void )
{
    int  fp;
    RMainFiles rf;

    // default size
    m_stringBufMax  = MAX_STRING_BUFF;
    m_dictionaryMax = 32;
    m_stackMax      = 32;
    m_statementMax  = 32;
    m_on_callMax    = 64;

    // Read System.cfg
    fp = rf.Open( RCODE_SYSTEM_FILE );
    if( fp >= 0 ) {
        char buf[MAX_STRING_BUFF];
        while( rf.GetLine( buf ) ) {
            switch( buf[0] ) {
            case 'B': m_stringBufMax  = AtoI( buf+2 );   break;
            case 'D': m_dictionaryMax = AtoI( buf+2 );   break;
            case 'S': m_stackMax      = AtoI( buf+2 );   break;
            case 'R': m_statementMax  = AtoI( buf+2 );   break;
            case 'O': m_on_callMax    = AtoI( buf+2 );   break;
            case 'T': m_trace         = AtoI( buf+2 );   break;
            case 'G': m_debug         = AtoI( buf+2 );   break;
            case 'N':
                switch( AtoI( buf+2 ) ) {
                case  1: strcpy( m_newline, MAC_NEWLINE     ); break;
                case  2: strcpy( m_newline, UNIX_NEWLINE    ); break;
                default: strcpy( m_newline, DEFAULT_NEWLINE ); break;
                }
                break;
            case 'C': m_control      = AtoI( buf+2 );     break;
            case 'Z': m_battRestMin  = AtoI( buf+2 );     break;
            case 'X': m_shutdownMode = AtoI( buf+2 );     break;
            case 'Y': m_clockMode    = AtoI( buf+2 );     break;
            case 'P': m_initialPose  = AtoL( buf+2 );     break;
            case 'Q': m_recoverPose  = AtoL( buf+2 );     break;
            case 'A': m_chagePose    = AtoL( buf+2 );     break;
            case 'V': m_baseID       = AtoL( buf+2 );     break;
            case 'H': m_tempHi       = AtoL( buf+2 );     break;
            case 'I': m_imageLY      = AtoL( buf+2 );     break;
            case 's': m_stationMode  = AtoL( buf+2 );     break;
            case '*': m_delsMode     = AtoI( buf+2 );     break;
            case 'L': m_startLog     = AtoI( buf+2 );     break;
            case 'U': m_legacyMode   = AtoI( buf+2 );     break;
            }
        }
        rf.Close();
    }

    m_stringBufMax  = m_stringBufMax  * 1024;
    m_dictionaryMax = m_dictionaryMax * 1024;
    m_stackMax      = m_stackMax      * 1024;
    m_statementMax  = m_statementMax  * 1024;

    // Allocate
    m_stringBuf    = new char[m_stringBufMax];
    m_dictionary   = new RWord[m_dictionaryMax];
    m_stack        = new RVariable[m_stackMax];
    m_statement    = new RStatement[m_statementMax];
    m_on_call      = new RSnapInfo[m_on_callMax];
    m_dimVariable  = 0;

    if( !m_stringBuf    ) Error( "can't alloc stringBuf!" );
    if( !m_dictionary   ) Error( "can't alloc dictionary!" );
    if( !m_stack        ) Error( "can't alloc stack!"      );
    if( !m_statement    ) Error( "can't alloc statement!"  );
    if( !m_on_call      ) Error( "can't alloc on_call!"    );
}

/////////////////////////////////////////////////////////////////
//
// Error stuff
//
/////////////////////////////////////////////////////////////////
void
RMain::Error ( char *fmt, ... )
{
    va_list args;
    char msg[MAX_STRING_BUFF];
    int fp;

    va_start( args, fmt );
    vsprintf( msg, fmt, args );
    va_end( args );

    if( m_editFlag == WAIT_SCRIPT ) Printf( "ERROR: %s", msg );

    m_errorFlag = 1;

    fp = Fopen( ERROR_LOG, "a" );
    Fputs( "ERROR: ", fp );
    Fputs(  msg,      fp );
    Fputs(  m_newline,  fp );
    Fclose( fp );

    if( m_execFlag ) StopRun(1);
}

void
RMain::ClearError ( void )
{
    int fp;
    fp = Fopen( ERROR_LOG, "w" );
    Fclose( fp );
}

void
RMain::ShowErrors ( void )
{
    int fp;
    char buf[MAX_STRING_BUFF];
    fp = Fopen( ERROR_LOG, "r" );
    while( Fgets( buf, MAX_STRING_BUFF, fp ) ) {
        char *p = strchr( buf, '\n' );
        if( p ) *p = '\0';
        Printf( "%s", buf );
    }
    Fclose( fp );
}


/////////////////////////////////////////////////////////////////
//
// Header
//
/////////////////////////////////////////////////////////////////
void RMain::ShowHeader ( void )
{
    short   major_ver( 2 );
    short   minor_ver( 1 );
    short   rel_yy( 2005 );
    short   rel_mm(    1 );
    short   rel_dd(   12 );

    size_t sz;
    Printf( "============================================" );
    Printf( "R-CODE ver%d.%d (%d/%02d/%02d) ",
            major_ver, minor_ver, rel_yy, rel_mm, rel_dd );
    Printf( "============================================" );
    Printf( "string_buf  %2d * %4dK = %4dK (used %4.1f%%)",
            sizeof(char),       m_stringBufMax  /1024,
            sizeof(char)      *(m_stringBufMax  /1024),
            (double)m_stringBuf_n/(double)m_stringBufMax*100.0 );
    Printf( "dictionary  %2d * %4dK = %4dK (used %4.1f%%)",
            sizeof(RWord),      m_dictionaryMax /1024,
            sizeof(RWord)     *(m_dictionaryMax /1024),
            (double)m_dictionary_n/(double)m_dictionaryMax*100.0 );
    Printf( "stack       %2d * %4dK = %4dK (used %4.1f%%)",
            sizeof(RVariable),  m_stackMax      /1024,
            sizeof(RVariable) *(m_stackMax      /1024),
            (double)m_sp          /(double)m_stackMax     *100.0 );
    Printf( "statement   %2d * %4dK = %4dK (used %4.1f%%)",
            sizeof(RStatement), m_statementMax  /1024,
            sizeof(RStatement)*(m_statementMax  /1024),
            (double)m_statement_n /(double)m_statementMax *100.0 );
    Printf( "on_call     %2d * %4d  = %4d  (used %4.1f%%)",
            sizeof(RSnapInfo),  m_on_callMax,
            sizeof(RSnapInfo) *(m_on_callMax         ),
            (double)m_ip          /(double)m_on_callMax   *100.0 );
    Printf( "============================================" );
    GetFreeMemorySize( &sz );
    Printf( "free mem.   %d", sz );
    Printf( "============================================" );
    Printf( "<READY>" );
    Ack();
}

/////////////////////////////////////////////////////////////////
//
// Command Interface
//
/////////////////////////////////////////////////////////////////
void
RMain::NotifyCommand ( const ONotifyEvent& event )
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("RMain::NotifyCommand()\n"));
#endif //_RMAIN_DEBUG_

    if( m_state_receive_ready == false ){
        return;
    }

    char *cmd = (char*)event.Data(0);

    Printf( "%s", cmd );

    if( cmd[0] == '!' ) {
        /* ! */
        if( cmd[1] == '\0' ) {
            StopRun(0);
            StopAIBO(1);
            Ack();
        } else if( cmd[1] == '!' ) {
            /* !! */
            if( cmd[2] == '\0' ) {
                StopRun(0);
                StopAIBO(0);
                Ack();
            } else if( cmd[2] == '!' ) {
                /* !!! */
                if( cmd[3] == '\0' ) {
#ifdef _MACRO_
                    m_macro_i      = 0;
                    m_macro_n      = 0;
#endif // _MACRO_
                    m_execFlag  = WAIT_EXEC;
                    m_execStop  = 0;
                    m_waiting   = 0;
                    m_waitPart  = 0;
                    m_clockDown = 0;
                    m_syncFlag  = 0;
                    m_trace     = 0;
                    m_debug     = 0;
                    StopAIBO(1);
                    Ack();
                }
            }
        } else if( cmd[1] == '?' ) {
            /* !? */
            if( cmd[2] == '\0' ) {
                if( m_execFlag ) {
                    StopAIBO(0);
                    m_execStop = 1;
                    Execute(0);
                }
                Ack();
            } else if( cmd[2] == '?' ) {
                /* !?? */
                if( cmd[3] == '\0' ) {
                    if( m_execFlag ) {
                        m_execStop = 1;
                        Execute(0);
                    }
                    Ack();
                }
            }
        }
    } else if( m_syncFlag ) {
        if( !strcmp( cmd, "SYNC" ) ) {
            m_syncFlag = 0;
            Execute(0);
        }
    } else if( m_sendFlag ) {
        if( !strcmp( cmd, "END" ) ) {
            Fclose( m_sendFp );
            m_sendFlag = 0;
            Ack();
        } else {
            Fputs( cmd,  m_sendFp );
            Fputs( "\n", m_sendFp );
        }
    } else if( !strcmp( cmd, "SEND" ) ) {
        m_sendFp = Fopen( RCODE, "w" );
        m_sendFlag = 1;
    } else if( !strcmp( cmd, "KILL" ) ) {
        Fdelete( RCODE );
        Fdelete( ERROR_LOG );
        Ack();
    } else ParseCommand( cmd );

    observer[event.ObsIndex()]->AssertReady( event.SenderID() );
}

void
RMain::SETX ( RStatement *st )
{
    int   idx = m_dictionary[st->op2].data;
    int val = ARG0;
    if( m_jointVar[idx].type != 'R' ) {
        PutDirectMsg( idx, val );
    }
    m_jointVar[idx].value = val;
}

/////////////////////////////////////////////////////////////////
//
// Execute
//
/////////////////////////////////////////////////////////////////
void
RMain::Run ( void )
{
    if ( m_battRest < m_battRestMin ) {
        Printf("Low Battery !! Can't RUN");
        return;
    }

    if( !m_execFlag ) {
        m_sysStatus = 0;
        m_pc = 0;
        m_ct = 0;
        m_sp = m_sp1;
        m_lb = m_sp1;
        m_stringBuf_n  = m_stringBuf_1;
        m_dictionary_n = m_dictionary_1;
        m_execStop  = 0;
        m_syncFlag  = 0;
        m_waiting   = 0;
        m_waitPart  = 0;
        m_interrupt = 0;
    }
    if( m_debug ) {
        m_debug = 1;
        Printf( "<RUN>" );
    }
    m_execFlag = RUNING_EXEC;
    Execute(1);
}

void
RMain::Execute ( int run )
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("Execute 1:%d, %d,%d,%d, %d\n", run, m_execFlag, m_waiting, m_wait, m_sysEvent ));
#endif //_RMAIN_DEBUG_

    RStatement *st;

    if( run == 0 ) {
        if( m_wlanWait != NO_WAIT_DATA_WLAN) {
            m_wlanWait = EXECUTE_DATA_WLAN;
            return;
        }
        if( m_sysEvent ) {
            m_waiting = 1;
            return;
        }
    }

    if( m_stepMode ) {
        if( run ) {
            if( m_stepMode == 2 ) m_stepMode = 0;
        } else {
            return;
        }
    }

    /* Execute Loop */
    while( 1 ) {
#ifdef _RMAIN_DEBUG_
//    DPRINTF(("Execute 2:%d, %d,%d,%d, %d\n",  run, m_execFlag, m_waiting, m_wait, m_pc ));
#endif // _RMAIN_DEBUG_
#ifdef _MACRO_
        if( m_macro_i < m_macro_n ) {
            if( ProcessStatement( &(m_macro[m_macro_i++]), 0 ) ) break;
            continue;
        }
#endif // _MACRO_


        if( m_execFlag == WAIT_EXEC  ) break;
        if( m_waiting == 1 ) break;

        if( m_pc >= m_statement_n ) {
            StopRun(1);
            break;
        }

        if( m_ip ) CheckInterrupt();

        st = &(m_statement[m_pc]);

        if( ProcessStatement( st, m_pc++ ) || m_stepMode || st->type.bp || m_execStop ) {
            if( m_stepMode || st->type.bp || m_execStop ) {
                m_execStop = 0;
                Printf( "Break at #%05d", st->line_num );
                if( !m_stepMode ) m_stepMode = 2;
            }
            break;
        }

        if( m_execFlag == PAUSE_EXEC )  break;
    }
}


void
RMain::StopRun ( int ack )
{
#ifdef _MACRO_
    m_macro_i      = 0;
    m_macro_n      = 0;
#endif // _MACRO_

    m_execFlag  = WAIT_EXEC;
    m_execStop  = 0;
    m_waiting   = 0;
    m_waitPart  = 0;
    m_clockDown = 0;
    Printf( "Stopped." );
    if( ack ) Ack();
}

void
RMain::HaltRun ( int ack )
{
    StopRun(ack);
    SHUTDOWN();
}

void
RMain::LoadProgram ( const char *filename )
{
    int fp = Fopen( filename, "r" );
    char line[MAX_STRING_BUFF];

    if( fp < 0 ) {
        Error( "can't open [%s].", filename );
        return;
    }

    RunCommand( "EDIT" );

    while( Fgets( line, MAX_STRING_BUFF, fp ) ) {
        ParseCommand( line );
    }

    RunCommand( "END" );

    Fclose( fp );
}


/////////////////////////////////////////////////////////////////
//
// Command
//
/////////////////////////////////////////////////////////////////

#define POSITION_H_ANGLE  (1<<8)
#define POSITION_V_ANGLE  (1<<9)
#define POSITION_DISTANCE (1<<10)
#define POSTURE_H_ANGLE   (1<<12)
#define POSTURE_V_ANGLE   (1<<13)
#define POSTURE_ROLL      (1<<14)

void
RMain::CommandSEND ( RMessage *msg )
{
    switch( msg->type ) {
    case tAPPID:
    {
        APPCSubInfo subinfo;
        CCCCommandID cccid = MKLONG( msg->arg0, msg->arg1 );
        CCCommandQueu ccc( cccid, observer[obsComplete]->GetID() );
        ccc.SetTarget(ccctAPP);

        long subinfo1 = MKLONG( msg->arg2, msg->arg3 );
        long subinfo2 = MKLONG( msg->arg4, 0 );
        long subinfo3 = 0;
        long subinfo4 = 0;

        subinfo.SetMember(INDEX_SUBINFO1, subinfo1);
        subinfo.SetMember(INDEX_SUBINFO2, subinfo2);
        subinfo.SetMember(INDEX_SUBINFO3, subinfo3);
        subinfo.SetMember(INDEX_SUBINFO4, subinfo4);
        ccc.SetSubInfo( subinfo );
        PutCCCMsg( &ccc, 0 );
        break;
    }
    case tAPPTAG:
    {
        APPCSubInfo subinfo;
        CCCCommandID cccid = m_apptag[msg->arg0].appcomid;
        CCCommandQueu ccc( cccid, observer[obsComplete]->GetID() );
        ccc.SetTarget(ccctAPP);

        long subinfo1 = m_apptag[msg->arg0].subinfo.GetMember(INDEX_SUBINFO1);
        long subinfo2 = m_apptag[msg->arg0].subinfo.GetMember(INDEX_SUBINFO2);
        long subinfo3 = m_apptag[msg->arg0].subinfo.GetMember(INDEX_SUBINFO3);
        long subinfo4 = m_apptag[msg->arg0].subinfo.GetMember(INDEX_SUBINFO4);

        subinfo.SetMember(INDEX_SUBINFO1, subinfo1);
        subinfo.SetMember(INDEX_SUBINFO2, subinfo2);
        subinfo.SetMember(INDEX_SUBINFO3, subinfo3);
        subinfo.SetMember(INDEX_SUBINFO4, subinfo4);
        ccc.SetSubInfo( subinfo );

        if( m_apptag[msg->arg0].sem_id != 0 ) {
            ISSemanticsID sem_id = 0x00010000 | m_apptag[msg->arg0].sem_id;
            SubjectID subjectid = GetSemanticsState( sem_id )->GetSubjectID();
            ccc.SetSubjectID( subjectid );
            DPRINTF(( "sbj[oid:0x%08x:%2u] sem[0x%x]",
                        ((OServiceEntry)subjectid).GetOID().GetAddress(),
                        ((OServiceEntry)subjectid).GetSelector(), sem_id ));

        }
        PutCCCMsg( &ccc, 0 );

        DPRINTF(("m_apptag:%d %x SUB[%x:%x:%x:%x] %x %x %x %x %x %x\n",
               msg->arg0,
               m_apptag[msg->arg0].appcomid,
               subinfo1,
               subinfo2,
               subinfo3,
               subinfo4,
               msg->mode,
               msg->type,
               msg->arg1,
               msg->arg2,
               msg->arg3,
               msg->arg4));

        break;
    }
    }
}

void
RMain::CommandPLAY ( RMessage *msg, int pose )
{
    switch( msg->type ) {
    case tACTION:
    {
        CCCCommandID cccid = m_action[msg->arg0].cccid;
        short type = m_action[msg->arg0].type;
        TargetPos target_pos;
        AppID appId;

        CCCommandQueu ccc( cccid, observer[obsComplete]->GetID());

        switch( type )
        {
        case 0: break;
        case 1:
            {
            target_pos.GetPosition().SetHAngle( oradians(msg->arg1) );
            Length len;
            if ( msg->arg2<0 ) { len = 0x1FFFFFFF; }
            else               { len = (longword)msg->arg2*1000; }
            target_pos.GetPosition().SetDistance( len );
            ccc.SetTargetPos( target_pos );
            }
            break;
        case 2:
            target_pos.GetPosition().SetHAngle( oradians(msg->arg1) );
            ccc.SetTargetPos( target_pos );
            break;
        case 3:
            target_pos.GetPosition().SetHAngle( oradians(msg->arg1) );
            target_pos.GetPosition().SetVAngle( oradians(msg->arg2) );
            ccc.SetTargetPos( target_pos );
            break;
        case 4:
            appId = m_baseID + msg->arg1;
            m_TRsbjID = GetSemanticsState( appId )->GetSubjectID();
            if( m_trace & TRACE_TARGET )
            {
                Printf( "sbj[oid:0x%08x:%2u] AppID[0x%x]",
                        ((OServiceEntry)m_TRsbjID).GetOID().GetAddress(),
                        ((OServiceEntry)m_TRsbjID).GetSelector(), appId );
            }
            ccc.SetSubjectID( m_TRsbjID );
            ccc.SetAppID( appId );
            ccc.SetTargetPos( target_pos );
            break;

        default:
            {
            int arg[4];
            int i = 0;
            arg[0] = msg->arg1;
            arg[1] = msg->arg2;
            arg[2] = msg->arg3;
            arg[3] = msg->arg4;
            if( type & POSITION_H_ANGLE  ) {
                target_pos.GetPosition().SetHAngle( oradians(arg[i]) );
                i++;
            }
            if( type & POSITION_V_ANGLE  ) {
                target_pos.GetPosition().SetVAngle( oradians(arg[i]) );
                i++;
            }
            if( type & POSITION_DISTANCE ) {
                Length len;
                if ( arg[i]<0 ) { len = 0x1FFFFFFF; }
                else            { len = (longword)(arg[i])*1000; }
                target_pos.GetPosition().SetDistance( len );
                i++;
            }
            if( type & POSTURE_H_ANGLE   ) {
                target_pos.GetPosture().SetHAngle( oradians(arg[i]) );
                i++;
            }
            if( type & POSTURE_V_ANGLE   ) {
                target_pos.GetPosture().SetVAngle( oradians(arg[i]) );
                i++;
            }
            if( type & POSTURE_ROLL      ) {
                target_pos.GetPosture().SetRoll( oradians(arg[i]) );
                i++;
            }
            ccc.SetTargetPos( target_pos );
            }
        }
        PutCCCMsg( &ccc, 0 );
    }
    break;

    case tACTIONX:
    {
        CCCCommandID cccid = m_action[msg->arg0].cccid;
        short type = m_action[msg->arg0].type;
        TargetPos target_pos;
        AppID appId;

        CCCommandQueu ccc( cccid, observer[obsComplete]->GetID() );

        type = msg->arg1;

        switch( type ) {
        case 0: break;
        case 1:
            {
            target_pos.GetPosition().SetHAngle( oradians(msg->arg2) );
            Length len;
            if( msg->arg3<0 ) { len = 0x1FFFFFFF; }
            else              { len = (longword)msg->arg3*1000; }
            target_pos.GetPosition().SetDistance( len );
            ccc.SetTargetPos( target_pos );
            }
            break;
        case 2:
            target_pos.GetPosition().SetHAngle( oradians(msg->arg2) );
            ccc.SetTargetPos( target_pos );
            break;
        case 3:
            target_pos.GetPosition().SetHAngle( oradians(msg->arg2) );
            target_pos.GetPosition().SetVAngle( oradians(msg->arg3) );
            ccc.SetTargetPos( target_pos );
            break;
        case 4:
            appId = m_baseID + msg->arg1;
            m_TRsbjID = GetSemanticsState( appId )->GetSubjectID();
            if( m_trace & TRACE_TARGET )
            {
                Printf( "sbj[oid:0x%08x:%2u] AppID[0x%x]",
                        ((OServiceEntry)m_TRsbjID).GetOID().GetAddress(),
                        ((OServiceEntry)m_TRsbjID).GetSelector(), appId );
            }
            ccc.SetSubjectID( m_TRsbjID );
            ccc.SetAppID( appId );
            ccc.SetTargetPos( target_pos );
            break;

        case 5:
            {
            target_pos.GetPosition().SetHAngle( oradians(msg->arg2) );
            target_pos.GetPosition().SetVAngle( oradians(msg->arg3) );
            Length len;
            if ( msg->arg4<0 ) { len = 0x1FFFFFFF; }
            else               { len = (longword)msg->arg4*1000; }
            target_pos.GetPosition().SetDistance( len );
            ccc.SetTargetPos( target_pos );
            }
            break;

        default:
            {
            int arg[3];
            int i = 0;
            arg[0] = msg->arg2;
            arg[1] = msg->arg3;
            arg[2] = msg->arg4;
            if( type & POSITION_H_ANGLE  ) {
                target_pos.GetPosition().SetHAngle( oradians(arg[i]) );
                i++;
            }
            if( type & POSITION_V_ANGLE  ) {
                target_pos.GetPosition().SetVAngle( oradians(arg[i]) );
                i++;
            }
            if( type & POSITION_DISTANCE ) {
                Length len;
                if ( arg[i]<0 ) { len = 0x1FFFFFFF; }
                else            { len = (longword)(arg[i])*1000; }
                target_pos.GetPosition().SetDistance( len );
                i++;
            }
            if( type & POSTURE_H_ANGLE   ) {
                target_pos.GetPosture().SetHAngle( oradians(arg[i]) );
                i++;
            }
            if( type & POSTURE_V_ANGLE   ) {
                target_pos.GetPosture().SetVAngle( oradians(arg[i]) );
                i++;
            }
            if( type & POSTURE_ROLL      ) {
                target_pos.GetPosture().SetRoll( oradians(arg[i]) );
                i++;
            }
            ccc.SetTargetPos( target_pos );
            }
        }
        PutCCCMsg( &ccc, 0 );
    }
    break;


    case tMWCID:
        {
        CCCCommandID cccid = MKLONG( msg->arg0, msg->arg1 );
        CCCommandQueu ccc( cccid, observer[obsComplete]->GetID() );

        TargetPos target_pos;
        target_pos.GetPosition().SetHAngle( oradians(msg->arg2) );
        target_pos.GetPosition().SetVAngle( oradians(msg->arg3) );
        Length len;
        if ( msg->arg4<0 ) { len = 0x1FFFFFFF; }
        else               { len = (longword)msg->arg4*1000; }
        target_pos.GetPosition().SetDistance( len );
        ccc.SetTargetPos( target_pos );

        PutCCCMsg( &ccc, 0 );
        }
        break;

    case tCOMMAND:
        break;

    case tMTN:
        PlayMtn();
        break;

    case tLMS:
        break;

    case tLMSID:
    {
        CCCommandQueu ccc( mwcUNDEFINE_ID, observer[obsComplete]->GetID() );
        ccc.SetSelectFlag(cccsfCOMMAND_FIX | cccsfPOSTURE_FREE);
        ccc.SetArbitrationFlag( cccafCHANGE0 );
        ccc.SetTarget( ccctMW );
        ccc.SetCategoryL( msg->arg0 );
        ccc.SetCategoryM( msg->arg1 );
        ccc.SetCategoryS( msg->arg2 );
        PutCCCMsg( &ccc, 0 );
        break;
    }
    default: Error( "unknown type [%d].", msg->type );
    }
}

/* Process STOP command */
void
RMain::CommandSTOP ( RMessage *msg, int emergency )
{
    if( msg->type != tMTN ) {
        StopAIBO( emergency );
    } else {
        m_syncFlag = 0;
        m_waiting  = 0;
        m_waitPart = 0;
        m_wait     = 0;
        StopMtn();
    }
}

/* Operation stop */
void
RMain::StopAIBO ( int emergency )
{
    ClrCCCMsg();
    if( emergency ) m_cccBusy = 0;
    if( emergency != 2 ) StopMtn();

    CCCommandQueu ccc( NULL, observer[obsComplete]->GetID() );

    if( m_sysEvent == 0 )
    {
        if( emergency ) {
            m_waiting  = 0;
            m_waitPart = 0;
            m_wait     = 0;
            m_syncFlag = 0;

            ccc.SetCommandID( 0xFFFEFFFF ); /* EMERGENCY_STOP */
            PutCCCMsg( &ccc, 0 );
            if( (m_mwcPos != mwcSTATION)  &&
                (m_mwcPos != mwcPSTATION) &&
                (m_mwcPos != mwcLIFT)     &&
                (m_mwcPos != mwcTEMPORARY_POSTURE) ) {
                ccc.SetCommandID( 0xFFF00000 ); /* JUMP_TO_NT */
                PutCCCMsg( &ccc, 0 );
            }
        } else {
            ccc.SetCommandID( mwcNORMAL_STOP );
            PutCCCMsg( &ccc, 0 );
        }
    }

    ClrPlugInMsg();
    for( int i=0; i<m_plugin_n; i++ ) {
        if( m_plugin[i].notify & ((emergency)?(NOTIFY_QUIT):(NOTIFY_STOP)) ) {
            PlugInMsgData data;
            data.type = PLUGIN_MSG;
            data.mode = MSG_R_CODE;
            strcpy( data.msg, ((emergency)?("QUIT"):("STOP")) );
            PutPlugInMsg( i, (PlugInData*)(&data) );
        }
    }
}

/* Power supply shutdown */
void
RMain::SHUTDOWN ( void )
{
    m_cccMsgHead = NULL;
    m_sysMsgHead = NULL;

    if ( m_shutdownMode == SHUTDOWN_API ) {
        OPENR::Shutdown( OBootCondition( obcbPAUSE_SW ) );
    } else if ( m_shutdownMode == SHUTDOWN_EXCPTION ) {
        /* N/A */
    } else if ( m_shutdownMode == SHUTDOWN_APPCMD ) {
        CCCommandQueu ccc( CCCC_SHUTDOWN_ID, observer[obsComplete]->GetID() );
        ccc.SetTarget(ccctAPP);
        m_sysEvent = 1;
        PutCCCMsg( &ccc, 0 );
    } else {
        Error( "unknown Shutdown mode [%d].", m_shutdownMode );
    }
}

void
RMain::EmergencyExec( OcaInputSemantics *data )
{
    ISSemanticsID sem_id    = data->GetSemanticsID();

    if ( ( sem_id & issemMASK_SEM_ID ) == g_labels.GetSemanticsID("EMERGENCY") ) {
        char str[MAX_STRING_BUFF];
        g_labels.GetSubInfoStr( str,  g_labels.GetSemanticsID("EMERGENCY"), 1, 1 );
        DPRINTF(("EmergencyExec: %s\n", str ));

        if ( ( data->GetSubInfo(0) & 0x0000ffff ) == 0x00000001 ) {
            SHUTDOWN();
        }
    }
}


/////////////////////////////////////////////////////////////////
//
// TIME
//
/////////////////////////////////////////////////////////////////
void
RMain::SetPeriodicTimeRequest(void)
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("RMain::SetPeriodicTimeRequest\n"));
#endif //_RMAIN_DEBUG_

    OcaTimeRequest periodic_time_req;
    periodic_time_req.SetObserverId(observer[obsPeriodicTimeReply]->GetID());
    periodic_time_req.SetRequestType(timerequestINTERVAL);
    periodic_time_req.SetRequestId(0);

    longword req_sec  = PERIODIC_TIME_INTERVAL / 1000;
    longword req_msec = PERIODIC_TIME_INTERVAL % 1000;

    periodic_time_req.SetSec(req_sec);
    periodic_time_req.SetMilliSec(req_msec);

    subject[sbjPeriodicTimeRequest]->
            SetData(&periodic_time_req, sizeof(OcaTimeRequest));
    return;
}

void RMain::ReadyTimeRequest( const OReadyEvent& event )
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("RMain::ReadyTimeRequest()\n"));
#endif //_RMAIN_DEBUG_


}

void RMain::NotifyTimeReply( const ONotifyEvent& event )
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("RMain::NotifyTimeReply()\n"));
#endif //_RMAIN_DEBUG_

    if( m_state_receive_ready == false ){
        return;
    }
}

void RMain::ReadyPeriodicTimeRequest( const OReadyEvent& event )
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("RMain::ReadyPeriodicTimeRequest()\n"));
#endif //_RMAIN_DEBUG_

    if ( m_state_periodic_time_request == false ) {
        SetPeriodicTimeRequest();
        subject[sbjPeriodicTimeRequest]->NotifyObservers();
        observer[obsPeriodicTimeReply]->AssertReady();
        m_state_periodic_time_request = true;
    }
    return;
}

void
RMain::NotifyPeriodicTimeReply( const ONotifyEvent& event )
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("RMain::NotifyPeriodicTimeReply()\n"));
#endif //_RMAIN_DEBUG_

    if( m_state_receive_ready == false ){
        return;
    }
    SubjectID subjectID = event.SenderID();

    m_clock += CLOCK_COUNT_VALUE;
    m_clock++;

    if( !m_sysEvent ) {
        if( (m_initFlag == RCODE_FINISH_INIT) &&
            (m_autoExec != NOT_AUTO)  ) {
            int x = m_autoExec;
            m_autoExec = NOT_AUTO;
            switch( x ) {
            case 1: Run(); break;
            case 2: ShowHeader(); break;
            }
        }

        if( m_execFlag == PAUSE_EXEC ) {
#ifdef _RMAIN_DEBUG_
            DPRINTF(("Periodic step:%d\n", m_waiting));
#endif // _RMAIN_DEBUG_
            if ( m_waiting == 0 )
                Execute(0);
        }

        if( (m_clockDown > 0) || m_waiting ) {
            static int c = 0;
            c++;
            if( c == 10 ) {
                c = 0;
                if( CheckInterrupt() ) {
                    m_clockDown = 0;
                    m_syncFlag  = 0;
                    m_waiting   = 0;
                    m_waitPart  = 0;
                    Execute(0);
                }
            }
        }

        if( m_clockDown > 0 ) {
            m_clockDown--;
            if( m_clockDown == 0 ) {
                Execute(0);
            }
        }
    }

    if( m_countDown > 0 ) {
        m_countDown--;
        if( m_countDown == 0 ) {
            SHUTDOWN();
        }
    }
    observer[obsPeriodicTimeReply]->AssertReady(subjectID);
}


void
RMain::NotifyInputSemantics( const ONotifyEvent& event )
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("RMain::NotifyInputSemantics()\n"));
#endif //_RMAIN_DEBUG_

    if( m_state_receive_ready == false ){
        return;
    }
    SubjectID subjectID = event.SenderID();

    for( int i=0;i<event.NumOfData();i++ ) {
        OcaInputSemantics *data = (OcaInputSemantics*)event.Data(i);
        OpenrTime     openrtime = data->GetOpenrTime();
        ISComType     comtype   = data->GetComType();
        ISComStatus   comstat   = data->GetComStatus();
        SubjectID     subject   = data->GetSubjectID();
        SensorId      sensor    = data->GetSensorID();
        ISSemanticsID sem_id    = data->GetSemanticsID();
        ISSubInfo     subinfo[MAX_OF_SUBINFO];
        for( int j=0;j<MAX_OF_SUBINFO;j++ ) {
            subinfo[j]=data->GetSubInfo(j);
        }
        ISLevel     level       = data->GetLevel();
        ISPitch     pitch       = data->GetPitch();
        ISPosition  position    = data->GetPosition();

        OcaInputSemantics* sem_state;
        // for initial registration
        if ( isctREGISTER == comtype || isctNSUPPORT == comtype ) {
            char buf[MAX_STRING_BUFF];
            if ( g_labels.GetSemanticsStr(buf, sem_id) ) {
                sprintf(g_debug_buf, "sr> [%s]\t\0", buf);
            } else {
                sprintf(g_debug_buf, "sr> id[%x]\t\0", sem_id);
            }
            sem_state = GetSemanticsState( sem_id );
            if ( sem_state ) {
                *sem_state = *data;
            } else {
                g_labels.GetSemanticsStr(buf, sem_id);
                DPRINTF(( "%s\n", buf ));
            }
            if ( m_startLog ) {
                char tmp[MAX_STRING_BUFF];
                strcpy( buf, "" );
                g_labels.GetSemanticsStr(tmp, sem_id);
                DPRINTF(("sem[%12s/0x%08x] %x \tREGISTER... OK!! sbj[oid:0x%08x:%2u]\n",
                         tmp, sem_id, comtype,
                         ((OServiceEntry)subject).GetOID().GetAddress(),
                         ((OServiceEntry)subject).GetSelector()));
            }
        }


        // do nothing for semantics sent with mw command
        if ( sem_id & issemMASK_WITH_MWC ) {
            DPRINTF(("Pass NotifyInputSemantics:%x\n", sem_id ));
            /* do nothing */
        }else {
            if( comtype == isctNOTIFY ) {
                if( ( sem_id & issemMASK_SEM_ID)  == SEM_RISE_POSE ) {
                    m_initFlag = RCODE_BEING_INIT;
                    DPRINTF(("SEM_RISE_POSE\n"));
                }
                if( m_trace & TRACE_SEMALL )  PrintInputSemantics( *data );
                for( int j=0; j<m_sensVar_n; j++ ) {
                    /* Legacy Processing is not done here, but somewhere else. */
                    if( m_sensVar[j].modeID == LEGACY_CONFIG ) continue;
                    if( m_sensVar[j].semOrmwiid == (sem_id & issemMASK_SEM_ID) ) {
                        ISSubInfo subinfoDAT;
                        int validSubInfo;
                        if( m_sensVar[j].validSubInfo ) {
                            validSubInfo  = m_sensVar[j].validSubInfo & SENS_SUB_MASK;
                            switch( validSubInfo ) {
                            case SENS_SUBINFO1: subinfoDAT = subinfo[INDEX_SUBINFO1]; break;
                            case SENS_SUBINFO2: subinfoDAT = subinfo[INDEX_SUBINFO2]; break;
                            case SENS_SUBINFO3: subinfoDAT = subinfo[INDEX_SUBINFO3]; break;
                            case SENS_SUBINFO4: subinfoDAT = subinfo[INDEX_SUBINFO4]; break;
                            default:
                                subinfoDAT = 0;
                                if( m_sensVar[j].subinfo != subinfo[INDEX_SUBINFO1]) continue;
                                break;
                            }
                        }

                        int flag = 1;

                        switch( m_sensVar[j].type ) {
                        case 'N': m_sensVar[j].value = 1;            break;
                        case 'L': m_sensVar[j].value = 1;            break;
                        case 'P': m_sensVar[j].value = 1;            break;
                        case 'H': m_sensVar[j].value = 1;            break;
                        case 'T': m_sensVar[j].value = 1;            break;
                        case '0': m_sensVar[j].value = 1;            break;
                        case '1': m_sensVar[j].value = 1;            break;
                        case '*':
                            m_sensVar[j].value = ((comstat==iscsON)?(1):(0));
                            break;
                        case 'a':
                            m_sensVar[j].value = 1;
                            break;
                        case 'b':
                            m_sensVar[j].value = subinfo[0] & arsubMASK_AUDIO_ELEMENT;
                            break;
                        case 'l':
                            m_sensVar[j].value = level;
                            break;
                        case 'p':
                            m_sensVar[j].value = pitch;
                            break;
                        case 'd':
                            switch( position ) {
                            case isposFORWARD:   m_sensVar[j].value = 1; break;
                            case isposRFORWARD:  m_sensVar[j].value = 2; break;
                            case isposRIGHT:     m_sensVar[j].value = 3; break;
                            case isposRBACKWARD: m_sensVar[j].value = 4; break;
                            case isposBACKWARD:  m_sensVar[j].value = 5; break;
                            case isposLBACKWARD: m_sensVar[j].value = 6; break;
                            case isposLEFT:      m_sensVar[j].value = 7; break;
                            case isposLFORWARD:  m_sensVar[j].value = 8; break;
                            default:             m_sensVar[j].value = 0; break;
                            }
                            break;
                        case 'R':
                        {
                            if( validSubInfo ) {
                                DPRINTF(("Batt_Rest:%d:", m_battRest ));
                                m_sensVar[j].value = m_battRest = subinfoDAT;
                                DPRINTF(("%d\n", m_battRest ));
                            }

                            if( m_battRest < m_battRestMin ) {
                                if( m_execFlag ) {
                                    StopRun(0);
                                    StopAIBO(1);
                                    char buf[32];
                                    sprintf( buf, "PLAY:MWCID:%d", m_chagePose );
                                    DPRINTF(("Change to chagepose:%s\n", buf ));
                                    RunCommand( buf );
                                    Printf( "Low Battery !!" );
                                }
                            }
                        }
                        break;

                        default:
                            flag = 0;
                            break;
                        }

                        if( (m_trace & TRACE_SEM) && flag ) PrintInputSemantics( *data );
                    }
                }
            }
        }
    }

    observer[event.ObsIndex()]->AssertReady( event.SenderID() );

    /* ReRun */
    if( m_execFlag == PAUSE_EXEC ) {
        DPRINTF(("RERUN Execute(0) at NotifyInputSemantics\n"));
        m_execFlag = RUNING_EXEC;
        Execute(0);
    }
}

/////////////////////////////////////////////////////////////////
//
// CCCommand Command
//
/////////////////////////////////////////////////////////////////
void
RMain::ReadyCCCommand( const OReadyEvent& event )
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("RMain::ReadyCCCommand()\n"));
#endif //_RMAIN_DEBUG_

    SendCCCMsg(0);
}

void
RMain::PutCCCMsg ( CCCommandQueu* ccc, int cmd )
{
    CCCMsg *tmp = new CCCMsg;
    if( tmp == NULL ) {
        Error(" CCCMsg alloc failed!" );
        return;
    }

    if( ccc ) {
        if( m_sysEvent ) m_sysSerial = m_cccSerial;
        else m_wait++;

        ccc->SetSerial( m_cccSerial++ );

        if( m_trace & TRACE_CMD ) {
            Printf( "Que: %06d command=%d", ccc->GetSerial(), ccc->GetCommandID() );
        }
    } else {
        if( m_trace & TRACE_CMD ) Printf( "Que: cmd=%d", cmd );
    }

    tmp->next = NULL;
    if( ccc ) tmp->ccc = *ccc;
    tmp->cmd = cmd;

    if( m_cccMsgHead ) {
        CCCMsg *x = m_cccMsgHead;
        while( x->next ) x = x->next;
        x->next = tmp;
    } else m_cccMsgHead = tmp;

    SendCCCMsg(0);
}

void
RMain::ClrCCCMsg ( void )
{
    if( m_sysEvent ) return;

    CCCMsg **msg = &m_cccMsgHead;
    while( *msg ) {
        CCCMsg *tmp = *msg;
        *msg = tmp->next;
        delete tmp;
    }
}

/* Take out MWCommand from the queue, and send it to CC. */
/* mode 0:Execute/Ready */
/*      1:2nd.Result    */
/*      2:Last.Result   */
void
RMain::SendCCCMsg ( int mode )
{
    OSubject *sbj = subject[sbjCCCommand];
    static int pending = 0;

    if( m_trace & TRACE_SENDMW ){
        Printf( "SendCC mode=%d pending=%d cccBusy=%d",
                mode, pending, m_cccBusy );
    }

    if( m_cccMsgHead == NULL ) {
        pending = 0;
        if( mode == m_cccBusy ) m_cccBusy = 0;
        return;
    }

    if( !(sbj->IsReady()) ) {
        pending = mode;
        if( m_trace & TRACE_CMD ) {
            Printf( "Cmd: CCCommand is not ready." );
        }
        return;
    }

    if( (mode == 0) && pending ) {
        mode = pending;
    }
    pending = 0;

    CCCMsg *tmp = m_cccMsgHead;
    if( tmp ) {
        /* The processing of the special command */
        if( tmp->cmd ) {
            m_cccMsgHead = tmp->next;

            if( m_trace & TRACE_CMD ) Printf( "Cmd: cmd=%d", tmp->cmd );

            m_cccBusy = 0;

            switch( tmp->cmd ) {
            case 2:
                if( m_execFlag ) {
                    m_sysStatus = 1;
                    m_bootType  = 0;
                    ResetEnv( -1, 1 );
                    Execute(0);
                }
                break;

            case 7:
            case 8:
                m_countDown = 9; // SHUTDOWN
                break;

            case 9:
                SHUTDOWN();
                break;
            }
        } else {
            CCCSerial serial = tmp->ccc.GetSerial();
            CCCCommandID cccid  = tmp->ccc.GetCommandID();
            int mtn    = 0;
            int busy   = 1;
            int i;

            if ( m_sysEvent > 0 && m_waitSysCommand == 1 ) {
                return;
            }

            CCCArbitrationFlag caf = tmp->ccc.GetArbitrationFlag();
            if (caf == cccafSEQUENCE ) {
                m_waitSysCommand = 1;
                m_waitSysSerial = serial;
            }

            /* NORMAL_STOP is assumed */
            if( (mode < 2) && (m_cccBusy == 2) && (cccid == mwcNORMAL_STOP) ) {
                return;
            }

            /* Check whether it is the command to wait for Last. */
            for( i=0; i<m_MtnN; i++ ) {
                if( m_MtnMWCID[i] == cccid ) {
                    mtn = 1;
                    if( m_MtnType[i] == 1 ) {
                        busy = 2;
                    }
                    break;
                }
            }

            if( m_trace & TRACE_SENDMW ) {
                Printf( "cccid=%d mtn=%d busy=%d cccBusy=%d",
                        cccid, mtn, busy, m_cccBusy );
            }

            /* Don't wait for 2nd.Result if SysEvent and mode=0. */
            if ( (m_cccBusy == 1) && (mode < 1) && (m_sysEvent == 1) ) {
                if ( m_trace & TRACE_CMD ) {
                    Printf("Wait TypeB.....");
                }
                return;
            }

            if( (m_cccBusy == 2) && (mode < 2) ) {
                /* Wait for Last.Result if MWCID includes a mtn */
                if( mtn ) return;
                /* MWCommand which doesn't contain mtn transmits a message as is. */
            } else {
                /* mwc_busy is matched to a new MWCommand, it updates, and transmits */
                m_cccBusy = busy;
            }

            if( m_trace & TRACE_CMD ) {
                Printf( "Cmd: %06d mwcid=%d", serial, cccid );
            }

            m_cccMsgHead = tmp->next;

            if( busy == 1 ) m_wait2Serial = serial;
            else            m_waitLSerial = serial;

            sbj->SetData( &(tmp->ccc.GetCCCommandData() ), sizeof(OcaCCCommandData) );
            sbj->NotifyObservers();
        }
        delete tmp;
    } else {
        m_cccBusy = 0;
    }
}

void
RMain::NotifyComplete( const ONotifyEvent& event )
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("RMain::NotifyComplete()\n"));
#endif //_RMAIN_DEBUG_

    if( m_state_receive_ready == false ){
        return;
    }

    for( int i=0;i<event.NumOfData();i++ ) {
        OcaCCCommandResult* msg = (OcaCCCommandResult*)event.Data(i);
        CCCSerial     serial    = msg->GetSerial();
        CCCStatus     status    = msg->GetStatus();
        MWCEffect     effect    = msg->GetEffect();
        CCCCommand    command   = msg->GetCommand();
        MWCResource   resource  = msg->GetResource();
        MWCPosture    posture   = msg->GetPosture();

        CCCTarget     target    = command.GetTarget();
        CCategoryL    categoryl = command.GetCategoryL();
        CCategoryM    categorym = command.GetCategoryM();
        CCategoryS    categorys = command.GetCategoryS();
        CCCCommandID  commandid = command.GetCommandID();

        if( m_trace & TRACE_CMD ) {
            char buf[MAX_STRING_BUFF];
            char tmp[MAX_STRING_BUFF];

            sprintf( buf, "%06d ", serial );
            sprintf( tmp, "T:0x%02X ", target );
            strcat( buf, tmp );
            sprintf( tmp, "L:0x%02X ", categoryl );
            strcat( buf, tmp );
            sprintf( tmp, "M:0x%02X ", categorym );
            strcat( buf, tmp );
            sprintf( tmp, "S:0x%02X ", categorys );
            strcat( buf, tmp );
            sprintf( tmp, "COM_ID:0x%08X ", commandid );
            strcat( buf, tmp );
            sprintf( tmp, "resource=0x%08x ", resource );
            strcat( buf, tmp );
            sprintf( tmp, "posture=%08x ", posture );
            strcat( buf, tmp );

            switch( status ) {
            case cccsINVALID:       strcat( buf, "cccsINVALID     " ); break;
            case cccsCOMPLETE:      strcat( buf, "cccsCOMPLETE    " ); break;
            case cccsINCOMPLETE:    strcat( buf, "cccsINCOMPLETE  " ); break;
            case cccsUNKNOWN:       strcat( buf, "cccsUNKNOWN     " ); break;
            case cccsSTATUS_MASK:   strcat( buf, "cccsSTATUS_MASK " ); break;
            case cccsTYPE_MASK:     strcat( buf, "cccsTYPE_MASK   " ); break;
            case cccsTYPE_NOTICE:   strcat( buf, "cccsTYPE_NOTICE " ); break;
            default:                strcat( buf, "...             " ); break;
            }

            switch( effect ) {
            case mwcFAIL:           strcat( buf, "FAIL    "         ); break;
            case mwcSUCCESS:        strcat( buf, "SUCCESS "         ); break;
            case mwcUNKNOWN_EFFECT: strcat( buf, "UNKNOWN "         ); break;
            default:                strcat( buf, "...     "         ); break;
            }

            Printf( "Rst: %s", buf );
        }

        m_mwcPos = posture;
        m_posture1 = (m_mwcPos >> 16) & 0xFFFF;
        m_posture2 = (m_mwcPos      ) & 0xFFFF;

        /* result variable renewal */
        m_resultType = status;
        m_resultNo   = serial;

        switch( status ) {
        case cccsINVALID:
        case cccsCOMPLETE:
        case cccsINCOMPLETE:
        case cccsUNKNOWN:
            if( m_sysEvent ) {
                if ( serial == m_waitSysSerial ) {
                    m_waitSysCommand = 0;
                }
                DPRINTF(("Serial %d:%d:delayMWC:%d, delayFlag:%d\n",
                         serial, m_sysSerial, m_delayMwcid, m_delayFlag ));
                if( serial == m_sysSerial ) {
                    if( m_delayFlag ) {
                        DPRINTF(("Dlay:Getput\n"));
                        CCCommandQueu ccc( m_delayMwcid, observer[obsComplete]->GetID() );
                        ccc.SetArbitrationFlag( cccafCHANGE0 );
                        PutCCCMsg( &ccc, 0 );
                        m_delayFlag = 0;
                    }
                }
                if( m_sysEvent > 1 ) break;
                if( serial == m_sysSerial ) {
                    DPRINTF(("SYSEVENT END\n"));
                    if ( m_sysEvent2Down == 1 ) {
                        SHUTDOWN();
                        return;
                    }
                    m_sysEvent = 0;

                    if( m_initFlag == RCODE_BEING_INIT ) {
                        if( m_autoExec == NOT_AUTO ) ShowHeader();
                        m_initFlag = RCODE_FINISH_INIT;
                    }
                    m_wait = 0;
                    m_soundWait = 0;
                    if( m_waiting ) {
                        m_waiting = 0;
                        m_waitPart = 0;
                        Execute(0);
                    }
                }
            } else if ( m_soundWait ) {
                m_soundWait = 0;
                OSubject *sbj = subject[sbjMtnCommand];
                char msg[MAX_STRING_BUFF];
                if( !(sbj->IsReady()) ) return;
                sprintf( msg, "COMPLETE" );
                sbj->SetData( msg, strlen(msg)+1 );
                sbj->NotifyObservers();
            } else {
                if( m_wait > 0 ) m_wait--;
                if( m_waiting && (m_cccMsgHead == NULL )) {
                    if ( m_wait == 0 ) {
                        m_waiting = 0;
                        m_waitPart = 0;
                        Execute(0);
                    }
                }
            }
            break;
        }

        // Result Type
        if ( m_trace & TRACE_CMD ) {
            Printf( "CCC Status:%d", status );
        }

        if( (serial == m_wait2Serial) ||
            (serial == m_waitLSerial) ) {
            switch( status ) {
            case cccsCOMPLETE:
            case cccsINCOMPLETE:
                m_cccBusy=0;
                SendCCCMsg(1);
                break;
            }
        }
    }
    observer[obsComplete]->AssertReady();
}

#ifdef _RCLOCK_MODE_
/////////////////////////////////////////////////////////////////
//
// Clock
//
/////////////////////////////////////////////////////////////////
void
RMain::Clock ( const ONotifyEvent& event )
{
    m_intClock++;

    if( m_sysEvent ) {
    } else {
        if( m_execFlag == PAUSE_EXEC ) {
            m_execFlag = RUNING_EXEC;
            Execute(0);
        }
    }

    if( m_clockMode == 0 ) {
        observer[obsClock]->AssertReady();
    }
}
#endif // _RCLOCK_MODE_

void
RMain::Clock2 ( const ONotifyEvent& event )
{
    if( m_sysEvent ) {
    } else {
        if( (m_initFlag == RCODE_FINISH_INIT) && ( m_autoExec != NOT_AUTO ) ) {
            int x = m_autoExec;
            m_autoExec = NOT_AUTO;
            switch( x ) {
            case 1: Run(); break;
            case 2: ShowHeader(); break;
            }
        }
        if( m_execFlag == PAUSE_EXEC ) {
            m_execFlag = RUNING_EXEC;
            Execute(0);
        }

        if( (m_clockDown > 0) || m_waiting ) {
            static int c = 0;
            c++;
            if( c == 10 ) {
                c = 0;
                if( CheckInterrupt() ) {
                    m_clockDown = 0;
                    m_syncFlag  = 0;
                    m_waiting   = 0;
                    m_waitPart  = 0;
                    Execute(0);
                }
            }
        }

        m_clock++;

        if( m_clockDown > 0 ) {
            m_clockDown--;
            if( m_clockDown == 0 ) {
                Execute(0);
            }
        }
    }

    if( m_countDown > 0 ) {
        m_countDown--;
        if( m_countDown == 0 ) {
            SHUTDOWN();
        }
    }
    observer[obsClock2]->AssertReady();
}

/////////////////////////////////////////////////////////////////
//
// MWCommand
//
/////////////////////////////////////////////////////////////////
/* MWCommand Subject */
void
RMain::NotifyMWCommand( const ONotifyEvent& event )
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("RMain::NotifyMWCommand()\n"));
#endif //_RMAIN_DEBUG_

    if( m_state_receive_ready == false ) {
        return;
    }

    SubjectID subjectID = event.SenderID();

    for( int i=0;i<event.NumOfData();i++ ) {
        OcaMWCommand      *mwc       = (OcaMWCommand*)event.Data(i);
        OcaInputSemantics  semantics = mwc->GetInputSemantics();
        ISSemanticsID      sem_id    = mwc->GetSemanticsID();
        ISSubInfo          sub_info  = mwc->GetSubInfo();
        longword           num       = mwc->GetCommandNum();

        if( m_startLog ) {
            char buf[MAX_STRING_BUFF];
            g_labels.GetSemanticsStr(buf, sem_id);
            DPRINTF(("MWCommand: sem[%12s/0x%08x] subInfo[0x%08X] num[%X] MWCID[",
                     buf, sem_id, sub_info, num ));
            for( int j=0; j < num; j++ ) {
                DPRINTF((":0x%08x", mwc->GetMWCID( j )));
            }
            DPRINTF(("]\n"));
        }

        OpenrTime   openrtime   = semantics.GetOpenrTime();
        ISComType     comtype   = semantics.GetComType();
        ISComStatus   comstat   = semantics.GetComStatus();
        SubjectID     subject   = semantics.GetSubjectID();
        SensorId      sensor    = semantics.GetSensorID();
        ISSemanticsID sem_sem_id= semantics.GetSemanticsID();

        DPRINTF(("NotifyMWCommand MW SysEvent:%x\n", m_sysEvent ));

        ISSemanticsID semID = sem_id & issemMASK_SEM_ID;
        ISSubInfo subinfo0 = semantics.GetSubInfo(0);
        ISSubInfo subinfo1 = semantics.GetSubInfo(1);


        if (semID == g_labels.GetSemanticsID("SYS_POSTURE")){
            m_spos = subinfo0;

            if (subinfo0 == SPOS_FALL_DOWN &&  m_nofalldown) {
                continue;
            }

            if (subinfo0 == SPOS_GET_UP) {
                m_fdCount = 0;
            }
        }

        if( num > 0 ) {
            m_delayFlag = 0;
            if( mwc->GetMWCID( 0 ) == 0xFFFEFFFF ) ClrCCCMsg();
            m_cccBusy = 0;
            StopMtn();

            char str[MAX_STRING_BUFF];
            g_labels.GetSemanticsStr(str, sem_id);
            DPRINTF(("NotifyMWCommand SEM:%s[%X], %X, %X\n", str, sem_id, subinfo0, subinfo1 ));

            if( semID == g_labels.GetSemanticsID("SYS_POSTURE") ){
                DPRINTF(("NotifyMWCommand MW:SUB:%x:%x, FDCOUNT:%d\n", subinfo0, subinfo1, m_fdCount ));
                if( ( subinfo0 & SPOS_ON ) > 0 ) {
                    if( subinfo0 == SPOS_FALL_DOWN ) m_fdCount++;

                    if( subinfo0 == SPOS_STATION_ON ) {
                        switch( m_stationMode ) {
                        case 0:
                            num=0;
                            OPENR::Fatal( ofatalMEMORY_STICK );
                            break;
                        case 1:
                            m_sysEvent = 2;
                            break;
                        case 2:
                            m_sysEvent = 1;
                            break;
                        }
                    } else if ( m_sysEvent < 2 ) {
                        m_sysEvent = 2;
                    } else {
                        m_sysEvent++;
                        if ( m_fdCount > 3 ) {
                            OPENR::Fatal( ofatalMEMORY_STICK );
                            break;
                        }
                    }
                } else {
                    m_sysEvent = 1;
                }
            }  else {
                m_sysEvent = 1;
            }
        }

        if ( semID == g_labels.GetSemanticsID("EMERGENCY") ){
            m_sysEvent2Down = 1;
            if ( !num ) SHUTDOWN();
        }

        if ( semID == g_labels.GetSemanticsID("BOOT") ) {
            if ( ( subinfo0 & MASK_BOOT_STAT ) == BOOT_STATION &&
                m_stationMode == 0 ) {
                SHUTDOWN();
                return;
            }
        }

        DPRINTF(("SYSEVENT:%d\n", m_sysEvent));
        for( int j=0; j<num ;j++ ) {
            CCCommandQueu ccc( mwc->GetMWCID( j ), observer[obsComplete]->GetID() );
            ccc.SetArbitrationFlag(cccafSEQUENCE);
            PutCCCMsg( &ccc, 0 );
            DPRINTF(("ACTION ID:%X\n", mwc->GetMWCID(j) ));
        }

        if ( num > 0 ){
            if( m_sysEvent == 1 ){
                DPRINTF(("ReStart::::::::::::::\n"));
                PutCCCMsg( NULL, 2 );
            } else {
                if(( semID == g_labels.GetSemanticsID("SYS_POSTURE")) &&
                   ( subinfo0 == SPOS_FALL_DOWN )) {
                    if ( m_recoverPose != 0 ) {
                        DPRINTF(("DelayFlag:ON\n"));
                        m_delayMwcid = m_recoverPose;
                        m_delayFlag = 1;
                    } else {
                        PutCCCMsg( NULL, 2 );
                    }
                }
            }
        }

    }
    observer[obsMWCommand]->AssertReady(subjectID);
}

/////////////////////////////////////////////////////////////////
//
// Message
//
/////////////////////////////////////////////////////////////////
/* Message Subject */
void
RMain::ReadyMessage ( const OReadyEvent& event )
{
    if( m_wlanFlag == NOT_AVAILABLE_WLANFLAG ) return;

    SendSysMsg();
    if( ( m_wlanWait != NO_WAIT_DATA_WLAN ) &&
        ( m_sysMsgHead == NULL ) ) {
        int w = m_wlanWait;
        m_wlanWait = 0;
        if( w == EXECUTE_DATA_WLAN ) Execute(0);
    }
}

void
RMain::PutSysMsg ( char *msg )
{
    if( m_wlanFlag == AVAILABLE_WLANFLAG ) {
        SysMsg *tmp = new SysMsg;
        if( tmp == NULL ) {
            Error( "SysMsg alloc failed!" );
            return;
        }
        tmp->msg[  0] = '\0'; // sentinel
        tmp->msg[199] = '\0'; // sentinel

        tmp->next = NULL;
        if( msg ) strncpy( tmp->msg, msg, 200 );

        if( m_sysMsgHead ) {
            int n = 0;
            SysMsg *x = m_sysMsgHead;
            while( x->next ) { n++; x = x->next; }
            x->next = tmp;
            if( n > 1000 ) printf( "*** SysMsgQueue too long! (>1000) ***\n" );
        } else m_sysMsgHead = tmp;
        SendSysMsg();
    } else {
        if( (unsigned int)(msg[0]) >= 0x20 ) {
            if( msg[strlen(msg)-1] == '\n' ) printf( "%s", msg );
            else                             printf( "%s\n", msg );
        }
    }
}

void
RMain::ClrSysMsg ( void )
{
    SysMsg **msg = &m_sysMsgHead;
    while( *msg ) {
        SysMsg *tmp = *msg;
        *msg = tmp->next;
        delete tmp;
    }
}

void
RMain::SendSysMsg ( void )
{
    if( m_wlanFlag == NOT_AVAILABLE_WLANFLAG ) return;

    OSubject *sbj = subject[sbjMessage];

    if( !(sbj->IsReady()) ) {
        if( m_wlanWait == NO_WAIT_DATA_WLAN ) {
            m_wlanWait = WAITING_DATA_WLAN;
        }
        return;
    }

    SysMsg *tmp = m_sysMsgHead;
    if( tmp ) {
        m_sysMsgHead = tmp->next;
        sbj->SetData( tmp->msg, strlen(tmp->msg)+1 );
        sbj->NotifyObservers();
        delete tmp;
    }
}

void
RMain::Ack ( void )
{
    if( m_control & (1<< 6) ) PutSysMsg( "\006" );
}

void
RMain::Nak ( void )
{
    if( m_control & (1<<21) ) PutSysMsg( "\025" );
}

void
RMain::Syn ( void )
{
    if( m_control & (1<<22) ) PutSysMsg( "\026" );
}

void
RMain::Printf ( char *fmt, ... )
{
    va_list args;
    char msg[MAX_STRING_BUFF];

    va_start( args, fmt );
    vsprintf( msg, fmt, args );
    va_end( args );

    strcat( msg, m_newline );

    PutSysMsg( msg );
}


/////////////////////////////////////////////////////////////////
//
// Database stuff
//
/////////////////////////////////////////////////////////////////
/* The function that scans *.MWC and initialize MtnType[] and MtnMWCID[] */
void
RMain::InitMWC ( void )
{
    ODesignDataID dataID = odesigndataID_UNDEF;
    byte    *data;
    size_t   size;
    OStatus  stat;

    stat = OPENR::FindDesignData( "SO_MWC", &dataID, &data, &size );
    if( stat == oSUCCESS ) {
        byte *p = data+16;
        byte *e = data+size;
        MWCID mwcid;
        ulong n;
        ulong agent;
        long  loopx;
        uchar c;
        int   i;
        int   flag;

        while(1) {
            if( p >= e ) break;

            p += 4;
            memcpy( &mwcid, p, 4 );
            p += 4;
            memcpy( &n,     p, 4 );
            p += 4;

            flag = 0;
            m_MtnType[m_MtnN] = 0;
            for( i=0; i<n; i++ ) {
                p += 4;
                memcpy( &agent, p+0, 4 );
                memcpy( &loopx, p+4, 4 );

                if( (agent & 0xF000) == 0x1000 ) {
                    flag = 1;
                    m_MtnMWCID[m_MtnN] = mwcid;
                    switch( agent ) {
                    case 0x1000:
                    case 0x1010:
                    case 0x1020:
                    case 0x1030:
                    case 0x1040:
                    case 0x1050:
                    case 0x1060:
                        if( loopx != -1 ) {
                            m_MtnType[m_MtnN] = 1;
                        }
                        break;
                    }
                }
                p += 20;
                memcpy( &c,     p, 1 );
                p += 1;
                p += c;
            }

            if( flag ) {
                m_MtnN++;
                if( m_MtnN >= (8*1024) ) Error( "too many motion in *.MWC!" );
            }
        }
    } else {
        Printf( "InitMWC failed (%d)", stat );
    }
}

void
RMain::DumpCDB ( void )
{
    ODesignDataID dataID = odesigndataID_UNDEF;
    byte    *data;
    size_t   size;
    OStatus  stat;
    size_t   n;
    longword m;
    int      i;

    stat = OPENR::FindDesignData( "MWCDB", &dataID, &data, &size );
    if( stat == oSUCCESS ) {
        CommandData *cmdData;

        Printf( "SUCCESS! %4.4s", data );

        data += sizeof(longword);   // m_magic
        data += sizeof(word);       // m_majorVersion
        data += sizeof(word);       // m_minorVersion

        Printf( "section %4.4s", data );
        data += sizeof(longword);   // m_sectionIdC
        n = *((size_t*)data);
        Printf( "size = %d", n );
        data += sizeof(size_t);     // m_sectionSizeC
        data += n;

        Printf( "section %4.4s", data );
        data += sizeof(longword);   // m_sectionIdP
        n = *((size_t*)data);
        Printf( "size = %d", n );
        data += sizeof(size_t);     // m_sectionSizeP
        data += n;

        Printf( "section %4.4s", data );
        data += sizeof(longword);   // m_sectionIdS
        n = *((size_t*)data);
        Printf( "size = %d", n );
        data += sizeof(size_t);     // m_sectionSizeS
        data += n;

        Printf( "section %4.4s", data );
        data += sizeof(longword);   // m_sectionId
        n = *((size_t*)data);
        Printf( "size = %d", n );
        data += sizeof(size_t);     // m_sectionSize
        data += sizeof(word);       // m_majorVersion
        data += sizeof(word);       // m_minorVersion
        data += sizeof(word);       // m_vendorId
        data += sizeof(word);       // m_expCategoryId
        m = *((longword*)data);
        Printf( "nCmd = %d", m );
        data += sizeof(longword);   // m_nCmd

        cmdData = (CommandData*)data;
        Printf( "mwcid---:ex:cL:cM:cS:st:lp:in:lv:pC:startPos:endPos--:resource:sensorId--------" );
        for( i=0; i<m; i++ ) {
            unsigned long *p = (unsigned long *)&(cmdData->m_sensorId);
            Printf( "%08x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%08x:%08x:%08x:%08x%08x",
                    cmdData->m_cid,
                    cmdData->m_exp,
                    cmdData->m_catL,
                    cmdData->m_catM,
                    cmdData->m_catS,
                    cmdData->m_stage,
                    cmdData->m_loop,
                    cmdData->m_independent,
                    cmdData->m_level,
                    cmdData->m_posCange,
                    cmdData->m_startPos,
                    cmdData->m_endPos,
                    cmdData->m_resource,
                    p[0],p[1] );
            cmdData++;
        }
        Printf( "Total:%d", m );
    } else {
        Printf( "FAIL! %d", stat );
    }
}


void
RMain::DumpIDB ( void )
{
    ODesignDataID dataID = odesigndataID_UNDEF;
    byte    *data;
    size_t   size;
    OStatus  stat;
    size_t   n;
    longword m;
    int      i;

    stat = OPENR::FindDesignData( "MWIDB", &dataID, &data, &size );
    if( stat == oSUCCESS ) {
        InformationData *infoData;

        Printf( "SUCCESS! %4.4s", data );

        data += sizeof(longword);   // m_magic
        data += sizeof(word);       // m_majorVersion
        data += sizeof(word);       // m_minorVersion

        Printf( "section %4.4s", data );
        data += sizeof(longword);   // m_sectionIdC
        n = *((size_t*)data);
        Printf( "size = %d", n );
        data += sizeof(size_t);     // m_sectionSizeC
        data += n;

        Printf( "section %4.4s", data );
        data += sizeof(longword);   // m_sectionIdD
        n = *((size_t*)data);
        Printf( "size = %d", n );
        data += sizeof(size_t);     // m_sectionSizeD
        data += n;

        Printf( "section %4.4s", data );
        data += sizeof(longword);   // m_sectionId
        n = *((size_t*)data);
        Printf( "size = %d", n );
        data += sizeof(size_t);     // m_sectionSize
        m = *((longword*)data);
        Printf( "nData = %d", m );
        data += sizeof(longword);   // m_nData

        infoData = (InformationData*)data;
        Printf( "mwiid---:ex:cL:cM:cS:min-----:max-----:res-----:unit----:x1:ty:x2:pt:nu" );
        for( i=0; i<m; i++ ) {
            Printf( "%08x:%02x:%02x:%02x:%02x:%08x:%08x:%08x:%08x:%02x:%02x:%02x:%02x:%02x",
                    infoData->m_iid,
                    infoData->m_exp,
                    infoData->m_catL,
                    infoData->m_catM,
                    infoData->m_catS,
                    infoData->m_min,
                    infoData->m_max,
                    infoData->m_resolution,
                    infoData->m_unit,
                    infoData->m_exp1,
                    infoData->m_type,
                    infoData->m_exp2,
                    infoData->m_part,
                    infoData->m_number );
            infoData++;
        }
        Printf( "Total:%d", m );
    } else {
        Printf( "FAIL! %d", stat );
    }
}

void
RMain::DumpDIC ( void )
{
    char type[32];
    for( short i=0; i<m_dictionary_n; i++ ) {
        switch( GetDictionaryType(i) ) {
        case ARG_NONE:  strcpy( type, "ARG_NONE"   ); break;
        case ARG_VALUE: strcpy( type, "ARG_VALUE"  ); break;
        case ARG_CONST: strcpy( type, "ARG_CONST"  ); break;
        case ARG_STRING:strcpy( type, "ARG_STRING" ); break;
        case ARG_UNDEF: strcpy( type, "ARG_UNDEF"  ); break;
        case ARG_SYSVAR:strcpy( type, "ARG_SYSVAR" ); break;
        case ARG_USRVAR:strcpy( type, "ARG_USRVAR" ); break;
        case ARG_EXTVAR:strcpy( type, "ARG_EXTVAR" ); break;
        case ARG_LABEL: strcpy( type, "ARG_LABEL"  ); break;
        case ARG_DIMVAR:strcpy( type, "ARG_DIMVAR" ); break;
        default:        strcpy( type, "---"        ); break;
        }

        Printf( "[%03d] %-16s %-10s   %d",
                i,
                m_dictionary[i].word,
                type,
                m_dictionary[i].data );
    }
    Printf( "Total:%d", m_dictionary_n );
}

void
RMain::MWSensor ( const OReadyEvent& event )
{
}

/////////////////////////////////////////////////////////////////
//
// RReplay stuff
//
/////////////////////////////////////////////////////////////////
void
RMain::LoadMtn ( char *filename )
{
    OSubject *sbj = subject[sbjMtnCommand];
    char msg[MAX_STRING_BUFF];
    if( !(sbj->IsReady()) ) return;
    m_wait++;
    sprintf( msg, "LOAD:%s", filename );
    sbj->SetData( msg, strlen(msg)+1 );
    sbj->NotifyObservers();
}

void
RMain::LoadLED ( char *filename )
{
    OSubject *sbj = subject[sbjMtnCommand];
    char msg[MAX_STRING_BUFF];
    if( !(sbj->IsReady()) ) return;
    m_wait++;
    sprintf( msg, "LED:%s", filename );
    sbj->SetData( msg, strlen(msg)+1 );
    sbj->NotifyObservers();
}

void
RMain::LoadSND ( char *filename, int n )
{
    OSubject *sbj = subject[sbjMtnCommand];
    char msg[MAX_STRING_BUFF];
    if( !(sbj->IsReady()) ) return;
    m_wait++;
    sprintf( msg, "SND:%s:%d", filename, n );
    sbj->SetData( msg, strlen(msg)+1 );
    sbj->NotifyObservers();
}

void
RMain::PlayMtn ( void )
{
    OSubject *sbj = subject[sbjMtnCommand];
    char msg[MAX_STRING_BUFF];
    if( !(sbj->IsReady()) ) return;
    m_wait++;

    int flag = 0;
    if( (m_mwcPos != mwcSTATION) &&
        (m_mwcPos != mwcPSTATION) &&
        (m_mwcPos != mwcLIFT) &&
        (m_mwcPos != mwcTEMPORARY_POSTURE) ) {
        flag = 1;
    }

    sprintf( msg, "PLAY:%d", flag );
    sbj->SetData( msg, strlen(msg)+1 );
    sbj->NotifyObservers();
}

void
RMain::StopMtn ( void )
{
    OSubject *sbj = subject[sbjMtnCommand];
    char msg[MAX_STRING_BUFF];
    if( !(sbj->IsReady()) ) return;
    m_wait++;
    sprintf( msg, "STOP" );
    sbj->SetData( msg, strlen(msg)+1 );
    sbj->NotifyObservers();
}

void
RMain::InitMtn ( void )
{
    OSubject *sbj = subject[sbjMtnCommand];
    char msg[MAX_STRING_BUFF];
    if( !(sbj->IsReady()) ) return;
    m_wait++;
    sprintf( msg, "INIT" );
    sbj->SetData( msg, strlen(msg)+1 );
    sbj->NotifyObservers();
}

void
RMain::MtnCommand ( const OReadyEvent& event )
{
}

void
RMain::MtnMessage ( const ONotifyEvent& event )
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("MtnMessage\n"));
#endif //_RMAIN_DEBUG_

    char* msg = (char*)event.Data(0);
    PutSysMsg( msg );

    if( m_sysEvent ) {
    } else {
        if( m_wait > 0 ) m_wait--;
        if( m_waiting ) {
            if( m_wait == 0 ) {
                m_waiting   = 0;
                m_waitPart = 0;
                Execute(0);
            }
        }
    }
    observer[obsMtnMessage]->AssertReady();
}

void
RMain::MWCommand2 ( const ONotifyEvent& event )
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("MWCommand2\n"));
#endif //_RMAIN_DEBUG_

    OccMWCommand *mwc = (OccMWCommand*)event.Data(0);

    OSubject *sbj = subject[sbjCCCommand];
    if( !(sbj->IsReady()) ) return;

    if( m_sysEvent ) {
    } else {
        m_soundWait = 1;
        mwc->SetSerial( m_cccSerial++ );
        sbj->SetData( mwc, sizeof(OccMWCommand) );
        sbj->NotifyObservers();
    }
    observer[obsMWCommand2]->AssertReady();
}

void
RMain::RReplayCmd ( char *cmd )
{
    OSubject *sbj = subject[sbjMtnCommand];
    if( !(sbj->IsReady()) ) return;
    sbj->SetData( cmd, strlen(cmd)+1 );
    sbj->NotifyObservers();
}

//
// Direct Message stuff
//
void
RMain::Effector ( const OReadyEvent& event )
{
    SendDirectMsg();
}

void
RMain::PutDirectMsg ( int index, int value )
{
    DirectMsg *tmp = new DirectMsg;
    if( tmp == NULL ) {
        cout << "DirectMsg alloc failed!" << endl;
        return;
    }

    tmp->next  = NULL;
    tmp->index = index;
    tmp->value = value;

    if( m_directMsgHead ) {
        DirectMsg *x = m_directMsgHead;
        while( x->next ) x = x->next;
        x->next = tmp;
    } else m_directMsgHead = tmp;
    SendDirectMsg();
}

void
RMain::SendDirectMsg ( void )
{
    OSubject *sbj = subject[sbjEffector];

    if( !(sbj->IsReady()) ) return;

    DirectMsg *tmp = m_directMsgHead;
    if( tmp ) {
        int idx = tmp->index;
        int val = tmp->value;
        int refcount = sbj->NumberOfReference( m_jointVar[idx].cmdVecID, 0 );
        if( refcount != 0 ) return;

        m_directMsgHead = tmp->next;

        OCommandData* data = m_jointVar[idx].cmdVec->GetData(0);
        switch( m_jointVar[idx].type ) {
        case '3':
        {
            OJointCommandValue3* jcv3 = (OJointCommandValue3*)data->value;
            if ( val == 0 ) {
                jcv3->value = ojoint3_STATE0;
            } else {
                jcv3->value = ojoint3_STATE1;
            }
        }
        break;

        case '2':
        {
            OLEDCommandValue2* lcv2 = (OLEDCommandValue2*)data->value;
            if ( val == 0 ) {
                lcv2->led = oledOFF;
            } else {
                lcv2->led = oledON;
            }
            lcv2->period = 1;
        }
        break;

        default:
        {
            OJointCommandValue2* jcv2 = (OJointCommandValue2*)data->value;
            jcv2->value = micro( radians( (double)val ) );
        }
        break;
        }

        sbj->SetData( m_jointVar[idx].cmdVecID, 0 );
        sbj->NotifyObservers();

        delete tmp;
    }
}

void
RMain::DefReplay ( const OReadyEvent& event )
{
    SendReplayMsg();
}

void
RMain::PutReplayMsg ( ReplayData *data )
{
    ReplayMsg *tmp = new ReplayMsg;
    if( tmp == NULL ) {
        cout << "ReplayMsg alloc failed!" << endl;
        return;
    }

    m_wait++;

    tmp->next = NULL;
    tmp->data = *data;

    if( m_replayMsgHead ) {
        ReplayMsg *x = m_replayMsgHead;
        while( x->next ) x = x->next;
        x->next = tmp;
    } else m_replayMsgHead = tmp;

    SendReplayMsg();
}


void
RMain::SendReplayMsg ( void )
{
    OSubject *sbj = subject[sbjDefReplay];

    if( !(sbj->IsReady()) ) return;

    ReplayMsg *tmp = m_replayMsgHead;
    if( tmp ) {
        m_replayMsgHead = tmp->next;

        sbj->SetData( &(tmp->data), sizeof(ReplayData) );
        sbj->NotifyObservers();

        delete tmp;
    }
}

///////////////////////////////////////////////////
//
// R-CODE 1.2 Legacy Support
// Level 1 IF  -> Image
// Level 1 IF  -> Sensor
// TR.BIN      -> TargetInfo
//
///////////////////////////////////////////////////
//////////////////////////////////
// Image brightness
/////////////////////////////////
void
RMain::NotifyImage ( const ONotifyEvent& event )
{
    if( m_state_receive_ready == false ){
        return;
    }

    static int c = 0;

    if( c == 0 ) {
        OccImageData data( event.Data(0), event.Data(1) );
        OccSensorInformation* osi = (OccSensorInformation*)data.GetSensorInformation();
        int w = osi->GetHorizontal( m_imageLY );
        int h = osi->GetVertical( m_imageLY );
        int d = osi->GetSkip( m_imageLY );
        byte *p = (byte*)data.GetValue( m_imageLY );
        long sum = 0;
        int n = 0;
        int x,y;

        for( y=0; y<h; y++ ) {
            for( x=0; x<w; x++ ) {
                sum += *(p + n);
                n++;
            }
            n += d;
        }

        m_brightness = (short)(sum / (w * h));
    }

    c++;
    if( c > 10 ) c = 0;

    observer[obsImage]->AssertReady();
}

void
RMain::NotifySensor ( const ONotifyEvent& event )
{
    if( m_state_receive_ready == false ){
        return;
    }

    OccSensorData data( event.Data(0), event.Data(1) );
    for( int i=0; i<m_sensVar_n; i++ ) {
        /* It doesn't deal with it except for legacy */
        if( m_sensVar[i].modeID != LEGACY_CONFIG ) continue;

        MWIID mwiid = m_sensVar[i].semOrmwiid;
        switch( m_sensVar[i].type ) {

        case 'R':
        {
            StandardBatteryRest *rest = (StandardBatteryRest*)data.GetValue( mwiid );
            m_sensVar[i].value = m_battRest = rest->GetBatteryRest() / 1000000;
            if( m_trace & TRACE_SENSOR ) {
                Printf( "Sen: %d %s = %d", mwiid, m_sensVar[i].name, m_sensVar[i].value );
            }
            if( m_battRest < m_battRestMin ) {
                if( m_execFlag ) {
                    StopRun(0);
                    StopAIBO(1);
                    char buf[32];
                    sprintf( buf, "PLAY:MWCID:%d", m_chagePose );
                    RunCommand( buf );
                    Printf( "Low Battery !!" );
                }
            }
        }
        break;

        case 'C':
        {
            StandardTemperature *temp = (StandardTemperature*)data.GetValue( mwiid );
            m_sensVar[i].value = temp->GetTemperature() / 1000000;
            if( m_trace & TRACE_SENSOR ) {
                Printf( "Sen: %d %s = %d", mwiid, m_sensVar[i].name, m_sensVar[i].value );
            }
        }
        break;

        case 'D':
        {
            StandardDistance *dist = (StandardDistance*)data.GetValue( mwiid );
            m_sensVar[i].value = dist->GetDistance() / 1000;
            if( m_trace & TRACE_SENSOR ) {
                Printf( "Sen: %d %s = %d", mwiid, m_sensVar[i].name, m_sensVar[i].value );
            }
        }
        break;
        }
    }

    observer[obsSensor]->AssertReady();
}


void
RMain::NotifyTargetInfo ( const ONotifyEvent& event )
{
    if( m_state_receive_ready == false ){
        return;
    }

    m_TRsbjID = event.SenderID();

    for( int i=0; i<event.NumOfData(); i++ ) {
        OccTargetInfo  *info = (OccTargetInfo*)event.Data(i);
        AppID           appId = info->GetAppID();
        TargetStatus    stat  = info->GetTargetStatus();
        TargetPos       pos   = info->GetTargetPos();
        Position3D      pos3d = pos.GetPosition();

        for( int j=0; j<m_sensVar_n; j++ ) {
            if( m_sensVar[j].semOrmwiid == appId ) {
                int flag = 1;

                switch( m_sensVar[j].type ) {
                case '*':
                    m_sensVar[j].value = ((stat==tsFind)?(1):(0));
                    break;
                case 'h':
                    m_sensVar[j].value = (short)degrees( pos3d.GetHAngle() / 1000000.0 );
                    break;
                case 'v':
                    m_sensVar[j].value = (short)degrees( pos3d.GetVAngle() / 1000000.0 );
                    break;
                case 'z':
                    m_sensVar[j].value = pos3d.GetDistance() / 1000;
                    break;

                default:
                    flag = 0;
                    break;
                }

                if( flag && (m_trace & TRACE_TARGET) ) {
                    char *s = "...";
                    switch( stat ) {
                    case tsNotSupport:  s = "NotSupport";   break;
                    case tsUnknown:     s = "Unknown";      break;
                    case tsFind:        s = "Find";         break;
                    case tsLose:        s = "Lose";         break;
                    }
                    Printf( "Tgt: %x %s %s hAngle=%d:%d vAngle=%d:%d Distance=%d:%d",
                            appId,
                            m_sensVar[j].name,
                            s,
                            pos3d.IsEnHAngle(),
                            pos3d.GetHAngle(),
                            pos3d.IsEnVAngle(),
                            pos3d.GetVAngle(),
                            pos3d.IsEnDistance(),
                            pos3d.GetDistance() );
                }
            }
        }
    }

    observer[obsTarget]->AssertReady();
}

void
RMain::NotifyEvent ( const ONotifyEvent& event )
{
    if( m_state_receive_ready == false ){
        return;
    }

    for( int i=0; i<event.NumOfData(); i++ ) {
        OccEventData *data  = (OccEventData*)event.Data(i);
        MWIID         mwiid = data->GetEventID();
        longword      value = data->GetValue();
        for( int j=0; j<m_sensVar_n; j++ ) {
            if( m_sensVar[j].semOrmwiid == mwiid ) {
                switch( m_sensVar[j].type ) {
                    case 'N': m_sensVar[j].value = 1;            break;
                    case 'L': m_sensVar[j].value = 1;            break;
                    case 'P': m_sensVar[j].value = 1;            break;
                    case 'H': m_sensVar[j].value = 1;            break;
                    case 'T': m_sensVar[j].value = 1;            break;
                    case '0': m_sensVar[j].value = 1;            break;
                    case '1': m_sensVar[j].value = 1;            break;
                }

                if( m_trace & TRACE_EVT ) Printf( "Evt: %d %s = %d",
                                                mwiid,
                                                m_sensVar[j].name,
                                                m_sensVar[j].value );
            }
        }
        if( m_trace & TRACE_EV2 ) Printf( "Evt: %08x = %d", mwiid, value );
    }

    observer[obsEvent]->AssertReady();
}


/////////////////////////////////////////////
// PlugIn stuff
/////////////////////////////////////////////
void
RMain::ReadPlugInCfg ( int i, int phase )
{
    char line[MAX_STRING_BUFF];
    int  mode  = 0;
    int  var_i = 0;
    int  cmd_i = 0;
    RMainFiles rf;

    int fp = rf.Open( m_plugin[i].cfg );
    if( fp < 0 ) return;

    while( rf.GetLine( line ) ) {
        char *p = IgnoreSpace(line);
        if( *p == '[' ) {
            mode = 0;
            if( !strncmp( p, "[VAR]", 5 ) ) mode = 1;
            if( !strncmp( p, "[CMD]", 5 ) ) mode = 2;
            continue;
        }
        if( !isalnum(*p) ) continue;

        switch( phase ) {
        case 1:
            switch( mode ) {
            case 1: m_plugin[i].var_n++; break;
            case 2: m_plugin[i].cmd_n++; break;
            }
            break;

        case 2:
            switch( mode ) {
            case 1:
                {
                char var[32];
                char val[32];
                sscanf( p, "%s %s", var, val );
                m_plugin[i].var[var_i] = CopyWord(var);
                m_plugin[i].val[var_i] = AtoI(val);
                AddPlugInVariable( m_plugin[i].var[var_i], &(m_plugin[i].val[var_i]) );
                var_i++;
                }
                break;

            case 2:
                {
                char  cmd[MAX_STRING_BUFF];
                char *c = cmd;
                char *d = cmd;
                int   l = 0;
                sscanf( p, "%s", cmd );
                while(1) {
                    if( (*c == ':') || (*c == '\0') ) {
                        char tmp[32];
                        char *q = tmp;
                        while( d < c ) *q++ = *d++;
                        *q = '\0';
                        d++;
                        q = CopyWord(tmp);
                        if( !SearchID( q, -2, NULL ) ) {
                            RegIDraw( q, ARG_CONST, 0 );
                        }
                    }
                    if( (*c == '%') || (*c == '\0') ) break;
                    l++;
                    c++;
                }
                m_plugin[i].cmd[cmd_i] = CopyWord(cmd);
                m_plugin[i].len[cmd_i] = l;
                cmd_i++;
                }
                break;
            }
            break;
        }
    }
    rf.Close();
}

void
RMain::ReadPlugIn ( void )
{
    char line[MAX_STRING_BUFF];
    char buf1[32];
    char buf2[32];
    RMainFiles rf;

    int n = LineCount( RCODE_PLUGIN_FILE );
    if( n == 0 ) return;
    m_plugin = new PlugIn[n];
    if( !m_plugin ) {
        Error( "can't alloc plugin!" );
        return;
    }

    int fp = rf.Open( RCODE_PLUGIN_FILE );
    if( fp < 0 ) return;
    while( rf.GetLine( line ) ) {
        char *p = IgnoreSpace(line);
        if( !isalnum(*p) ) continue;

        strcpy( buf1, "" );
        strcpy( buf2, "" );

        sscanf( p, "%s %s", buf1, buf2 );

        strcpy( m_plugin[m_plugin_n].name, buf1 );
        strcpy( m_plugin[m_plugin_n].cfg,  buf2 );

        m_plugin_n++;
    }
    rf.Close();
}

void
RMain::RegistPlugIn ( void )
{
    for( int i=0; i<m_plugin_n; i++ )
    {
        if( strlen( m_plugin[i].cfg ) == 0 ) continue;

        if( m_plugin[i].var ) {
            delete m_plugin[i].var;
            delete m_plugin[i].val;
        }
        if( m_plugin[i].cmd ) {
            delete m_plugin[i].cmd;
            delete m_plugin[i].len;
        }
        m_plugin[i].var_n = 0;
        m_plugin[i].var   = NULL;
        m_plugin[i].val   = NULL;
        m_plugin[i].cmd_n = 0;
        m_plugin[i].cmd   = NULL;
        m_plugin[i].len   = NULL;

        ReadPlugInCfg( i, 1 );

        if( m_plugin[i].var_n > 0 ) {
            m_plugin[i].var = new char*[m_plugin[i].var_n];
            m_plugin[i].val = new int[m_plugin[i].var_n];
        }
        if( m_plugin[i].cmd_n > 0 ) {
            m_plugin[i].cmd = new char*[m_plugin[i].cmd_n];
            m_plugin[i].len = new short[m_plugin[i].cmd_n];
        }

        ReadPlugInCfg( i, 2 );
    }
}

/* Subject of PlugIn.Command */
void
RMain::PlugInCommand ( const OReadyEvent& event )
{
    SendPlugInMsg();
}

void
RMain::PutPlugInMsg ( int i, PlugInData* data )
{
    PlugInMsg *tmp = new PlugInMsg;
    if( tmp == NULL ) {
        Error( "PlugInMsg alloc failed!" );
        return;
    }

    tmp->next = NULL;
    tmp->i    = i;
    tmp->data = *data;

    if( m_pluginMsgHead ) {
        PlugInMsg *x = m_pluginMsgHead;
        while( x->next ) x = x->next;
        x->next = tmp;
    } else m_pluginMsgHead = tmp;

    SendPlugInMsg();
}

void
RMain::ClrPlugInMsg ( void )
{
    PlugInMsg **msg = &m_pluginMsgHead;
    while( *msg ) {
        PlugInMsg *tmp = *msg;
        *msg = tmp->next;
        delete tmp;
    }
}

void
RMain::SendPlugInMsg ( void )
{
    OSubject *sbj = subject[sbjPlugInCommand];

    PlugInMsg *tmp = m_pluginMsgHead;
    if( tmp ) {
        if( !(sbj->IsReady( m_plugin[tmp->i].obsID )) ) {
            if( m_trace & TRACE_PLUGIN ) {
                Printf( "Ext: PlugInCommand is not ready." );
            }
            return;
        }

        m_pluginMsgHead = tmp->next;

        sbj->SetData( &(tmp->data), sizeof(PlugInData) );
        sbj->NotifyObserver( m_plugin[tmp->i].obsID );

        delete tmp;
    }
}


void
RMain::PlugInMessage ( const ONotifyEvent& event )
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("PlugInMessage\n"));
#endif //_RMAIN_DEBUG_

    for( int i=0; i<event.NumOfData(); i++ ) {
        PlugInMsgData *data = (PlugInMsgData*)event.Data(i);

        switch( data->type ) {
        case PLUGIN_INI:
            {
                PlugInIniData *ini = (PlugInIniData*)data;
                DPRINTF(( "PlugIn: %s\n", ini->name ));
                for( int j=0; j<m_plugin_n; j++ ) {
                    if( !strcmp( ini->name, m_plugin[j].name ) ) {
                        m_plugin[j].sbjID  = ini->sbjID;
                        m_plugin[j].obsID  = ini->obsID;
                        m_plugin[j].notify = ini->notify;
                        break;
                    }
                }
            }
            break;

        case PLUGIN_MSG:
            {
                PlugInMsgData *msg = (PlugInMsgData*)data;
                switch( msg->mode ) {
                case MSG_STRING:
                    Printf( msg->msg );
                    break;

                case MSG_R_CODE:
                    RunCommand( msg->msg );
                    break;
                }
            }
            break;

        case PLUGIN_VAR:
            {
                PlugInVarData *var = (PlugInVarData*)data;
                int i,j,k;
                for( i=0; i<m_plugin_n; i++ ) {
                    if( event.SenderID() == m_plugin[i].sbjID ) {
                        break;
                    }
                }
                for( k=0; k<var->n; k++ ) {
                    j = var->index[k];
                    m_plugin[i].val[j] = var->value[k];
                }
            }
            break;
        }
    }
    observer[event.ObsIndex()]->AssertReady( event.SenderID() );
}

//////////////////////////////////////////////
// AbleInfo
//////////////////////////////////////////////
void
RMain::NotifyAbleInfo ( const ONotifyEvent& event )
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("RMain::NotifyAbleInfo()\n"));
#endif //_RMAIN_DEBUG_

    if( m_state_receive_ready == false ){
        return;
    }

    for( int i=0; i<event.NumOfData(); i++ ) {
        OccAbleInfo   *able = (OccAbleInfo*)event.Data(i);
        AppID         appId = able->GetAppID();
        CommandCategory cat = able->GetCommandCategory();
        TargetPos       pos = able->GetTargetPos();
        AbleStatus      sts = able->GetAbleStatus();

        CCategoryL catL = cat.GetCategoryL();
        CCategoryM catM = cat.GetCategoryM();
        CCategoryS catS = cat.GetCategoryS();

        for( int j=0; j<m_sensVar_n; j++ ) {
            if( m_sensVar[j].semOrmwiid == appId ) {
                int flag = 1;

                switch( m_sensVar[j].type ) {
                case 's':
                    m_sensVar[j].value = sts;
                    break;

                default:
                    flag = 0;
                    break;
                }

                if( flag && (m_trace & TRACE_ABLE) ) {
                    Printf( "Abl: %x %s %d (%02x.%02x.%02x)",
                            appId,
                            m_sensVar[j].name,
                            m_sensVar[j].value,
                            catL,
                            catM,
                            catS );
                }
            }
        }
    }
    observer[event.ObsIndex()]->AssertReady( event.SenderID() );
}

//////////////////////////////////////////////////////
//
// Semantics & Subinfo
//
/////////////////////////////////////////////////////
bool
RMain::GetRMainSemanticsLabels(void)
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("RMain::GetRMainSemanticsLabels()\n"));
#endif //_RMAIN_DEBUG_

    if ( GetSemLabels() == false ) {
        Printf("GetRMainSemanticsLabels  SEM false");
        return false;
    }
    if ( GetSubLabels() == false ) {
        Printf("GetRMainSemanticsLabels  SUB false");
        return false;
    }
    return true;
}

bool
RMain::GetSemLabels(void)
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("GetSemLabels()\n"));
#endif //_RMAIN_DEBUG_

    char filename[MAX_PATH_LEN];
    RMainFiles rf;

    strcpy( filename, SEM_LABEL_FILE );
    int fp = rf.Open( filename );
    if ( fp < 0 ) {
        return false;
    }

    void* buf = rf.GetBuff();
    size_t size = rf.GetSize();

    *(static_cast<char*>(buf) + size) = '\0';
    char* p = static_cast<char*>(buf);
    int sem_label_cnt = 0;
    while ( *p ) {
        while ( *p == ' ' || *p == '\t' ) {
            ++p;
        }
        if ( *p == '#' ) {
            while ( *p ) {
                if ( strncmp(p, "\r\n", 2) == 0 ) {
                    p += 2;
                    break;
                }
                ++p;
            }
        } else if ( *p && (strncmp(p, "\r\n", 2) == 0) ) {
            p += 2;
        } else {
            ++sem_label_cnt;
            while ( *p ) {
                if ( *p && (strncmp(p, "\r\n", 2) == 0) ) {
                    p += 2;
                    break;
                }
                ++p;
            }
        }
    }
    g_labels.m_num_of_debug_sem = sem_label_cnt;

    int buf_size = g_labels.m_num_of_debug_sem * sizeof(DebugSem);
    sError error = NewRegion(buf_size,
                            reinterpret_cast<void**>(&g_labels.m_debug_sem));
    if ( error != sSUCCESS ) {
        // Memory allocation failure
        OSYSLOG1((osyslogERROR, "err! (memory allocation error),"
                " num_of_debug_sem:%d GtSemLbl", g_labels.m_num_of_debug_sem));
        g_labels.m_debug_sem = static_cast<DebugSem*>(NULL);
        if ( DeleteRegion(buf) != sSUCCESS ) {
            OSYSLOG1((osyslogERROR, "err! (delete buffer error 2),"
                    " file:%s GtSemLbl", filename));
        }
        return false;
    } else {
        memset(g_labels.m_debug_sem, 0, buf_size);
    }

    p = static_cast<char*>(buf);
    sem_label_cnt = 0;
    while ( *p ) {
        DebugSem& debug_sem = g_labels.m_debug_sem[sem_label_cnt];
        while ( *p == ' ' || *p == '\t' ) {
            ++p;
        }
        if ( p && ( *p == '#'
                || (strncmp(p, "[Version]", strlen("[Version]")) == 0) ) ) {
            while ( *p ) {
                if ( strncmp(p, "\r\n", 2) == 0 ) {
                    p += 2;
                    break;
                }
                ++p;
            }
        } else if ( *p && (strncmp(p, "\r\n", 2) == 0) ) {
            p += 2;
        } else {
            // semantics name
            if ( isalnum(*p) ) {
                char* tmp_p = debug_sem.m_semantics_str;
                while ( *p != ' ' && *p != '\t' ) {
                    *tmp_p = *p;
                    ++tmp_p;
                    ++p;
                }
                *tmp_p = '\0';
            }
            while ( *p && (*p == ' ' || *p == '\t') ) {
                ++p;
            }

            // skip tag
            if ( isalnum(*p) ) {
                while ( *p != ' ' && *p != '\t' ) {
                    ++p;
                }
            }
            while ( *p && (*p == ' ' || *p == '\t') ) {
                ++p;
            }

            // skip local semantics id
            if ( isalnum(*p) ) {
                while ( *p != ' ' && *p != '\t' ) {
                    ++p;
                }
            }
            while ( *p && (*p == ' ' || *p == '\t') ) {
                ++p;
            }

            // semantics id
            longword    tmp_data;
            if ( *p && strncmp(p, "0x", 2) == 0 ) {
                p += 2;
                tmp_data = 0;
                while ( *p && isalnum(*p) ) {
                    if ( isdigit(*p) ) {
                        tmp_data += *p - '0';
                    } else if ( *p >= 'A' && *p <= 'Z' ) {
                        tmp_data += 10 + *p - 'A';
                    } else if ( *p >= 'a' && *p <= 'z' ) {
                        tmp_data += 10 + *p - 'a';
                    }
                    ++p;
                    if ( *p && isalnum(*p) ) {
                        tmp_data <<= 4;
                    }
                }
            } else {
                tmp_data = atoi(p);
                while ( *p && isalnum(*p) ) {
                    ++p;
                }
            }
            debug_sem.m_semantics_id = tmp_data;
            ++sem_label_cnt;

            while ( *p ) {
                if ( *p && (strncmp(p, "\r\n", 2) == 0) ) {
                    p += 2;
                    break;
                }
                ++p;
            }
        }
    }

    rf.Close();
    return true;
}

bool
RMain::GetSubLabels(void)
{
#ifdef _RMAIN_DEBUG_
    DPRINTF(("GetSubLabels()\n"));
#endif //_RMAIN_DEBUG_

    char filename[MAX_PATH_LEN];
    RMainFiles rf;

    strcpy( filename, SUB_LABEL_FILE );
    int fp = rf.Open( filename );
    if ( fp < 0 ) {
        return false;
    }

    void* buf = rf.GetBuff();
    size_t size = rf.GetSize();

    *(static_cast<char*>(buf) + size) = '\0';
    char* p = static_cast<char*>(buf);
    int mask_cnt = 0;
    while ( *p ) {
        while ( *p == ' ' || *p == '\t' ) {
            ++p;
        }
        if ( *p == '#' ) {
            while ( *p ) {
                if ( strncmp(p, "\r\n", 2) == 0 ) {
                    p += 2;
                    break;
                }
                ++p;
            }
        } else if ( *p && (strncmp(p, "\r\n", 2) == 0) ) {
            p += 2;
        } else {
            if ( *p == 'M' ) {
                ++mask_cnt;
            }
            while ( *p ) {
                if ( *p && (strncmp(p, "\r\n", 2) == 0) ) {
                    p += 2;
                    break;
                }
                ++p;
            }
        }
    }
    g_labels.m_num_of_debug_mask = mask_cnt;

    int buf_size = g_labels.m_num_of_debug_mask * sizeof(DebugMask);
    sError error = NewRegion(buf_size,
                            reinterpret_cast<void**>(&g_labels.m_debug_mask));
    if ( error != sSUCCESS ) {
        // Memory allocation failure
        OSYSLOG1((osyslogERROR, "err! (memory allocation error 2),"
                " num_of_debug_mask:%d, GtSubLbl",
                g_labels.m_num_of_debug_mask));
        g_labels.m_debug_mask = static_cast<DebugMask*>(NULL);
        if ( DeleteRegion(buf) != sSUCCESS ) {
            OSYSLOG1((osyslogERROR, "err! (delete buffer error 2),"
                    " file:%s GtSubLbl", filename));
        }
        return false;
    } else {
        for ( int i = 0; i < g_labels.m_num_of_debug_mask; ++i ) {
            new (&g_labels.m_debug_mask[i]) DebugMask();
        }
    }

    p = static_cast<char*>(buf);
    mask_cnt = 0;
    longword tmp_subinfo_index = SEM_SUBINFO_1;

    while ( *p ) {
        while ( *p == ' ' || *p == '\t' ) {
            ++p;
        }
        if ( *p == '#' ) {
            while ( *p ) {
                if ( strncmp(p, "\r\n", 2) == 0 ) {
                    p += 2;
                    break;
                }
                ++p;
            }
        } else if ( *p && (strncmp(p, "\r\n", 2) == 0) ) {
            p += 2;
        } else if ( *p && (strncmp(p, "INDEX", 5) == 0) ) {
            tmp_subinfo_index = 0;
            p += 5;
            while ( *p == ' ' || *p == '\t' ) {
                ++p;
            }
            while ( *p ) {
                if ( *p && isdigit(*p) ) {
                    switch ( atoi(p) ) {
                    case 1:
                        tmp_subinfo_index |= SEM_SUBINFO_1;
                        break;
                    case 2:
                        tmp_subinfo_index |= SEM_SUBINFO_2;
                        break;
                    case 3:
                        tmp_subinfo_index |= SEM_SUBINFO_3;
                        break;
                    case 4:
                        tmp_subinfo_index |= SEM_SUBINFO_4;
                        break;
                    default:
                        ++p;
                        break;
                    }
                }
                while ( *p && isalnum(*p) ) {
                    ++p;
                }
                while ( *p == ' ' || *p == '\t' ) {
                    ++p;
                }
                if ( strncmp(p, "\r\n", 2) == 0 ) {
                    p += 2;
                    break;
                }
            }
        } else if ( *p == 'M' ) {
            DebugMask&    debug_mask = g_labels.m_debug_mask[mask_cnt];
            ++p;
            while ( *p == ' ' || *p == '\t' ) {
                ++p;
            }

            // subinfo index
            debug_mask.m_subinfo_index = tmp_subinfo_index;

            // mask name
            if ( isalnum(*p) ) {
                char* tmp_p = debug_mask.m_mask_str;
                while ( *p != ' ' && *p != '\t' ) {
                    *tmp_p = *p;
                    ++tmp_p;
                    ++p;
                }
                *tmp_p = '\0';
            }
            while ( *p && (*p == ' ' || *p == '\t') ) {
                ++p;
            }

            // mask id
            longword    tmp_data;
            if ( *p && strncmp(p, "0x", 2) == 0 ) {
                p += 2;
                tmp_data = 0;
                while ( *p && isalnum(*p) ) {
                    if ( isdigit(*p) ) {
                        tmp_data += *p - '0';
                    } else if ( *p >= 'A' && *p <= 'Z' ) {
                        tmp_data += 10 + *p - 'A';
                    } else if ( *p >= 'a' && *p <= 'z' ) {
                        tmp_data += 10 + *p - 'a';
                    }
                    ++p;
                    if ( *p && isalnum(*p) ) {
                        tmp_data <<= 4;
                    }
                }
            } else {
                tmp_data = atoi(p);
                while ( *p && isalnum(*p) ) {
                    ++p;
                }
            }
            debug_mask.m_mask_id = tmp_data;

            while ( *p && (*p == ' ' || *p == '\t') ) {
                ++p;
            }

            while ( *p ) {
                char    sem_buf[32];
                sem_buf[0] = '\0';
                char* tmp_p = sem_buf;
                while ( *p != ' ' && *p != '\t'
                        && strncmp(p, "\r\n", 2) != 0 ) {
                    *tmp_p = *p;
                    ++tmp_p;
                    ++p;
                }
                *tmp_p = '\0';
                debug_mask.m_semantics_id_list.push_back(
                        g_labels.GetSemanticsID(sem_buf));
                while ( *p && (*p == ' ' || *p == '\t') ) {
                    ++p;
                }
                if ( *p && (strncmp(p, "\r\n", 2) == 0) ) {
                    p += 2;
                    break;
                }
            }
            ++mask_cnt;

        } else if ( *p == 'S' ) {
            DebugSubInfo  debug_subinfo;
            ++p;
            while ( *p == ' ' || *p == '\t' ) {
                ++p;
            }

            // subinfo name
            if ( isalnum(*p) ) {
                char* tmp_p = debug_subinfo.m_subinfo_str;
                while ( *p != ' ' && *p != '\t' ) {
                    *tmp_p = *p;
                    ++tmp_p;
                    ++p;
                }
                *tmp_p = '\0';
            }
            while ( *p && (*p == ' ' || *p == '\t') ) {
                ++p;
            }

            // subinfo id
            longword    tmp_data;
            if ( *p && strncmp(p, "0x", 2) == 0 ) {
                p += 2;
                tmp_data = 0;
                while ( *p && isalnum(*p) ) {
                    if ( isdigit(*p) ) {
                        tmp_data += *p - '0';
                    } else if ( *p >= 'A' && *p <= 'Z' ) {
                        tmp_data += 10 + *p - 'A';
                    } else if ( *p >= 'a' && *p <= 'z' ) {
                        tmp_data += 10 + *p - 'a';
                    }
                    ++p;
                    if ( *p && isalnum(*p) ) {
                        tmp_data <<= 4;
                    }
                }
            } else {
                tmp_data = atoi(p);
                while ( *p && isalnum(*p) ) {
                    ++p;
                }
            }
            debug_subinfo.m_subinfo = tmp_data;

            while ( *p && (*p == ' ' || *p == '\t') ) {
                ++p;
            }

            // mask name and id
            if ( isalnum(*p) ) {
                char    mask_buf[32];
                mask_buf[0] = '\0';
                char* tmp_p = mask_buf;
                while ( *p != ' ' && *p != '\t'
                        && strncmp(p, "\r\n", 2) != 0 ) {
                    *tmp_p = *p;
                    ++tmp_p;
                    ++p;
                }
                *tmp_p = '\0';
                for ( int i = 0; i < g_labels.m_num_of_debug_mask; ++i ) {
                    DebugMask& tmp_mask = g_labels.m_debug_mask[i];
                    if ( strcmp(tmp_mask.m_mask_str, mask_buf) == 0 ) {
                        tmp_mask.m_subinfo_list.push_back(debug_subinfo);
                        break;
                    }
                }
                if ( *p && (strncmp(p, "\r\n", 2) == 0) ) {
                    p += 2;
                }
            }
            while ( *p && (*p == ' ' || *p == '\t') ) {
                ++p;
            }
        } else {
            ++p;
        }
    }

    rf.Close();
    return true;
}


//
// Get semantics state by semantics id
//
OcaInputSemantics*
RMain::GetSemanticsState(ISSemanticsID sem_id)
{
    int num_of_sem_state = m_num_of_sem_state_unit * SEM_STATE_UNIT_SIZE;
    for ( int i = 0; i < num_of_sem_state; ++i ) {
        if ( m_sem_state[i].GetSemanticsID() == sem_id ) {
            return &m_sem_state[i];
        } else if ( m_sem_state[i].GetSemanticsID() == issemINVALID ) {
            return &m_sem_state[i];
        }
    }

    OcaInputSemantics* tmp_sem_state;
    int org_size = 0;
    if ( num_of_sem_state > 0 ) {
        // Memory allocation for file.
        org_size = num_of_sem_state * sizeof(OcaInputSemantics);
        sError error = NewRegion(org_size,
                        reinterpret_cast<void**>(&tmp_sem_state));
        if ( error != sSUCCESS ) {
            // Memory allocation failure
            OSYSLOG4((osyslogWARNING, "err! (memory allocation error),"
                    " num_of_sem:%d tmp_buf, GtSemSt", num_of_sem_state));
            return false;
        }
        memcpy(tmp_sem_state, m_sem_state, org_size);
        if ( DeleteRegion(m_sem_state) != sSUCCESS ) {
            OSYSLOG1((osyslogERROR, "err! "
                    "(delete memory region error, m_sem_state),"
                    " GtSemSt"));
            return false;
        }
    }

    int size = (num_of_sem_state + SEM_STATE_UNIT_SIZE)
                    * sizeof(OcaInputSemantics);
    sError error = NewRegion(size,
                    reinterpret_cast<void**>(&m_sem_state));
    if ( error != sSUCCESS ) {
        // Memory allocation failure
        OSYSLOG4((osyslogWARNING, "err! (memory allocation error),"
                " num_of_sem:%d, GtSemSt",
                (num_of_sem_state + SEM_STATE_UNIT_SIZE)));
        m_sem_state = static_cast<OcaInputSemantics*>(NULL);
        return (OcaInputSemantics *)NULL;
    } else {
        if ( num_of_sem_state > 0 ) {
            memcpy(m_sem_state, tmp_sem_state, org_size);
        }
        for ( int i = num_of_sem_state;
                i < num_of_sem_state + SEM_STATE_UNIT_SIZE; ++i ) {
            new (&m_sem_state[i]) OcaInputSemantics();
        }
        ++m_num_of_sem_state_unit;
        return &m_sem_state[num_of_sem_state];
    }
}

void
RMain::PrintInputSemantics( OcaInputSemantics &sem )
{

    OpenrTime     openrtime = sem.GetOpenrTime();
    ISComType     comtype   = sem.GetComType();
    ISComStatus   comstat   = sem.GetComStatus();
    SubjectID     subject   = sem.GetSubjectID();
    SensorId      sensor    = sem.GetSensorID();
    ISSemanticsID sem_id    = sem.GetSemanticsID();
    ISSubInfo     subinfo[MAX_OF_SUBINFO];
    for( int j=0;j<MAX_OF_SUBINFO;j++ ) {
        subinfo[j]=sem.GetSubInfo(j);
    }
    ISLevel     level       = sem.GetLevel();
    ISPitch     pitch       = sem.GetPitch();
    ISPosition  position    = sem.GetPosition();

    char buf[MAX_STRING_BUFF];
    char tmp[MAX_STRING_BUFF];
    strcpy( buf, "" );

    sprintf( tmp, "%08X ", openrtime );
    strcat( buf, tmp );

    switch( comtype ) {
        // isctINVALID, isctINVALID_COMTYPE, isctINVALID_DISCARD
    case isctINVALID:       strcat( buf, "INVALID  " ); break;
    case isctREGISTER:      strcat( buf, "REGISTER " ); break;
    case isctNSUPPORT:      strcat( buf, "NSUPPORT " ); break;
    case isctNOTIFY:        strcat( buf, "NOTIFY   " ); break;
    case isctSUB_NOTIFY:    strcat( buf, "S_NOTIFY " ); break;
    case isctDISCARD:       strcat( buf, "DISCARD  " ); break;
    case isctMASK_COMTYPE:  strcat( buf, "M_COMTYPE" ); break;
    case isctMASK_DISCARD:  strcat( buf, "M_DISCARD" ); break;
    default:
        sprintf( tmp, "....%04X ", comtype );
        strcat( buf, tmp );
        break;
    }

    switch( comstat ) {
    case iscsINVALID:       strcat( buf, "INVALID  " ); break;
    case iscsON:            strcat( buf, "ON       " ); break;
    case iscsOFF:           strcat( buf, "OFF      " ); break;
    case iscsDONTCARE:      strcat( buf, "DONTCARE " ); break;
    default:
        sprintf( tmp, "....%04X ", comstat );
        strcat( buf, tmp );
        break;
    }

    g_labels.GetSemanticsStr(tmp, sem_id);
    sprintf(g_debug_buf, "sem[%12s/0x%08x] \0", tmp, sem_id);
    strcat( buf, g_debug_buf );

    sprintf( tmp, "oid[0x%08x] Sel[0x%08x] ",
             subject.GetOID(), subject.GetSelector() );
    strcat( buf, tmp );

    sprintf( tmp, "sensorId[0x%08x] ", sensor.GetParts() );
    strcat( buf, tmp );

    sprintf( tmp, "sub:0x%08X/0x%08X/0x%08X/0x%08X ",
             subinfo[0], subinfo[1], subinfo[2], subinfo[3] );
    strcat( buf, tmp );

    switch( position ) {
    case isposINVALID:      strcat( buf, "INVALID   " ); break;
    case isposFAR:          strcat( buf, "FAR       " ); break;
    case isposMID:          strcat( buf, "MID       " ); break;
    case isposNEAR:         strcat( buf, "NEAR      " ); break;
    case isposFORWARD:      strcat( buf, "FORWARD   " ); break;
    case isposRFORWARD:     strcat( buf, "RFORWARD  " ); break;
    case isposRIGHT:        strcat( buf, "RIGHT     " ); break;
    case isposRBACKWARD:    strcat( buf, "RBACKWARD " ); break;
    case isposBACKWARD:     strcat( buf, "BACKWARD  " ); break;
    case isposLBACKWARD:    strcat( buf, "LBACKWARD " ); break;
    case isposLEFT:         strcat( buf, "LEFT      " ); break;
    case isposLFORWARD:     strcat( buf, "LFORWARD  " ); break;
    case isposOVER:         strcat( buf, "OVER      " ); break;
    case isposABOVE:        strcat( buf, "ABOVE     " ); break;
    case isposLEVEL:        strcat( buf, "LEVEL     " ); break;
    case isposBELOW:        strcat( buf, "BELOW     " ); break;
    case isposUNDER:        strcat( buf, "UNDER     " ); break;
    default:
        sprintf( tmp, ".%08X ", position );
        strcat( buf, tmp );
        break;
    }

    switch( level ) {
    case islvlINVALID:      strcat( buf, "INVALID  " ); break;
    case islvlVERYLOW:      strcat( buf, "VERYLOW  " ); break;
    case islvlLOW:          strcat( buf, "LOW      " ); break;
    case islvlMIDDLE:       strcat( buf, "MIDDLE   " ); break;
    case islvlHIGH:         strcat( buf, "HIGH     " ); break;
    case islvlVERYHIGH:     strcat( buf, "VERYHIGH " ); break;
    default:
        sprintf( tmp, "...%04X ", level );
        strcat( buf, tmp );
        break;
    }

    switch( pitch ) {
    case ispchINVALID:      strcat( buf, "INVALID  " ); break;
    case ispchVERYLOW:      strcat( buf, "VERYLOW  " ); break;
    case ispchLOW:          strcat( buf, "LOW      " ); break;
    case ispchMIDDLE:       strcat( buf, "MIDDLE   " ); break;
    case ispchHIGH:         strcat( buf, "HIGH     " ); break;
    case ispchVERYHIGH:     strcat( buf, "VERYHIGH " ); break;
    default:
        sprintf( tmp, "...%04X ", pitch );
        strcat( buf, tmp );
        break;
    }

    Printf( "SEM: %s", buf );
}
