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


#ifndef _RMainSemantics_h_DEFINED
#define _RMainSemantics_h_DEFINED

#include <APP/OcaInputSemantics.h>
#include <OPENR/OccAbleInfo.h>

const ISSubInfo arsubMASK_AUDIO_ELEMENT = 0x0000ffff;

//
//  Inner Class
//
class RMainSemanticsConfig {
  private:
    static const word Learning      =  0x0001; // Learning           update ON/OFF
    static const word Growing       =  0x0002; // Growing            update ON/OFF
    static const word GlobalVar     =  0x0004; // Global variable     print ON/OFF
    static const word Destination   =  0x0008; // Destination         print ON/OFF
    static const word Command       =  0x0010; // Command             print ON/OFF
    static const word CommandDetail =  0x0020; // Command detail      print ON/OFF
    static const word Semantics     =  0x0040; // Semantics           print ON/OFF
    static const word AbleInfo      =  0x0080; // AbleInfo            print ON/OFF
    static const word ExtCommand    =  0x0100; // ExtCommand          print ON/OFF
    static const word Condition     =  0x0200; // Trans. Condition    print ON/OFF
    static const word Light         =  0x0400; // Print Light         print ON/OFF
    static const word Mode          =  0x0800; // Print Mode          print ON/OFF
    static const word Syslog        =  0x1000; // Syslog Print        print ON/OFF
    static const word Time          =  0x2000; // Time information    print ON/OFF
    static const word RawData       =  0x4000; // Raw data            print ON/OFF

  private:
    word       m_functionality;
    word       m_debug_print;

    slongword  m_num_of_benet;

  public:
    slongword* m_dst_print_benet;
    slongword* m_cmd_print_benet;
    slongword* m_cnd_print_benet;

    RMainSemanticsConfig(void)
        : m_functionality(RMainSemanticsConfig::Learning |
                        RMainSemanticsConfig::Growing),
          m_debug_print(0x0000),
          m_num_of_benet(0),
          m_dst_print_benet(static_cast<slongword*>(NULL)),
          m_cmd_print_benet(static_cast<slongword*>(NULL)),
          m_cnd_print_benet(static_cast<slongword*>(NULL)) {}
    ~RMainSemanticsConfig() {}

    bool LearningON(void) const {
        return ((m_functionality & RMainSemanticsConfig::Learning) != 0x0000);
    }
    bool GrowingON(void) const {
        return ((m_functionality & RMainSemanticsConfig::Growing) != 0x0000);
    }

    bool PrintGlobalVar(void) const {
        return ((m_debug_print & RMainSemanticsConfig::GlobalVar) != 0x0000);
    }
    bool PrintDestination(void) const {
        return ((m_debug_print & RMainSemanticsConfig::Destination) != 0x0000);
    }

    bool PrintCommand(void) const {
        return ((m_debug_print & RMainSemanticsConfig::Command) != 0x0000);
    }

    bool PrintCommandDetail(void) const {
        return ((m_debug_print & RMainSemanticsConfig::CommandDetail) != 0x0000);
    }
    bool PrintSemantics(void) const {
        return ((m_debug_print & RMainSemanticsConfig::Semantics) != 0x0000);
    }

    bool PrintAbleInfo(void) const {
        return ((m_debug_print & RMainSemanticsConfig::AbleInfo) != 0x0000);
    }

    bool PrintExtCommand(void) const {
        return ((m_debug_print & RMainSemanticsConfig::ExtCommand) != 0x0000);
    }

    bool PrintCondition(void) const {
        return ((m_debug_print & RMainSemanticsConfig::Condition) != 0x0000);
    }

    bool PrintLight(void) const {
        return ((m_debug_print & RMainSemanticsConfig::Light) != 0x0000);
    }

    bool PrintMode(void) const {
        return ((m_debug_print & RMainSemanticsConfig::Mode) != 0x0000);
    }

    bool PrintSyslog(void) const {
        return ((m_debug_print & RMainSemanticsConfig::Syslog) != 0x0000);
    }

    bool PrintTime(void) const {
        return ((m_debug_print & RMainSemanticsConfig::Time) != 0x0000);
    }

    bool PrintRawData(void) const {
        return ((m_debug_print & RMainSemanticsConfig::RawData) != 0x0000);
    }

    void DisableLearning(void) {
        m_functionality &= ~RMainSemanticsConfig::Learning;
        return;
    }

    void DisableGrowing(void) {
        m_functionality &= ~RMainSemanticsConfig::Growing;
        return;
    }

    void SetPrintGlobalVar(void) {
        m_debug_print |= RMainSemanticsConfig::GlobalVar;
        return;
    }

