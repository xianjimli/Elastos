
#ifndef __CALERTCONTROLLERRECYCLELISTVIEW_H__
#define __CALERTCONTROLLERRECYCLELISTVIEW_H__

#include "ext/frameworkext.h"
#include "_CAlertControllerRecycleListView.h"
#include "view/ViewMacro.h"
#include "widget/ListView.h"
#include "widget/AdapterViewMacro.h"
#include "widget/AbsListViewMacro.h"
#include "widget/ListViewMacro.h"

/**
 * Used by dialogs to change the font size and number of lines to try to fit
 * the text to the available space.
 */
CarClass(CAlertControllerRecycleListView), public ListView
{
public:
    CAlertControllerRecycleListView();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IVIEW_METHODS_DECL();

    IVIEWGROUP_METHODS_DECL();

    IVIEWPARENT_METHODS_DECL();

    IVIEWMANAGER_METHODS_DECL();

    IADAPTERVIEW_METHODS_DECL();

    IABSLISTVIEW_METHODS_DECL();

    ILISTVIEW_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    CARAPI OnTouchModeChanged(
        /* [in] */ Boolean isInTouchMode);

    CARAPI OnGlobalLayout();

    CARAPI BeforeTextChanged(
        /* [in] */ ICharSequence* s,
        /* [in] */ Int32 start,
        /* [in] */ Int32 count,
        /* [in] */ Int32 after);

    CARAPI OnTextChanged(
        /* [in] */ ICharSequence* s,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 count);

    CARAPI AfterTextChanged(
        /* [in] */ IEditable* s);

    CARAPI OnFilterComplete(
        /* [in] */ Int32 count);

protected:
    //@Override
    CARAPI_(Boolean) RecycleOnMeasure();

public:
    Boolean mRecycleOnMeasure;
};

#endif //__CALERTCONTROLLERRECYCLELISTVIEW_H__
