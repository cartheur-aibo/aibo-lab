//
// Copyright 1997-2001,2006 Sony Corporation
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

typedef struct
{
    int nr;
    int ng;
    int nb;
}RGB;

//YCrCb -> RGB
RGB f(int y, int u, int v)
{
    RGB rgb;
    rgb.nr=(int)((double)y+1.4020*((double)v-128));
    rgb.ng=(int)((double)y-0.3441*((double)u-128)-0.7139*((double)v-128));
    rgb.nb=(int)((double)y+1.7718*((double)u-128)-0.0012*((double)v-128));
    return rgb;
}

//YCrCb -> RGB
RGB f5(int y, int cr, int cb)
{
    RGB rgb;
    double Y,Cr,Cb;

    Cr = (double)cr - 128;
    Cb = (double)cb - 128;
    Y  = 1.164 * (double)y  - 16 ;

    rgb.nr=(int)(Y + 1.596 * Cr);
    rgb.ng=(int)(Y - 0.813 * Cr + 0.391 * Cb);
    rgb.nb=(int)(Y              + 2.018 * Cb);

    if ( rgb.nr > 255 ) rgb.nr = 255;
    if ( rgb.ng > 255 ) rgb.ng = 255;
    if ( rgb.nb > 255 ) rgb.nb = 255;

    return rgb;
}


void
RPlugIn::UpdateImage(const ONotifyEvent& event)
{
    if ( m_imageSave == false )
    {
        observer[event.ObsIndex()]->AssertReady(event.SenderID());
        return;
    }
    m_imageSave = false;

    for(int e = 0; e < event.NumOfData(); e+=2) {
      
        OccImageData data( event.Data(0), event.Data(1) );
        OccSensorInformation* osi =
            (OccSensorInformation*)data.GetSensorInformation();
      
        int nHorizon  = osi->GetHorizontal( Image_LayerH_Y );
        int nVertical = osi->GetVertical( Image_LayerH_Y );
        int nSkip     = osi->GetSkip( Image_LayerH_Y);
      
        byte* pDataYH   = (byte*)data.GetValue( Image_LayerH_Y );
        byte* pDataCrH  = (byte*)data.GetValue( Image_LayerH_Cr );
        byte* pDataCbH  = (byte*)data.GetValue( Image_LayerH_Cb );
      
        // memory allocation
        byte* buf;
        NewRegion( nHorizon * nVertical * 3, (void**)&buf);
      
        int nIndex=0, i=0;
        for(int nV = 0; nV < nVertical; nV++){
            for(int nH = 0 ; nH < nHorizon; nH++){
      
                // Change YCrCb to RGB
                RGB rgb = f5(*(pDataYH+nIndex), 
                             *(pDataCrH+nIndex), 
                             *(pDataCbH+nIndex));
                nIndex++;
      
                buf[i++] = rgb.nr;  // Set R
                buf[i++] = rgb.ng;  // Set G
                buf[i++] = rgb.nb;  // Set B
            }
            nIndex += nSkip;
        }
        observer[event.ObsIndex()]->AssertReady(event.SenderID());
        // Save ImageData
        DPRINTF(("SAVE IMAGE ..."));
        SaveBitmap(m_bmp_filename, nVertical , nHorizon, buf);
        DPRINTF(("IMAGE FILE :%s SAVE DONE\n", m_bmp_filename));
    }
    observer[event.ObsIndex()]->AssertReady(event.SenderID());
}

//
// Change big endian to Little Endian
//
inline void b2l(char *p, int value)
{
    *p++ =  value        & 0xff;
    *p++ = (value >>  8) & 0xff;
    *p++ = (value >> 16) & 0xff;
    *p++ = (value >> 24) & 0xff;
}

//
// Save Bitmap 
//
void
RPlugIn::SaveBitmap(char* fn, int y, int x, unsigned char* pImage)
{
    // Set Bitmap Header 
    char bf[54];
    memset(bf, 0, sizeof(bf));
    strcpy( bf, "BM");  // File type
    b2l(&bf[2] , sizeof(bf) + x * y * 3 );  // File size
    b2l(&bf[10], 0x36); // Seek pos
    b2l(&bf[14], 0x28); // Header size
    b2l(&bf[18], x);    // Width
    b2l(&bf[22], y);    // Height
    b2l(&bf[26], 0x01); // Planes
    b2l(&bf[28], 0x18); // Bit Counts

    // memory allocation
    byte* buf;
    NewRegion( x * y * 3 + sizeof(bf), (void**)&buf);
    
    byte* pbuf = buf;

    memcpy(buf,bf, sizeof(bf));
    buf+=sizeof(bf);
    
    for ( int yy = y-1; yy >= 0; yy -- ){
        for ( int xx = 0; xx < x ; xx ++){
            int xy = yy * x * 3 + xx * 3;
            *buf = *(pImage + xy +2);   buf++; //Set B
            *buf = *(pImage + xy +1);   buf++; //Set G
            *buf = *(pImage + xy +0);   buf++; //Set R
        }
    }

    FILE* fp = fopen(fn, "w");
    if (fp == 0) {
        OSYSLOG1((osyslogERROR,
                  "RPlugIn::SaveBitmap() : can't open %s", fn));
        DeleteRegion( pbuf );
        return;
    }
    size_t filesize = x*y*3+sizeof(bf);
    size_t nwrite = fwrite(pbuf, 1, filesize, fp);
    if (nwrite != filesize) {
        OSYSLOG1((osyslogERROR, "%s : %s %d",
                  "RPlugIn::SaveBitmap()",
                  "fwrite() FAILED", nwrite));
    }
    fclose(fp);

    DeleteRegion( pbuf );
}

