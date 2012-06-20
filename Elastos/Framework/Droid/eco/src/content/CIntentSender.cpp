
#include "content/CIntentSender.h"

ECode CIntentSender::constructor(
    /* [in] */ IIntentSender* target)
{
    mTarget = target;
    return NOERROR;
}

ECode CIntentSender::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CIntentSender::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    return E_NOT_IMPLEMENTED;
}
