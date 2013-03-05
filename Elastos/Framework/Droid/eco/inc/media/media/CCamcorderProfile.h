
#ifndef __CCAMCORDERPROFILE_H__
#define __CCAMCORDERPROFILE_H__

#include "_CCamcorderProfile.h"
#include "media/CamcorderProfile.h"

CarClass(CCamcorderProfile), public CamcorderProfile
{
public:
    CARAPI Get(
        /* [in] */ Int32 quality,
        /* [out] */ ICamcorderProfile ** ppCamcordProfile);

    CARAPI GetEx(
        /* [in] */ Int32 cameraId,
        /* [in] */ Int32 quality,
        /* [out] */ ICamcorderProfile ** ppCamcordProfile);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CCAMCORDERPROFILE_H__
