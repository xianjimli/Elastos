
#include "os/Process.h"


const Int32 Process::SYSTEM_UID;
const Int32 Process::PHONE_UID;
const Int32 Process::SHELL_UID;
const Int32 Process::LOG_UID;
const Int32 Process::WIFI_UID;
const Int32 Process::NFC_UID;
const Int32 Process::FIRST_APPLICATION_UID;
const Int32 Process::LAST_APPLICATION_UID;
const Int32 Process::BLUETOOTH_GID;
const Int32 Process::THREAD_PRIORITY_DEFAULT;
const Int32 Process::THREAD_PRIORITY_LOWEST;
const Int32 Process::THREAD_PRIORITY_BACKGROUND;
const Int32 Process::THREAD_PRIORITY_FOREGROUND;
const Int32 Process::THREAD_PRIORITY_DISPLAY;
const Int32 Process::THREAD_PRIORITY_URGENT_DISPLAY;
const Int32 Process::THREAD_PRIORITY_AUDIO;
const Int32 Process::THREAD_PRIORITY_URGENT_AUDIO;
const Int32 Process::THREAD_PRIORITY_MORE_FAVORABLE;
const Int32 Process::THREAD_PRIORITY_LESS_FAVORABLE;
const Int32 Process::THREAD_GROUP_DEFAULT;
const Int32 Process::THREAD_GROUP_BG_NONINTERACTIVE;

Int32 Process::GetCallingPid()
{
    //not implemented;
    return -1;
}

Int32 Process::GetCallingUid()
{
    //not implemented;
    return -1;
}

Int64 Process::ClearCallingIdentity()
{
    //int64_t token = ((int64_t)mCallingUid<<32) | mCallingPid;
    //clearCaller();
    //return token;
    return -1;
}

void Process::RestoreCallingIdentity(
	/*[in]*/Int64 token)
{
//    mCallingUid = (int)(token>>32);
//    mCallingPid = (int)token;
}



ECode Process::SetProcessGroup(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 group)
{
    return E_NOT_IMPLEMENTED;
}

void Process::SetThreadPriority(
    /* [in] */ Int32 priority)
{
    // E_NOT_IMPLEMENTED
}

Boolean Process::SupportsProcesses()
{
    //not implemented;
    return TRUE;
}

Boolean Process::SetOomAdj(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 amt)
{
    //not implemented;
    return TRUE;
}

void Process::KillProcess(
    /* [in] */ Int32 pid)
{
    //not implemented;
}

void Process::KillProcessQuiet(
    /* [in] */ Int32 pid)
{
    //not implemented;
}

Int32 Process::MyUid()
{
    return -1;
}

Int32 Process::GetUidForName(
    /* [in] */ const String& name)
{
    return -1;
}

Int32 Process::GetGidForName(
    /* [in] */ const String& name)
{
    return -1;
}


