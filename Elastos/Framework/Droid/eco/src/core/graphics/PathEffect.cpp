
#include "graphics/PathEffect.h"


// {29CC1A9D-F188-4165-AD7D-CAB9A43079EC}
extern "C" const InterfaceID EIID_PathEffect =
{ 0x29cc1a9d, 0xf188, 0x4165, { 0xad, 0x7d, 0xca, 0xb9, 0xa4, 0x30, 0x79, 0xec } };


PathEffect::~PathEffect()
{
    NativeDestructor(mNativeInstance);
}

void PathEffect::NativeDestructor(
    /* [in] */ SkPathEffect* nObj)
{
    nObj->safeUnref();
}

SkPathEffect* PathEffect::Ni()
{
	return mNativeInstance;
}

