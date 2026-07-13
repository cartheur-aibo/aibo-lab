//
// OccMWCommand.h
//
// Copyright 2000 Sony Corporation
//

#ifndef _OccMWCommand_h_DEFINED_
#define _OccMWCommand_h_DEFINED_

#include <OPENR/OPENR.h>
#include <OPENR/ObjcommTypes.h>
#include <OPENR/MWCommandTypes.h>
#include <OPENR/StandardPosture.h>
#include <OPENR/OccTypes.h>
#include <OPENR/TargetPos.h>

//
// OccMWCommandData
//
struct OccMWCommand {
    MWCSerial       m_serial;
    MWCID           m_command;
    SubjectID       m_subject;
    AppID           m_app;
    TargetPos       m_target_pos;
  
    OccMWCommand():
        m_serial(mwcUNDEFINE_SERIAL), m_command(mwcUNDEFINE_ID), 
        m_subject(OServiceEntry()),  m_app(INVALID_APPID), m_target_pos() {}

    OccMWCommand(MWCSerial serial, MWCID command, SubjectID subject,
                 AppID app, TargetPos target_pos):
        m_serial(serial), m_command(command), 
        m_subject(subject), m_app(app), m_target_pos(target_pos) {}

    // Get
    MWCSerial  GetSerial()    { return m_serial;     }
    MWCID      GetCommand()   { return m_command;    }
    SubjectID& GetSubjectID() { return m_subject;    }
    AppID      GetAppID()     { return m_app;        }
    TargetPos& GetTargetPos() { return m_target_pos; }

    // Set
    OStatus SetSerial(const MWCSerial serial) {
        m_serial  = serial;
        return oSUCCESS;
    }
    OStatus  SetCommand(const MWCID command) {
        m_command = command;
        return oSUCCESS;
    }
    OStatus SetSubjectID(const SubjectID& subject) {
        m_subject = subject;
        return oSUCCESS;
    }
    OStatus SetAppID(const AppID app) {
        m_app     = app;
        return oSUCCESS;
    }
    OStatus SetTargetPos(const TargetPos& target_pos) {
        m_target_pos  = target_pos;
        return oSUCCESS;
    }
    OStatus SetMWCommand(const MWCSerial serial, 
                         const MWCID command, 
                         const SubjectID& subject,
                         const AppID app, 
                         const TargetPos& target_pos) {
        m_serial  = serial;  
        m_command = command;
        m_subject = subject;
        m_app     = app;
        m_target_pos  = target_pos; 
        return oSUCCESS;
    }
};


//
// OccMWCommandResultData
//
struct OccMWCommandResult {
    MWCSerial     m_serial;
    MWCRType      m_rtype;
    MWCStatus     m_status;
    MWCResource   m_resource;
    MWCPosture    m_posture;
    MWCEffect     m_effect;

    OccMWCommandResult():
        m_serial(mwcUNDEFINE_SERIAL), m_rtype(mwcUNDEFINE_TYPE), 
        m_status(mwcUNDEFINE_STATUS), m_resource(mwcNONE), 
        m_posture(mwcUNKNOWN_POSTURE), m_effect(mwcUNKNOWN_EFFECT) {}
  
    OccMWCommandResult(MWCSerial serial, MWCRType type, MWCStatus status,
                       MWCResource resource, MWCPosture posture, 
                       MWCEffect effect):
        m_serial(serial), m_rtype(type), m_status(status), 
        m_resource(resource), m_posture(posture), m_effect(effect) {}

    // Get
    MWCSerial   GetSerial()  { return m_serial;  }
    MWCRType    GetRType()   { return m_rtype;   }
    MWCStatus   GetStatus()  { return m_status;  }
    MWCResource GetResource(){ return m_resource;}
    MWCPosture  GetPosture() { return m_posture; }
    MWCEffect   GetEffect()  { return m_effect;  }

    // Set
    OStatus SetSerial(const MWCSerial serial) {
        m_serial = serial;
        return oSUCCESS;
    }
    OStatus SetRType(const MWCRType rtype) {
        m_rtype = rtype;
        return oSUCCESS;
    }
    OStatus SetStatus(const MWCStatus status) {
        m_status = status;
        return oSUCCESS;
    }
    OStatus SetResource(const MWCResource resource) {
        m_resource = resource;
        return oSUCCESS;
    }
    OStatus SetPosture(const MWCPosture posture) {
        m_posture  = posture;
        return oSUCCESS;
    }
    OStatus SetEffect(const MWCEffect effect) {
        m_effect   = effect;
        return oSUCCESS;
    }
    OStatus SetMWCommandResult(const MWCSerial serial, 
                               const MWCRType rtype, 
                               const MWCStatus status,
                               const MWCResource resource, 
                               const MWCPosture posture, 
                               const MWCEffect effect) {
        m_serial   = serial;  
        m_rtype    = rtype;   
        m_status   = status;  
        m_resource = resource;
        m_posture  = posture; 
        m_effect   = effect; 
        return oSUCCESS;
    }

}; 

#endif  /* _OccMWCommand_h_DEFINED_ */

