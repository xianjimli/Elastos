#ifndef __BULKCURSORNATIVE_H__
#define __BULKCURSORNATIVE_H__

#include "ext/frameworkext.h"
/**
 * Native implementation of the bulk cursor. This is only for use in implementing
 * IPC, application code should use the Cursor interface.
 */
class BulkCursorNative : public IBinder
{
public:
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
};
#endif //__BULKCURSORNATIVE_H__

#ifndef __BULKCURSORPROXY_H__
#define __BULKCURSORPROXY_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
class BulkCursorProxy
{
public:
    BulkCursorProxy();

    BulkCursorProxy(
        /* [in] */ IBinder* remote);

    CARAPI AsBinder(
        /* [out] */ IBinder** b);

    CARAPI GetWindow(
        /* [in] */ Int32 startPos,
        /* [out] */ ICursorWindow** cw);

    CARAPI OnMove(
        /* [in] */ Int32 position);

    CARAPI Count(
        /* [out] */ Int32* position);

    CARAPI GetColumnNames(
        /* [out] */ ArrayOf<String>** names);

    CARAPI Deactivate();

    CARAPI Close();

    CARAPI Requery(
        /* [in] */ IContentObserver* observer,
        /* [in] */ ICursorWindow* window,
        /* [out] */ Int32* value);

    
private:
    AutoPtr<IBinder> mRemote;
    AutoPtr<IBundle> mExtras;
};
#endif //__BULKCURSORPROXY_H__