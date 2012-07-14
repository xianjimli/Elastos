
#ifndef __CGALLERYLAYOUTPARAMS_H__
#define __CGALLERYLAYOUTPARAMS_H__

#include "_CGalleryLayoutParams.h"
#include "widget/GalleryLayoutParams.h"

CarClass(CGalleryLayoutParams) ,public GalleryLayoutParams
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

    CARAPI constructor(
        /* [in] */ IContext * pC,
        /* [in] */ IAttributeSet * pAttrs);

    CARAPI constructor(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams * pSource);

private:
    // TODO: Add your private member variables here.
};

#endif // __CGALLERYLAYOUTPARAMS_H__
