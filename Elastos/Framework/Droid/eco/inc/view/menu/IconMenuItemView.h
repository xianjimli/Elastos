
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
        /* [in] */ int defStyle);

    IconMenuItemView(
        /* [in] */ IContextEx* context,
        /* [in] */ AttributeSet attrs);

    CARAPI_(void) Initialize(
        /* [in] */ MenuItemImpl itemData,
        /* [in] */ int menuType);

    //@Override
    CARAPI_(Boolean) PerformClick();

    CARAPI_(void) SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI_(void) SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI_(void) SetItemInvoker(
        /* [in] */ ItemInvoker itemInvoker);

    //@ViewDebug.CapturedViewProperty(retrieveReturn = true)
    CARAPI getItemData(
        /* [out] */ MenuItemImpl** itemData);

    //@Override
    CARAPI_(void) SetVisibility(
        /* [in] */ int v);

    CARAPI_(void) SetIconMenuView(
        /* [in] */ IconMenuView* iconMenuView);

    CARAPI_(void) SetCheckable(
        /* [in] */ Boolean checkable);

    CARAPI_(void) SetChecked(
        /* [in] */ Boolean checked);

    CARAPI_(void) SetShortcut(
        /* [in] */ Boolean showShortcut,
        /* [in] */ char shortcutKey);

    CARAPI_(Boolean) PrefersCondensedTitle();

    CARAPI_(Boolean) ShowsIcon();

protected:
    /**
     * Initializes with the provided title and icon
     * @param title The title of this item
     * @param icon The icon of this item
     */
    CARAPI_(void) Initialize(
        /* [in] */ ICharSequence* title,
        /* [in] */ IDrawable* icon)£»

    CARAPI_(void) SetCaptionMode(
        /* [in] */ Boolean shortcut);

    //@Override
    CARAPI_(void) DrawableStateChanged();

    //@Override
    CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ int left,
        /* [in] */ int top,
        /* [in] */ int right,
        /* [in] */ int bottom) {
        super.onLayout(changed, left, top, right, bottom);

        positionIcon();
    }

    //@Override
    void OnTextChanged(
        /* [in] */ ICharSequence* text,
        /* [in] */ int start,
        /* [in] */ int before,
        /* [in] */ int after);

    /**
     * @return layout params appropriate for this view.  If layout params already exist, it will
     *         augment them to be appropriate to the current text size.
     */
    IconMenuView.LayoutParams getTextAppropriateLayoutParams() {
        IconMenuView.LayoutParams lp = (IconMenuView.LayoutParams) getLayoutParams();
        if (lp == null) {
            // Default layout parameters
            lp = new IconMenuView.LayoutParams(
                    IconMenuView.LayoutParams.MATCH_PARENT, IconMenuView.LayoutParams.MATCH_PARENT);
        }

        // Set the desired width of item
        lp.desiredWidth = (int) Layout.getDesiredWidth(getText(), getPaint());

        return lp;
    }

private:
    /**
     * Positions the icon vertically (horizontal centering is taken care of by
     * the TextView's gravity).
     */
    void positionIcon() {

        if (mIcon == null) {
            return;
        }

        // We reuse the output rectangle as a temp rect
        Rect tmpRect = mPositionIconOutput;
        getLineBounds(0, tmpRect);
        mPositionIconAvailable.set(0, 0, getWidth(), tmpRect.top);
        Gravity.apply(Gravity.CENTER_VERTICAL | Gravity.LEFT, mIcon.getIntrinsicWidth(), mIcon
                .getIntrinsicHeight(), mPositionIconAvailable, mPositionIconOutput);
        mIcon.setBounds(mPositionIconOutput);
    }


private:
    static final int NO_ALPHA = 0xFF;

    IconMenuView mIconMenuView;

    ItemInvoker mItemInvoker;
    MenuItemImpl mItemData;

    Drawable mIcon;

    int mTextAppearance;
    Context mTextAppearanceContext;

    float mDisabledAlpha;

    Rect mPositionIconAvailable = new Rect();
    Rect mPositionIconOutput = new Rect();

    boolean mShortcutCaptionMode;
    String mShortcutCaption;

    static String sPrependShortcutLabel;

}

#endif //__ICONMENIITEMVIEW_H__
