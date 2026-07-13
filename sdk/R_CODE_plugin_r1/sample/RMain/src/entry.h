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


#ifndef _entry_h_DEFINED
#define _entry_h_DEFINED

#include <Types.h>
#include "def.h"

const int numOfHook = 4;

const Selector Entry_Hook   [numOfHook]     = { 0, 1, 2, 3 };
const Selector Entry_Control[numOfSubject]  = { 4, 5, 6, 7, 8, 9, 10, 11, 12 };
const Selector Entry_Ready[numOfSubject]    = { 13, 14, 15, 16, 17, 18, 19, 20, 21 };
const Selector Entry_Connect[numOfObserver] = { 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37 };
const Selector Entry_Notify[numOfObserver]  = { 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53 };

//
//  Number of entries is 
//      numOfHook + (numOfSubject + numOfObserver) * 2
//
const longword numOfEntries = 54;

#endif // _entry_h_DEFINED
