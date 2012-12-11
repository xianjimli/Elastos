
#ifndef __BULKCURSORPROXY_H__
#define __BULKCURSORPROXY_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
class BulkCursorProxy
{
public:
    BulkCursorProxy();

    virtual ~BulkCursorProxy();

    CARAPI Init(
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

    //public boolean updateRows(Map values) throws RemoteException

    CARAPI DeleteRow(
        /* [in] */ Int32 position,
        /* [out] */ Boolean* result);

    CARAPI GetWantsAllOnMoveCalls(
        /* [out] */ Boolean* result);

    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    CARAPI Respond(
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** returnExtras);

public:
    static const String IBulkCursor_descriptor;
    
private:
    AutoPtr<IBinder> mRemote;
    AutoPtr<IBundle> mExtras;
};
#endif //__BULKCURSORPROXY_H__