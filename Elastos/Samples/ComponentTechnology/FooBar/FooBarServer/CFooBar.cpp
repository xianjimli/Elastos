//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CFooBar.h"
#include <stdio.h>

ECode CFooBar::Foo()
{
    printf("Foo\n");
    Callback::FooEvent();
    return NOERROR;
}

ECode CFooBar::Bar()
{
    printf("Bar\n");
    return NOERROR;
}

Boolean __stdcall DllMain(
    PVoid hDllHandle,
    UInt32 dwReason,
    PVoid preserved)
{
printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
    return TRUE;
}
