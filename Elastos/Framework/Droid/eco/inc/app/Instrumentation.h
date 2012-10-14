
#ifndef __INSTRUMENTATION_H__
#define __INSTRUMENTATION_H__

#ifdef _FRAMEWORK_CORE
#include "ext/frameworkext.h"
#else
#define __USE_MALLOC
#include "Elastos.Framework.Core.h"
#endif
#include <elastos/ElRefBase.h>

using namespace Elastos;

class Instrumentation
    : public ElRefBase
    , public IObject
    , public IInstrumentation
{
public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Aggregate(
        /* [in] */ AggrType aggrType,
        /* [in] */ PInterface pObject);

    CARAPI GetDomain(
        /* [out] */ PInterface *ppObject);

    CARAPI GetClassID(
        /* [out] */ ClassID *pCLSID);

    CARAPI NewApplication(
        /* [in] */ const String& moduleName,
        /* [in] */ const String& className,
        /* [in] */ IContext* ctx,
        /* [out] */ IApplication** app);

    CARAPI CallActivityOnNewIntent(
        /* [in] */ IActivity *activity,
        /* [in] */ IIntent *intent);

private:

};

#endif // __INSTRUMENTATION_H__
