
#include "view/CWindowManagerImpl.h"
#include "view/CDisplay.h"
#include <Logger.h>
#include <StringBuffer.h>
#include "view/inputmethod/CLocalInputMethodManager.h"

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;


static AutoPtr<CWindowManagerImpl> mWindowManager;

AutoPtr<CWindowManagerImpl> CWindowManagerImpl::GetDefault()
{
    if (mWindowManager == NULL) {
        CWindowManagerImpl::AcquireSingletonByFriend((CWindowManagerImpl**)&mWindowManager);
    }

    return mWindowManager;
}

ECode CWindowManagerImpl::AddViewEx5(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    return AddView(view, params, FALSE);
}

ECode CWindowManagerImpl::UpdateViewLayout(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (IWindowManagerLayoutParams::Probe(params) == NULL) {
        Logger::W("WindowManager", "Params must be WindowManager.LayoutParams");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    view->SetLayoutParams(params);

    Mutex::Autolock lock(_m_syncLock);

    Int32 index = FindViewLocked(view, TRUE);
    mParams[index] = (IWindowManagerLayoutParams*)params;
    mRoots[index]->SetLayoutParams((IWindowManagerLayoutParams*)params, FALSE);

    return NOERROR;
}

ECode CWindowManagerImpl::RemoveView(
    /* [in] */ IView* view)
{
    Mutex::Autolock lock(_m_syncLock);

    Int32 index = FindViewLocked(view, TRUE);

    IView* curView = RemoveViewLocked(index);
    if (curView == view) {
        return NOERROR;
    }

//    throw new IllegalStateException("Calling with view " + view
//                    + " but the ViewRoot is attached to " + curView);
    return E_ILLEGAL_STATE_EXCEPTION;
}

ECode CWindowManagerImpl::RemoveViewImmediate(
    /* [in] */ IView* view)
{
    Mutex::Autolock lock(_m_syncLock);

    Int32 index = FindViewLocked(view, TRUE);
    AutoPtr<ViewRoot> root = mRoots[index];
    IView* curView = root->GetView();

    root->mAddNesting = 0;
    root->Die(TRUE);
    FinishRemoveViewLocked(curView, index);
    if (curView == view) {
        return NOERROR;
    }

//    throw new IllegalStateException("Calling with view " + view
//                    + " but the ViewRoot is attached to " + curView);
    return E_ILLEGAL_STATE_EXCEPTION;
}

ECode CWindowManagerImpl::GetDefaultDisplay(
    /* [out] */ IDisplay** display)
{
    return CDisplay::New(CDisplay::DEFAULT_DISPLAY, display);
}

IView* CWindowManagerImpl::RemoveViewLocked(
    /* [in] */ Int32 index)
{
    AutoPtr<ViewRoot> root = mRoots[index];
    IView* view = root->GetView();

    // Don't really remove until we have matched all calls to add().
    root->mAddNesting--;
    if (root->mAddNesting > 0) {
        return view;
    }

    AutoPtr<IContext> ctx;
    view->GetContext((IContext**)&ctx);
    AutoPtr<ILocalInputMethodManager> imm = CLocalInputMethodManager::GetInstance(ctx);
    if (imm != NULL) {
        IView* v = mViews[index];
        assert(v != NULL);
        AutoPtr<IBinder> binder;
        v->GetWindowToken((IBinder**)&binder);
        imm->WindowDismissed(binder);
    }
    root->Die(FALSE);
    FinishRemoveViewLocked(view, index);

    return view;
}

void CWindowManagerImpl::FinishRemoveViewLocked(
    /* [in] */ IView* view,
    /* [in] */ Int32 index)
{
    mViews.Erase(mViews.Begin() + index);
    mRoots.Erase(mRoots.Begin() + index);
    mParams.Erase(mParams.Begin() + index);

    view->AssignParent(NULL);
}

ECode CWindowManagerImpl::CloseAll(
    /* [in] */ IBinder* token,
    /* [in] */ const String& who,
    /* [in] */ const String& what)
{
    Mutex::Autolock lock(_m_syncLock);

    if (mViews.IsEmpty()) {
        return NOERROR;
    }

    Vector<AutoPtr<IView> >::Iterator vwit = mViews.Begin();
    Vector<AutoPtr<ViewRoot> >::Iterator rtit = mRoots.Begin();
    Vector<AutoPtr<IWindowManagerLayoutParams> >::Iterator pmit = mParams.Begin();
    for (Int32 index = 0; vwit!= mViews.End(); ++vwit, ++rtit, ++pmit, ++index) {
        AutoPtr<IBinder> paramToken =
            ((CWindowManagerLayoutParams*)(*pmit).Get())->mToken;

        if (token == NULL || paramToken.Get() == token) {
            AutoPtr<ViewRoot> root = *rtit;
            root->mAddNesting = 1;

            if (!who.IsEmpty()) {
//                WindowLeaked leak = new WindowLeaked(
//                                what + " " + who + " has leaked window "
//                                + root.getView() + " that was originally added here");
//                        leak.setStackTrace(root.getLocation().getStackTrace());
//                        Log.e("WindowManager", leak.getMessage(), leak);
                Logger::E("WindowManager", StringBuffer(what) + " " + who +
                        " has leaked window that was originally added here");
            }

            IView* view = root->GetView();

            // Don't really remove until we have matched all calls to add().
            root->mAddNesting--;
            if (root->mAddNesting > 0) {
                continue;
            }

            AutoPtr<IContext> ctx;
            view->GetContext((IContext**)&ctx);
            AutoPtr<ILocalInputMethodManager> imm = CLocalInputMethodManager::GetInstance(ctx);
            if (imm != NULL) {
                IView* v = mViews[index];
                assert(v != NULL);
                AutoPtr<IBinder> binder;
                v->GetWindowToken((IBinder**)&binder);
                imm->WindowDismissed(binder);
            }

            root->Die(FALSE);
            view->AssignParent(NULL);

            vwit = mViews.Erase(vwit); --vwit;
            rtit = mRoots.Erase(rtit); --rtit;
            pmit = mParams.Erase(pmit); --pmit;
            index--;
        }
    }

    return NOERROR;
}

ECode CWindowManagerImpl::AddView(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params,
    /* [in] */ Boolean nest)
{
//    if (Config.LOGV) Log.v("WindowManager", "addView view=" + view);

    if (IWindowManagerLayoutParams::Probe(params) == NULL) {
        Logger::W("WindowManager", "Params must be WindowManager.LayoutParams");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ViewRoot> root;
    AutoPtr<IView> panelParentView;

    {
        Mutex::Autolock lock(_m_syncLock);

        // Here's an odd/questionable case: if someone tries to add a
        // view multiple times, then we simply bump up a nesting count
        // and they need to remove the view the corresponding number of
        // times to have it actually removed from the window manager.
        // This is useful specifically for the notification manager,
        // which can continually add/remove the same view as a
        // notification gets updated.
        //
        Int32 index = FindViewLocked(view, FALSE);
        if (index >= 0) {
            if (!nest) {
//            throw new IllegalStateException("View " + view
//                            + " has already been added to the window manager.");
                return E_ILLEGAL_STATE_EXCEPTION;
            }
            root = mRoots[index];
            root->mAddNesting++;
            // Update layout parameters.
            view->SetLayoutParams(params);
            root->SetLayoutParams((IWindowManagerLayoutParams*)params, TRUE);

            return NOERROR;
        }

        // If this is a panel window, then find the window it is being
        // attached to for future reference.
        //
        Int32 type = ((CWindowManagerLayoutParams*)params)->mType;
        if (type >= WindowManagerLayoutParams_FIRST_SUB_WINDOW &&
            type <= WindowManagerLayoutParams_LAST_SUB_WINDOW) {
            Int32 count = mViews.GetSize();
            for (Int32 i=0; i<count; i++) {
                if ((IBinder*)mRoots[i]->mWindow->Probe(EIID_IBinder) ==
                    ((CWindowManagerLayoutParams*)params)->mToken) {
                    panelParentView = mViews[i];
                }
            }
        }

        AutoPtr<IContext> context;
        view->GetContext((IContext**)&context);
        root = new ViewRoot(context);
        root->mAddNesting = 1;

        view->SetLayoutParams(params);

        mViews.PushBack(view);
        mRoots.PushBack(root);
        mParams.PushBack((IWindowManagerLayoutParams*)params);
    }

    // do this last because it fires off messages to start doing things
    root->SetView(view, (IWindowManagerLayoutParams*)params, panelParentView);

    return NOERROR;
}

Int32 CWindowManagerImpl::FindViewLocked(
    /* [in] */ IView* view,
    /* [in] */ Boolean required)
{
    Vector<AutoPtr<IView> >::Iterator it = mViews.Begin();
    for (Int32 i = 0; it != mViews.End(); ++it, ++i) {
        if ((*it).Get() == view) {
            return i;
        }
    }

    if (required) {
//        throw new IllegalArgumentException(
//                        "View not attached to window manager");
        assert(0);
    }

    return -1;
}
