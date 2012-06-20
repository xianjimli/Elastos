
#include "graphics/CBitmapFactoryOptions.h"

CBitmapFactoryOptions::CBitmapFactoryOptions()
    : mInJustDecodeBounds(FALSE)
    , mInSampleSize(0)
    , mInPreferredConfig(BitmapConfig_ARGB_8888)
    , mInDither(FALSE)
    , mInDensity(0)
    , mInTargetDensity(0)
    , mInScreenDensity(0)
    , mInScaled(TRUE)
    , mInPurgeable(FALSE)
    , mInInputShareable(FALSE)
    , mInNativeAlloc(FALSE)
    , mInPreferQualityOverSpeed(FALSE)
    , mOutWidth(0)
    , mOutHeight(0)
    , mCancel(FALSE)
{
}
