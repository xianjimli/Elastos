
#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;

class Drawable
{
public:
    Drawable();

    virtual ~Drawable();

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    /**
     * Draw in its bounds (set via setBounds) respecting optional effects such
     * as alpha (set via setAlpha) and color filter (set via setColorFilter).
     *
     * @param canvas The canvas to draw into
     */
    virtual CARAPI Draw(
        /* [in] */ ICanvas* canvas) = 0;

    /**
     * Specify a bounding rectangle for the Drawable. This is where the drawable
     * will draw when its draw() method is called.
     */
    virtual CARAPI SetBounds(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    /**
     * Specify a bounding rectangle for the Drawable. This is where the drawable
     * will draw when its draw() method is called.
     */
    virtual CARAPI SetBounds(
        /* [in] */ IRect* bounds);

    /**
     * Return a copy of the drawable's bounds in the specified Rect (allocated
     * by the caller). The bounds specify where this will draw when its draw()
     * method is called.
     *
     * @param bounds Rect to receive the drawable's bounds (allocated by the
     *               caller).
     */
    CARAPI_(void) CopyBounds(
        /* [in] */ IRect* bounds);

    /**
     * Return a copy of the drawable's bounds in a new Rect. This returns the
     * same values as getBounds(), but the returned object is guaranteed to not
     * be changed later by the drawable (i.e. it retains no reference to this
     * rect). If the caller already has a Rect allocated, call copyBounds(rect).
     *
     * @return A copy of the drawable's bounds
     */
    CARAPI_(AutoPtr<IRect>) CopyBounds();

    /**
     * Return the drawable's bounds Rect. Note: for efficiency, the returned
     * object may be the same object stored in the drawable (though this is not
     * guaranteed), so if a persistent copy of the bounds is needed, call
     * copyBounds(rect) instead.
     * You should also not change the object returned by this method as it may
     * be the same object stored in the drawable.
     *
     * @return The bounds of the drawable (which may change later, so caller
     *         beware). DO NOT ALTER the returned object as it may change the
     *         stored bounds of this drawable.
     *
     * @see #copyBounds()
     * @see #copyBounds(android.graphics.Rect)
     */
    CARAPI_(AutoPtr<IRect>) GetBounds();

    /**
     * Set a mask of the configuration parameters for which this drawable
     * may change, requiring that it be re-created.
     *
     * @param configs A mask of the changing configuration parameters, as
     * defined by {@link android.content.res.Configuration}.
     *
     * @see android.content.res.Configuration
     */
    virtual CARAPI SetChangingConfigurations(
        /* [in] */ Int32 configs);

    /**
     * Return a mask of the configuration parameters for which this drawable
     * may change, requiring that it be re-created.  The default implementation
     * returns whatever was provided through
     * {@link #setChangingConfigurations(int)} or 0 by default.  Subclasses
     * may extend this to or in the changing configurations of any other
     * drawables they hold.
     *
     * @return Returns a mask of the changing configuration parameters, as
     * defined by {@link android.content.res.Configuration}.
     *
     * @see android.content.res.Configuration
     */
   virtual CARAPI_(Int32) GetChangingConfigurations();

    /**
     * Set to true to have the drawable dither its colors when drawn to a device
     * with fewer than 8-bits per color component. This can improve the look on
     * those devices, but can also slow down the drawing a little.
     */
    virtual CARAPI SetDither(
        /* [in] */ Boolean dither);

    /**
     * Set to true to have the drawable filter its bitmap when scaled or rotated
     * (for drawables that use bitmaps). If the drawable does not use bitmaps,
     * this call is ignored. This can improve the look when scaled or rotated,
     * but also slows down the drawing.
     */
    virtual CARAPI SetFilterBitmap(
        /* [in] */ Boolean filter);

    /**
     * Bind a {@link Callback} object to this Drawable.  Required for clients
     * that want to support animated drawables.
     *
     * @param cb The client's Callback implementation.
     */
    CARAPI SetCallback(
        /* [in] */ IDrawableCallback* cb);

    /**
     * Use the current {@link Callback} implementation to have this Drawable
     * redrawn.  Does nothing if there is no Callback attached to the
     * Drawable.
     *
     * @see Callback#invalidateDrawable
     */
    virtual CARAPI InvalidateSelf();

    /**
     * Use the current {@link Callback} implementation to have this Drawable
     * scheduled.  Does nothing if there is no Callback attached to the
     * Drawable.
     *
     * @param what The action being scheduled.
     * @param when The time (in milliseconds) to run.
     *
     * @see Callback#scheduleDrawable
     */
    virtual CARAPI ScheduleSelf(
        /* [in] */ IRunnable* what,
        /* [in] */ Int64 when);

    /**
     * Use the current {@link Callback} implementation to have this Drawable
     * unscheduled.  Does nothing if there is no Callback attached to the
     * Drawable.
     *
     * @param what The runnable that you no longer want called.
     *
     * @see Callback#unscheduleDrawable
     */
    virtual CARAPI UnscheduleSelf(
        /* [in] */ IRunnable* what);

    /**
     * Specify an alpha value for the drawable. 0 means fully transparent, and
     * 255 means fully opaque.
     */
    virtual CARAPI SetAlpha(
        /* [in] */ Int32 alpha) = 0;

    /**
     * Specify an optional colorFilter for the drawable. Pass null to remove
     * any filters.
    */
    virtual CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf) = 0;

