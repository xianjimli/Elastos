
#ifndef __ICONMENIITEMVIEW_H__
#define __ICONMENIITEMVIEW_H__


/**
 * The item view for each item in the {@link IconMenuView}.
 */
class IconMenuItemView: public TextView {
public:
    IconMenuItemView(
        /* [in] */ IContextEx* context,
        /* [in] */ AttributeSet attrs,
        /* [in] */ Int32 defStyle);

    IconMenuItemView(
        /* [in] */ IContextEx* context,
        /* [in] */ AttributeSet attrs);

    CARAPI Initialize(
        /* [in] */ MenuItemImpl itemData,
        /* [in] */ Int32 menuType);

    //@Override
    CARAPI PerformClick(
        /* [in] */ Boolean* flag);

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI SetItemInvoker(
        /* [in] */ ItemInvoker itemInvoker);

    //@ViewDebug.CapturedViewProperty(retrieveReturn = true)
    CARAPI GetItemData(
        /* [out] */ IMenuItem** itemData);

    //@Override
    CARAPI SetVisibility(
        /* [in] */ Int32 v);

    CARAPI SetIconMenuView(
        /* [in] */ IconMenuView* iconMenuView);

    CARAPI SetCheckable(
        /* [in] */ Boolean checkable);

    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    CARAPI SetShortcut(
        /* [in] */ Boolean showShortcut,
        /* [in] */ char shortcutKey);

    CARAPI PrefersCondensedTitle(
        /* [out] */ Boolean* flag);

    CARAPI ShowsIcon(
        /* [out] */ Boolean* flag);

protected:
    /**
     * Initializes with the provided title and icon
     * @param title The title of this item
     * @param icon The icon of this item
     */
    CARAPI Initialize(
        /* [in] */ ICharSequence* title,
        /* [in] */ IDrawable* icon)£»

    CARAPI SetCaptionMode(
        /* [in] */ Boolean shortcut);

    //@Override
    CARAPI DrawableStateChanged();

    //@Override
    CARAPI OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom) {
        super.onLayout(changed, left, top, right, bottom);

        positionIcon();
    }

    //@Override
    CARAPI OnTextChanged(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 after);

    /**
     * @return layout params appropriate for this view.  If layout params already exist, it will
     *         augment them to be appropriate to the current text size.
     */
    IconMenuView.LayoutParams GetTextAppropriateLayoutParams() {
        IconMenuView.LayoutParams lp = (IconMenuView.LayoutParams) getLayoutParams();
        if (lp == null) {
            // Default layout parameters
            lp = new IconMenuView.LayoutParams(
                    IconMenuView.LayoutParams.MATCH_PARENT, IconMenuView.LayoutParams.MATCH_PARENT);
        }

        // Set the desired width of item
        lp.desiredWidth = (Int32) Layout.getDesiredWidth(getText(), getPaint());

        return lp;
    }

private:
    /**
     * Positions the icon vertically (horizontal centering is taken care of by
     * the TextView's gravity).
     */
    ECode PositionIcon() {
        if (mIcon == null) {
            return NOERROR;
        }

        // We reuse the output rectangle as a temp rect
        AutoPtr<IRect> tmpRect = mPositionIconOutput;
        getLineBounds(0, tmpRect);
        mPositionIconAvailable.set(0, 0, getWidth(), tmpRect.top);
        Gravity.apply(Gravity.CENTER_VERTICAL | Gravity.LEFT, mIcon.getIntrinsicWidth(), mIcon
                .getIntrinsicHeight(), mPositionIconAvailable, mPositionIconOutput);
        mIcon.setBounds(mPositionIconOutput);
    }


private:
    static final Int32 NO_ALPHA = 0xFF;

    IconMenuView mIconMenuView;

    ItemInvoker mItemInvoker;
    MenuItemImpl mItemData;

    AutoPtr<IDrawable> mIcon;

    Int32 mTextAppearance;
    Context mTextAppearanceContext;

    float mDisabledAlpha;

    AutoPtr<IRect> mPositionIconAvailable = new Rect();
    AutoPtr<IRect> mPositionIconOutput = new Rect();

    boolean mShortcutCaptionMode;
    String mShortcutCaption;

    static String sPrependShortcutLabel;

};

#endif //__ICONMENIITEMVIEW_H__
