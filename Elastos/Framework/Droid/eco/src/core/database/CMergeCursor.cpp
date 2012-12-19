
#include "database/CMergeCursor.h"


ECode CMergeCursor::constructor(
    /* [in] */ ArrayOf<ICursor*>* cursors)
{
    return MergeCursor::Init(cursors);
}

ICURSOR_METHODS_IMPL(CMergeCursor, MergeCursor, MergeCursor);

ECode CMergeCursor::GetWindow(
    /* [out] */ ICursorWindow** window)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::FillWindow(
    /* [in] */ Int32 pos,
    /* [in] */ ICursorWindow* window)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::OnMove(
    /* [in] */ Int32 oldPosition,
    /* [in] */ Int32 newPosition,
    /* [out] */ Boolean* succeeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}
