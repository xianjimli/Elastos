
#ifndef __CGECKOCONNECTIVITYRECEIVER_H__
#define __CGECKOCONNECTIVITYRECEIVER_H__

#include "CBroadcastReceiver.h"
#include "_CGeckoConnectivityReceiver.h"

class CGeckoConnectivityReceiver : public CBroadcastReceiver
{
public:

protected:
    CARAPI OnReciever(
        /* [in] */ IIntent *pIntent);

private:
    // TODO: Add your private member variables here.
};

#endif // __CGECKOCONNECTIVITYRECEIVER_H__

