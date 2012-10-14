
#ifndef __GALLERYLAYOUTPARAMS_H__
#define __GALLERYLAYOUTPARAMS_H__

#include "view/ViewGroupLayoutParams.h"

/**
 * Gallery extends LayoutParams to provide a place to hold current
 * Transformation information along with previous position/transformation
 * info.
 *
 */

class GalleryLayoutParams : public ViewGroupLayoutParams
{
public:
    GalleryLayoutParams(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    GalleryLayoutParams(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    GalleryLayoutParams(
        /* [in] */ ViewGroupLayoutParams* source);

protected:
    GalleryLayoutParams();
};

#endif

