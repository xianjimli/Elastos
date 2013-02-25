
#ifndef __INPUTMANAGER_H__
#define __INPUTMANAGER_H__

#include "server/CWindowManagerService.h"
#include "server/InputManagerCallbacks.h"
#include "server/InputWindow.h"
#include "server/InputApplication.h"

class CWindowManagerService;
class InputManagerCallbacks;

class VirtualKeyDefinition
{
public:
    Int32 mScanCode;

    // configured position data, specified in display coords
    Int32 mCenterX;
    Int32 mCenterY;
    Int32 mWidth;
    Int32 mHeight;
};

class InputDeviceCalibration
{
public:
    ArrayOf<String>* mKeys;
    ArrayOf<String>* mValues;
};

class InputManager
{
public:
    // Input event injection constants defined in InputDispatcher.h.
    static const Int32 INPUT_EVENT_INJECTION_SUCCEEDED = 0;
    static const Int32 INPUT_EVENT_INJECTION_PERMISSION_DENIED = 1;
    static const Int32 INPUT_EVENT_INJECTION_FAILED = 2;
    static const Int32 INPUT_EVENT_INJECTION_TIMED_OUT = 3;

    // Input event injection synchronization modes defined in InputDispatcher.h
    static const Int32 INPUT_EVENT_INJECTION_SYNC_NONE = 0;
    static const Int32 INPUT_EVENT_INJECTION_SYNC_WAIT_FOR_RESULT = 1;
    static const Int32 INPUT_EVENT_INJECTION_SYNC_WAIT_FOR_FINISH = 2;

    // Key states (may be returned by queries about the current state of a
    // particular key code, scan code or switch).

    /** The key state is unknown or the requested key itself is not supported. */
    static const Int32 KEY_STATE_UNKNOWN = -1;

    /** The key is up. /*/
    static const Int32 KEY_STATE_UP = 0;

    /** The key is down. */
    static const Int32 KEY_STATE_DOWN = 1;

    /** The key is down but is a virtual key press that is being emulated by the system. */
    static const Int32 KEY_STATE_VIRTUAL = 2;

public:
    InputManager(
        /* [in] */ IContext* context,
        /* [in] */ CWindowManagerService* windowManagerService);

    CARAPI Start();

    CARAPI SetDisplaySize(
        /* [in] */ Int32 displayId,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI SetDisplayOrientation(
        /* [in] */ Int32 displayId,
        /* [in] */ Int32 rotation);

    CARAPI GetInputConfiguration(
        /* [in] */ IConfiguration* config);

    CARAPI_(Int32) GetKeyCodeState(
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 sourceMask,
        /* [in] */ Int32 keyCode);

    CARAPI_(Int32) GetScanCodeState(
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 sourceMask,
        /* [in] */ Int32 scanCode);

    CARAPI_(Int32) GetSwitchState(
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 sourceMask,
        /* [in] */ Int32 switchCode);

//    CARAPI_(Boolean) HasKeys(
//        /* [in] */ Int32 deviceId,
//        /* [in] */ Int32 sourceMask,
//        /* [in] */ Int32[] keyCodes,
//        /* [in] */ Boolean[] keyExists);

    CARAPI MonitorInput(
        /* [in] */ const String& inputChannelName,
        /* [out] */ IInputChannel** inputChannel);

    CARAPI RegisterInputChannel(
        /* [in] */ IInputChannel* _inputChannel);

    CARAPI UnregisterInputChannel(
        /* [in] */ IInputChannel* _inputChannel);

    CARAPI_(Int32) InjectInputEvent(
        /* [in] */ IInputEvent* event,
        /* [in] */ Int32 injectorPid,
        /* [in] */ Int32 injectorUid,
        /* [in] */ Int32 syncMode,
        /* [in] */ Int32 timeoutMillis);

    CARAPI SetInputWindows(
        /* [in] */ InputWindow** windows);

    CARAPI SetFocusedApplication(
        /* [in] */ InputApplication* application);

    CARAPI SetInputDispatchMode(
        /* [in] */ Boolean enabled,
        /* [in] */ Boolean frozen);

private:
    CARAPI Init();

private:
    static const char* TAG;

    static const Boolean DEBUG = FALSE;

    InputManagerCallbacks* mCallbacks;
    AutoPtr<IContext> mContext;
    AutoPtr<CWindowManagerService> mWindowManagerService;
};

#endif //__INPUTMANAGER_H__