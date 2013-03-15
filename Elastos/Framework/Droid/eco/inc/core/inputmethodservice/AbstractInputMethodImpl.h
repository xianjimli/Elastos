
#ifndef  __ABSTRACTINPUTMETHODIMPL_H__
#define  __ABSTRACTINPUTMETHODIMPL_H__

#ifdef _FRAMEWORK_CORE
#include "ext/frameworkext.h"
#include "inputmethodservice/AbstractInputMethodService.h"
#else
#include "Elastos.Framework.Core.h"
#include "AbstractInputMethodService.h"
#endif

#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

class AbstractInputMethodImpl
    : public ElRefBase
    , public IAbstractInputMethodImpl
{
public:
    AbstractInputMethodImpl(
        /* [in] */ AbstractInputMethodService* host);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI CreateSession(
        /* [in] */ ILocalInputMethodSessionCallback* callback);

    CARAPI SetSessionEnabled(
        /* [in] */ ILocalInputMethodSession* session,
        /* [in] */ Boolean enabled);

    CARAPI RevokeSession(
        /* [in] */ ILocalInputMethodSession* session);

private:
	AbstractInputMethodService* mHost;
};

#endif  //__ABSTRACTINPUTMETHODIMPL_H__