    /**
     * Specify a color and porterduff mode to be the colorfilter for this
     * drawable.
     */
    virtual CARAPI SetColorFilter(
        /* [in] */ Int32 color,
        /* [in] */ PorterDuffMode mode);

    virtual CARAPI ClearColorFilter();

    /**
     * Indicates whether this view will change its appearance based on state.
     * Clients can use this to determine whether it is necessary to calculate
     * their state and call setState.
     *
     * @return True if this view changes its appearance based on state, false
     *         otherwise.
     *
     * @see #setState(int[])
     */
    virtual CARAPI_(Boolean) IsStateful();

    /**
     * Specify a set of states for the drawable. These are use-case specific,
     * so see the relevant documentation. As an example, the background for
     * widgets like Button understand the following states:
     * [{@link android.R.attr#state_focused},
     *  {@link android.R.attr#state_pressed}].
     *
     * <p>If the new state you are supplying causes the appearance of the
     * Drawable to change, then it is responsible for calling
     * {@link #invalidateSelf} in order to have itself redrawn, <em>and</em>
     * true will be returned from this function.
     *
     * <p>Note: The Drawable holds a reference on to <var>stateSet</var>
     * until a new state array is given to it, so you must not modify this
     * array during that time.</p>
     *
     * @param stateSet The new set of states to be displayed.
     *
     * @return Returns true if this change in state has caused the appearance
     * of the Drawable to change (hence requiring an invalidate), otherwise
     * returns false.
     */
    virtual CARAPI_(Boolean) SetState(
        /* [in] */ const ArrayOf<Int32>* stateSet);

    /**
     * Describes the current state, as a union of primitve states, such as
     * {@link android.R.attr#state_focused},
     * {@link android.R.attr#state_selected}, etc.
     * Some drawables may modify their imagery based on the selected state.
     * @return An array of resource Ids describing the current state.
     */
    virtual CARAPI_(ArrayOf<Int32>*) GetState();

    /**
     * @return The current drawable that will be used by this drawable. For simple drawables, this
     *         is just the drawable itself. For drawables that change state like
     *         {@link StateListDrawable} and {@link LevelListDrawable} this will be the child drawable
     *         currently in use.
     */
    virtual CARAPI_(AutoPtr<IDrawable>) GetCurrent();

    /**
     * Specify the level for the drawable.  This allows a drawable to vary its
     * imagery based on a continuous controller, for example to show progress
     * or volume level.
     *
     * <p>If the new level you are supplying causes the appearance of the
     * Drawable to change, then it is responsible for calling
     * {@link #invalidateSelf} in order to have itself redrawn, <em>and</em>
     * true will be returned from this function.
     *
     * @param level The new level, from 0 (minimum) to 10000 (maximum).
     *
     * @return Returns true if this change in level has caused the appearance
     * of the Drawable to change (hence requiring an invalidate), otherwise
     * returns false.
     */
    CARAPI_(Boolean) SetLevel(
        /* [in] */ Int32 level);

    /**
     * Retrieve the current level.
     *
     * @return int Current level, from 0 (minimum) to 10000 (maximum).
     */
    CARAPI_(Int32) GetLevel();

