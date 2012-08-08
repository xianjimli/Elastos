
#include "widget/GalleryLayoutParams.h"

GalleryLayoutParams::GalleryLayoutParams()
{}

GalleryLayoutParams::GalleryLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : ViewGroupLayoutParams(c, attrs)
{}

GalleryLayoutParams::GalleryLayoutParams(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
    : ViewGroupLayoutParams(w, h)
{}

GalleryLayoutParams::GalleryLayoutParams(
    /* [in] */ ViewGroupLayoutParams* source)
    : ViewGroupLayoutParams(source)
{}
