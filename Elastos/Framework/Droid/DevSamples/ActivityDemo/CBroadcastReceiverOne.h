
#ifndef __CBROADCASTRECEIVERONE_H__
#define __CBROADCASTRECEIVERONE_H__

#include "BroadcastReceiver.h"
#include "_CBroadcastReceiverOne.h"

class CBroadcastReceiverOne : public BroadcastReceiver
{
public:

protected:
    CARAPI OnReceive(
        /* [in] */ IContext *pContext,
        /* [in] */ IIntent *pIntent);

private:
    // TODO: Add your private member variables here.
};

#endif // __CBROADCASTRECEIVERONE_H__
