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


#include <MCOOP.h>
#include <OPENR/OPENRAPI.h>
#include <OPENR/OSyslog.h>
#include <OPENR/ODebug.h>
#include <OPENR/core_macro.h>
#include <OUtil/GZIPFile.h>
#include <Middlewares/zlib.h>
#include "RMainDefine.h"
#include "RMainFiles.h"

RMainFiles::RMainFiles()
    : m_index( 0 ),
      m_size( 0 ),
      m_file( "" )
{
}

RMainFiles::~RMainFiles()
{
}

OStatus
RMainFiles::CheckPath( const char* filename, char* path )
{
    struct stat fs;

    char buff[MAX_PATH_LEN];
    sprintf( buff, "%s%s\0", AMS_FILE_PATH, filename );
    if (stat( buff, &fs ) != sSUCCESS ) {
        return oFAIL;
    }

    strcpy(path, buff);
    return oSUCCESS;
}

int
RMainFiles::Open(const char* filename)
{
    strcpy( m_file, filename );
    char path[MAX_PATH_LEN];
    if ( CheckPath( filename, path ) != oSUCCESS ) {
        return -1;
    }

    byte*  data;
    size_t size;
    OStatus st = GZIPFileLoad( path, &data, &size );
    if ( st != oSUCCESS ) return -1;

    m_buf = data;
    m_size = size;
    return size;
}

OStatus
RMainFiles::Close ( void )
{
    if ( m_buf == NULL ) return oFAIL;

    if ( DeleteRegion(m_buf) != sSUCCESS ) {
        OSYSLOG1((osyslogERROR, "err! (delete buffer error close),"));
        return oFAIL;
    }
    return oSUCCESS;
}

int
RMainFiles::GetLine( char* data )
{
    char* rs = data;
    char ch;
    int ret = 1;
    char* p = static_cast<char*>(m_buf);
    int topVal;

    topVal = 0;
    while(1) {
        ch = *(p + m_index);
        if ( ( ch > 128 ) && ( topVal == 0 ) ) {
            OSYSLOG1((osyslogERROR, "err! Invalid character file:%s %d",m_file, m_index ));
            m_index++;
            break;
        }

        topVal=1;

        if ( ch == 0x0A || ch == 0x0D ) {
            *rs++ = '\n';
            m_index++;
            ch = *(p + m_index);
            if ( ch == 0x0A || ch == 0x0D ) m_index++;
            break;
        }else if ( m_size < m_index ) {
            ret = 0;
            break;
        }
        *rs++ = ch;
        m_index++;
    }
    *rs = '\0';
    return ret;
}

OStatus
RMainFiles::GZIPFileLoad(const char* path, byte** data, size_t* size)
{
    sError error;
    struct stat statbuf;
    int fd = -1;
    byte* readBuf = 0;
    int readBufSize;

    /* When there is an error *data = 0 and *size = 0 */
    *data = 0;
    *size = 0;

    int ret = stat(path, &statbuf);
    if (ret != 0) return oNOT_FOUND;

    /*
     * ReadBuf is secured for the limitations of gunzip() by a large size.
     */
    readBufSize = statbuf.size + 64;
    DPRINTF(("%s size %d readBufSize %d\n", path, statbuf.size, readBufSize));
    error = NewRegion(readBufSize, (void**)&readBuf);
    if (error != sSUCCESS) return oNO_MEMORY;
    memset(readBuf, 0, readBufSize);

    fd = open(path, O_RDONLY);
    if (fd < 0) {
    DeleteRegion(readBuf);
    return oFAIL;
    }

    ret = read(fd, readBuf, statbuf.size);
    close(fd);
    if (ret != statbuf.size) {
    DeleteRegion(readBuf);
    return oFAIL;
    }

    /*
     * gzip ID check
     */
    if ((*readBuf == 0x1f) && (*(readBuf + 1) == 0x8b)) {

    int unzipSize = unzippedsize(readBuf, statbuf.size);
    if (unzipSize == -1) {
        DeleteRegion(readBuf);
        DPRINTF(("GZIP:oINVALID_DATA\n"));
        return oINVALID_DATA;
    }

    byte* unzipBuf;
    error = NewRegion(unzipSize, (void**)&unzipBuf);
    if (error != sSUCCESS) {
        DeleteRegion(readBuf);
        DPRINTF(("GZIP:oNO_MEMORY\n"));
        return oNO_MEMORY;
    }
    memset(unzipBuf, 0, unzipSize);

    uLong unzipLen = (uLong)unzipSize;
    ret = gunzip(unzipBuf, &unzipLen, readBuf, readBufSize);
    if (ret != Z_OK) {
        DeleteRegion(readBuf);
        DeleteRegion(unzipBuf);
        DPRINTF(("GZIP:oINVALID/oNO_MEMORY"));
        return (ret == Z_DATA_ERROR) ? oINVALID_DATA : oNO_MEMORY;
    }

    DeleteRegion(readBuf);
    *data = unzipBuf;
    *size = unzipLen;

    } else {

    *data = readBuf;
    *size = statbuf.size;
    }

    return oSUCCESS;
}





