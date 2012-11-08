
#ifndef __CEXPANDEDMENUVIEW_H__
#define __CEXPANDEDMENUVIEW_H__

#include "ext/frameworkext.h"
#include "view/ViewMacro.h"
#include "widget/AdapterViewMacro.h"
#include "widget/AbsListViewMacro.h"
#include "widget/ListViewMacro.h"
#include "_CExpandedMenuView.h"
#include "view/menu/ExpandedMenuView.h"


CarClass(CExpandedMenuView), public ExpandedMenuView
{
public:
    /**
     * Instantiates the ExpandedMenuView that is linked with the provided MenuBuilder.
     * @param menu The model for the menu which this MenuView will display
     */
    constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

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

    //from IMenuBuilderItemInvoker
    CARAPI InvokeItem(
        /* [in] */ IMenuItemImpl* item,
        /* [out] */ Boolean* state);

    //from IMenuView
    CARAPI Initialize(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Int32 menuType);

    CARAPI UpdateChildren(
        /* [in] */ Boolean cleared);

    CARAPI GetWindowAnimations(
        /* [out] */ Int32* animations);

    //from IOnItemClickListener
    CARAPI OnItemClick(
        /* [in] */ IAdapterView* parent,
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

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
};

#endif //__CEXPANDEDMENUVIEW_H__
