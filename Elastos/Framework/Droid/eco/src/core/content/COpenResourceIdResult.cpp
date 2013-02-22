
#include "ext/frameworkext.h"
#include "content/COpenResourceIdResult.h"

ECode COpenResourceIdResult::GetResources(
    /* [out] */ IResources** resources)
{
    VALIDATE_NOT_NULL(resources);

    *resources = mResources;
    mResources->AddRef();

    return NOERROR;
}

ECode COpenResourceIdResult::SetResources(
    /* [in] */ IResources* resources)
{
    mResources = resources;

    return NOERROR;
}

ECode COpenResourceIdResult::GetResourceId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);

    *id = mId;

    return NOERROR;
}

ECode COpenResourceIdResult::SetResourceId(
    /* [in] */ Int32 id)
{
    mId = id;

    return NOERROR;
}

ECode COpenResourceIdResult::constructor()
{
    return NOERROR;
}

