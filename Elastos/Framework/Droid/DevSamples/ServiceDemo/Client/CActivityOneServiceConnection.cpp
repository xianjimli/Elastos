
#include "CActivityOneServiceConnection.h"
#include <stdio.h>

ECode CActivityOneServiceConnection::OnServiceConnected(
    /* [in] */ IComponentName *name,
    /* [in] */ IBinder * service)
{
    printf("-------------ActivityOne:OnServiceConnected!\n");
    return NOERROR;
}

ECode CActivityOneServiceConnection::OnServiceDisconnected(
    /* [in] */ IComponentName *name)
{
    return NOERROR;
}
