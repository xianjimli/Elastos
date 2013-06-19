#include <stdio.h>
#include "CBroadcastReceiverOne.h"

ECode CBroadcastReceiverOne::OnReceive(
	/* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // TODO: Add your code here
    printf("\n\n===Get Broadcast message!===\n\n");
    return NOERROR;
}

