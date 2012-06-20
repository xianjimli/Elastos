
#ifndef __CLISTVIEW_H__
#define __CLISTVIEW_H__

#include "_CListView.h"
#include "widget/ListView.h"
#include "view/ViewMacro.h"
#include "widget/AdapterViewMacro.h"
#include "widget/AbsListViewMacro.h"
#include "widget/ListViewMacro.h"

CarClass(CListView), public ListView
{
public:
    IVIEW_METHODS_DECL();

    IVIEWGROUP_METHODS_DECL();

    IVIEWPARENT_METHODS_DECL();

    IVIEWMANAGER_METHODS_DECL();

    IADAPTERVIEW_METHODS_DECL();

    IABSLISTVIEW_METHODS_DECL();

    ILISTVIEW_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

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

    //CARAPI AfterTextChanged(
    //    /* [in] */ IEditable* s);

    CARAPI OnFilterComplete(
        /* [in] */ Int32 count);

private:
    // TODO: Add your private member variables here.
};

#endif // __CLISTVIEW_H__
