#ifndef __BULKCURSORNATIVE_H__
#define __BULKCURSORNATIVE_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include "database/CBulkCursorProxy.h"
#include <elastos/ElRefBase.h>
/**
 * Native implementation of the bulk cursor. This is only for use in implementing
 * IPC, application code should use the Cursor interface.
 */
class BulkCursorNative : public IBinder, public ElRefBase
{
public:

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    BulkCursorNative();

    /**
     * Cast a Binder object into a content resolver interface, generating
     * a proxy if needed.
     */
    static CARAPI AsInterface(
        /* [in] */ IBinder* obj,
        /* [out] */ IBulkCursor** bc);

    CARAPI OnTransact(
        /* [in] */ Int32 code,
        /* [in] */ IParcel* data,
        /* [in] */ IParcel* reply,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* rst);

    virtual CARAPI AsBinder(
        /* [out] */ IBinder** b);

public:
    static const String IBulkCursor_descriptor;
};
#endif //__BULKCURSORNATIVE_H__


