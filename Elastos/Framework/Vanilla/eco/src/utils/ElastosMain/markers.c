//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "crt0.h"

#ifdef _GNUC
PF __CTOR_LIST__[] = {(PF)-1};

PF __DTOR_LIST__[] = {(PF)-1};
#else
#pragma data_seg(".CRT$XCA", "DATA")
PF __xc_a[] = { 0 };                    /* C++ initializers */

#pragma data_seg(".CRT$XCZ", "DATA")
PF __xc_z[] = { 0 };

#pragma data_seg(".CRT$XTA", "DATA")
PF __xt_a[] = { 0 };                    /* C terminators */

#pragma data_seg(".CRT$XTZ", "DATA")
PF __xt_z[] = { 0 };
#endif
