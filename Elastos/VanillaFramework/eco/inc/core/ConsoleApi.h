
#ifndef __CONSOLEAPI_H__
#define __CONSOLEAPI_H__

#include <elastos.h>

using namespace Elastos;

ELAPI _Console_RequestToFinish();

ELAPI _Console_ReleaseAtExit(
    /* [in] */ IInterface* object);

ELAPI _Console_AddExitRoutine(
    /* [in] */ IInterface* object,
    /* [in] */ PVoid userData);

#endif //__CONSOLEAPI_H__
