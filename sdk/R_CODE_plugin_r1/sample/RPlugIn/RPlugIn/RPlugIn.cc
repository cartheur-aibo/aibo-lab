//
// Copyright 1997,1998,1999,2000,2001,2006 Sony Corporation
//
// Permission to use, copy, modify, and redistribute this software for
// non-commercial use is hereby granted.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//
#include <OPENR/InformationCategoryTypes.h>
#include <OPENR/OccDataFormats.h>
#include <OPENR/OFbkImage.h>
#include <OPENR/ODebug.h>
#include <OPENR/core_macro.h>
#include <OPENR/OSyslog.h>
#include "RPlugIn.h"

#include "PlugIn.h"

RPlugIn::RPlugIn(): m_imageSave(false),
                    m_soundRecState(SRS_IDLE), m_soundBuf(0), m_soundBufPtr(0),
                    m_ntp()
{
    DPRINTF(("RPlugIn::RPlugIn()"));

    sprintf(m_bmp_filename, "%s/%s", RCODE_BASE_PATH, "DEFAULT.BMP");
    sprintf(m_wav_filename, "%s/%s", RCODE_BASE_PATH, "DEFAULT.WAV");
    memset(m_msgBuf, 0, sizeof(m_msgBuf));
}

RPlugIn::~RPlugIn()
{
}

OStatus 
RPlugIn::DoInit(const OSystemEvent& event)
{
    DPRINTF(("RPlugIn::DoInit() is invoked.\n"));

    // ObjComm
    NEW_ALL_SUBJECT_AND_OBSERVER;
    REGISTER_ALL_ENTRY;

    // Access information database
    if ( m_idb.ReadInfoDB()    != oSUCCESS ) 
        DPRINTF(("camera::DoInit ReadInfoDB Error\n")); 

    // Image

    // Audio
    NewSoundBuffer();

    // Network
    m_ntp.SetMsgBuf(m_msgBuf, sizeof(m_msgBuf));
    m_ntp.DoInit(event);
    m_es.SetMsgBuf(m_msgBuf, sizeof(m_msgBuf));
    m_es.DoInit(event);

    // ObjComm
    SET_ALL_READY_AND_NOTIFY_ENTRY;

    return oSUCCESS;
}

OStatus 
RPlugIn::DoStart(const OSystemEvent& event)
{
    DPRINTF(("RPlugIn::DoStart() is invoked.\n"));

    // RequireMWInfo
    MWIID iid=0;

    // RequireMWInfo Image_LayerH_Y
    m_idb.SetInfoExp1( efCOMPATIBLE );
    m_idb.SetInfoCatL( iclLAYER_H );
    m_idb.SetInfoCatM( icmIMAGE_Y );
    m_idb.SetInfoCatS( icsOccImageData );
    if ( m_idb.FindAllMWIID( &iid ) == oSUCCESS ) 
    {
        Image_LayerH_Y = iid;
        OPENR::RequireMWInfo( Image_LayerH_Y );
    }
    
    // RequireMWInfo Image_LayerH_Cr
    m_idb.SetInfoExp1( efCOMPATIBLE );
    m_idb.SetInfoCatL( iclLAYER_H );
    m_idb.SetInfoCatM( icmIMAGE_CR );
    m_idb.SetInfoCatS( icsOccImageData );
    if ( m_idb.FindAllMWIID( &iid ) == oSUCCESS )
    {
        Image_LayerH_Cr = iid;
        OPENR::RequireMWInfo( Image_LayerH_Cr );
    }
    
    // RequireMWInfo Image_LayerH_Cb
    m_idb.SetInfoExp1( efCOMPATIBLE );
    m_idb.SetInfoCatL( iclLAYER_H );
    m_idb.SetInfoCatM( icmIMAGE_CB );
    m_idb.SetInfoCatS( icsOccImageData );
    if ( m_idb.FindAllMWIID( &iid ) == oSUCCESS )
    {
        Image_LayerH_Cb = iid;
        OPENR::RequireMWInfo( Image_LayerH_Cb );
    }

    // RequireMWInfo Sound_Stereo
    m_idb.SetInfoExp1( efCOMPATIBLE );
    m_idb.SetInfoCatL( iclSOUND );
    m_idb.SetInfoCatM( icmSTEREO );
    m_idb.SetInfoCatS( icsOccAudioData );
    if ( m_idb.FindAllMWIID( &iid ) == oSUCCESS )
    {
        Sound_Stereo = iid;
        OPENR::RequireMWInfo( Sound_Stereo );
    }

    // Image

    // Audio

    // Network
    m_ntp.DoStart(event);
    m_es.DoStart(event);

    // ObjComm
    ENABLE_ALL_SUBJECT;
    ASSERT_READY_TO_ALL_OBSERVER;
    
    return oSUCCESS;
}

