
#ifndef __CCURSORTOBULKCURSORADAPTOR_H__
#define __CCURSORTOBULKCURSORADAPTOR_H__

#include "_CCursorToBulkCursorAdaptor.h"
#include "database/CursorToBulkCursorAdaptor.h"
CarClass(CCursorToBulkCursorAdaptor), public CursorToBulkCursorAdaptor
{
public:
    CARAPI GetDescription(
        /* [out] */ String * pDescription);

    CARAPI AsInterface(
        /* [in] */ IBinder * pObj,
        /* [out] */ IBulkCursor ** ppBc);

    CARAPI OnTransact(
        /* [in] */ Int32 code,
        /* [in] */ IParcel * pData,
        /* [in] */ IParcel * pReply,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean * pRst);

    CARAPI AsBinder(
        /* [out] */ IBinder ** ppB);

    CARAPI BinderDied();

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

    CARAPI constructor(
        /* [in] */ ICursor * pCursor,
        /* [in] */ IContentObserver * pObserver,
        /* [in] */ const String& providerName,
        /* [in] */ Boolean allowWrite,
        /* [in] */ ICursorWindow * pWindow);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCURSORTOBULKCURSORADAPTOR_H__
