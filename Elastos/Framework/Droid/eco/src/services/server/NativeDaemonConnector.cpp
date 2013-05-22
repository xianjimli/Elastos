
#include "server/NativeDaemonConnector.h"

NativeDaemonConnector::NativeDaemonConnector()
{
}

void NativeDaemonConnector::Run()
{
}

void NativeDaemonConnector::ListenToSocket()
{
}

void NativeDaemonConnector::SendCommand(
    /* [in] */ const String& command)
{
}

void NativeDaemonConnector::SendCommandII(
    /* [in] */ const String& command,
    /* [in] */ const String& argument)
{
}

List<String>* NativeDaemonConnector::DoCommand(
    /* [in] */ const String& cmd)
{
}

List<String>* NativeDaemonConnector::DoListCommand(
    /* [in] */ const String& cmd,
    /* [in] */ Int32 expectedResponseCode)
{
}

