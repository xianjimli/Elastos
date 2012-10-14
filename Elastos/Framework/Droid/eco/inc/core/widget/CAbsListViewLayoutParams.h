
#ifndef __CABSLISTVIEWLAYOUTPARAMS_H__
#define __CABSLISTVIEWLAYOUTPARAMS_H__

#include "_CAbsListViewLayoutParams.h"
#include "widget/AbsListViewLayoutParams.h"

CarClass(CAbsListViewLayoutParams), public AbsListViewLayoutParams
{
public:
    CARAPI GetDescription(
        /* [out] */ String* description);

    CARAPI SetWidth(
        /* [in] */ Int32 width);

    CARAPI SetHeight(
        /* [in] */ Int32 height);

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI GetViewType(
        /* [out] */ Int32* viewType);

    CARAPI constructor(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 viewType);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* source);

private:
    // TODO: Add your private member variables here.
};

#endif // __CABSLISTVIEWLAYOUTPARAMS_H__
