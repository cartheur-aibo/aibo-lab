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


#ifndef OPENR_STUBGEN
#define OPENR_STUBGEN
#endif

#include <MCOOP.h>
#include <InitInfo.h>
#include <iostream.h>
#include <Context.h>
#include <Types.h>
#include <OPENR/stub_macro.h>
#include "def.h"
#include "entry.h"
#include "RMainStub.h"

//
//  Object Core
//
#include "RMain.h"
RMain Self;

//
//  Stub Function Definitions
//
StubFuncDefine_Basic(Init)
StubFuncDefine_Basic(Start)
StubFuncDefine_Basic(Stop)
StubFuncDefine_Basic(Destroy)
StubFuncDefine_Control_nofunc(0)
StubFuncDefine_Ready(0, ReadyMessage)
StubFuncDefine_Control_nofunc(1)
StubFuncDefine_Ready(1, ReadyCCCommand)
StubFuncDefine_Control_nofunc(2)
StubFuncDefine_Ready(2, ReadyTimeRequest)
StubFuncDefine_Control_nofunc(3)
StubFuncDefine_Ready(3, ReadyPeriodicTimeRequest)
StubFuncDefine_Control_nofunc(4)
StubFuncDefine_Ready(4, PlugInCommand)
StubFuncDefine_Control_nofunc(5)
StubFuncDefine_Ready(5, Effector)
StubFuncDefine_Control_nofunc(6)
StubFuncDefine_Ready(6, DefReplay)
StubFuncDefine_Control_nofunc(7)
StubFuncDefine_Ready(7, MtnCommand)
StubFuncDefine_Control_nofunc(8)
StubFuncDefine_Ready(8, MWSensor)
StubFuncDefine_Connect_nofunc(0)
StubFuncDefine_Notify(0, NotifyCommand)
StubFuncDefine_Connect_nofunc(1)
StubFuncDefine_Notify(1, NotifyComplete)
StubFuncDefine_Connect_nofunc(2)
StubFuncDefine_Notify(2, NotifyTimeReply)
StubFuncDefine_Connect_nofunc(3)
StubFuncDefine_Notify(3, NotifyMWCommand)
StubFuncDefine_Connect_nofunc(4)
StubFuncDefine_Notify(4, NotifyPeriodicTimeReply)
StubFuncDefine_Connect_nofunc(5)
StubFuncDefine_Notify(5, NotifyAbleInfo)
StubFuncDefine_Connect_nofunc(6)
StubFuncDefine_Notify(6, NotifyInputSemantics)
StubFuncDefine_Connect_nofunc(7)
StubFuncDefine_Notify(7, PlugInMessage)
StubFuncDefine_Connect_nofunc(8)
StubFuncDefine_Notify(8, Clock2)
StubFuncDefine_Connect_nofunc(9)
StubFuncDefine_Notify(9, MtnMessage)
StubFuncDefine_Connect_nofunc(10)
StubFuncDefine_Notify(10, MWCommand2)
StubFuncDefine_Connect_nofunc(11)
StubFuncDefine_Notify(11, NotifyImage)
StubFuncDefine_Connect_nofunc(12)
StubFuncDefine_Notify(12, NotifyTargetInfo)
StubFuncDefine_Connect_nofunc(13)
StubFuncDefine_Notify(13, NotifySensor)
StubFuncDefine_Connect_nofunc(14)
StubFuncDefine_Notify(14, NotifyEvent)
StubFuncDefine_Connect_nofunc(15)
StubFuncDefine_Notify(15, Clock)

//
//  Stub Function Pointer
//  
_Hook _hook[numOfHook] = { _Hook(Init), _Hook(Start), _Hook(Stop), _Hook(Destroy) };
_Control _control[numOfSubject]  = { _Control(0), _Control(1), _Control(2), _Control(3), _Control(4), _Control(5), _Control(6), _Control(7), _Control(8) };
_Ready   _ready  [numOfSubject]  = { _Ready(0), _Ready(1), _Ready(2), _Ready(3), _Ready(4), _Ready(5), _Ready(6), _Ready(7), _Ready(8) };
_Connect _connect[numOfObserver] = { _Connect(0), _Connect(1), _Connect(2), _Connect(3), _Connect(4), _Connect(5), _Connect(6), _Connect(7), _Connect(8), _Connect(9), _Connect(10), _Connect(11), _Connect(12), _Connect(13), _Connect(14), _Connect(15) };
_Notify  _notify [numOfObserver] = { _Notify(0), _Notify(1), _Notify(2), _Notify(3), _Notify(4), _Notify(5), _Notify(6), _Notify(7), _Notify(8), _Notify(9), _Notify(10), _Notify(11), _Notify(12), _Notify(13), _Notify(14), _Notify(15) };

//
//  Prologue() 
//
#ifndef EXTRA_SETENTRY
#define EXTRA_SETENTRY
#endif

PROLOGUE(EXTRA_SETENTRY)

