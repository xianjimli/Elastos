
#include "inputmethodservice/SoftInputWindow.h"

IDIALOG_METHODS_IMPL(SoftInputWindow, Dialog, Dialog);
IWINDOWCALLBACK_METHODS_IMPL(SoftInputWindow, Dialog, Dialog);
IKeyEventCallback_METHODS_IMPL(SoftInputWindow, Dialog, Dialog);


_SoftInputWindow::_SoftInputWindow(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme,
    /* [in] */ IDispatcherState* dispatcherState):
    Dialog(context, theme)
{

}

_SoftInputWindow::~_SoftInputWindow()
{

}

ECode _SoftInputWindow::OnWindowFocusChanged(
    /* [in] */ Boolean hasFocus)
{
    Dialog::OnWindowFocusChanged(hasFocus);
    return mDispatcherState->Reset();
}

SoftInputWindow::SoftInputWindow(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme,
    /* [in] */ IDispatcherState* dispatcherState):
    _SoftInputWindow(context, theme, dispatcherState)
{
    mDispatcherState = dispatcherState;
    InitDockWindow();
}

PInterface SoftInputWindow::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IDialogInterface == riid) {
        return (IDialogInterface *)(IDialog *)this;
    }
    if (EIID_IDialog == riid) {
        return (IDialog *)this;
    }
    if (EIID_IWindowCallback == riid) {
        return (IWindowCallback *)this;
    }
    if (EIID_IKeyEventCallback == riid) {
        return (IKeyEventCallback *)this;
    }
    if (EIID_IViewOnCreateContextMenuListener == riid) {
        return (IViewOnCreateContextMenuListener *)this;
    }

    return NULL;
}

UInt32 SoftInputWindow::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 SoftInputWindow::Release()
{
    return ElRefBase::Release();
}

ECode SoftInputWindow::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)(IDialog*)this) {
        *pIID = EIID_IDialog;
    }
    else if (pObject == (IInterface*)(IWindowCallback*)this) {
        *pIID = EIID_IWindowCallback;
    }
    else if (pObject == (IInterface*)(IKeyEventCallback*)this) {
        *pIID = EIID_IKeyEventCallback;
    }
    else if (pObject == (IInterface*)(IViewOnCreateContextMenuListener*)this) {
        *pIID = EIID_IViewOnCreateContextMenuListener;
    }
    else if (pObject == (IInterface*)(IObject*)this) {
        *pIID = EIID_IObject;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode SoftInputWindow::SetToken(
    /* [in] */ IBinder* token)
{
	AutoPtr<IWindow> window;
	GetWindow((IWindow**) &window);
	assert(window != NULL);

	AutoPtr<IWindowManagerLayoutParams> lp;
	window->GetAttributes((IWindowManagerLayoutParams**) &lp);

	((CWindowManagerLayoutParams*)lp.Get())->mToken = token;

	return window->SetAttributes(lp.Get());
}

// ECode SoftInputWindow::OnWindowFocusChanged(
//     /* [in] */ Boolean hasFocus)
// {
// 	Dialog::OnWindowFocusChanged(hasFocus);
//     return mDispatcherState->Reset();
// }

ECode SoftInputWindow::GetSize(
    /* [out] */ Int32* size)
{
	AutoPtr<IWindow> window;
	GetWindow((IWindow**) &window);
	assert(window != NULL);

	AutoPtr<IWindowManagerLayoutParams> lp;
	window->GetAttributes((IWindowManagerLayoutParams**) &lp);

    if (((CWindowManagerLayoutParams*)lp.Get())->mGravity == Gravity_TOP
    		|| ((CWindowManagerLayoutParams*)lp.Get())->mGravity == Gravity_BOTTOM) {
    	*size = ((CWindowManagerLayoutParams*)lp.Get())->mHeight;
    } else {
        *size = ((CWindowManagerLayoutParams*)lp.Get())->mWidth;
    }

    return NOERROR;
}

ECode SoftInputWindow::SetSize(
    /* [in] */ Int32 size)
{
	AutoPtr<IWindow> window;
	GetWindow((IWindow**) &window);
	assert(window != NULL);

	AutoPtr<IWindowManagerLayoutParams> lp;
	window->GetAttributes((IWindowManagerLayoutParams**) &lp);

    if (((CWindowManagerLayoutParams*)lp.Get())->mGravity == Gravity_TOP
    	|| ((CWindowManagerLayoutParams*)lp.Get())->mGravity == Gravity_BOTTOM) {
    	((CWindowManagerLayoutParams*)lp.Get())->mWidth = -1;
        ((CWindowManagerLayoutParams*)lp.Get())->mHeight = size;
    } else {
    	((CWindowManagerLayoutParams*)lp.Get())->mWidth = size;
        ((CWindowManagerLayoutParams*)lp.Get())->mHeight = -1;
    }

    return window->SetAttributes(lp.Get());
}

ECode SoftInputWindow::SetGravity(
    /* [in] */ Int32 gravity)
{
	AutoPtr<IWindow> window;
	GetWindow((IWindow**) &window);
	assert(window != NULL);

	AutoPtr<IWindowManagerLayoutParams> lp;
	window->GetAttributes((IWindowManagerLayoutParams**) &lp);

    Boolean oldIsVertical = (((CWindowManagerLayoutParams*)lp.Get())->mGravity == Gravity_TOP
    	|| ((CWindowManagerLayoutParams*)lp.Get())->mGravity == Gravity_BOTTOM);

    ((CWindowManagerLayoutParams*)lp.Get())->mGravity = gravity;

    Boolean newIsVertical = (((CWindowManagerLayoutParams*)lp.Get())->mGravity == Gravity_TOP
    	|| ((CWindowManagerLayoutParams*)lp.Get())->mGravity == Gravity_BOTTOM);

    if (oldIsVertical != newIsVertical) {
        Int32 tmp = ((CWindowManagerLayoutParams*)lp.Get())->mWidth;
        ((CWindowManagerLayoutParams*)lp.Get())->mWidth = ((CWindowManagerLayoutParams*)lp.Get())->mHeight;
        ((CWindowManagerLayoutParams*)lp.Get())->mHeight = tmp;
        return window->SetAttributes(lp.Get());
    }

    return NOERROR;
}

void SoftInputWindow::InitDockWindow()
{
	AutoPtr<IWindow> window;
	GetWindow((IWindow**) &window);
	assert(window != NULL);

	AutoPtr<IWindowManagerLayoutParams> lp;
	window->GetAttributes((IWindowManagerLayoutParams**) &lp);

	((CWindowManagerLayoutParams*)lp.Get())->mType = WindowManagerLayoutParams_TYPE_INPUT_METHOD;

    AutoPtr<ICharSequence> text;
    CStringWrapper::New(String("InputMethod"), (ICharSequence**) &text);
    lp->SetTitle(text);

    ((CWindowManagerLayoutParams*)lp.Get())->mGravity = Gravity_BOTTOM;
    ((CWindowManagerLayoutParams*)lp.Get())->mWidth = -1;
    // Let the input method window's orientation follow sensor based rotation
    // Turn this off for now, it is very problematic.
    //lp.screenOrientation = ActivityInfo.SCREEN_ORIENTATION_USER;

    window->SetAttributes(lp.Get());
    window->SetFlags(
            WindowManagerLayoutParams_FLAG_LAYOUT_IN_SCREEN |
            WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE,
            WindowManagerLayoutParams_FLAG_LAYOUT_IN_SCREEN |
            WindowManagerLayoutParams_FLAG_NOT_FOCUSABLE |
            WindowManagerLayoutParams_FLAG_DIM_BEHIND);
}
