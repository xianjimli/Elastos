
#include "database/CCursorToBulkCursorAdaptor.h"
ECode CCursorToBulkCursorAdaptor::GetDescription(
    /* [out] */ String * pDescription)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorToBulkCursorAdaptor::AsInterface(
    /* [in] */ IBinder * pObj,
    /* [out] */ IBulkCursor ** ppBc)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorToBulkCursorAdaptor::OnTransact(
    /* [in] */ Int32 code,
    /* [in] */ IParcel * pData,
    /* [in] */ IParcel * pReply,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean * pRst)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorToBulkCursorAdaptor::AsBinder(
    /* [out] */ IBinder ** ppB)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorToBulkCursorAdaptor::BinderDied()
{
    CursorToBulkCursorAdaptor::BinderDied();
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::GetWindow(
    /* [in] */ Int32 startPos,
    /* [out] */ ICursorWindow ** ppCw)
{
    CursorToBulkCursorAdaptor::GetWindow(startPos, ppCw);
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::OnMove(
    /* [in] */ Int32 position)
{
    CursorToBulkCursorAdaptor::OnMove(position);
    return E_NOT_IMPLEMENTED;
}

ECode CCursorToBulkCursorAdaptor::Count(
    /* [out] */ Int32 * pCount)
{
    CursorToBulkCursorAdaptor::Count(pCount);
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::GetColumnNames(
    /* [out, callee] */ ArrayOf<String> ** ppNames)
{
    CursorToBulkCursorAdaptor::GetColumnNames(ppNames);
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::DeleteRow(
    /* [in] */ Int32 position,
    /* [out] */ Boolean * pSucceeded)
{
    CursorToBulkCursorAdaptor::DeleteRow(position, pSucceeded);
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::Deactivate()
{
    CursorToBulkCursorAdaptor::Deactivate();
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::Close()
{
    CursorToBulkCursorAdaptor::Close();
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::Requery(
    /* [in] */ IContentObserver * pObserver,
    /* [in] */ ICursorWindow * pWindow,
    /* [out] */ Int32 * pValue)
{
    CursorToBulkCursorAdaptor::Requery(pObserver, pWindow, pValue);
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::GetWantsAllOnMoveCalls(
    /* [out] */ Boolean * pResult)
{
    CursorToBulkCursorAdaptor::GetWantsAllOnMoveCalls(pResult);
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::GetExtras(
    /* [out] */ IBundle ** ppExtras)
{
    CursorToBulkCursorAdaptor::GetExtras(ppExtras);
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::Respond(
    /* [in] */ IBundle * pExtras,
    /* [out] */ IBundle ** ppResult)
{
    CursorToBulkCursorAdaptor::Respond(pExtras, ppResult);
    return NOERROR;
}

ECode CCursorToBulkCursorAdaptor::constructor(
    /* [in] */ ICursor * pCursor,
    /* [in] */ IContentObserver * pObserver,
    /* [in] */ const String& providerName,
    /* [in] */ Boolean allowWrite,
    /* [in] */ ICursorWindow * pWindow)
{
    CursorToBulkCursorAdaptor::Init(pCursor, pObserver, providerName, allowWrite, pWindow);
    return NOERROR;
}

