
#include "ext/frameworkdef.h"
#include "content/CComponentNameHelper.h"

ECode CComponentNameHelper::UnflattenFromString(
    /* [in] */ const String& str,
    /* [out] */ IComponentName** component)
{
    VALIDATE_NOT_NULL(component);
    *component = NULL;
    return E_NOT_IMPLEMENTED;
}
