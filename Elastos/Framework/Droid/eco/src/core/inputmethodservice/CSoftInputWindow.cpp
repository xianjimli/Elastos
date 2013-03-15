
#include "inputmethodservice/CSoftInputWindow.h"
#include "view/CWindowManagerLayoutParams.h"


ECode SoftInputWindow::SetToken(
    /* [in] */ IBinder* token)
{
    AutoPtr<IWindow> window = GetWindow();
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    lp->SetToken(token);
    return window->SetAttributes(lp);
}

ECode SoftInputWindow::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme,
    /* [in] */ IDispatcherState* dispatcherState)
{
    FAIL_RETURN(Dialog::Init(context, theme));
    mDispatcherState = dispatcherState;
    InitDockWindow();
    return NOERROR;
}

ECode SoftInputWindow::OnWindowFocusChanged(
    /* [in] */ Boolean hasFocus)
{
    FAIL_RETURN(Dialog::OnWindowFocusChanged(hasFocus));
    return mDispatcherState->Reset();
}

ECode SoftInputWindow::GetSize(
    /* [out] */ Int32* size)
{
    assert(size != NULL);
    AutoPtr<IWindow> window = GetWindow();
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    CWindowManagerLayoutParams* lpObj = (CWindowManagerLayoutParams*)lp.Get();
    if (lpObj->mGravity == Gravity_TOP || lpObj->mGravity == Gravity_BOTTOM) {
        *size = lpObj->mHeight;
    }
    else {
        *size = lpObj->mWidth;
    }
    return NOERROR;
}

ECode SoftInputWindow::SetSize(
    /* [in] */ Int32 size)
{
    AutoPtr<IWindow> window = GetWindow();
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    CWindowManagerLayoutParams* lpObj = (CWindowManagerLayoutParams*)lp.Get();
    if (lpObj->mGravity == Gravity_TOP || lpObj->mGravity == Gravity_BOTTOM) {
        lpObj->mWidth = -1;
        lpObj->mHeight = size;
    }
    else {
        lpObj->mWidth = size;
        lpObj->mHeight = -1;
    }
    return window->SetAttributes(lp);
}

ECode SoftInputWindow::SetGravity(
    /* [in] */ Int32 gravity)
{
    AutoPtr<IWindow> window = GetWindow();
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    CWindowManagerLayoutParams* lpObj = (CWindowManagerLayoutParams*)lp.Get();

    Boolean oldIsVertical = (lpObj->mGravity == Gravity_TOP || lpObj->mGravity == Gravity_BOTTOM);

    lpObj->mGravity = gravity;

    Boolean newIsVertical = (lpObj->mGravity == Gravity_TOP || lpObj->mGravity == Gravity_BOTTOM);

    if (oldIsVertical != newIsVertical) {
        Int32 tmp = lpObj->mWidth;
        lpObj->mWidth = lpObj->mHeight;
        lpObj->mHeight = tmp;
        return window->SetAttributes(lp);
    }
    return NOERROR;
}

void SoftInputWindow::InitDockWindow()
{
    AutoPtr<IWindow> window = GetWindow();
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);
    CWindowManagerLayoutParams* lpObj = (CWindowManagerLayoutParams*)lp.Get();

    lpObj->mType = WindowManagerLayoutParams_TYPE_INPUT_METHOD;
    AutoPtr<ICharSequence> text;
    CStringWrapper::New(String("InputMethod"), (ICharSequence**)&text);
    lp->SetTitle(text);

    lpObj->mGravity = Gravity_BOTTOM;
    lpObj->mWidth = -1;
    // Let the input method window's orientation follow sensor based rotation
    // Turn this off for now, it is very problematic.
    //lp.screenOrientation = ActivityInfo.SCREEN_ORIENTATION_USER;

    window->SetAttributes(lp);
    window->SetFlags(
            WindowManagerLayoutParams_FLAG_LAYOUT_IN_SCREEN |
            WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE,
            WindowManagerLayoutParams_FLAG_LAYOUT_IN_SCREEN |
            WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE |
            WindowManagerLayoutParams_FLAG_DIM_BEHIND);
}


IDIALOG_METHODS_IMPL(CSoftInputWindow, Dialog, Dialog);
IWINDOWCALLBACK_METHODS_IMPL(CSoftInputWindow, Dialog, Dialog);
IKeyEventCallback_METHODS_IMPL(CSoftInputWindow, Dialog, Dialog);

ECode CSoftInputWindow::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme,
    /* [in] */ IDispatcherState* dispatcherState)
{
    return SoftInputWindow::Init(context, theme, dispatcherState);
}

PInterface CSoftInputWindow::Probe(
    /* [in] */ REIID riid)
{
    return _CSoftInputWindow::Probe(riid);
}

ECode CSoftInputWindow::SetToken(
    /* [in] */ IBinder* token)
{
    return SoftInputWindow::SetToken(token);
}

ECode CSoftInputWindow::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return SoftInputWindow::GetSize(size);
}

ECode CSoftInputWindow::SetSize(
    /* [in] */ Int32 size)
{
    return SoftInputWindow::SetSize(size);
}

ECode CSoftInputWindow::SetGravity(
    /* [in] */ Int32 gravity)
{
    return SoftInputWindow::SetGravity(gravity);
}

ECode CSoftInputWindow::OnCreateContextMenu(
    /* [in] */ IContextMenu* menu,
    /* [in] */ IView* v,
    /* [in] */ IContextMenuInfo* menuInfo)
{
    return Dialog::OnCreateContextMenu(menu, v, menuInfo);
}
