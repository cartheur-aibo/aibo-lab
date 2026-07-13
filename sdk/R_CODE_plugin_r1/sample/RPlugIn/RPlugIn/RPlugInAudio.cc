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

#include <OPENR/OccDataFormats.h>
#include "RPlugIn.h"

void 
RPlugIn::UpdateAudio(const ONotifyEvent& event)
{
    if(m_soundRecState == SRS_DOSTOP) return ;

    // 
    // RPlugIn::Notify() is invoked : every 16ms (ERS-7)
    //                               : every 32ms (ERS-210/220)
    //

    if (m_soundRecState == SRS_START) {

        OSoundVectorData* soundVecData;

        for(int i = 0; i < event.NumOfData(); i += 2) {
            OccAudioData data( event.Data(0), event.Data(1) );
            soundVecData = (OSoundVectorData*)data.GetValue();
        }

        int size = CopyToSoundBuffer(soundVecData);

        if (size >= SOUND_BUFFER_SIZE) {
            OSYSPRINT(("SAVE %s. WAIT ... ", m_wav_filename));
            SaveSoundBufferAsWAV(m_wav_filename);
            OSYSPRINT(("DONE\n"));

            memset(m_soundBuf, 0, SOUND_BUFFER_SIZE);
            m_soundBufPtr = m_soundBuf;

            m_soundRecState = SRS_IDLE;
        }

    }

    observer[event.ObsIndex()]->AssertReady(event.SenderID());

    return ;
}

void
RPlugIn::NewSoundBuffer()
{
    m_soundBuf = (byte*)malloc(SOUND_BUFFER_SIZE);
    if (m_soundBuf == 0) {
        OSYSLOG1((osyslogERROR,
                  "RPlugIn::NewSoundBuffer() : malloc() failed"));
    }
    memset(m_soundBuf, 0, SOUND_BUFFER_SIZE);

    m_soundBufPtr = m_soundBuf;    
}

void
RPlugIn::DeleteSoundBuffer()
{
    if (m_soundBuf != 0) {
        free(m_soundBuf);
        m_soundBuf = m_soundBufPtr = 0;
    }
}

int
RPlugIn::CopyToSoundBuffer(OSoundVectorData* soundVecData)
{
    OSoundInfo* info = soundVecData->GetInfo(0);
    byte* data = soundVecData->GetData(0);
    memcpy(m_soundBufPtr, data, info->dataSize);
    m_soundBufPtr += info->dataSize;

    return (int)(m_soundBufPtr - m_soundBuf);
}

void
RPlugIn::SaveSoundBufferAsWAV(char* path)
{
    longword lwd;
    word     wd;

    FILE* fp = fopen(path, "w");
    if (fp == 0) {
        OSYSLOG1((osyslogERROR,
                  "RPlugIn::SaveSoundBufferAsWAV() : can't open %s", path));
        return;
    }

    fwrite("RIFF", 1, 4, fp);

    // RIFF data size
    lwd = WAVE_HEADER_SIZE + SOUND_BUFFER_SIZE;
    fwrite(&lwd, sizeof(lwd), 1, fp);

    // "WAVE"
    fwrite("WAVE", 1, 4, fp);

    // "fmt "
    fwrite("fmt ", 1, 4, fp);

    // Length of the fmt chunk
    lwd = FMTSIZE_WITHOUT_EXTINFO;
    fwrite(&lwd, sizeof(lwd), 1, fp);

    // WAVE File Encoding Tag (PCM)
    wd = 0x01;
    fwrite(&wd, sizeof(wd), 1, fp);

    // Channels (STEREO)
    wd = 0x02;
    fwrite(&wd, sizeof(wd), 1, fp);

    // Sampling Rate (16kHz)
    lwd = 16000;
    fwrite(&lwd, sizeof(lwd), 1, fp);

    // bytes/sec (16KHz * 16bits * STEREO)
    lwd = 16000 * 2 * 2;
    fwrite(&lwd, sizeof(lwd), 1, fp);

    // Block Align (channels * bits/sample / 8)
    wd = 0x04;
    fwrite(&wd, sizeof(wd), 1, fp);

    // bits/sample
    wd = 16;
    fwrite(&wd, sizeof(wd), 1, fp);

    // "data"
    fwrite("data", 1, 4, fp);

    // data size
    lwd = SOUND_BUFFER_SIZE;
    fwrite(&lwd, sizeof(lwd), 1, fp);

    // data
    size_t nwrite = fwrite(m_soundBuf, 1, lwd, fp);
    if (nwrite != lwd) {
        OSYSLOG1((osyslogERROR, "%s : %s %d",
                  "RPlugIn::SaveSoundBufferAsWAV()",
                  "fwrite() FAILED", nwrite));
    }

    fclose(fp);
}
