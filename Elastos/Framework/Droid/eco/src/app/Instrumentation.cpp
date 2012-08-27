
#ifdef _FRAMEWORK
#include "app/Instrumentation.h"
#else
#include "Instrumentation.h"
#endif

PInterface Instrumentation::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IActivity*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    else if (riid == EIID_IInstrumentation) {
        return (IInstrumentation*)this;
    }

    return NULL;
}

UInt32 Instrumentation::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Instrumentation::Release()
{
    return ElRefBase::Release();
}

ECode Instrumentation::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IInstrumentation *)this) {
        *pIID = EIID_IInstrumentation;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IObject *)this) {
        *pIID = EIID_IObject;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

ECode Instrumentation::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode Instrumentation::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode Instrumentation::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return E_NOT_IMPLEMENTED;
}

ECode Instrumentation::NewApplication(
    /* [in] */ const String& moduleName,
    /* [in] */ const String& className,
    /* [in] */ IContext* ctx,
    /* [out] */ IApplication** app)
{
    if (app == NULL) return E_INVALID_ARGUMENT;

    *app = NULL;
    return NOERROR;
//    Application app = (Application)clazz.newInstance();
//    app.attach(context);
//    return app;
}

ECode Instrumentation::CallActivityOnNewIntent(
    /* [in] */ IActivity *activity,
    /* [in] */ IIntent *intent)
{
    return activity->DispatchNewIntent(intent);
}

