
#ifndef __CGALLERYLAYOUTPARAMS_H__
#define __CGALLERYLAYOUTPARAMS_H__

#include "_CGalleryLayoutParams.h"
#include "widget/GalleryLayoutParams.h"

CarClass(CGalleryLayoutParams) ,public GalleryLayoutParams
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

    CARAPI constructor(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* source);

private:
    // TODO: Add your private member variables here.
};

#endif // __CGALLERYLAYOUTPARAMS_H__
