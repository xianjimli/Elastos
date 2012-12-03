//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <Elastos.Frameworks.Vanilla.Console.h>
#include "CFooBar.h"
#include <stdio.h>

ECode CFooBar::Foo()
{
    CConsole::WriteLine("============CFooBar::Foo();");
    Callback::FooEvent();
    return NOERROR;
}

ECode CFooBar::Bar()
{
    CConsole::WriteLine("============CFooBar::Bar();");
    return NOERROR;
}

/*
Boolean __stdcall DllMain(
    PVoid hDllHandle,
    UInt32 dwReason,
    PVoid preserved)
{
printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
    return TRUE;
}
*/