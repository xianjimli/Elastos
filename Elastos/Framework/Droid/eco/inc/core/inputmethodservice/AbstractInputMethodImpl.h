
#ifndef  _ABSTRACTINPUTMETHODIMPL_H__
#define  _ABSTRACTINPUTMETHODIMPL_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include "inputmethodservice/CAbstractInputMethodService.h"


class AbstractInputMethodImpl:
    public ElRefBase,
    public IAbstractInputMethodImpl,
    public ILocalInputMethod
{
public:
    AbstractInputMethodImpl(
        /* [in] */ CAbstractInputMethodService* host);

    ~AbstractInputMethodImpl();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI AttachToken(
        /* [in] */ IBinder* token);

    CARAPI BindInput(
        /* [in] */ IInputBinding* binding);

    CARAPI UnbindInput();

    CARAPI StartInput(
        /* [in] */ IInputConnection* inputConnection,
        /* [in] */ IEditorInfo* attribute);

    CARAPI RestartInput(
        /* [in] */ IInputConnection* inputConnection,
        /* [in] */ IEditorInfo* attribute);

    CARAPI CreateSession(
        /* [in] */ ILocalInputMethodSessionCallback* callback);

    CARAPI SetSessionEnabled(
        /* [in] */ ILocalInputMethodSession* session,
        /* [in] */ Boolean enabled);

    CARAPI RevokeSession(
        /* [in] */ ILocalInputMethodSession* session);

    CARAPI ShowSoftInput(
        /* [in] */ Int32 flags,
        /* [in] */ IResultReceiver* resultReceiver);

    CARAPI HideSoftInput(
        /* [in] */ Int32 flags,
        /* [in] */ IResultReceiver* resultReceiver);

private:
	CAbstractInputMethodService* mHost;
};

#endif  //_ABSTRACTINPUTMETHODIMPL_H__
