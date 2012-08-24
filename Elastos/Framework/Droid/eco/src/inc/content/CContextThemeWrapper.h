
#ifndef __CCONTEXTTHEMEWRAPPER_H__
#define __CCONTEXTTHEMEWRAPPER_H__

#include "_CContextThemeWrapper.h"
#include "content/ContextThemeWrapper.h"
#include "content/ContextMacro.h"

CarClass(CContextThemeWrapper), public ContextThemeWrapper
{
public:
    ICONTEXT_METHODS_DECL();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* base,
        /* [in] */ Int32 themeres);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetBaseContext(
        /* [out] */ IContext** context);
};

#endif // __CCONTEXTTHEMEWRAPPER_H__
