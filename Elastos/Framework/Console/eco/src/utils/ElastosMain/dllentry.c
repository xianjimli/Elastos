//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

/* dllentry.c
 *
 * This file is included in crt0.lib to provide the entry point
 * -- _DllMainCRTStartup() for users' .DLL.
 */

#include <stdlib.h>
#include "crt0.h"

EXTERN_C _ELASTOS Boolean __stdcall _DllMainCRTStartup(
    PVoid hDllHandle,
    UInt32 dwReason,
    PVoid preserved)
{
    _ELASTOS Boolean ret;
    if (dwReason == DLL_PROCESS_ATTACH) {
#if !defined(_linux) || !defined(__GNUC__) || __GNUC__ < 4
        _crtinit(1);
#endif
    }
#if defined(_win32) || defined(_wince) || defined(_linux)
    if (dwReason == DLL_THREAD_ATTACH
        ||dwReason == DLL_THREAD_DETACH) {
        return TRUE;
    }
#endif
    ret = DllMain(hDllHandle, dwReason, preserved);

    if (dwReason == DLL_PROCESS_DETACH) {
#if !defined(_linux) || !defined(__GNUC__) || __GNUC__ < 4
        atexit((EXIT_FUNCTION)-1);
        _crtinit(0);
#endif
    }

    return ret;
}
