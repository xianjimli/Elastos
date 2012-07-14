
#ifndef __CCONTEXTWRAPPER_H__
#define __CCONTEXTWRAPPER_H__

#include "_CContextWrapper.h"
#include "content/ContextWrapper.h"
#include "content/ContextMacro.h"

CarClass(CContextWrapper), public ContextWrapper
{
public:
    ICONTEXT_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetBaseContext(
        /* [out] */ IContext** context);
};

#endif // __CCONTEXTWRAPPER_H__
