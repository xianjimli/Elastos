#ifndef __TCallback31_H__
#define __TCallback31_H__

#include "_TCallback31.h"

class TCallback31 : public _TCallback31
{
public:
    CARAPI RequestService(
        /* [in] */ String serviceName,
        /* [in] */ String arguments,
        /* [in] */ IAppletSession * pSession);

    CARAPI Notify(
        /* [in] */ String eventName,
        /* [in] */ const MemoryBuf & arguments);

    CARAPI ObjectEnter(
        /* [in] */ PInterface pObject);

    CARAPI ObjectLeave(
        /* [in] */ PInterface pObject);

    CARAPI CreateObject(
        /* [in] */ const ClassID & clsid,
        /* [in] */ const InterfaceID & iid,
        /* [out] */ IInterface ** ppObject);

    CARAPI Hello31();

    CARAPI Main(
        /* [in] */ const BufferOf<String> & args);

    CARAPI Activate() { return E_NOT_IMPLEMENTED; }

    CARAPI Deactivate() { return E_NOT_IMPLEMENTED; }

    CARAPI IsActivated(
        /* [out] */ Boolean * pActivated) { return E_NOT_IMPLEMENTED; }

    CARAPI Snooze() { return E_NOT_IMPLEMENTED; }

    CARAPI Wakeup() { return E_NOT_IMPLEMENTED; }

    CARAPI GetName(
        /* [out] */ StringBuf * pName) { return E_NOT_IMPLEMENTED; }

private:
    // TODO: Add your private member variables here.
};

#endif // __TCallback31_H__
