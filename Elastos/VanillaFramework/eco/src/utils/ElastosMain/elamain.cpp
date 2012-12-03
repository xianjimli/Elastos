//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include "crt0.h"
#include <elastos.h>

_ELASTOS_NAMESPACE_USING

EXTERN_C ECode __cdecl ElastosMain(const BufferOf<String>& argv)
{
    ECode ret;

    pOrgCallbackContext = (PInterface)pthread_getspecific(TL_CALLBACK_SLOT);
    if (NULL != pOrgCallbackContext) {
        pOrgCallbackContext->Release();
    }
    pthread_setspecific(TL_CALLBACK_SLOT, NULL);

    ret =  main(argv.GetUsed(), (char **)argv.GetPayload());
    exit(ret);
    return ret;
}
