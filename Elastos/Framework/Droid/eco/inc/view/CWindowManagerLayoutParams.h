
#ifndef __CWINDOWMANAGERLAYOUTPARAMS_H__
#define __CWINDOWMANAGERLAYOUTPARAMS_H__

#include "_CWindowManagerLayoutParams.h"
#include "view/ViewGroupLayoutParams.h"
#include <elastos/AutoPtr.h>

CarClass(CWindowManagerLayoutParams), public ViewGroupLayoutParams
{
public:
    /**
    * Given a particular set of window manager flags, determine whether
    * such a window may be a target for an input method when it has
    * focus.  In particular, this checks the
    * {@link #FLAG_NOT_FOCUSABLE} and {@link #FLAG_ALT_FOCUSABLE_IM}
    * flags and returns true if the combination of the two corresponds
    * to a window that needs to be behind the input method so that the
    * user can type into it.
    *
    * @param flags The current window manager flags.
    *
    * @return Returns true if such a window should be behind/interact
    * with an input method, false if not.
    */
    static Boolean MayUseInputMethod(
        /* [in] */ Int32 flags);

public:
    CWindowManagerLayoutParams();

    ~CWindowManagerLayoutParams();

    CARAPI GetDescription(
        /* [out] */ String* description);

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI GetTitle(
        /* [out] */ ICharSequence** title);

    CARAPI CopyFrom(
        /* [in] */ IWindowManagerLayoutParams* source,
        /* [out] */ Int32* changes);

    CARAPI Scale(
        /* [in] */ Float scale);

    CARAPI Backup();

    CARAPI Restore();

    CARAPI GetX(
        /* [out] */ Int32* x);

    CARAPI SetX(
        /* [in] */ Int32 x);

    CARAPI GetY(
        /* [out] */ Int32* y);

    CARAPI SetY(
        /* [in] */ Int32 y);

    CARAPI GetType(
        /* [out] */ Int32* type);

    CARAPI SetType(
        /* [in] */ Int32 type);

    CARAPI GetMemoryType(
        /* [out] */ Int32* type);

    CARAPI SetMemoryType(
        /* [in] */ Int32 type);

    CARAPI GetFlags(
        /* [out] */ Int32* flags);

    CARAPI SetFlags(
        /* [in] */ Int32 flags);

    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI GetHorizontalMargin(
        /* [out] */ Float* horizontalMargin);

    CARAPI SetHorizontalMargin(
        /* [in] */ Float horizontalMargin);

    CARAPI GetVerticalMargin(
        /* [out] */ Float* verticalMargin);

    CARAPI SetVerticalMargin(
        /* [in] */ Float verticalMargin);

    CARAPI GetFormat(
        /* [out] */ Int32* format);

    CARAPI SetFormat(
        /* [in] */ Int32 format);

    CARAPI GetAlpha(
        /* [out] */ Float* alpha);

    CARAPI SetAlpha(
        /* [in] */ Float alpha);

    CARAPI GetDimAmount(
        /* [out] */ Float* dimAmount);

    CARAPI SetDimAmount(
        /* [in] */ Float dimAmount);

    CARAPI GetScreenBrightness(
        /* [out] */ Float* brightness);

    CARAPI SetScreenBrightness(
        /* [in] */ Float brightness);

    CARAPI GetButtonBrightness(
        /* [out] */ Float* brightness);

    CARAPI SetButtonBrightness(
        /* [in] */ Float brightness);

    CARAPI GetToken(
        /* [out] */ IBinder** token);

    CARAPI SetToken(
        /* [in] */ IBinder* token);

    CARAPI GetScreenOrientation(
        /* [out] */ Int32* orientation);

    CARAPI SetScreenOrientation(
        /* [in] */ Int32 orientation);

    CARAPI SetWidth(
        /* [in] */ Int32 width);

    CARAPI SetHeight(
        /* [in] */ Int32 height);

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI constructor();

public:
    /**
     * X position for this window.  With the default gravity it is ignored.
     * When using {@link Gravity#LEFT} or {@link Gravity#RIGHT} it provides
     * an offset from the given edge.
     */
    Int32 mX;

    /**
     * Y position for this window.  With the default gravity it is ignored.
     * When using {@link Gravity#TOP} or {@link Gravity#BOTTOM} it provides
     * an offset from the given edge.
     */
    Int32 mY;

    /**
     * Indicates how much of the extra space will be allocated horizontally
     * to the view associated with these LayoutParams. Specify 0 if the view
     * should not be stretched. Otherwise the extra pixels will be pro-rated
     * among all views whose weight is greater than 0.
     */
    Float mHorizontalWeight;

    /**
     * Indicates how much of the extra space will be allocated vertically
     * to the view associated with these LayoutParams. Specify 0 if the view
     * should not be stretched. Otherwise the extra pixels will be pro-rated
     * among all views whose weight is greater than 0.
     */
    Float mVerticalWeight;

    /**
     * The general type of window.  There are three main classes of
     * window types:
     * <ul>
     * <li> <strong>Application windows</strong> (ranging from
     * {@link #FIRST_APPLICATION_WINDOW} to
     * {@link #LAST_APPLICATION_WINDOW}) are normal top-level application
     * windows.  For these types of windows, the {@link #token} must be
     * set to the token of the activity they are a part of (this will
     * normally be done for you if {@link #token} is null).
     * <li> <strong>Sub-windows</strong> (ranging from
     * {@link #FIRST_SUB_WINDOW} to
     * {@link #LAST_SUB_WINDOW}) are associated with another top-level
     * window.  For these types of windows, the {@link #token} must be
     * the token of the window it is attached to.
     * <li> <strong>System windows</strong> (ranging from
     * {@link #FIRST_SYSTEM_WINDOW} to
     * {@link #LAST_SYSTEM_WINDOW}) are special types of windows for
     * use by the system for specific purposes.  They should not normally
     * be used by applications, and a special permission is required
     * to use them.
     */
    Int32 mType;

    /**
     * Specifies what type of memory buffers should be used by this window.
     * Default is normal.
     */
    Int32 mMemoryType;

    Int32 mFlags;

    /**
     * Desired operating mode for any soft input area.
     */
    Int32 mSoftInputMode;

    /**
     * Placement of window within the screen as per {@link Gravity}
     */
    Int32 mGravity;

    /**
     * The horizontal margin, as a percentage of the container's width,
     * between the container and the widget.
     */
    Float mHorizontalMargin;

    /**
     * The vertical margin, as a percentage of the container's height,
     * between the container and the widget.
     */
    Float mVerticalMargin;

    /**
     * The desired bitmap format.  May be one of the constants in
     * {@link android.graphics.PixelFormat}.  Default is OPAQUE.
     */
    Int32 mFormat;

    /**
     * A style resource defining the animations to use for this window.
     * This must be a system resource; it can not be an application resource
     * because the window manager does not have access to applications.
     */
    Int32 mWindowAnimations;

    /**
     * An alpha value to apply to this entire window.
     * An alpha of 1.0 means fully opaque and 0.0 means fully transparent
     */
    Float mAlpha;

    /**
     * When {@link #FLAG_DIM_BEHIND} is set, this is the amount of dimming
     * to apply.  Range is from 1.0 for completely opaque to 0.0 for no
     * dim.
     */
    Float mDimAmount;

    /**
     * This can be used to override the user's preferred brightness of
     * the screen.  A value of less than 0, the default, means to use the
     * preferred screen brightness.  0 to 1 adjusts the brightness from
     * dark to full bright.
     * It's real type is float.
     */
    Float mScreenBrightness;

    /**
     * This can be used to override the standard behavior of the button and
     * keyboard backlights.  A value of less than 0, the default, means to
     * use the standard backlight behavior.  0 to 1 adjusts the brightness
     * from dark to full bright.
     * It's real type is float.
     */
    Float mButtonBrightness;

    /**
     * Identifier for this window.  This will usually be filled in for
     * you.
     */
    AutoPtr<IBinder> mToken;

    /**
     * Name of the package owning this window.
     */
    String mCapsuleName;

    /**
     * Specific orientation value for a window.
     * May be any of the same values allowed
     * for {@link android.content.pm.ActivityInfo#screenOrientation}.
     * If not set, a default value of
     * {@link android.content.pm.ActivityInfo#SCREEN_ORIENTATION_UNSPECIFIED}
     * will be used.
     */
    Int32 mScreenOrientation;

private:
    // internal buffer to backup/restore parameters under compatibility mode.
    Int32* mCompatibilityParamsBackup;

    AutoPtr<ICharSequence> mTitle;
};

#endif //__CWINDOWMANAGERLAYOUTPARAMS_H__
