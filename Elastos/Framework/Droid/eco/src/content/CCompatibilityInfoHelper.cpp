
#include "content/CCompatibilityInfoHelper.h"
#include "content/CompatibilityInfo.h"


ECode CCompatibilityInfoHelper::UpdateCompatibleScreenFrame(
    /* [in] */ IDisplayMetrics* dm,
    /* [in] */ Int32 orientation,
    /* [in, out] */ IRect* outRect)
{
    VALIDATE_NOT_NULL(dm);
    VALIDATE_NOT_NULL(outRect);

    CompatibilityInfo::UpdateCompatibleScreenFrame(dm, orientation, outRect);
    return NOERROR;
}
