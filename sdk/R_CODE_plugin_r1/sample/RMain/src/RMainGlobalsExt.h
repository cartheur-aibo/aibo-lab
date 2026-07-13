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


#ifndef _RMainGlobalsExt_h_DEFINED
#define _RMainGlobalsExt_h_DEFINED

extern RMainSemanticsLabels     g_labels;
extern char                     g_debug_buf[1024];

extern char *IgnoreSpace ( char *p );

extern int Fopen (const char *filename, char *mode );
extern int Ropen ( const char *filename );
extern int Fclose ( int fd );

extern char *Fgets ( char *s, int n, int fd );
extern int Fputs ( const char *s, int fd );
extern int Fstat ( const char *filename );
extern int Fdelete ( const char *filename );

extern int AtoI ( char *s );
extern ulong AtoL ( char *s );

#endif // _RMainGlobalsExt_h_DEFINED


