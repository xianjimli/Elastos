//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CFooBar.h"
#include <Elastos.Framework.Vanilla>

ECode CFooBar::Foo()
{
    CConsole::WriteLine("====== CALLing Foo ======");
    Callback::FooEvent();
    return NOERROR;
}

ECode CFooBar::Bar()
{
    CConsole::WriteLine("====== CALLing Bar ======");
    return NOERROR;
}
