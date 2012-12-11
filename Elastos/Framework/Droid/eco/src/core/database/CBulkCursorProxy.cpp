
#include "database/CBulkCursorProxy.h"
ECode CBulkCursorProxy::AsBinder(
    /* [out] */ IBinder ** ppB)
{
    BulkCursorProxy::AsBinder(ppB);
    return NOERROR;
}

ECode CBulkCursorProxy::GetWindow(
    /* [in] */ Int32 startPos,
    /* [out] */ ICursorWindow ** ppCw)
{
    BulkCursorProxy::GetWindow(startPos, ppCw);
    return NOERROR;
}

ECode CBulkCursorProxy::OnMove(
    /* [in] */ Int32 position)
{
    BulkCursorProxy::OnMove(position);
    return NOERROR;
}

ECode CBulkCursorProxy::Count(
    /* [out] */ Int32 * pCount)
{
    BulkCursorProxy::Count(pCount);
    return NOERROR;
}

ECode CBulkCursorProxy::GetColumnNames(
    /* [out, callee] */ ArrayOf<String> ** ppNames)
{
    BulkCursorProxy::GetColumnNames(ppNames);
    return NOERROR;
}

ECode CBulkCursorProxy::DeleteRow(
    /* [in] */ Int32 position,
    /* [out] */ Boolean * pSucceeded)
{
    BulkCursorProxy::DeleteRow(position, pSucceeded);
    return NOERROR;
}

ECode CBulkCursorProxy::Deactivate()
{
    BulkCursorProxy::Deactivate();
    return NOERROR;
}

ECode CBulkCursorProxy::Close()
{
    BulkCursorProxy::Close();
    return NOERROR;
}

ECode CBulkCursorProxy::Requery(
    /* [in] */ IContentObserver * pObserver,
    /* [in] */ ICursorWindow * pWindow,
    /* [out] */ Int32 * pValue)
{
    BulkCursorProxy::Requery(pObserver, pWindow, pValue);
    return NOERROR;
}

ECode CBulkCursorProxy::GetWantsAllOnMoveCalls(
    /* [out] */ Boolean * pResult)
{
    BulkCursorProxy::GetWantsAllOnMoveCalls(pResult);
    return NOERROR;
}

ECode CBulkCursorProxy::GetExtras(
    /* [out] */ IBundle ** ppExtras)
{
    BulkCursorProxy::GetExtras(ppExtras);
    return NOERROR;
}

ECode CBulkCursorProxy::Respond(
    /* [in] */ IBundle * pExtras,
    /* [out] */ IBundle ** ppResult)
{
    BulkCursorProxy::Respond(pExtras, ppResult);
    return NOERROR;
}

ECode CBulkCursorProxy::constructor()
{
    BulkCursorProxy::BulkCursorProxy();
    return NOERROR;
}

ECode CBulkCursorProxy::constructor(
    /* [in] */ IBinder * pRemote)
{
    BulkCursorProxy::Init(pRemote);
    return NOERROR;
}