    /**
     * Set whether this Drawable is visible.  This generally does not impact
     * the Drawable's behavior, but is a hint that can be used by some
     * Drawables, for example, to decide whether run animations.
     *
     * @param visible Set to true if visible, false if not.
     * @param restart You can supply true here to force the drawable to behave
     *                as if it has just become visible, even if it had last
     *                been set visible.  Used for example to force animations
     *                to restart.
     *
     * @return boolean Returns true if the new visibility is different than
     *         its previous state.
     */
    virtual CARAPI_(Boolean) SetVisible(
        /* [in] */ Boolean visible,
        /* [in] */ Boolean restart);

    CARAPI_(Boolean) IsVisible();

    /**
     * Return the opacity/transparency of this Drawable.  The returned value is
     * one of the abstract format constants in
     * {@link android.graphics.PixelFormat}:
     * {@link android.graphics.PixelFormat#UNKNOWN},
     * {@link android.graphics.PixelFormat#TRANSLUCENT},
     * {@link android.graphics.PixelFormat#TRANSPARENT}, or
     * {@link android.graphics.PixelFormat#OPAQUE}.
     *
     * <p>Generally a Drawable should be as conservative as possible with the
     * value it returns.  For example, if it contains multiple child drawables
     * and only shows one of them at a time, if only one of the children is
     * TRANSLUCENT and the others are OPAQUE then TRANSLUCENT should be
     * returned.  You can use the method {@link #resolveOpacity} to perform a
     * standard reduction of two opacities to the appropriate single output.
     *
     * <p>Note that the returned value does <em>not</em> take into account a
     * custom alpha or color filter that has been applied by the client through
     * the {@link #setAlpha} or {@link #setColorFilter} methods.
     *
     * @return int The opacity class of the Drawable.
     *
     * @see android.graphics.PixelFormat
     */
    virtual CARAPI_(Int32) GetOpacity() = 0;

    /**
     * Return the appropriate opacity value for two source opacities.  If
     * either is UNKNOWN, that is returned; else, if either is TRANSLUCENT,
     * that is returned; else, if either is TRANSPARENT, that is returned;
     * else, OPAQUE is returned.
     *
     * <p>This is to help in implementing {@link #getOpacity}.
     *
     * @param op1 One opacity value.
     * @param op2 Another opacity value.
     *
     * @return int The combined opacity value.
     *
     * @see #getOpacity
     */
    static CARAPI_(Int32) ResolveOpacity(
        /* [in] */ Int32 op1,
        /* [in] */ Int32 op2);

    /**
     * Returns a Region representing the part of the Drawable that is completely
     * transparent.  This can be used to perform drawing operations, identifying
     * which parts of the target will not change when rendering the Drawable.
     * The default implementation returns null, indicating no transparent
     * region; subclasses can optionally override this to return an actual
     * Region if they want to supply this optimization information, but it is
     * not required that they do so.
     *
     * @return Returns null if the Drawables has no transparent region to
     * report, else a Region holding the parts of the Drawable's bounds that
     * are transparent.
     */
    virtual CARAPI_(AutoPtr<IRegion>) GetTransparentRegion();

    /**
     * Return the intrinsic width of the underlying drawable object.  Returns
     * -1 if it has no intrinsic width, such as with a solid color.
     */
    virtual CARAPI_(Int32) GetIntrinsicWidth();

    /**
     * Return the intrinsic height of the underlying drawable object. Returns
     * -1 if it has no intrinsic height, such as with a solid color.
     */
    virtual CARAPI_(Int32) GetIntrinsicHeight();

    /**
     * Returns the minimum width suggested by this Drawable. If a View uses this
     * Drawable as a background, it is suggested that the View use at least this
     * value for its width. (There will be some scenarios where this will not be
     * possible.) This value should INCLUDE any padding.
     *
     * @return The minimum width suggested by this Drawable. If this Drawable
     *         doesn't have a suggested minimum width, 0 is returned.
     */
    virtual CARAPI_(Int32) GetMinimumWidth();

    /**
     * Returns the minimum height suggested by this Drawable. If a View uses this
     * Drawable as a background, it is suggested that the View use at least this
     * value for its height. (There will be some scenarios where this will not be
     * possible.) This value should INCLUDE any padding.
     *
     * @return The minimum height suggested by this Drawable. If this Drawable
     *         doesn't have a suggested minimum height, 0 is returned.
     */
    virtual CARAPI_(Int32) GetMinimumHeight();

