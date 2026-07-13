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

#ifndef _stub_h_DEFINED
#define _stub_h_DEFINED

#ifndef OPENR_STUBGEN
#define OPENR_STUBGEN
#endif

#include <OPENR/OPENR.h>
#include <OPENR/OPENRMessages.h>
#include <OPENR/ObjcommMessages.h>
#include <OPENR/stub_macro.h>
#include "def.h"

extern "C" {
    StubFuncDeclare_Basic(Init)
    StubFuncDeclare_Basic(Start)
    StubFuncDeclare_Basic(Stop)
    StubFuncDeclare_Basic(Destroy)
    StubFuncDeclare_Subject(0)
    StubFuncDeclare_Subject(1)
    StubFuncDeclare_Subject(2)
    StubFuncDeclare_Subject(3)
    StubFuncDeclare_Subject(4)
    StubFuncDeclare_Subject(5)
    StubFuncDeclare_Subject(6)
    StubFuncDeclare_Subject(7)
    StubFuncDeclare_Subject(8)
    StubFuncDeclare_Observer(0)
    StubFuncDeclare_Observer(1)
    StubFuncDeclare_Observer(2)
    StubFuncDeclare_Observer(3)
    StubFuncDeclare_Observer(4)
    StubFuncDeclare_Observer(5)
    StubFuncDeclare_Observer(6)
    StubFuncDeclare_Observer(7)
    StubFuncDeclare_Observer(8)
    StubFuncDeclare_Observer(9)
    StubFuncDeclare_Observer(10)
    StubFuncDeclare_Observer(11)
    StubFuncDeclare_Observer(12)
    StubFuncDeclare_Observer(13)
    StubFuncDeclare_Observer(14)
    StubFuncDeclare_Observer(15)
}

#endif // _stub_h_DEFINED

