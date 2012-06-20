
#ifndef __CACTIVITYONESERVICECONNECTION_H__
#define __CACTIVITYONESERVICECONNECTION_H__

#include "_CActivityOneServiceConnection.h"

CarClass(CActivityOneServiceConnection)
{
public:
    CARAPI OnServiceConnected(
        /* [in] */ IComponentName* name,
        /* [in] */ IBinder* service);

    CARAPI OnServiceDisconnected(
        /* [in] */ IComponentName* name);

private:

};

#endif // __CACTIVITYONESERVICECONNECTION_H__
