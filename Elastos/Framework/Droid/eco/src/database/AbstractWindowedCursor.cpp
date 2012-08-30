
#include "database/AbstractWindowedCursor.h"

ECode AbstractWindowedCursor::GetBlob(
        /* [in] */ Int32 columnIndex,
        /* [out] */ ArrayOf<Byte>** blob)
{
    CheckPosition();

    Mutex::Autolock lock(mUpdatedRowsLock);
    Boolean rst;
    IsFieldUpdated(columnIndex, &rst);
    if (rst) {
//        GetUpdatedField(columnIndex, &blob);
        return NOERROR;
    }
//    mWindow->GetBlob(mPos, columnIndex, blob);
    return NOERROR;
}

//ECode AbstractWindowedCursor::GetString(
//        /* [in] */ Int32 columnIndex,
//        /* [out] */ String* value)
//{
//    assert(value != NULL);

//    Mutex::Autolock lock(mUpdatedRowsLock);
//    Boolean rst;
//    IsFieldUpdated(columnIndex, &rst);
//    if (rst) {
//        GetUpdatedField(columnIndex, &value);
//        return NOERROR;
//    }
//    mWindow->GetString(mPos, columnIndex, value);
//    return NOERROR;
//}

ECode AbstractWindowedCursor::CheckPosition()
{
    FAIL_RETURN(CheckPosition());
    if (mWindow == NULL) {
//        throw new StaleDataException("Access closed cursor");
    }
    return NOERROR;
}

ECode AbstractWindowedCursor::GetWindow(
        /* [out] */ ICursorWindow** window)
{
    *window = mWindow;
    return NOERROR;
}

ECode AbstractWindowedCursor::SetWindow(
        /* [in] */ ICursorWindow* window)
{
    if (mWindow != NULL) {
        mWindow->Close();
    }
    mWindow = window;
    return NOERROR;
}

ECode AbstractWindowedCursor::HasWindow(
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    *rst = (mWindow != NULL) ? TRUE : FALSE;
    return NOERROR;
}