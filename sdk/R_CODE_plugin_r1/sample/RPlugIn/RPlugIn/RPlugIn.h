//
// Copyright 1997,1998,1999,2006 Sony Corporation
//
// Permission to use, copy, modify, and redistribute this software for
// non-commercial use is hereby granted.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//

#ifndef _RPlugIn_h_DEFINED
#define _RPlugIn_h_DEFINED

#include <OPENR/OPENR.h>
#include <OPENR/OPENRAPI.h>
#include <OPENR/OPENRMessages.h>
#include <OPENR/ODataFormats.h>
#include <OPENR/OObject.h>
#include <OPENR/OObserverVector.h>
#include <OPENR/OSubject.h>
#include "def.h"

#include "InfoDB.h"

#include "NTP/NTP.h"
#include "EchoServer/EchoServer.h"

enum SoundRecState {
    SRS_IDLE,
    SRS_START,
    SRS_DOSTOP
};

#define RCMD_SAVE_IMAGE "SAVE:IMAGE:"
#define RCMD_SAVE_AUDIO "SAVE:AUDIO:"
#define RCMD_SEND_NTP   "SEND:NTP"

#define FILENAME_SIZE   64
#define RCODE_BASE_PATH "/MS/OPEN-R/APP/PC/AMS"

class RPlugIn : public OObject {
public:
    RPlugIn();
    ~RPlugIn();

    OSubject*        subject [numOfSubject];
    OObserverVector* observer[numOfObserver];

    // Image:ImageSave
    void UpdateImage(const ONotifyEvent& event);
    // Audio:SoundRec
    void UpdateAudio(const ONotifyEvent& event);
    // Network:NTP
    void GetHostByNameContNTP (ANTENVMSG msg);
    void SendContNTP          (ANTENVMSG msg);
    void ReceiveContNTP      (ANTENVMSG msg);
    // Network:EchoServer
    void ListenContES (ANTENVMSG msg);
    void SendContES   (ANTENVMSG msg);
    void ReceiveContES(ANTENVMSG msg);
    void CloseContES  (ANTENVMSG msg);

    // R-CODE
    void PlugInMessage( const OReadyEvent& event );
    void PlugInCommand( const ONotifyEvent& event );

private:

    virtual OStatus DoInit   (const OSystemEvent& event);
    virtual OStatus DoStart  (const OSystemEvent& event);
    virtual OStatus DoStop   (const OSystemEvent& event);
    virtual OStatus DoDestroy(const OSystemEvent& event);

    InfoDB  m_idb;      // Information Database

    MWIID   Image_LayerH_Y;
    MWIID   Image_LayerH_Cr;
    MWIID   Image_LayerH_Cb;
    MWIID   Sound_Stereo;

    // Image
    void SaveBitmap(char* fn, int y, int x, unsigned char* pImage);

    bool m_imageSave;
    char m_bmp_filename[FILENAME_SIZE];

    // Audio
    void NewSoundBuffer();
    void DeleteSoundBuffer();
    int  CopyToSoundBuffer(OSoundVectorData* soundVecData);
    void SaveSoundBufferAsWAV(char* path);
    //
    // 16KHz 16bits STEREO
    //
    // 16 * 2 * 2 =   64 bytes /  1 ms
    // 64 * 32    = 2048 bytes / 32 ms
    //
    static const size_t NUMOF_REC_FRAME   = 256; // 32ms * 256 = 8192 ms
    static const size_t SOUND_BUFFER_SIZE = 2048 * NUMOF_REC_FRAME;
    static const size_t WAVE_HEADER_SIZE  = 4 + 8 + 16 + 8;
    static const size_t FMTSIZE_WITHOUT_EXTINFO = 16;

    byte*          m_soundBuf;
    byte*          m_soundBufPtr;
    SoundRecState  m_soundRecState;
    char           m_wav_filename[FILENAME_SIZE];

    // Network
    NTP m_ntp;
    EchoServer m_es;

    static const size_t MSGBUF_SIZE = 1024;
    char m_msgBuf[MSGBUF_SIZE];
};

#endif // _RPlugIn_h_DEFINED
