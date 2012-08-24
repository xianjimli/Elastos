#include "ACallbackAspect.h"

ECode ACallbackAspect::Hello2()
{
    Callback::OnTriggerAspect();

    return NOERROR;
}
