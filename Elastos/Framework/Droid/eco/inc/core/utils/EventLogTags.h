
#ifndef __EVENTLOGTAGS_H__
#define __EVENTLOGTAGS_H__

#include <elastos.h>

using namespace Elastos;

class EventLogTags
{
public:
    // Event log tags
    static const Int32 CONFIGURATION_CHANGED = 2719;
    static const Int32 CPU = 2721;

    static const Int32 BOOT_PROGRESS_AMS_READY = 3040;
    static const Int32 BOOT_PROGRESS_ENABLE_SCREEN = 3050;

    static const Int32 AM_FINISH_ACTIVITY = 30001;
    static const Int32 AM_TASK_TO_FRONT = 30002;
    static const Int32 AM_NEW_INTENT = 30003;
    static const Int32 AM_CREATE_TASK = 30004;
    static const Int32 AM_CREATE_ACTIVITY = 30005;
    static const Int32 AM_RESTART_ACTIVITY = 30006;
    static const Int32 AM_RESUME_ACTIVITY = 30007;
    static const Int32 AM_ANR = 30008;
    static const Int32 ACTIVITY_LAUNCH_TIME = 30009;
    static const Int32 AM_PROCESS_BOUND = 30010;
    static const Int32 AM_PROCESS_DIED = 30011;
    static const Int32 AM_FAILED_TO_PAUSE = 30012;
    static const Int32 AM_PAUSE_ACTIVITY = 30013;
    static const Int32 AM_PROCESS_START = 30014;
    static const Int32 AM_PROCESS_BAD = 30015;
    static const Int32 AM_PROCESS_GOOD = 30016;
    static const Int32 AM_LOW_MEMORY = 30017;
    static const Int32 AM_DESTROY_ACTIVITY = 30018;
    static const Int32 AM_RELAUNCH_RESUME_ACTIVITY = 30019;
    static const Int32 AM_RELAUNCH_ACTIVITY = 30020;
    static const Int32 AM_ON_PAUSED_CALLED = 30021;
    static const Int32 AM_ON_RESUME_CALLED = 30022;
    static const Int32 AM_KILL = 30023;
    static const Int32 AM_BROADCAST_DISCARD_FILTER = 30024;
    static const Int32 AM_BROADCAST_DISCARD_APP = 30025;
    static const Int32 AM_CREATE_SERVICE = 30030;
    static const Int32 AM_DESTROY_SERVICE = 30031;
    static const Int32 AM_PROCESS_CRASHED_TOO_MUCH = 30032;
    static const Int32 AM_DROP_PROCESS = 30033;
    static const Int32 AM_SERVICE_CRASHED_TOO_MUCH = 30034;
    static const Int32 AM_SCHEDULE_SERVICE_RESTART = 30035;
    static const Int32 AM_PROVIDER_LOST_PROCESS = 30036;
    static const Int32 AM_PROCESS_START_TIMEOUT = 30037;

    static const Int32 AM_CRASH = 30039;
    static const Int32 AM_WTF = 30040;
};

#endif //__EVENTLOGTAGS_H__
