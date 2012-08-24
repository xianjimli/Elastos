#ifndef __CCBSERVER_H__
#define __CCBSERVER_H__

#include "_CCBServer.h"

class CCBServer : public _CCBServer
{
public:
    CARAPI TriggerA();
    CARAPI TriggerB();
    CARAPI TriggerC();

    CARAPI Trigger1();

    CARAPI Trigger2();

    CARAPI Trigger3();

    CARAPI Trigger4();

    CARAPI Trigger5();

    CARAPI Trigger6(Int32 nIndex);

private:
};

#endif // __CCBSERVER_H__
