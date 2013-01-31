
#ifndef __VIEWTREEOBSERVER_H__
#define __VIEWTREEOBSERVER_H__

#ifdef _FRAMEWORK_CORE
#include "Elastos.Framework.Core_server.h"
#elif defined(_FRAMEWORK_SERVER)
#include "Elastos.Framework.Server_server.h"
#endif

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/List.h>

class ViewTreeObserver : public ElRefBase, public IViewTreeObserver
{
public:
    class InternalInsetsInfo:
        public ElRefBase,
        public IInternalInsetsInfo
    {
    public:
        /**
         * Option for {@link #setTouchableInsets(Int32)}: the entire window frame
         * can be touched.
         */
        static const Int32 TOUCHABLE_INSETS_FRAME = 0;

        /**
         * Option for {@link #setTouchableInsets(Int32)}: the area inside of
         * the content insets can be touched.
         */
        static const Int32 TOUCHABLE_INSETS_CONTENT = 1;

        /**
         * Option for {@link #setTouchableInsets(Int32)}: the area inside of
         * the visible insets can be touched.
         */
        static const Int32 TOUCHABLE_INSETS_VISIBLE = 2;

        /**
         * Option for {@link #setTouchableInsets(int)}: the area inside of
         * the provided touchable region in {@link #touchableRegion} can be tou
         */
        static const Int32 TOUCHABLE_INSETS_REGION = 3;

    public:
        InternalInsetsInfo();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        /**
         * Set which parts of the window can be touched: either
         * {@link #TOUCHABLE_INSETS_FRAME}, {@link #TOUCHABLE_INSETS_CONTENT},
         * or {@link #TOUCHABLE_INSETS_VISIBLE}.
         */
        CARAPI SetTouchableInsets(
            /* [in] */ Int32 val);

        CARAPI GetTouchableInsets(
            /* [out] */ Int32* val);

        CARAPI Equals(
            /* [in] */ IInternalInsetsInfo* o,
            /* [out] */ Boolean* equal);

        CARAPI_(void) Reset();

        CARAPI_(void) Set(
            /* [in] */ InternalInsetsInfo* other);

    public:
        /**
         * Offsets from the frame of the window at which the content of
         * windows behind it should be placed.
         */
        AutoPtr<IRect> mContentInsets;

        /**
         * Offsets from the fram of the window at which windows behind it
         * are visible.
         */
        AutoPtr<IRect> mVisibleInsets;

    private:
        Int32 mTouchableInsets;
    };

public:
    ViewTreeObserver();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    /**
     * Register a callback to be invoked when a view has been scrolled.
     *
     * @param listener The callback to add
     *
     * @throws IllegalStateException If {@link #isAlive()} returns false
     */
    CARAPI AddOnScrollChangedListener(
        /* [in] */ IOnScrollChangedListener* listener);

    /**
     * Remove a previously installed scroll-changed callback
     *
     * @param victim The callback to remove
     *
     * @throws IllegalStateException If {@link #isAlive()} returns false
     *
     * @see #addOnScrollChangedListener(OnScrollChangedListener)
     */
    CARAPI RemoveOnScrollChangedListener(
        /* [in] */ IOnScrollChangedListener* victim);

    /**
     * Register a callback to be invoked when the invoked when the touch mode changes.
     *
     * @param listener The callback to add
     *
     * @throws IllegalStateException If {@link #isAlive()} returns false
     */
    CARAPI AddOnTouchModeChangeListener(
        /* [in] */ IOnTouchModeChangeListener* listener);

    /**
     * Remove a previously installed touch mode change callback
     *
     * @param victim The callback to remove
     *
     * @throws IllegalStateException If {@link #isAlive()} returns false
     *
     * @see #addOnTouchModeChangeListener(OnTouchModeChangeListener)
     */
    CARAPI RemoveOnTouchModeChangeListener(
        /* [in] */ IOnTouchModeChangeListener* victim);

    /**
     * Register a callback to be invoked when the invoked when it is time to
     * compute the window's internal insets.
     *
     * @param listener The callback to add
     *
     * @throws IllegalStateException If {@link #isAlive()} returns false
     *
     * We are not yet ready to commit to this API and support it, so
     * @hide
     */
    CARAPI AddOnComputeInternalInsetsListener(
        /* [in] */ IOnComputeInternalInsetsListener* listener);

    /**
     * Remove a previously installed internal insets computation callback
     *
     * @param victim The callback to remove
     *
     * @throws IllegalStateException If {@link #isAlive()} returns false
     *
     * @see #addOnComputeInternalInsetsListener(OnComputeInternalInsetsListener)
     *
     * We are not yet ready to commit to this API and support it, so
     * @hide
     */
    CARAPI RemoveOnComputeInternalInsetsListener(
        /* [in] */ IOnComputeInternalInsetsListener* victim);

    /**
     * Register a callback to be invoked when the view tree is about to be drawn
     *
     * @param listener The callback to add
     *
     * @throws IllegalStateException If {@link #isAlive()} returns false
     */
    CARAPI AddOnPreDrawListener(
        /* [in] */ IOnPreDrawListener* listener);

     /**
     * Remove a previously installed pre-draw callback
     *
     * @param victim The callback to remove
     *
     * @throws IllegalStateException If {@link #isAlive()} returns false
     *
     * @see #addOnPreDrawListener(OnPreDrawListener)
     */
    CARAPI RemoveOnPreDrawListener(
        /* [in] */ IOnPreDrawListener* victim);

