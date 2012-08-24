
#ifndef __CIMAGEVIEW_H__
#define __CIMAGEVIEW_H__

#include "_CImageView.h"
#include "widget/ImageView.h"
#include "view/ViewMacro.h"

CarClass(CImageView), public ImageView
{
public:
    IVIEW_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

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

    CARAPI SetAdjustViewBounds(
        /* [in] */ Boolean adjustViewBounds);

    CARAPI SetMaxWidth(
        /* [in] */ Int32 maxWidth);

    CARAPI SetMaxHeight(
        /* [in] */ Int32 maxHeight);

    CARAPI GetDrawable(
        /* [out] */ IDrawable** drawable);

    CARAPI SetImageResource(
        /* [in] */ Int32 resId);

    CARAPI SetImageURI(
        /* [in] */ IUri* uri);

    CARAPI SetImageDrawable(
        /* [in] */ IDrawable* drawable);

    CARAPI SetImageBitmap(
        /* [in] */ IBitmap* bm);

    CARAPI SetImageState(
        /* [in] */ const ArrayOf<Int32>& state,
        /* [in] */ Boolean merge);

    CARAPI SetImageLevel(
        /* [in] */ Int32 level);

    CARAPI SetScaleType(
        /* [in] */ ImageViewScaleType scaleType);

    CARAPI GetScaleType(
        /* [out] */ ImageViewScaleType* scaleType);

    CARAPI GetImageMatrix(
        /* [out] */ IMatrix** martix);

    CARAPI SetImageMatrix(
        /* [in] */ IMatrix* matrix);
};

#endif //__CIMAGEVIEW_H__
