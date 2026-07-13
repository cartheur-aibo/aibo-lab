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


#include <OPENR/ODebug.h>
#include <stdio.h>
#include "RMainSemantics.h"

//
//  Inner Class
//

RMainSemanticsLabels::RMainSemanticsLabels()
{
    // ComStat x 3
    m_debug_com_stat[0].m_com_stat = iscsON;    
    strcpy(m_debug_com_stat[0].m_com_stat_str, "ON");
    m_debug_com_stat[1].m_com_stat = iscsOFF;   
    strcpy(m_debug_com_stat[1].m_com_stat_str, "OFF");
    m_debug_com_stat[2].m_com_stat = iscsINVALID;   
    strcpy(m_debug_com_stat[2].m_com_stat_str, "-");

    // Level x 6
    m_debug_level[0].m_level = islvlVERYLOW;    
    strcpy(m_debug_level[0].m_level_str, "VLOW");
    m_debug_level[1].m_level = islvlLOW;    
    strcpy(m_debug_level[1].m_level_str, "LOW");
    m_debug_level[2].m_level = islvlMIDDLE; 
    strcpy(m_debug_level[2].m_level_str, "MID");
    m_debug_level[3].m_level = islvlHIGH;   
    strcpy(m_debug_level[3].m_level_str, "HIGH");
    m_debug_level[4].m_level = islvlVERYHIGH;   
    strcpy(m_debug_level[4].m_level_str, "VHIGH");
    m_debug_level[5].m_level = islvlINVALID;    
    strcpy(m_debug_level[5].m_level_str, "-");

    // Pitch x 6
    m_debug_pitch[0].m_pitch = ispchVERYLOW;    
    strcpy(m_debug_pitch[0].m_pitch_str, "VLOW");
    m_debug_pitch[1].m_pitch = ispchLOW;    
    strcpy(m_debug_pitch[1].m_pitch_str, "LOW");
    m_debug_pitch[2].m_pitch = ispchMIDDLE; 
    strcpy(m_debug_pitch[2].m_pitch_str, "MID");
    m_debug_pitch[3].m_pitch = ispchHIGH;   
    strcpy(m_debug_pitch[3].m_pitch_str, "HIGH");
    m_debug_pitch[4].m_pitch = ispchVERYHIGH;   
    strcpy(m_debug_pitch[4].m_pitch_str, "VHIGH");
    m_debug_pitch[5].m_pitch = ispchINVALID;    
    strcpy(m_debug_pitch[5].m_pitch_str, "-");

    // Position: Horizontal Angle x 9
    m_debug_pos_hang[0].m_pos = isposFORWARD;   
    strcpy(m_debug_pos_hang[0].m_pos_str, "FWD");
    m_debug_pos_hang[1].m_pos = isposRFORWARD;  
    strcpy(m_debug_pos_hang[1].m_pos_str, "RFWD");
    m_debug_pos_hang[2].m_pos = isposRIGHT; 
    strcpy(m_debug_pos_hang[2].m_pos_str, "RGHT");
    m_debug_pos_hang[3].m_pos = isposRBACKWARD; 
    strcpy(m_debug_pos_hang[3].m_pos_str, "RBWD");
    m_debug_pos_hang[4].m_pos = isposBACKWARD;  
    strcpy(m_debug_pos_hang[4].m_pos_str, "BWD");
    m_debug_pos_hang[5].m_pos = isposLBACKWARD; 
    strcpy(m_debug_pos_hang[5].m_pos_str, "LBWD");
    m_debug_pos_hang[6].m_pos = isposLEFT;  
    strcpy(m_debug_pos_hang[6].m_pos_str, "LEFT");
    m_debug_pos_hang[7].m_pos = isposLFORWARD;  
    strcpy(m_debug_pos_hang[7].m_pos_str, "LFWD");
    m_debug_pos_hang[8].m_pos = isposINVALID_H_ANGLE;   
    strcpy(m_debug_pos_hang[8].m_pos_str, "-");

    // Position: Vertical Angle x 6
    m_debug_pos_vang[0].m_pos = isposOVER;  
    strcpy(m_debug_pos_vang[0].m_pos_str, "OVR");
    m_debug_pos_vang[1].m_pos = isposABOVE; 
    strcpy(m_debug_pos_vang[1].m_pos_str, "ABV");
    m_debug_pos_vang[2].m_pos = isposLEVEL; 
    strcpy(m_debug_pos_vang[2].m_pos_str, "LVL");
    m_debug_pos_vang[3].m_pos = isposBELOW; 
    strcpy(m_debug_pos_vang[3].m_pos_str, "BLW");
    m_debug_pos_vang[4].m_pos = isposUNDER; 
    strcpy(m_debug_pos_vang[4].m_pos_str, "UDR");
    m_debug_pos_vang[5].m_pos = isposINVALID_V_ANGLE;   
    strcpy(m_debug_pos_vang[5].m_pos_str, "-");

    // Position: Distance x 4
    m_debug_pos_dist[0].m_pos = isposFAR;   
    strcpy(m_debug_pos_dist[0].m_pos_str, "FAR");
    m_debug_pos_dist[1].m_pos = isposMID;   
    strcpy(m_debug_pos_dist[1].m_pos_str, "MID");
    m_debug_pos_dist[2].m_pos = isposNEAR;  
    strcpy(m_debug_pos_dist[2].m_pos_str, "NEAR");
    m_debug_pos_dist[3].m_pos = isposINVALID_DIST;  
    strcpy(m_debug_pos_dist[3].m_pos_str, "-");

    // Able Information x 4
    m_debug_able[0].m_status = asFar;   
    strcpy(m_debug_able[0].m_status_str, "FAR");
    m_debug_able[1].m_status = asNear;  
    strcpy(m_debug_able[1].m_status_str, "NEAR");
    m_debug_able[2].m_status = asTooClose;  
    strcpy(m_debug_able[2].m_status_str, "TOOCLOSE");
    m_debug_able[3].m_status = asJust;  
    strcpy(m_debug_able[3].m_status_str, "JUST");
}


