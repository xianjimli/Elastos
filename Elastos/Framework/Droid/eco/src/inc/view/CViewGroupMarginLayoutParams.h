
#ifndef __CVIEWGROUPMARGINLAYOUTPARAMS_H__
#define __CVIEWGROUPMARGINLAYOUTPARAMS_H__

#include "_CViewGroupMarginLayoutParams.h"
#include "view/ViewGroupMarginLayoutParams.h"

CarClass(CViewGroupMarginLayoutParams), public ViewGroupMarginLayoutParams
{
public:
    CARAPI GetDescription(
        /* [out] */ String* pDescription);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ IViewGroupMarginLayoutParams* source);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* source);

    CARAPI SetWidth(
        /* [in] */ Int32 width);

    CARAPI SetHeight(
        /* [in] */ Int32 height);

    CARAPI GetWidth(
        /* [out] */ Int32 * width);

    CARAPI GetHeight(
        /* [out] */ Int32 * height);

    CARAPI SetMargins(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI GetMargins(
        /* [out] */ Int32* left,
        /* [out] */ Int32* top,
        /* [out] */ Int32* right,
        /* [out] */ Int32* bottom);

private:
    // TODO: Add your private member variables here.
};

#endif // __CVIEWGROUPMARGINLAYOUTPARAMS_H__
