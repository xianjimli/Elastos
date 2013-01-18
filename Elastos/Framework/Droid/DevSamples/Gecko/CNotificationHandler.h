
#ifndef __CNOTIFICATIONHANDLER_H__
#define __CNOTIFICATIONHANDLER_H__

#include "BroadcastReceiver.h"
#include "_CNotificationHandler.h"

class CNotificationHandler : public BroadcastReceiver
{
public:
    CARAPI OnReciever(
        /* [in] */ IContext* pContext,
        /* [in] */ IIntent *pIntent);

protected:
    ECode HandleIntent(
        /* [in] */ IContext* pContext,
        /* [in] */ IIntent* pNotificationIntent);

private:
    // TODO: Add your private member variables here.
};

#endif // __CNOTIFICATIONHANDLER_H__

