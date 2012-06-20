
#ifndef __CBROADCASTRECEIVERONE_H__
#define __CBROADCASTRECEIVERONE_H__

#include "CBroadcastReceiver.h"
#include "_CBroadcastReceiverOne.h"

class CBroadcastReceiverOne : public CBroadcastReceiver
{
public:

protected:
    CARAPI OnReciever(
        /* [in] */ IIntent *pIntent);

private:
    // TODO: Add your private member variables here.
};

#endif // __CBROADCASTRECEIVERONE_H__
