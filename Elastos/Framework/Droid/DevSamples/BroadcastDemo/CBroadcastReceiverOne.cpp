#include <stdio.h>
#include "CBroadcastReceiverOne.h"


CBroadcastReceiverOne::CBroadcastReceiverOne()
{
	printf("\n\n==== FUNC : %s  ==== Start CBroadcastReceiverOne!!!\n\n",  __FUNCTION__);
	//OnReceive(NULL,NULL);
}


ECode CBroadcastReceiverOne::OnReceive(
	/* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    // TODO: Add your code here
    printf("\n\n===Get Broadcast message!===\n\n");
    return NOERROR;
}

