
#ifndef _BASEMENUITEMVIEW_H__
#define _BASEMENUITEMVIEW_H__

#include "view/ViewMacro.h"
#include <elastos/ElRefBase.h>


class BaseMenuItemView: public ElRefBase, public IView, public IMenuItemView {
public:
	IVIEW_METHODS_DECL();

	BaseMenuItemView() {}

    CARAPI_(UInt32) AddRef() {
        return ElRefBase::AddRef();
    }

    CARAPI_(UInt32) Release() {
        return ElRefBase::Release();
    }

    CARAPI Initialize(
        /* [in] */ IMenuItem* itemData,
        /* [in] */ Int32 menuType) = 0;

    /**
     * Initializes with the provided title and icon
     * @param title The title of this item
     * @param icon The icon of this item
     */
    CARAPI InitializeEx(
        /* [in] */ ICharSequence* title,
        /* [in] */ IDrawable* icon) = 0;

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title) = 0;

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI GetItemData(
        /* [out] */ IMenuItem** itemData) = 0;

    CARAPI SetCheckable(
        /* [in] */ Boolean checkable) = 0;

    CARAPI SetChecked(
        /* [in] */ Boolean checked) = 0;

    CARAPI SetShortcut(
        /* [in] */ Boolean showShortcut,
        /* [in] */ Char16 shortcutKey) = 0;

    CARAPI PrefersCondensedTitle(
        /* [out] */ Boolean* flag) = 0;

    CARAPI ShowsIcon(
        /* [out] */ Boolean* flag) = 0;

};





















#endif  //_BASEMENUITEMVIEW_H__