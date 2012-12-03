
#ifndef  _ListMenuItemView_H__
#define  _ListMenuItemView_H__

#include "view/menu/MenuItemImpl.h"
#include "widget/LinearLayout.h"
#include <elastos/AutoPtr.h>

class ListMenuItemView : public LinearLayout
{
public:
    ListMenuItemView();

    ListMenuItemView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle = 0);

    virtual CARAPI Initialize(
        /* [in] */ IMenuItemImpl* itemData,
        /* [in] */ Int32 menuType);

    virtual CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    virtual CARAPI_(AutoPtr<IMenuItemImpl>) GetItemData();

    virtual CARAPI SetCheckable(
        /* [in] */ Boolean checkable);

    virtual CARAPI SetChecked(
        /* [in] */ Boolean checked);

    virtual CARAPI SetShortcut(
        /* [in] */ Boolean showShortcut,
        /* [in] */ Char32 shortcutKey);

    virtual CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    virtual CARAPI_(Boolean) PrefersCondensedTitle();

    virtual CARAPI_(Boolean) ShowsIcon();

protected:
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle = 0);

    //Override
    CARAPI OnFinishInflate();

private:
    CARAPI_(void) InsertIconView();

    CARAPI_(void) InsertRadioButton();

    CARAPI_(void) InsertCheckBox();

    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

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

#endif  //_ListMenuItemView_H__
