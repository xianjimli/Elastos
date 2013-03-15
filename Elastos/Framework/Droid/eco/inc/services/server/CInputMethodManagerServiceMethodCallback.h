
#ifndef __CINPUTMETHODMANAGERSERVICEMETHODCALLBACK_H__
#define __CINPUTMETHODMANAGERSERVICEMETHODCALLBACK_H__

#include <_CInputMethodManagerServiceMethodCallback.h>
#include <server/CInputMethodManagerService.h>


CarClass(CInputMethodManagerServiceMethodCallback)
{
public:
    CARAPI constructor(
        /* [in] */ IInputMethod* method,
        /* [in] */ Handle32 host);

    CARAPI FinishedEvent(
        /* [in] */ Int32 seq,
        /* [in] */ Boolean handled);

    CARAPI SessionCreated(
        /* [in] */ IInputMethodSession* session);

    CARAPI GetDescription(
            /* [out] */ String* description);

private:
    AutoPtr<IInputMethod> mMethod;
    CInputMethodManagerService* mHost;
};


#endif //__CINPUTMETHODMANAGERSERVICEMETHODCALLBACK_H__
