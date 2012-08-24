
#ifndef __VIEWTREEOBSERVER_H__
#define __VIEWTREEOBSERVER_H__

#include <elastos/ElRefBase.h>
#include "graphics/CRect.h"
#include <elastos/AutoPtr.h>

class ViewTreeObserver : public ElRefBase, public IViewTreeObserver
{
public:
    class InternalInsetsInfo
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

    public:
        InternalInsetsInfo();

        CARAPI_(void) SetTouchableInsets(
            /* [in] */ Int32 val);

        CARAPI_(Int32) GetTouchableInsets();

        CARAPI_(void) Reset();

        CARAPI_(Boolean) Equals(
            /* [in] */ InternalInsetsInfo* other);

        CARAPI_(void) Set(
            /* [in] */ InternalInsetsInfo* other);

    public:
        /**
         * Offsets from the frame of the window at which the content of
         * windows behind it should be placed.
         */
        AutoPtr<CRect> mContentInsets;

        /**
         * Offsets from the fram of the window at which windows behind it
         * are visible.
         */
        AutoPtr<CRect> mVisibleInsets;

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
};

#endif //__VIEWTREEOBSERVER_H__