OStatus 
RPlugIn::DoStop(const OSystemEvent& event)
{
    DPRINTF(("RPlugIn::DoStop() is invoked.\n"));

    // Image

    // Audio
    m_soundRecState = SRS_DOSTOP;

    // Network
    m_ntp.DoStop(event);
    m_es.DoStop(event);

    // ObjComm
    DISABLE_ALL_SUBJECT;
    DEASSERT_READY_TO_ALL_OBSERVER;

    return oSUCCESS;
}

OStatus 
RPlugIn::DoDestroy(const OSystemEvent& event)
{
    DPRINTF(("RPlugIn::DoDestroy() is invoked.\n"));

    // ObjComm
    DELETE_ALL_SUBJECT_AND_OBSERVER;

    // Image

    // Audio
    DeleteSoundBuffer();

    // Network
    m_ntp.DoDestroy(event);
    m_es.DoDestroy(event);

    return oSUCCESS;
}

void 
RPlugIn::PlugInMessage ( const OReadyEvent& event )
{
    DPRINTF(("RPlugIn::PlugInMessage() is invoked.\n"));
    static int ini = 1;
    OSubject *sbj  = subject[sbjPlugInMessage];

    if( ini ) {
        PlugInIniData data;
        data.type = PLUGIN_INI;
        strcpy( data.name, "RPlugIn" );
        data.sbjID  = subject[sbjPlugInMessage]->GetID();
        data.obsID  = observer[obsPlugInCommand]->GetID();
        //data.notify = NOTIFY_INIT|NOTIFY_VAR|NOTIFY_STOP|NOTIFY_QUIT;
        data.notify = 0;
      
        sbj->SetData( &data, sizeof(PlugInIniData) );
        sbj->NotifyObservers();
      
        ini = 0;
    }
}

inline int CheckCommand(const char* msg, const char* cmd)
{
    return strncmp(msg, cmd, strlen(cmd));
}

void 
RPlugIn::PlugInCommand ( const ONotifyEvent& event )
{
    PlugInData *data = (PlugInData*)event.Data(0);
    switch( data->type ) {
    case PLUGIN_MSG:
    {
        PlugInMsgData *msg_data = (PlugInMsgData*)data;
        DPRINTF(("RPlugIn: MSG [%s]\n", msg_data->msg ));
        if (msg_data->mode == MSG_R_CODE)
        {
            if (!CheckCommand( msg_data->msg, RCMD_SAVE_IMAGE))
            {
                // Image
                OSYSPRINT(("SAVE IMAGE\n"));
                char* filename = msg_data->msg + strlen(RCMD_SAVE_IMAGE);
                if(strlen(filename) < FILENAME_SIZE && m_imageSave == false);
                {
                    sprintf(m_bmp_filename, "%s/%s", 
                            RCODE_BASE_PATH, filename);
                    m_imageSave = true;
                }
            }
            else if (!CheckCommand(msg_data->msg, RCMD_SAVE_AUDIO))
            {
                // Audio
                char* filename = msg_data->msg + strlen(RCMD_SAVE_AUDIO);
                if (m_soundBuf != 0 && m_soundRecState == SRS_IDLE &&
                    strlen(filename) < FILENAME_SIZE)
                {
                    OSYSPRINT(("START RECORDING (about %d sec) ... \n",
                               (NUMOF_REC_FRAME * 32)/1000 ));
                    sprintf(m_wav_filename, "%s/%s", 
                            RCODE_BASE_PATH, filename);
                    m_soundRecState = SRS_START;
                }
                else if (m_soundRecState == SRS_START)
                {
                    OSYSPRINT(("NOW RECORDING !\n"));
                }
                else
                {
                    OSYSPRINT(("Can't start record\n"));
                }
            }
            else if (!CheckCommand(msg_data->msg, RCMD_SEND_NTP))
            {
                // Network
                OSYSPRINT(("SEND NTP packet ...\n"));
                m_ntp.SyncNTPServer();
            }
            else
            {
                OSYSPRINT(("Unknown command. %s\n", msg_data->msg));
            }
        }
    }
    break;
      
    case PLUGIN_VAR:
    {
        PlugInVarData *var_data = (PlugInVarData*)data;
        DPRINTF(( "RPlugIn: VAR " ));
        for( int i=0; i<var_data->n; i++ )
        {
            DPRINTF(( "[%d]=%d ", var_data->index[i], var_data->value[i] ));
        }
        DPRINTF(( "\n" ));
    }
    break;
    }

    observer[event.ObsIndex()]->AssertReady( event.SenderID() );
}










