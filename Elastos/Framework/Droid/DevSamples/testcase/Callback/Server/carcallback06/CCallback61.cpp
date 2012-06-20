#include "CCallback61.h"

ECode CCallback61::Hello61(
    /* [in] */ int nIndex)
{
    switch (nIndex) {
        case 1:
            Callback::OnTrigger61();
            break;
        case 2:
            Callback::OnTrigger62();
            break;
        case 3:
            Callback::OnTrigger61();
            Callback::OnTrigger62();
            break;
        default:
            break;
    }

    return NOERROR;
}