char* RMainSemanticsLabels::GetSemanticsStr(char* buf, ISSemanticsID semantics_id)
{
    buf[0] = '\0';
    if ( semantics_id == issemINVALID ) {
        return static_cast<char*>(NULL);
    }
    for ( int i = 0; i < m_num_of_debug_sem; ++i ) {
        if ( m_debug_sem[i].m_semantics_id
                    == (semantics_id & issemMASK_SEM_ID) ) {
            strcpy(buf, m_debug_sem[i].m_semantics_str);
            if ( buf[0] != '\0' ) {
                return buf;
            } else {
                return static_cast<char*>(NULL);
            }
        }
    }
    return static_cast<char*>(NULL);
}


char* RMainSemanticsLabels::GetComStatStr(char* buf, ISComStatus com_stat)
{
    buf[0] = '\0';
    if ( com_stat == iscsINVALID || com_stat == iscsDONTCARE ) {
        return static_cast<char*>(NULL);
    }
    for ( int i = 0; m_debug_com_stat[i].m_com_stat != iscsINVALID; ++i ) {
        if ( m_debug_com_stat[i].m_com_stat == com_stat ) {
            strcpy(buf, m_debug_com_stat[i].m_com_stat_str);
            return buf;
        }
    }
    return static_cast<char*>(NULL);
}


char* RMainSemanticsLabels::GetLevelStr(char* buf, ISLevel level)
{
    buf[0] = '\0';
    if ( level == islvlINVALID ) {
        return static_cast<char*>(NULL);
    }
    for ( int i = 0; m_debug_level[i].m_level != islvlINVALID; ++i ) {
        if ( m_debug_level[i].m_level == level ) {
            strcpy(buf, m_debug_level[i].m_level_str);
            return buf;
        }
    }
    return static_cast<char*>(NULL);
}

char* RMainSemanticsLabels::GetPitchStr(char* buf, ISPitch pitch)
{
    buf[0] = '\0';
    if ( pitch == ispchINVALID ) {
        return static_cast<char*>(NULL);
    }
    for ( int i = 0; m_debug_pitch[i].m_pitch != ispchINVALID; ++i ) {
        if ( m_debug_pitch[i].m_pitch == pitch ) {
            strcpy(buf, m_debug_pitch[i].m_pitch_str);
            return buf;
        }
    }
    return static_cast<char*>(NULL);
}

char* RMainSemanticsLabels::GetPosStr(char* buf, ISPosition pos)
{
    buf[0] = '\0';
    if ( pos == isposINVALID ) {
        return static_cast<char*>(NULL);
    }
    ISPosition tmp_pos = pos & isposMASK_DISTANCE;
    if ( tmp_pos == isposINVALID_DIST ) {
        strcat(buf, "-");
    } else {
        for ( int i = 0;
                m_debug_pos_dist[i].m_pos != isposINVALID_DIST; ++i ) {
            if ( tmp_pos == m_debug_pos_dist[i].m_pos ) {
                strcat(buf, m_debug_pos_dist[i].m_pos_str);
                break;
            }
        }
    }
    strcat(buf, "|");
    tmp_pos = pos & isposMASK_H_ANGLE;
    if ( tmp_pos == isposINVALID_H_ANGLE ) {
        strcat(buf, "-");
    } else {
        for ( int i = 0;
                m_debug_pos_hang[i].m_pos != isposINVALID_H_ANGLE; ++i ) {
            if ( tmp_pos == m_debug_pos_hang[i].m_pos ) {
                strcat(buf, m_debug_pos_hang[i].m_pos_str);
                break;
            }
        }
    }
    strcat(buf, "|");
    tmp_pos = pos & isposMASK_V_ANGLE;
    if ( tmp_pos == isposINVALID_V_ANGLE ) {
        strcat(buf, "-");
    } else {
        for ( int i = 0;
                m_debug_pos_vang[i].m_pos != isposINVALID_V_ANGLE; ++i ) {
            if ( tmp_pos == m_debug_pos_vang[i].m_pos ) {
                strcat(buf, m_debug_pos_vang[i].m_pos_str);
                break;
            }
        }
    }
    return buf;
}

