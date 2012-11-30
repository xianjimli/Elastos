
#ifndef  _ListMenuItemView_H__
#define  _ListMenuItemView_H__

#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <widget/LinearLayout.h>
#include "view/menu/MenuItemImpl.h"


class ListMenuItemView:
    public ElRefBase,
    public LinearLayout
{
public:
    ListMenuItemView();

    ~ListMenuItemView();

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

#endif  //_ListMenuItemView_H__
