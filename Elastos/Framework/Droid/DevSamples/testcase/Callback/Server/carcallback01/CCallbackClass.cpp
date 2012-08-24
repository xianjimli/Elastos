#include "CCallbackClass.h"
#include "stdio.h"

ECode CCallbackClass::Hello1(
    /* [in] */ int nIndex)
{
    IHello2* pIAspect = NULL;

    switch (nIndex) {
        case 1:
            Callback::OnTriggerClass();
            break;
        case 2:
            pIAspect = IHello2::Probe((_IInterface*)this);
            if (!pIAspect) {
                printf("Probe failed!\n");
                goto Exit;
            }
            pIAspect->Hello2();
            break;
        default:
            break;
    }

    return NOERROR;
Exit:
    return E_FAIL;
}
