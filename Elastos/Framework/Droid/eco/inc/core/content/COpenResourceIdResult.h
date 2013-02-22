
#ifndef __COPENRESOURCEIDRESULT_H__
#define __COPENRESOURCEIDRESULT_H__

#include "_COpenResourceIdResult.h"
#include <elastos/AutoPtr.h>

CarClass(COpenResourceIdResult)
{
public:
    CARAPI GetResources(
        /* [out] */ IResources** resources);

    CARAPI SetResources(
        /* [in] */ IResources* resources);

    CARAPI GetResourceId(
        /* [out] */ Int32* id);

    CARAPI SetResourceId(
        /* [in] */ Int32 id);

    CARAPI constructor();

private:
    AutoPtr<IResources> mResources;
    Int32 mId;
};

#endif // __COPENRESOURCEIDRESULT_H__
