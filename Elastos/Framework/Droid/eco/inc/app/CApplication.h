
#ifndef __CAPPLICATION_H__
#define __CAPPLICATION_H__

#include "_CApplication.h"
#include "content/ContextWrapper.h"
#include "content/ContextMacro.h"

CarClass(CApplication), public ContextWrapper
{
public:
    ICONTEXT_METHODS_DECL();

    CARAPI constructor();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetBaseContext(
        /* [out] */ IContext** ctx);

    CARAPI Create();

protected:
    CARAPI OnCreate();
};

#endif //__CAPPLICATION_H__
