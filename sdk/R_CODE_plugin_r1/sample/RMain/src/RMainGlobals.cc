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


#include <stdio.h>
#include <string.h>
#include <iostream.h>
#include <stdlib.h>

#include "RMainDefine.h"
#include "RMainGlobals.h"

char *IgnoreSpace ( char *p )
{
    while( *p && ((*p == ' ')||(*p == '\t')) ) p++;
    return p;
}

/////////////////////////////////////////////////////////////////
//
// File I/O functions
//
/////////////////////////////////////////////////////////////////

#define BLOCK_SIZE 512

static char buf[BLOCK_SIZE];
static int  buf_n;
static int  buf_i;

int Fopen (const char *filename, char *mode )
{
    int ret = -1;
    if(        strcmp( mode, "r" ) == 0 ) {
        ret = open( filename, O_RDONLY                         );
    } else if( strcmp( mode, "w" ) == 0 ) {
        ret = open( filename, O_WRONLY|O_TRUNC|O_CREAT );
    } else if( strcmp( mode, "a" ) == 0 ) {
        ret = open( filename, O_APPEND|O_RDWR |O_CREAT );
    }
    return ret;
}

int Ropen ( const char *filename )
{
    int fp;
    char path[MAX_PATH_LEN];
    if( fp >= 0 ) return fp;
    sprintf( path, "%s%s\0", AMS_FILE_PATH, filename );
    fp = Fopen( path, "r" );
    return fp;
}

int Fclose ( int fd )
{
    return close(fd);
}

char *Fgets ( char *s, int n, int fd )
{
    char *rs = s;
    int   eos = 0; // end_of_string
    char  ch;

    while(1)
    {
        if( buf_i < buf_n )
        {
            while( buf_i < buf_n )
            {
                ch = buf[buf_i];

                if( ch==0x0A || ch==0x0D )  // line feed character?
                {
                    if( ch == eos ) // CR+CR or LF+LF
                    {
                        eos = -1;
                        break;
                    }
                    buf_i++;
                    if( eos ) // CR+LF or LF+CR
                    {
                        eos = -1;
                        break;
                    }
                    eos = ch; // The first character of the linefeed codes
                    *s++ = '\n';
                }
                else
                {
                    if( eos ) // One character after the line feed?
                    {
                        eos = -1;
                        break;
                    }
                    else
                    {
                        *s++ = ch;
                        buf_i++;
                    }
                }
            }
            if( eos < 0 ) break;
        }
        else
        {
            buf_n = read( fd, buf, BLOCK_SIZE );
            if( buf_n == 0 ) // EOF
            {
                if( s == rs ) rs = NULL;
                break;
            }
            buf_i = 0;
        }
    }

    *s = '\0';

    return rs;
}

int Fputs ( const char *s, int fd )
{
    return write( fd,(void *)s, strlen(s) );
}

int Fstat ( const char *filename )
{
    struct stat fs;
    return stat( filename, &fs );
}

int Fdelete ( const char *filename )
{
    return unlink( filename );
}

/////////////////////////////////////////////////////////////////
//
// String to Num functions
//
/////////////////////////////////////////////////////////////////

int AtoI ( char *s )
{
    if( !s ) return 0;
    if( s[0] == '0' )
    {
        switch( s[1] )
        {
            case 'X':
            case 'x': return strtol( s+2, NULL, 16 );
            case 'O':
            case 'o': return strtol( s+2, NULL,  8 );
            case 'B':
            case 'b': return strtol( s+2, NULL,  2 );
            default : return atoi( s );
        }
    }
    return atoi( s );
}
ulong AtoL ( char *s )
{
    if( !s ) return 0;
    if( s[0] == '0' )
    {
        switch( s[1] )
        {
            case 'X':
            case 'x': return strtoul( s+2, NULL, 16 );
            case 'O':
            case 'o': return strtoul( s+2, NULL,  8 );
            case 'B':
            case 'b': return strtoul( s+2, NULL,  2 );
            default : return atol( s );
        }
    }
    return atol( s );
}