    void SetPrintDestination(void) {
        m_debug_print |= RMainSemanticsConfig::Destination;
        return;
    }

    void SetPrintCommand(void) {
        m_debug_print |= RMainSemanticsConfig::Command;
        return;
    }

    void SetPrintCommandDetail(void) {
        m_debug_print |= RMainSemanticsConfig::CommandDetail;
        return;
    }

    void SetPrintSemantics(void) {
        m_debug_print |= RMainSemanticsConfig::Semantics;
        return;
    }

    void SetPrintAbleInfo(void) {
        m_debug_print |= RMainSemanticsConfig::AbleInfo;
        return;
    }

    void SetPrintExtCommand(void) {
        m_debug_print |= RMainSemanticsConfig::ExtCommand;
        return;
    }

    void SetPrintCondition(void) {
        m_debug_print |= RMainSemanticsConfig::Condition;
        return;
    }

    void SetPrintLight(void) {
        m_debug_print |= RMainSemanticsConfig::Light;
        return;
    }

    void SetPrintMode(void) {
        m_debug_print |= RMainSemanticsConfig::Mode;
        return;
    }

    void SetPrintSyslog(void) {
        m_debug_print |= RMainSemanticsConfig::Syslog;
        return;
    }

    void SetPrintTime(void) {
        m_debug_print |= RMainSemanticsConfig::Time;
        return;
    }

    void SetPrintRawData(void) {
        m_debug_print |= RMainSemanticsConfig::RawData;
        return;
    }

    void SetNumOfBeNet(slongword num_of_benet) {
        m_num_of_benet = num_of_benet;
        return;
    }
};

struct DebugSem {
    ISSemanticsID   m_semantics_id;
    char            m_semantics_str[32];
};

struct DebugComStat {
    ISComStatus     m_com_stat;
    word            dummy;
    char            m_com_stat_str[32];
};

struct DebugLevel {
    ISLevel         m_level;
    word            dummy;
    char            m_level_str[32];
};

struct DebugPitch {
    ISPitch         m_pitch;
    word            dummy;
    char            m_pitch_str[32];
};

struct DebugPos {
    ISPosition      m_pos;
    char            m_pos_str[32];
};


const longword SEM_SUBINFO_1 = 0x01;
const longword SEM_SUBINFO_2 = 0x02;
const longword SEM_SUBINFO_3 = 0x04;
const longword SEM_SUBINFO_4 = 0x08;
struct DebugSubInfo {
    ISSubInfo       m_subinfo;
    char            m_subinfo_str[32];

    DebugSubInfo() {
        m_subinfo = issemINVALID;
        m_subinfo_str[0] = '\0';
    }
};

struct DebugMask {
    list<ISSemanticsID> m_semantics_id_list;
    list<DebugSubInfo>  m_subinfo_list;
    ISSubInfo       m_mask_id;
    longword        m_subinfo_index;        // 0x01, 0x02, 0x04, 0x08
    char            m_mask_str[32];

    DebugMask() {
        while ( m_semantics_id_list.size() ) {
            m_semantics_id_list.pop_back();
        }
        while ( m_subinfo_list.size() ) {
            m_subinfo_list.pop_back();
        }
        m_mask_id = issemINVALID;
        m_subinfo_index = 0;
        m_mask_str[0] = '\0';
    }
};

struct DebugAble {
    slongword       m_status;
    char            m_status_str[32];
};

class RMainSemanticsLabels {
public:
    longword        m_num_of_debug_sem;
    DebugSem*       m_debug_sem;
    DebugComStat    m_debug_com_stat[3];
    DebugLevel      m_debug_level[6];
    DebugPitch      m_debug_pitch[6];
    DebugPos        m_debug_pos_hang[9];
    DebugPos        m_debug_pos_vang[6];
    DebugPos        m_debug_pos_dist[4];
    longword        m_num_of_debug_mask;
    DebugMask*      m_debug_mask;
    DebugAble       m_debug_able[4];

    RMainSemanticsLabels();

    char* GetSemanticsStr(char* buf, ISSemanticsID semantics_id);
    char* GetComStatStr(char* buf, ISComStatus com_stat);
    char* GetLevelStr(char* buf, ISLevel level);
    char* GetPitchStr(char* buf, ISPitch pitch);
    char* GetPosStr(char* buf, ISPosition pos);
    char* GetSubInfoStr(char* buf, ISSemanticsID semantics_id,
            longword subinfo_index, ISSubInfo subinfo);
    char* GetAbleStatusStr(char* buf, slongword able_status);
    ISSemanticsID GetSemanticsID(char* semantics_name);
    ISSubInfo GetSubInfoID(char* subinfo);
};

#endif // _RMainSemantics_h_DEFINE
