
#ifndef __CWINDOWMANAGERIMPL_H__
#define __CWINDOWMANAGERIMPL_H__

#include "_CWindowManagerImpl.h"
#include "ext/frameworkext.h"
#include "view/ViewRoot.h"
#include <elastos/AutoPtr.h>
#include <elastos/Vector.h>

CarClass(CWindowManagerImpl)
{
public:
    /**
     * The user is navigating with keys (not the touch screen), so
     * navigational focus should be shown.
     */
    static const Int32 RELAYOUT_IN_TOUCH_MODE = 0x1;
    /**
     * This is the first time the window is being drawn,
     * so the client must call drawingFinished() when done
     */
    static const Int32 RELAYOUT_FIRST_TIME = 0x2;

    static const Int32 ADD_FLAG_APP_VISIBLE = 0x2;
    static const Int32 ADD_FLAG_IN_TOUCH_MODE = RELAYOUT_IN_TOUCH_MODE;

    static const Int32 ADD_OKAY = 0;
    static const Int32 ADD_BAD_APP_TOKEN = -1;
    static const Int32 ADD_BAD_SUBWINDOW_TOKEN = -2;
    static const Int32 ADD_NOT_APP_TOKEN = -3;
    static const Int32 ADD_APP_EXITING = -4;
    static const Int32 ADD_DUPLICATE_ADD = -5;
    static const Int32 ADD_STARTING_NOT_NEEDED = -6;
    static const Int32 ADD_MULTIPLE_SINGLETON = -7;
    static const Int32 ADD_PERMISSION_DENIED = -8;

public:
    static CARAPI_(AutoPtr<CWindowManagerImpl>) GetDefault();

    CARAPI AddViewEx5(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* params);

    CARAPI UpdateViewLayout(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* params);

    CARAPI RemoveView(
        /* [in] */ IView* view);

    CARAPI RemoveViewImmediate(
        /* [in] */ IView* view);

    CARAPI GetDefaultDisplay(
        /* [out] */ IDisplay** display);

public:
    CARAPI CloseAll(
        /* [in] */ IBinder* token,
        /* [in] */ const String& who,
        /* [in] */ const String& what);

private:
    CARAPI AddView(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* params,
        /* [in] */ Boolean nest);

    CARAPI_(IView*) RemoveViewLocked(
        /* [in] */ Int32 index);


    CARAPI_(void) FinishRemoveViewLocked(
        /* [in] */ IView* view,
        /* [in] */ Int32 index);

    CARAPI_(Int32) FindViewLocked(
        /* [in] */ IView* view,
        /* [in] */ Boolean required);

private:
    Vector<AutoPtr<IView> > mViews;
    Vector<AutoPtr<ViewRoot> > mRoots;
    Vector<AutoPtr<IWindowManagerLayoutParams> > mParams;
};

#endif //__CWINDOWMANAGERIMPL_H__
