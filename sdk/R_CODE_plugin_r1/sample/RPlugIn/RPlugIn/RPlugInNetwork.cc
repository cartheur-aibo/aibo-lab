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

#include <OPENR/OccDataFormats.h>
#include "RPlugIn.h"


// Network:NTP
void 
RPlugIn::GetHostByNameContNTP(ANTENVMSG msg)
{
    m_ntp.GetHostByNameCont(msg);
    return ;
}

void
RPlugIn::SendContNTP(ANTENVMSG msg)
{
    m_ntp.SendCont(msg);
    return ;
}

void
RPlugIn::ReceiveContNTP(ANTENVMSG msg)
{
    m_ntp.ReceiveCont(msg);
    return ;
}


// Network:EchoServer
void 
RPlugIn::ListenContES (ANTENVMSG msg)
{
    m_es.ListenCont(msg);
}

void 
RPlugIn::SendContES   (ANTENVMSG msg)
{
    m_es.SendCont(msg);
}

void 
RPlugIn::ReceiveContES(ANTENVMSG msg)
{
    m_es.ReceiveCont(msg);
}

void 
RPlugIn:: CloseContES  (ANTENVMSG msg)
{
    m_es.CloseCont(msg);
}