char* RMainSemanticsLabels::GetSubInfoStr(char* buf, ISSemanticsID semantics_id,
            longword subinfo_index, ISSubInfo subinfo)
{
    buf[0] = '\0';
    if ( subinfo == issubINVALID ) {
        return static_cast<char*>(NULL);
    }

    longword subinfo_serial = 0;
    switch ( subinfo_index ) {
    case SEM_SUBINFO_1: subinfo_serial = 1; break;
    case SEM_SUBINFO_2: subinfo_serial = 2; break;
    case SEM_SUBINFO_3: subinfo_serial = 3; break;
    case SEM_SUBINFO_4: subinfo_serial = 4; break;
    }
    for ( int i = 0; i < m_num_of_debug_mask; ++i ) {
        DebugMask& tmp_mask = m_debug_mask[i];
        if ( (tmp_mask.m_subinfo_index & subinfo_index) == 0 ) {
            continue;
        }
        list<ISSemanticsID>::iterator iter_sem
                = tmp_mask.m_semantics_id_list.begin();
        list<ISSemanticsID>::iterator iter_sem_end
                = tmp_mask.m_semantics_id_list.end();
        while ( iter_sem != iter_sem_end ) {
            if ( *iter_sem == (semantics_id & issemMASK_SEM_ID) ) {
                list<DebugSubInfo>::iterator iter_sub
                        = tmp_mask.m_subinfo_list.begin();
                list<DebugSubInfo>::iterator iter_sub_end
                        = tmp_mask.m_subinfo_list.end();
                while ( iter_sub != iter_sub_end ) {
                    if ( (*iter_sub).m_subinfo ==
                            (tmp_mask.m_mask_id & subinfo) ) {
                        if ( *buf ) {
                            strcat(buf, "|");
                        } else {
                            sprintf(buf, "%d:\0", subinfo_serial);
                        }
                        strcat(buf, (*iter_sub).m_subinfo_str);
                        break;
                    }
                    ++iter_sub;
                }
                if ( iter_sub == iter_sub_end ) {
                    if ( *buf ) {
                        strcat(buf, "|");
                    }
                    sprintf(buf, "%s%x\0", buf, tmp_mask.m_mask_id & subinfo);
                }
                break;
            }
            ++iter_sem;
        }
    }
    if ( !(*buf) ) {
        sprintf(buf, "%d:%x\0", subinfo_serial, subinfo);
    }
    return buf;
}

char* RMainSemanticsLabels::GetAbleStatusStr(char* buf, slongword able_status)
{
    buf[0] = '\0';
    for ( int i = 0; i < 4; ++i ) {
        if ( m_debug_able[i].m_status == able_status ) {
            strcpy(buf, m_debug_able[i].m_status_str);
            break;
        }
    }
    return buf;
}


ISSemanticsID RMainSemanticsLabels::GetSemanticsID(char* semantics_name)
{
    for ( int i = 0; i < m_num_of_debug_sem; ++i ) {
        if ( strcmp(m_debug_sem[i].m_semantics_str, semantics_name) == 0 ) {
            return m_debug_sem[i].m_semantics_id;
        }
    }
    return issemINVALID;
}

ISSubInfo RMainSemanticsLabels::GetSubInfoID(char* subinfoStr)
{
    for ( int i = 0; i < m_num_of_debug_mask; ++i ) {
        DebugMask& tmp_mask = m_debug_mask[i];
        if ( tmp_mask.m_subinfo_index == 0 ) {
            continue;
        }
        list<ISSemanticsID>::iterator iter_sem
            = tmp_mask.m_semantics_id_list.begin();
        list<ISSemanticsID>::iterator iter_sem_end
            = tmp_mask.m_semantics_id_list.end();
        while ( iter_sem != iter_sem_end ) {
            list<DebugSubInfo>::iterator iter_sub
                = tmp_mask.m_subinfo_list.begin();
            list<DebugSubInfo>::iterator iter_sub_end
                = tmp_mask.m_subinfo_list.end();
            while ( iter_sub != iter_sub_end ) {
                DPRINTF(("SUB:%d %x %s", i, *iter_sem, (*iter_sub).m_subinfo_str ));
                if ( strcmp( (*iter_sub).m_subinfo_str, subinfoStr ) == 0 )
                    return (*iter_sub).m_subinfo;
            }
            ++iter_sub;
        }
        ++iter_sem;
    }
    return 0;
}
