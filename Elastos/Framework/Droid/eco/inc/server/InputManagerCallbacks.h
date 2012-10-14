
#ifndef __INPUTMANAGERCALLBACKS_H__
#define __INPUTMANAGERCALLBACKS_H__

#include "server/CWindowManagerService.h"
#include "server/InputManager.h"

class VirtualKeyDefinition;
class InputDeviceCalibration;

class InputManagerCallbacks
{
public:
    InputManagerCallbacks(
        /* [in] */ IContext* context,
        /* [in] */ CWindowManagerService* windowManagerService);

    CARAPI NotifyConfigurationChanged(
        /* [in] */ Millisecond64 whenNanos);

    CARAPI NotifyLidSwitchChanged(
        /* [in] */ Millisecond64 whenNanos,
        /* [in] */ Boolean lidOpen);

    CARAPI NotifyInputChannelBroken(
        /* [in] */ IInputChannel* inputChannel);

    CARAPI NotifyANR(
        /* [in] */ void* token,
        /* [in] */ IInputChannel* inputChannel,
        /* [out] */ Int64* timeout);

    CARAPI InterceptKeyBeforeQueueing(
        /* [in] */ Millisecond64 whenNanos,
        /* [in] */ Int32 action,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 scanCode,
        /* [in] */ Int32 policyFlags,
        /* [in] */ Boolean isScreenOn,
        /* [out] */ Int32* result);

    CARAPI InterceptKeyBeforeDispatching(
        /* [in] */ IInputChannel* focus,
        /* [in] */ Int32 action,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 scanCode,
        /* [in] */ Int32 metaState,
        /* [in] */ Int32 repeatCount,
        /* [in] */ Int32 policyFlags,
        /* [out] */ Boolean* result);

    CARAPI CheckInjectEventsPermission(
        /* [in] */ Int32 injectorPid,
        /* [in] */ Int32 injectorUid,
        /* [out] */ Boolean* result);

    CARAPI FilterTouchEvents(
        /* [in] */ Boolean* result);

    CARAPI FilterJumpyTouchEvents(
        /* [in] */ Boolean* result);

    CARAPI GetVirtualKeyQuietTimeMillis(
        /* [in] */ Int32* result);

    CARAPI GetVirtualKeyDefinitions(
        /* [in] */ const String& deviceName,
        /* [out] */ ArrayOf<VirtualKeyDefinition>** result);

    CARAPI GetInputDeviceCalibration(
        /* [in] */ const String& deviceName,
        /* [out] */ InputDeviceCalibration** result);

    CARAPI GetExcludedDeviceNames(
        /* [out] */ ArrayOf<String>** result);

    CARAPI GetMaxEventsPerSecond(
        /* [out] */  Int32* result);

private:
    AutoPtr<IContext> mContext;
    AutoPtr<CWindowManagerService> mWindowManagerService;
};

#endif //__INPUTMANAGERCALLBACKS_H__
