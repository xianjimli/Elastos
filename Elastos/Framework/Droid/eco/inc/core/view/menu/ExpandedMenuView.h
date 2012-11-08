
#ifndef __EXPANDEDMENUVIEW_H__
#define __EXPANDEDMENUVIEW_H__

#include "ext/frameworkext.h"
#include "widget/ListView.h"
#include <elastos/AutoPtr.h>

/**
 * The expanded menu view is a list-like menu with all of the available menu items.  It is opened
 * by the user clicking no the 'More' button on the icon menu view.
 */
class ExpandedMenuView : public ListView
{
public:
    ExpandedMenuView();

    CARAPI Initialize(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Int32 menuType);

    CARAPI UpdateChildren(
        /* [in] */ Boolean cleared);

    CARAPI_(Boolean) InvokeItem(
        /* [in] */ IMenuItemImpl* item);

    CARAPI OnItemClick(
        /* [in] */ IAdapterView* parent,
        /* [in] */ IView* v,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    CARAPI_(Int32) GetWindowAnimations();

protected:
    /**
     * Instantiates the ExpandedMenuView that is linked with the provided MenuBuilder.
     * @param menu The model for the menu which this MenuView will display
     */
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    //@override
    CARAPI_(void) OnDetachedFromWindow();

    //@override
    CARAPI_(Boolean) RecycleOnMeasure();

private:
    AutoPtr<IMenuBuilder> mMenu;

    /** Default animations for this menu */
    Int32 mAnimations;

};

#endif //__EXPANDEDMENUVIEW_H__
