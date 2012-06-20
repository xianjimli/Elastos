
#include "graphics/MaskFilter.h"

// {7C225E13-F898-4DCA-8458-AC741F558DD2}
extern "C" const InterfaceID EIID_MaskFilter =
{ 0x7c225e13, 0xf898, 0x4dca, { 0x84, 0x58, 0xac, 0x74, 0x1f, 0x55, 0x8d, 0xd2 } };


MaskFilter::~MaskFilter()
{
    NativeDestructor(mNativeInstance);
}

void MaskFilter::NativeDestructor(
    /* [in] */ SkMaskFilter* nObj)
{
    nObj->safeUnref();
}
