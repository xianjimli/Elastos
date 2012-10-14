
#ifndef __CEXPANDEDMENUVIEW_H__
#define __CEXPANDEDMENUVIEW_H__

#include "ext/frameworkext.h"
#include "widget/ListView.h"
#include <elastos/ElRefBase.h>

class CExpandedMenuView:
     public ElRefBase
    ,public ListView {
private:
    AutoPtr<IMenuBuilder> mMenu;

    /** Default animations for this menu */
    Int32 mAnimations;

public:
    /**
     * Instantiates the ExpandedMenuView that is linked with the provided MenuBuilder.
     * @param menu The model for the menu which this MenuView will display
     */
    CExpandedMenuView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Initialize(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Int32 menuType);

    CARAPI UpdateChildren(
        /* [in] */ Boolean cleared);

    void OnDetachedFromWindow();

    CARAPI RecycleOnMeasure(
        /* [out] */ Boolean* recycled);

    CARAPI InvokeItem(
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* state);

    CARAPI OnItemClick(
        /* [in] */ IAdapterView* parent,
        /* [in] */ IView* v,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    CARAPI GetWindowAnimations(
        /* [out] */ Int32* animations);

};

#endif //__CEXPANDEDMENUVIEW_H__
