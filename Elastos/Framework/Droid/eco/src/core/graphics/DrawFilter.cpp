
#include "graphics/DrawFilter.h"

DrawFilter::DrawFilter()
{
    // TODO: Add your code here
}

DrawFilter::~DrawFilter()
{
    NativeDestructor(nativeDrawFilter);
}

void DrawFilter::NativeDestructor(
    /* [in] */ SkDrawFilter* nObj)
{
    nObj->safeUnref();
}
