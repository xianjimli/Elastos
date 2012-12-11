
#ifndef __CBULKCURSORPROXY_H__
#define __CBULKCURSORPROXY_H__

#include "_CBulkCursorProxy.h"
#include "database/BulkCursorProxy.h"
CarClass(CBulkCursorProxy), public BulkCursorProxy
{
public:
    CARAPI AsBinder(
        /* [out] */ IBinder ** ppB);

    CARAPI GetWindow(
        /* [in] */ Int32 startPos,
        /* [out] */ ICursorWindow ** ppCw);

    CARAPI OnMove(
        /* [in] */ Int32 position);

    CARAPI Count(
        /* [out] */ Int32 * pCount);

    CARAPI GetColumnNames(
        /* [out, callee] */ ArrayOf<String> ** ppNames);

    CARAPI DeleteRow(
        /* [in] */ Int32 position,
        /* [out] */ Boolean * pSucceeded);

    CARAPI Deactivate();

    CARAPI Close();

    CARAPI Requery(
        /* [in] */ IContentObserver * pObserver,
        /* [in] */ ICursorWindow * pWindow,
        /* [out] */ Int32 * pValue);

    CARAPI GetWantsAllOnMoveCalls(
        /* [out] */ Boolean * pResult);

    CARAPI GetExtras(
        /* [out] */ IBundle ** ppExtras);

    CARAPI Respond(
        /* [in] */ IBundle * pExtras,
        /* [out] */ IBundle ** ppResult);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IBinder * pRemote);

private:
    // TODO: Add your private member variables here.
};

#endif // __CBULKCURSORPROXY_H__
