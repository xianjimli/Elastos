
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
