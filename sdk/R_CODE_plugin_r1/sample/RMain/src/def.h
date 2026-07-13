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


#ifndef _def_h_DEFINED
#define _def_h_DEFINED

//
//  Object name
//
const char* const objectName = "RMain";

//
//  Number of OObserver & OSubeject instances
//
const int numOfSubject  = 9;
const int numOfObserver = 16;

//
//  Service name
//
const char* const subjectService[numOfSubject] = 
{
    "RMain.Message.STRING.S",
    "RMain.CCCommand.OcaCCCommandData.S",
    "RMain.TimeRequest.OcaTimeRequest.S",
    "RMain.PeriodicTimeRequest.OcaTimeRequest.S",
    "RMain.PlugInCommand.PlugInData.S",
    "RMain.Effector.OCommandVectorData.S",
    "RMain.DefReplay.ReplayData.S",
    "RMain.MtnCommand.STRING.S",
    "RMain.MWSensor.RStsData.S"
};

const char* const observerService[numOfObserver] = 
{
    "RMain.Command.STRING.O",
    "RMain.Complete.OcaCCCommandResult.O",
    "RMain.TimeReply.OcaTimeReply.O",
    "RMain.MWCommand.OcaMWCommand.O",
    "RMain.PeriodicTimeReply.OcaTimeReply.O",
    "RMain.AbleInfo.OccAbleInfo.O",
    "RMain.Semantics.OcaInputSemantics.O",
    "RMain.PlugInMessage.PlugInData.O",
    "RMain.Clock2.OSensorFrameVectorData.O",
    "RMain.MtnMessage.STRING.O",
    "RMain.MWCommand2.OccMWCommand.O",
    "RMain.Image.OccImageData.O",
    "RMain.Target.OccTargetInfo.O",
    "RMain.Sensor.OccSensorData.O",
    "RMain.Event.OccEventData.O",
    "RMain.Clock.CHAR.O"
};

//
//  OSubject Index
//
const int sbjMessage = 0;
const int sbjCCCommand = 1;
const int sbjTimeRequest = 2;
const int sbjPeriodicTimeRequest = 3;
const int sbjPlugInCommand = 4;
const int sbjEffector = 5;
const int sbjDefReplay = 6;
const int sbjMtnCommand = 7;
const int sbjMWSensor = 8;

//
//  OObserver Index
//
const int obsCommand = 0;
const int obsComplete = 1;
const int obsTimeReply = 2;
const int obsMWCommand = 3;
const int obsPeriodicTimeReply = 4;
const int obsAbleInfo = 5;
const int obsSemantics = 6;
const int obsPlugInMessage = 7;
const int obsClock2 = 8;
const int obsMtnMessage = 9;
const int obsMWCommand2 = 10;
const int obsImage = 11;
const int obsTarget = 12;
const int obsSensor = 13;
const int obsEvent = 14;
const int obsClock = 15;


#endif // _def_h_DEFINED
