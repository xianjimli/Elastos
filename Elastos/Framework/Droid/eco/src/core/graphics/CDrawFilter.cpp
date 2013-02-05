
#include "graphics/CDrawFilter.h"

ECode CDrawFilter::constructor()
{
    // TODO: Add your code here
    return NOERROR;
}

CDrawFilter::~CDrawFilter()
{
    //NativeDestructor(nativeDrawFilter);
    //DrawFilter::~DrawFilter();
}

void CDrawFilter::NativeDestructor(
    /* [in] */ SkDrawFilter* nObj)
{
    nObj->safeUnref();
    //DrawFilter::NativeDestructor(nObj);
}

