
#ifndef __CIMAGEBUTTON_H__
#define __CIMAGEBUTTON_H__

#include "_CImageButton.h"
#include "widget/ImageButton.h"
#include "view/ViewMacro.h"

CarClass(CImageButton), public ImageButton
{
public:
    IVIEW_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetAdjustViewBounds(
        /* [in] */ Boolean adjustViewBounds);

    CARAPI SetMaxWidth(
        /* [in] */ Int32 maxWidth);

    CARAPI SetMaxHeight(
        /* [in] */ Int32 maxHeight);

    CARAPI GetDrawable(
        /* [out] */ IDrawable ** ppDrawable);

    CARAPI SetImageResource(
        /* [in] */ Int32 resId);

    CARAPI SetImageURI(
        /* [in] */ IUri * pUri);

    CARAPI SetImageDrawable(
        /* [in] */ IDrawable * pDrawable);

    CARAPI SetImageBitmap(
        /* [in] */ IBitmap * pBm);

    CARAPI SetImageState(
        /* [in] */ const ArrayOf<Int32> & state,
        /* [in] */ Boolean mg);

    CARAPI SetImageLevel(
        /* [in] */ Int32 level);

    CARAPI SetScaleType(
        /* [in] */ ImageViewScaleType scaleType);

    CARAPI GetScaleType(
        /* [out] */ ImageViewScaleType * pScaleType);

    CARAPI GetImageMatrix(
        /* [out] */ IMatrix ** ppMartix);

    CARAPI SetImageMatrix(
        /* [in] */ IMatrix * pMatrix);

    CARAPI constructor(
        /* [in] */ IContext * pCtx);

    CARAPI constructor(
        /* [in] */ IContext * pCtx,
        /* [in] */ IAttributeSet * pAttrs);

    CARAPI constructor(
        /* [in] */ IContext * pCtx,
        /* [in] */ IAttributeSet * pAttrs,
        /* [in] */ Int32 defStyle);

private:
    // TODO: Add your private member variables here.
};

#endif // __CIMAGEBUTTON_H__
