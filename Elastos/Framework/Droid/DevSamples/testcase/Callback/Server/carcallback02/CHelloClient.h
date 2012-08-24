
#ifndef __CHELLOCLIENT_H__
#define __CHELLOCLIENT_H__

#include "_CHelloClient.h"

CarClass(CHelloClient)
{
public:
    CARAPI OnTrigger(PInterface pSender);
    CARAPI HelloClient(IHello* pIHello);

private:
    // TODO: Add your private member variables here.
};

#endif // __CHELLOCLIENT_H__
