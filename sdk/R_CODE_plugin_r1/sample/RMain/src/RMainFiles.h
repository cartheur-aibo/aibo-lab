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


#ifndef _RMainFiles_h_DEFINED
#define _RMainFiles_h_DEFINED

#include <OPENR/OPENR.h>
#include <OPENR/OStatus.h>
#include "RMainDefine.h"

class RMainFiles {
private:
    void* m_buf;
    int   m_index;
    int   m_size;
    char  m_file[MAX_PATH_LEN];

public:
    RMainFiles();
    ~RMainFiles();

    int Open( const char* filename );
    OStatus Close( void );
    int GetLine( char* data );
    OStatus GZIPFileLoad(const char* path, byte** data, size_t* size);
    OStatus CheckPath( const char* filename, char* path );
    void* GetBuff( void ) { return m_buf; };
    int   GetSize( void ) { return m_size; };
};

#endif // _RMainFiles_h_DEFINE
