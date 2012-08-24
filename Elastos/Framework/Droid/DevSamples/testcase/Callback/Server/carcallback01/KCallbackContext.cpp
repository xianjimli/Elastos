#include "KCallbackContext.h"

ECode KCallbackContext::Hello3()
{
    Callback::OnTriggerContext();

    return NOERROR;
}
