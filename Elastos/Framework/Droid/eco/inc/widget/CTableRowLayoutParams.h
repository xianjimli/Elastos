
#ifndef __CTABLEROWLAYOUTPARAMS_H__
#define __CTABLEROWLAYOUTPARAMS_H__

#include "_CTableRowLayoutParams.h"
#include "widget/TableRowLayoutParams.h"

CarClass(CTableRowLayoutParams), public TableRowLayoutParams
{
public:
    CARAPI GetDescription(
        /* [out] */ String * pDescription);

    CARAPI SetWidth(
        /* [in] */ Int32 width);

    CARAPI SetHeight(
        /* [in] */ Int32 height);

    CARAPI GetWidth(
        /* [out] */ Int32 * pWidth);

    CARAPI GetHeight(
        /* [out] */ Int32 * pHeight);

    CARAPI SetMargins(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI GetMargins(
        /* [out] */ Int32 * pLeft,
        /* [out] */ Int32 * pTop,
        /* [out] */ Int32 * pRight,
        /* [out] */ Int32 * pBottom);

    CARAPI GetWeight(
        /* [out] */ Float * pWeight);

    CARAPI GetGravity(
        /* [out] */ Int32 * pGravity);

    CARAPI constructor(
        /* [in] */ IContext * pC,
        /* [in] */ IAttributeSet * pAttrs);

    CARAPI constructor(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    CARAPI constructor(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Float initWeight);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 column);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams * pP);

    CARAPI constructor(
        /* [in] */ IViewGroupMarginLayoutParams * pSource);

private:
    // TODO: Add your private member variables here.
};

#endif // __CTABLEROWLAYOUTPARAMS_H__
