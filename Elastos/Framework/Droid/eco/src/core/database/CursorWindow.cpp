
#include "database/CursorWindow.h"

CursorWindow::CursorWindow() : mStartPos(0)
{
}

CursorWindow::~CursorWindow()
{

}

CursorWindow::CursorWindow(
        /* [in] */ Boolean localWindow)
{
}

CursorWindow::CursorWindow(
        /* [in] */ IParcel* source)
{
}

ECode CursorWindow::GetStartPosition(
        /* [out] */ Int32* pos)
{
    assert(pos != NULL);
    *pos = mStartPos;
    return NOERROR;
}

ECode CursorWindow::SetStartPosition(
        /* [in] */ Int32 pos)
{
    mStartPos = pos;
    return NOERROR;
}

ECode CursorWindow::Close()
{
    FAIL_RETURN(ReleaseReference());
    return NOERROR;
}

ECode CursorWindow::DescribeContents(
        /* [out] */ Int32* value)
{
    assert(value != NULL);
    *value = 0;
    return NOERROR;
}