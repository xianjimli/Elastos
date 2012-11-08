
#ifndef __ICONMENIITEMVIEW_H__
#define __ICONMENIITEMVIEW_H__


#include "ext/frameworkext.h"
#include "view/menu/MenuItemImpl.h"
#include "widget/TextView.h"
#include <elastos/AutoPtr.h>


/**
 * The item view for each item in the {@link IconMenuView}.
 */
class IconMenuItemView : public TextView
{
public:
    IconMenuItemView();

    /**
     * Initializes with the provided title and icon
     * @param title The title of this item
     * @param icon The icon of this item
     */
    CARAPI_(void) Initialize(
        /* [in] */ ICharSequence* title,
        /* [in] */ IDrawable* icon);

    CARAPI Initialize(
        /* [in] */ IMenuItemImpl* itemData,
        /* [in] */ Int32 menuType);

    //@override
    CARAPI_(Boolean) PerformClick();

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI_(void) SetCaptionMode(
        /* [in] */ Boolean shortcut);

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI SetItemInvoker(
        /* [in] */ IMenuBuilderItemInvoker* itemInvoker);

    //@override
    CARAPI SetVisibility(
        /* [in] */ Int32 v);

    CARAPI_(AutoPtr<IMenuItemImpl>) GetItemData();

    CARAPI_(void) SetIconMenuView(
        /* [in] */ IIconMenuView* iconMenuView);

    /**
     * @return layout params appropriate for this view.  If layout params already exist, it will
     *         augment them to be appropriate to the current text size.
     */
    CARAPI_(AutoPtr<IIconMenuViewLayoutParams>) GetTextAppropriateLayoutParams();

    CARAPI SetCheckable(
        /* [in] */ Boolean checkable);

    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    CARAPI SetShortcut(
        /* [in] */ Boolean showShortcut,
        /* [in] */ Char32 shortcutKey);

    CARAPI_(Boolean) PrefersCondensedTitle();

    CARAPI_(Boolean) ShowsIcon();

protected:
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle = 0);

    //@Override
    CARAPI DrawableStateChanged();

    //@Override
    CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    //@Override
    CARAPI_(void) OnTextChanged(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 after);

private:
    /**
     * Positions the icon vertically (horizontal centering is taken care of by
     * the TextView's gravity).
     */
    CARAPI_(void) PositionIcon();

private:
    static const Int32 NO_ALPHA = 0xFF;
    static String sPrependShortcutLabel;

    AutoPtr<IIconMenuView> mIconMenuView;

    AutoPtr<IMenuBuilderItemInvoker> mItemInvoker;
    AutoPtr<MenuItemImpl> mItemData;

    AutoPtr<IDrawable> mIcon;

    Int32 mTextAppearance;
    AutoPtr<IContext> mTextAppearanceContext;

    Float mDisabledAlpha;

    AutoPtr<IRect> mPositionIconAvailable;
    AutoPtr<IRect> mPositionIconOutput;

    Boolean mShortcutCaptionMode;
    String mShortcutCaption;
};

#endif //__ICONMENIITEMVIEW_H__
