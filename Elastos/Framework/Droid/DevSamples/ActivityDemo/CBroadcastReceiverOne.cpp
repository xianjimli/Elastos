
#include "CBroadcastReceiverOne.h"
#include<elastos/AutoPtr.h>

ECode CBroadcastReceiverOne::OnReceive(
    /* [in] */ IContext *pContext,
    /* [in] */ IIntent *pIntent)
{
    AutoPtr<IIntent> intent;
    CIntent::New((IIntent**)&intent);
    intent->SetClassName(String("ActivityDemo"), String("CActivityThree"));
    ECode ec = pContext->StartActivity(intent);

    return ec;
}
