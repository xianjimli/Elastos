
#include "view/CWindowManagerLayoutParamsHelper.h"
#include "view/CWindowManagerLayoutParams.h"

CWindowManagerLayoutParamsHelper::MayUseInputMethodHelper(
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* mayUse)
{
    assert(mayUse != NULL);
    *mayUse = CWindowManagerLayoutParams::MayUseInputMethod(flags);
    return NOERROR;
}
