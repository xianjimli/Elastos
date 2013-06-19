
#ifndef __CBROADCASTRECEIVERONE_H__
#define __CBROADCASTRECEIVERONE_H__

#include "_CBroadcastReceiverOne.h"
#include "BroadcastReceiver.h"

CarClass(CBroadcastReceiverOne), public BroadcastReceiver
{
public:

protected:
    CARAPI OnReceive(
    	/* [in] */ IContext* context,
        /* [in] */ IIntent* intent);

private:
    // TODO: Add your private member variables here.
};

#endif // __CBROADCASTRECEIVERONE_H__