    CARAPI Merge(
        /* [in] */ IViewTreeObserver* observer);

    /**
     * Register a callback to be invoked when the focus state within the view tree changes.
     *
     * @param listener The callback to add
     *
     * @throws IllegalStateException If {@link #isAlive()} returns false
     */
    CARAPI AddOnGlobalFocusChangeListener(
        /* [in] */ IOnGlobalFocusChangeListener* listener);

    /**
     * Remove a previously installed focus change callback.
     *
     * @param victim The callback to remove
     *
     * @throws IllegalStateException If {@link #isAlive()} returns false
     *
     * @see #addOnGlobalFocusChangeListener(OnGlobalFocusChangeListener)
     */
    CARAPI RemoveOnGlobalFocusChangeListener(
        /* [in] */ IOnGlobalFocusChangeListener* victim);

    /**
     * Register a callback to be invoked when the global layout state or the visibility of views
     * within the view tree changes
     *
     * @param listener The callback to add
     *
     * @throws IllegalStateException If {@link #isAlive()} returns false
     */
    CARAPI AddOnGlobalLayoutListener(
        /* [in] */ IOnGlobalLayoutListener* listener);

    /**
     * Remove a previously installed global layout callback
     *
     * @param victim The callback to remove
     *
     * @throws IllegalStateException If {@link #isAlive()} returns false
     *
     * @see #addOnGlobalLayoutListener(OnGlobalLayoutListener)
     */
    CARAPI RemoveGlobalOnLayoutListener(
        /* [in] */ IOnGlobalLayoutListener*  victim);

    /**
     * Indicates whether this ViewTreeObserver is alive. When an observer is not alive,
     * any call to a method (except this one) will throw an exception.
     *
     * If an application keeps a long-lived reference to this ViewTreeObserver, it should
     * always check for the result of this method before calling any other method.
     *
     * @return True if this object is alive and be used, false otherwise.
     */
    CARAPI IsAlive(
        /* [out] */ Boolean* alive);

    /**
     * Notifies registered listeners that focus has changed.
     */
    CARAPI DispatchOnGlobalFocusChange(
        /* [in] */ IView* oldFocus,
        /* [in] */ IView* newFocus);

    /**
     * Notifies registered listeners that a global layout happened. This can be called
     * manually if you are forcing a layout on a View or a hierarchy of Views that are
     * not attached to a Window or in the GONE state.
     */
    CARAPI DispatchOnGlobalLayout();

    /**
     * Notifies registered listeners that the drawing pass is about to start. If a
     * listener returns true, then the drawing pass is canceled and rescheduled. This can
     * be called manually if you are forcing the drawing on a View or a hierarchy of Views
     * that are not attached to a Window or in the GONE state.
     *
     * @return True if the current draw should be canceled and resceduled, false otherwise.
     */
    CARAPI DispatchOnPreDraw(
        /* [out] */ Boolean* cancelDraw);

    /**
     * Notifies registered listeners that the touch mode has changed.
     *
     * @param inTouchMode True if the touch mode is now enabled, false otherwise.
     */
    CARAPI DispatchOnTouchModeChanged(
        /* [in] */ Boolean inTouchMode);

    /**
     * Notifies registered listeners that something has scrolled.
     */
    CARAPI DispatchOnScrollChanged();

    /**
     * Returns whether there are listeners for computing internal insets.
     */
    CARAPI HasComputeInternalInsetsListeners(
        /* [out] */ Boolean* has);

    /**
     * Calls all listeners to compute the current insets.
     */
    CARAPI DispatchOnComputeInternalInsets(
        /* [in] */ IInternalInsetsInfo* inoutInfo);

private:
    void CheckIsAlive() {
        if (!mAlive) {
            // throw new IllegalStateException("This ViewTreeObserver is not alive, call "
            //         + "getViewTreeObserver() again");
            assert(0);
        }
    }

    /**
     * Marks this ViewTreeObserver as not alive. After invoking this method, invoking
     * any other method but {@link #isAlive()} and {@link #kill()} will throw an Exception.
     *
     * @hide
     */
    void Kill() {
        mAlive = FALSE;
    }

private:
    //TODO: CopyOnWriteArrayList ----Thread-safe; List---Don't know.

    // private CopyOnWriteArrayList<OnGlobalFocusChangeListener> mOnGlobalFocusListeners;
    // private CopyOnWriteArrayList<OnGlobalLayoutListener> mOnGlobalLayoutListeners;
    // private CopyOnWriteArrayList<OnPreDrawListener> mOnPreDrawListeners;
    // private CopyOnWriteArrayList<OnTouchModeChangeListener> mOnTouchModeChangeListeners;
    // private CopyOnWriteArrayList<OnComputeInternalInsetsListener> mOnComputeInternalInsetsListeners;
    List<AutoPtr<IOnComputeInternalInsetsListener> > mOnComputeInternalInsetsListeners;
    // private CopyOnWriteArrayList<OnScrollChangedListener> mOnScrollChangedListeners;

    Boolean mAlive;
    List<AutoPtr<IOnPreDrawListener> > mOnPreDrawListeners;
};

#endif //__VIEWTREEOBSERVER_H__