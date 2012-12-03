//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <stdlib.h>
#include "crt0.h"

PATEXIT_DATA g_pDllExitStack = NULL;

int __cdecl atexit(EXIT_FUNCTION func)
{
    if (g_pDllExitStack == (PATEXIT_DATA)-1) {
        return __exeatexit(func);   /* EXE */
    }
    else {
        return __doatexit(func, &g_pDllExitStack);  /* DLL */
    }
}
