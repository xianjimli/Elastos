
#ifndef _CLISTMENUITEMVIEW_H__
#define _CLISTMENUITEMVIEW_H__

#include "_CListMenuItemView.h"
#include "view/ViewMacro.h"
#include "view/menu/MenuItemImpl.h"
#include <widget/LinearLayout.h>
#include <elastos/AutoPtr.h>


class _ListMenuItemView:
    public ElRefBase,
    public LinearLayout
{
public:
    _ListMenuItemView();

    ~_ListMenuItemView();

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

protected:
    virtual CARAPI OnFinishInflate();

protected:
    AutoPtr<MenuItemImpl> mItemData;

    AutoPtr<IImageView> mIconView;
    AutoPtr<IRadioButton> mRadioButton;
    AutoPtr<ITextView> mTitleView;
    AutoPtr<ICheckBox> mCheckBox;
    AutoPtr<ITextView> mShortcutView;

    AutoPtr<IDrawable> mBackground;
    Int32 mTextAppearance;
    AutoPtr<IContext> mTextAppearanceContext;

    Int32 mMenuType;
};

CarClass(CListMenuItemView), public _ListMenuItemView
{
public:
    IVIEW_METHODS_DECL();

    IVIEWGROUP_METHODS_DECL();

    IVIEWPARENT_METHODS_DECL();

    IVIEWMANAGER_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    CListMenuItemView();

    ECode constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    ECode constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(PInterface) Probe(
    /* [in] */ REIID riid);

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Initialize(
        /* [in] */ IMenuItemImpl* itemData,
        /* [in] */ Int32 menuType);

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI_(AutoPtr<IMenuItemImpl>) GetItemData();

    CARAPI SetCheckable(
        /* [in] */ Boolean checkable);

    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    CARAPI SetShortcut(
        /* [in] */ Boolean showShortcut,
        /* [in] */ Char32 shortcutKey);

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI PrefersCondensedTitle(
        /* [out] */ Boolean* prefer);

    CARAPI ShowsIcon(
        /* [out] */ Boolean* show);

    CARAPI IsBaselineAligned(
        /* [out] */  Boolean* baselineAligned);

    CARAPI SetBaselineAligned(
        /* [in] */ Boolean baselineAligned);

    CARAPI GetBaselineAlignedChildIndex(
        /* [out] */ Int32 * pBaselineAlignedChildIndex);

    CARAPI SetBaselineAlignedChildIndex(
        /* [in] */ Int32 baselineAlignedChildIndex);

    CARAPI GetWeightSum(
        /* [out] */ Float * pWeightSum);

    CARAPI SetWeightSum(
        /* [in] */ Float weightSum);

    CARAPI SetOrientation(
        /* [in] */ Int32 orientation);

    CARAPI GetOrientation(
        /* [out] */ Int32* orientation);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI SetHorizontalGravity(
        /* [in] */ Int32 horizontalGravity);

    CARAPI SetVerticalGravity(
        /* [in] */ Int32 verticalGravity);

    CARAPI GetItemData(
        /* [out] */ IMenuItemImpl** itemData);

    CARAPI SetEnabledEx(
        /* [in] */ Boolean enabled);

private:
    CARAPI_(void) InsertIconView();

    CARAPI_(void) InsertRadioButton();

    CARAPI_(void) InsertCheckBox();
};


#endif   //_CLISTMENUITEMVIEW_H__
