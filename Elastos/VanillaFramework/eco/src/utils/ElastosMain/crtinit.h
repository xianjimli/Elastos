//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CRTINIT_H__
#define __CRTINIT_H__

#include "crt0.h"

typedef void (__cdecl *PF)(void);

EXTERN_C PF __CTOR_LIST__[];
EXTERN_C PF __DTOR_LIST__[];

EXTERN_C void _crtinit(int startup);

#endif //__CRTINIT_H__
