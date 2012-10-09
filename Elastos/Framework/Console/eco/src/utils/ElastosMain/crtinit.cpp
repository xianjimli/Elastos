//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include <elastos.h>
#include "crt0.h"

_ELASTOS_NAMESPACE_USING

#ifdef _GNUC
EXTERN_C void __cdecl __main()
{
    // dummy function for gcc compiler
    // while(1);
}

EXTERN_C void initterm(PF *pfbegin)
{
    // constructor and destructor should begin with 0xfffffff
    // and end of 0
    // assert(*pfbegin == (PF)-1);

    if (*pfbegin == (PF)-1) {
        pfbegin ++;
    }
    else {
        DebugBreak();
    }

    while (*pfbegin) {
        (**pfbegin)();
        pfbegin ++;
    }
}

EXTERN_C void _crtinit(int startup)
{
    if (startup) {
        initterm(__CTOR_LIST__);
    }
    else {
        initterm(__DTOR_LIST__);
    }
}

#else

EXTERN_C void  __cdecl _initterm(PF *pfbegin, PF *pfend)
{
    while (pfbegin < pfend) {
        if (*pfbegin != NULL) {
            (**pfbegin)();
        }
        pfbegin++;
    }
}

EXTERN_C void  _crtinit(int startup)
{
    if (startup) {
        _initterm(__xc_a, __xc_z);
    }
    else {
        _initterm(__xt_a, __xt_z);
    }
}

#endif

EXTERN_C void _dtorlist()
{
    _crtinit(0);
}
