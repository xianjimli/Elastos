
#ifndef __NATIVEDAEMONCONNECTOR_H__
#define __NATIVEDAEMONCONNECTOR_H__

#include "ext/frameworkext.h"
#include <elastos/List.h>

class NativeDaemonConnector
{
public:
    NativeDaemonConnector();
    ~NativeDaemonConnector();

    CARAPI_(void) Run();

    CARAPI_(void) ListenToSocket();

    CARAPI_(void) SendCommand(
        /* [in] */ const String& command);

    CARAPI_(void) SendCommandII(
        /* [in] */ const String& command,
        /* [in] */ const String& argument);

    CARAPI_(List<String>*) DoCommand(
        /* [in] */ const String& cmd);

    CARAPI_(List<String>*) DoListCommand(
        /* [in] */ const String& cmd,
        /* [in] */ Int32 expectedResponseCode);

private:
    // TODO: Add your private member variables here.
};

#endif // __NATIVEDAEMONCONNECTOR_H__
