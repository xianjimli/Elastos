
#ifndef __CIMAGEVIEW_H__
#define __CIMAGEVIEW_H__

#include "_CRelativeLayout.h"
#include "widget/RelativeLayout.h"
#include "view/ViewMacro.h"

CarClass(CRelativeLayout), public RelativeLayout
{
public:
    IVIEW_METHODS_DECL();

    IVIEWGROUP_METHODS_DECL();

    IVIEWPARENT_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
     * Defines which View is ignored when the gravity is applied. This setting has no
     * effect if the gravity is <code>Gravity.LEFT | Gravity.TOP</code>.
     *
     * @param viewId The id of the View to be ignored by gravity, or 0 if no View
     *        should be ignored.
     *
     * @see #setGravity(int)
     *
     * @attr ref android.R.styleable#RelativeLayout_ignoreGravity
     */
    CARAPI SetIgnoreGravity(
        /* [in] */ Int32 viewId);

    /**
     * Describes how the child views are positioned. Defaults to
     * <code>Gravity.LEFT | Gravity.TOP</code>.
     *
     * @param gravity See {@link android.view.Gravity}
     *
     * @see #setHorizontalGravity(int)
     * @see #setVerticalGravity(int)
     *
     * @attr ref android.R.styleable#RelativeLayout_gravity
     */
    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI SetHorizontalGravity(
        /* [in] */ Int32 horizontalGravity);

    CARAPI SetVerticalGravity(
        /* [in] */ Int32 verticalGravity);
};

#endif //__CIMAGEVIEW_H__