    /**
     * Return in padding the insets suggested by this Drawable for placing
     * content inside the drawable's bounds. Positive values move toward the
     * center of the Drawable (set Rect.inset). Returns true if this drawable
     * actually has a padding, else false. When false is returned, the padding
     * is always set to 0.
     */
    virtual CARAPI_(Boolean) GetPadding(
        /* [in, out] */ IRect* padding);

    /**
     * Make this drawable mutable. This operation cannot be reversed. A mutable
     * drawable is guaranteed to not share its state with any other drawable.
     * This is especially useful when you need to modify properties of drawables
     * loaded from resources. By default, all drawables instances loaded from
     * the same resource share a common state; if you modify the state of one
     * instance, all the other instances will receive the same modification.
     *
     * Calling this method on a mutable Drawable will have no effect.
     *
     * @return This drawable.
     */
    virtual CARAPI_(AutoPtr<IDrawable>) Mutate();

    /**
     * Create a drawable from an inputstream
     */
    static CARAPI CreateFromStream(
        /* [in] */ IInputStream* is,
        /* [in] */ String srcName,
        /* [out] */ IDrawable** drawable);

    /**
     * Create a drawable from an inputstream, using the given resources and
     * value to determine density information.
     */
    static CARAPI CreateFromResourceStream(
        /* [in] */ IResources* res,
        /* [in] */ ITypedValue* value,
        /* [in] */ IInputStream* is,
        /* [in] */ String srcName,
        /* [out] */ IDrawable** drawable);

    /**
     * Create a drawable from an inputstream, using the given resources and
     * value to determine density information.
     */
    static CARAPI CreateFromResourceStream(
        /* [in] */ IResources* res,
        /* [in] */ ITypedValue* value,
        /* [in] */ IInputStream* is,
        /* [in] */ String srcName,
        /* [in] */ IBitmapFactoryOptions* opts,
        /* [out] */ IDrawable** drawable);

    /**
     * Create a drawable from an XML document. For more information on how to
     * create resources in XML, see
     * <a href="{@docRoot}guide/topics/resources/drawable-resource.html">Drawable Resources</a>.
     */
    static CARAPI CreateFromXml(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [out] */ IDrawable** drawable);

    /**
     * Create from inside an XML document.  Called on a parser positioned at
     * a tag in an XML document, tries to create a Drawable from that tag.
     * Returns null if the tag is not a valid drawable.
     */
    static CARAPI CreateFromXmlInner(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IDrawable** drawable);

    /**
     * Create a drawable from file path name.
     */
    static CARAPI CreateFromPath(
        /* [in] */ String pathName,
        /* [out] */ IDrawable** drawable);

    virtual CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

    /* package */ virtual CARAPI InflateWithAttributes(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ ITypedArray* attrs,
        /* [in] */ Int32 visibleAttr);

    virtual CARAPI_(AutoPtr<IDrawableConstantState>) GetConstantState();

protected:
    /**
     * Override this in your subclass to change appearance if you recognize the
     * specified state.
     *
     * @return Returns true if the state change has caused the appearance of
     * the Drawable to change (that is, it needs to be drawn), else false
     * if it looks the same and there is no need to redraw it since its
     * last state.
     */
    virtual CARAPI_(Boolean) OnStateChange(
        /* [in] */ const ArrayOf<Int32>* state);

    /** Override this in your subclass to change appearance if you vary based
     *  on level.
     * @return Returns true if the level change has caused the appearance of
     * the Drawable to change (that is, it needs to be drawn), else false
     * if it looks the same and there is no need to redraw it since its
     * last level.
     */
    virtual CARAPI_(Boolean) OnLevelChange(
        /* [in] */ Int32 level);

    /**
     * Override this in your subclass to change appearance if you recognize the
     * specified state.
     */
    virtual CARAPI_(void) OnBoundsChange(
        /* [in] */ IRect* bounds);

private:
    static CARAPI DrawableFromBitmap(
        /* [in] */ IResources* res,
        /* [in] */ IBitmap* bm,
        /* [in] */ ArrayOf<Byte>* np,
        /* [in] */ IRect* pad,
        /* [in] */ String srcName,
        /* [out] */ IDrawable** drawable);

protected:
        /*package*/ AutoPtr<IDrawableCallback> mCallback;

private:
    ArrayOf<Int32>* mStateSet;
    Int32 mLevel;
    Int32 mChangingConfigurations;
    AutoPtr<IRect> mBounds;
    Boolean mVisible;
};

#endif //__DRAWABLE_H__

