
#include "server/InputManagerCallbacks.h"

InputManagerCallbacks::InputManagerCallbacks(
    /* [in] */ IContext* context,
    /* [in] */ CWindowManagerService* windowManagerService) :
    mContext(context),
    mWindowManagerService(windowManagerService)
{
}

ECode InputManagerCallbacks::NotifyConfigurationChanged(
    /* [in] */ Millisecond64 whenNanos)
{
    return mWindowManagerService->SendNewConfiguration();
}

ECode InputManagerCallbacks::NotifyLidSwitchChanged(
    /* [in] */ Millisecond64 whenNanos,
    /* [in] */ Boolean lidOpen)
{
    return mWindowManagerService->mInputMonitor->NotifyLidSwitchChanged(whenNanos, lidOpen);
}

ECode InputManagerCallbacks::NotifyInputChannelBroken(
    /* [in] */ IInputChannel* inputChannel)
{
    return mWindowManagerService->mInputMonitor->NotifyInputChannelBroken(inputChannel);
}

ECode InputManagerCallbacks::NotifyANR(
    /* [in] */ void* token,
    /* [in] */ IInputChannel* inputChannel,
    /* [out] */ Int64* timeout)
{
    return mWindowManagerService->mInputMonitor->NotifyANR(token, inputChannel, timeout);
}

ECode InputManagerCallbacks::InterceptKeyBeforeQueueing(
    /* [in] */ Millisecond64 whenNanos,
    /* [in] */ Int32 action,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 scanCode,
    /* [in] */ Int32 policyFlags,
    /* [in] */ Boolean isScreenOn,
    /* [out] */ Int32* result)
{
    return mWindowManagerService->mInputMonitor->InterceptKeyBeforeQueueing(
            whenNanos, action, flags, keyCode, scanCode, policyFlags, isScreenOn, result);
}

ECode InputManagerCallbacks::InterceptKeyBeforeDispatching(
    /* [in] */ IInputChannel* focus,
    /* [in] */ Int32 action,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 scanCode,
    /* [in] */ Int32 metaState,
    /* [in] */ Int32 repeatCount,
    /* [in] */ Int32 policyFlags,
    /* [out] */ Boolean* result)
{
    return mWindowManagerService->mInputMonitor->InterceptKeyBeforeDispatching(focus,
            action, flags, keyCode, scanCode, metaState, repeatCount, policyFlags, result);
}

ECode InputManagerCallbacks::CheckInjectEventsPermission(
    /* [in] */ Int32 injectorPid,
    /* [in] */ Int32 injectorUid,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode InputManagerCallbacks::FilterTouchEvents(
    /* [in] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode InputManagerCallbacks::FilterJumpyTouchEvents(
    /* [in] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode InputManagerCallbacks::GetVirtualKeyQuietTimeMillis(
    /* [in] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode InputManagerCallbacks::GetVirtualKeyDefinitions(
    /* [in] */ const String& deviceName,
    /* [out] */ ArrayOf<VirtualKeyDefinition>** result)
{
    return E_NOT_IMPLEMENTED;
}

ECode InputManagerCallbacks::GetInputDeviceCalibration(
    /* [in] */ const String& deviceName,
    /* [out] */ InputDeviceCalibration** result)
{
    return E_NOT_IMPLEMENTED;
}

ECode InputManagerCallbacks::GetExcludedDeviceNames(
    /* [out] */ ArrayOf<String>** result)
{
    return E_NOT_IMPLEMENTED;
}

ECode InputManagerCallbacks::GetMaxEventsPerSecond(
    /* [out] */  Int32* result)
{
    return E_NOT_IMPLEMENTED;
}

