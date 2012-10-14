
#include "server/InputManager.h"
#include "server/NativeInputManager.h"
#include "view/NativeInputChannel.h"
#include <Slogger.h>
#include <StringBuffer.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

static android::sp<NativeInputManager> gNativeInputManager;

const Int32 InputManager::INPUT_EVENT_INJECTION_SUCCEEDED;
const Int32 InputManager::INPUT_EVENT_INJECTION_PERMISSION_DENIED;
const Int32 InputManager::INPUT_EVENT_INJECTION_FAILED;
const Int32 InputManager::INPUT_EVENT_INJECTION_TIMED_OUT;
const Int32 InputManager::INPUT_EVENT_INJECTION_SYNC_NONE;
const Int32 InputManager::INPUT_EVENT_INJECTION_SYNC_WAIT_FOR_RESULT;
const Int32 InputManager::INPUT_EVENT_INJECTION_SYNC_WAIT_FOR_FINISH;
const Int32 InputManager::KEY_STATE_UNKNOWN;
const Int32 InputManager::KEY_STATE_UP;
const Int32 InputManager::KEY_STATE_DOWN;
const Int32 InputManager::KEY_STATE_VIRTUAL;
const char* InputManager::TAG = "InputManager";
const Boolean InputManager::DEBUG;

static Boolean CheckInputManagerUnitialized()
{
    if (gNativeInputManager == NULL) {
        Slogger::E("InputManager", "Input manager not initialized.");
        return TRUE;
    }
    return FALSE;
}

static void InputManager_handleInputChannelDisposed(
    /* [in] */ IInputChannel* inputChannelObj,
    /* [in] */ const android::sp<android::InputChannel>& inputChannel,
    /* [in] */ void* data)
{
    Slogger::W("InputManager", StringBuffer("Input channel object")
            + inputChannel->getName().string()
            + "was disposed without first being unregistered with "
            + "the input manager!");

    if (gNativeInputManager != NULL) {
        gNativeInputManager->unregisterInputChannel(inputChannel);
    }
}

InputManager::InputManager(
    /* [in] */ IContext* context,
    /* [in] */ CWindowManagerService* windowManagerService)
{
    mContext = context;
    mWindowManagerService = windowManagerService;

    mCallbacks = new InputManagerCallbacks(context, windowManagerService);

    assert(SUCCEEDED(Init()));
}

ECode InputManager::Init()
{
    if (gNativeInputManager == NULL) {
        Slogger::I(TAG, "Initializing input manager");
        gNativeInputManager = new NativeInputManager(mCallbacks);
        return NOERROR;
    } else {
        Slogger::E(TAG, "Input manager already initialized.");
        return E_RUNTIME_EXCEPTION;
    }
}

ECode InputManager::Start()
{
    if (CheckInputManagerUnitialized()) {
        return E_RUNTIME_EXCEPTION;
    }

    Slogger::I(TAG, "Starting input manager");
    android::status_t result = gNativeInputManager->getInputManager()->start();
    if (result) {
        Slogger::E(TAG, "Input manager could not be started.");
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode InputManager::SetDisplaySize(
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (width <= 0 || height <= 0) {
        Slogger::E(TAG, "Invalid display id or dimensions.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (DEBUG) {
        Slogger::D(TAG, StringBuffer("Setting display #")
                + displayId + " size to " + width + "x" + height);
    }

    if (CheckInputManagerUnitialized()) {
        return E_RUNTIME_EXCEPTION;
    }

    // XXX we could get this from the SurfaceFlinger directly instead of requiring it
    // to be passed in like this, not sure which is better but leaving it like this
    // keeps the window manager in direct control of when display transitions propagate down
    // to the input dispatcher
    gNativeInputManager->setDisplaySize(displayId, width, height);
    return NOERROR;
}

ECode InputManager::SetDisplayOrientation(
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 rotation)
{
    if (rotation < Surface_ROTATION_0 || rotation > Surface_ROTATION_270) {
        Slogger::E(TAG, "Invalid rotation.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (DEBUG) {
        Slogger::D(TAG, StringBuffer("Setting display #")
                + displayId + " orientation to " + rotation);
    }

    if (CheckInputManagerUnitialized()) {
        return E_RUNTIME_EXCEPTION;
    }

    gNativeInputManager->setDisplayOrientation(displayId, rotation);
    return NOERROR;
}

ECode InputManager::GetInputConfiguration(
    /* [in] */ IConfiguration* _config)
{
    if (_config == NULL) {
        Slogger::E(TAG, "config must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (CheckInputManagerUnitialized()) {
        return E_RUNTIME_EXCEPTION;
    }

    android::InputConfiguration config;
    gNativeInputManager->getInputManager()->getReader()->getInputConfiguration(&config);

    _config->SetTouchscreen(config.touchScreen);
    _config->SetKeyboard(config.keyboard);
    _config->SetNavigation(config.navigation);
    return NOERROR;
}

/**
 * Gets the current state of a key or button by key code.
 * @param deviceId The input device id, or -1 to consult all devices.
 * @param sourceMask The input sources to consult, or {@link InputDevice#SOURCE_ANY} to
 * consider all input sources.  An input device is consulted if at least one of its
 * non-class input source bits matches the specified source mask.
 * @param keyCode The key code to check.
 * @return The key state.
 */
Int32 InputManager::GetKeyCodeState(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 sourceMask,
    /* [in] */ Int32 keyCode)
{
    if (CheckInputManagerUnitialized()) {
        return AKEY_STATE_UNKNOWN;
    }

    return gNativeInputManager->getInputManager()->getReader()->getKeyCodeState(
            deviceId, uint32_t(sourceMask), keyCode);
}

/**
 * Gets the current state of a key or button by scan code.
 * @param deviceId The input device id, or -1 to consult all devices.
 * @param sourceMask The input sources to consult, or {@link InputDevice#SOURCE_ANY} to
 * consider all input sources.  An input device is consulted if at least one of its
 * non-class input source bits matches the specified source mask.
 * @param scanCode The scan code to check.
 * @return The key state.
 */
Int32 InputManager::GetScanCodeState(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 sourceMask,
    /* [in] */ Int32 scanCode)
{
    if (CheckInputManagerUnitialized()) {
        return AKEY_STATE_UNKNOWN;
    }

    return gNativeInputManager->getInputManager()->getReader()->getScanCodeState(
            deviceId, uint32_t(sourceMask), scanCode);
}

/**
 * Gets the current state of a switch by switch code.
 * @param deviceId The input device id, or -1 to consult all devices.
 * @param sourceMask The input sources to consult, or {@link InputDevice#SOURCE_ANY} to
 * consider all input sources.  An input device is consulted if at least one of its
 * non-class input source bits matches the specified source mask.
 * @param switchCode The switch code to check.
 * @return The switch state.
 */
Int32 InputManager::GetSwitchState(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 sourceMask,
    /* [in] */ Int32 switchCode)
{
    if (CheckInputManagerUnitialized()) {
        return AKEY_STATE_UNKNOWN;
    }

    return gNativeInputManager->getInputManager()->getReader()->getSwitchState(
            deviceId, uint32_t(sourceMask), switchCode);
}

/**
 * Determines whether the specified key codes are supported by a particular device.
 * @param deviceId The input device id, or -1 to consult all devices.
 * @param sourceMask The input sources to consult, or {@link InputDevice#SOURCE_ANY} to
 * consider all input sources.  An input device is consulted if at least one of its
 * non-class input source bits matches the specified source mask.
 * @param keyCodes The array of key codes to check.
 * @param keyExists An array at least as large as keyCodes whose entries will be set
 * to true or false based on the presence or absence of support for the corresponding
 * key codes.
 * @return True if the lookup was successful, false otherwise.
 */
//public Boolean InputManager::HasKeys(
//    /* [in] */ Int32 deviceId,
//    /* [in] */ Int32 sourceMask,
//    /* [in] */ Int32[] keyCodes,
//    /* [in] */ Boolean[] keyExists)
//{
//    if (keyCodes == null) {
//        throw new IllegalArgumentException("keyCodes must not be null.");
//    }
//    if (keyExists == null || keyExists.length < keyCodes.length) {
//        throw new IllegalArgumentException("keyExists must not be null and must be at "
//                + "least as large as keyCodes.");
//    }
//
//    return nativeHasKeys(deviceId, sourceMask, keyCodes, keyExists);
//
//    if (checkInputManagerUnitialized(env)) {
//        return JNI_FALSE;
//    }
//
//    int32_t* codes = env->GetIntArrayElements(keyCodes, NULL);
//    uint8_t* flags = env->GetBooleanArrayElements(outFlags, NULL);
//    jsize numCodes = env->GetArrayLength(keyCodes);
//    jboolean result;
//    if (numCodes == env->GetArrayLength(keyCodes)) {
//        result = gNativeInputManager->getInputManager()->getReader()->hasKeys(
//                deviceId, uint32_t(sourceMask), numCodes, codes, flags);
//    } else {
//        result = JNI_FALSE;
//    }
//
//    env->ReleaseBooleanArrayElements(outFlags, flags, 0);
//    env->ReleaseIntArrayElements(keyCodes, codes, 0);
//    return result;
//}

/**
 * Creates an input channel that will receive all input from the input dispatcher.
 * @param inputChannelName The input channel name.
 * @return The input channel.
 */
ECode InputManager::MonitorInput(
    /* [in] */ const String& inputChannelName,
    /* [out] */ IInputChannel** _inputChannel)
{
    assert(_inputChannel != NULL);

    if (inputChannelName.IsNull()) {
        Slogger::E(TAG, "inputChannelName must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IInputChannel> inputChannel0, inputChannel1;
    AutoPtr<IInputChannelHelper> helper;
    CInputChannelHelper::AcquireSingleton((IInputChannelHelper**)&helper);
    helper->OpenInputChannelPair(
            inputChannelName,
            (IInputChannel**)&inputChannel0,
            (IInputChannel**)&inputChannel1);

    if (CheckInputManagerUnitialized()) {
        return E_RUNTIME_EXCEPTION;
    }

    NativeInputChannel* nativeInputChannel;
    inputChannel0->GetNativeInputChannel((Handle32*)&nativeInputChannel);
    android::sp<android::InputChannel> inputChannel =
            nativeInputChannel != NULL ? nativeInputChannel->getInputChannel() : NULL;
    if (inputChannel == NULL) {
        Slogger::E(TAG, "inputChannel is not initialized.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    android::status_t status = gNativeInputManager->registerInputChannel(
            inputChannel, inputChannel0, TRUE);
    if (status) {
        Slogger::E(TAG, "Failed to register input channel.  "
                "Check logs for details.");
        return E_RUNTIME_EXCEPTION;
    }

    inputChannel0->Dispose(); // don't need to retain the Java object reference

    *_inputChannel = (IInputChannel*)inputChannel1.Get();
    if (*_inputChannel != NULL) (*_inputChannel)->AddRef();
    return NOERROR;
}

/**
 * Registers an input channel so that it can be used as an input event target.
 * @param inputChannel The input channel to register.
 */
ECode InputManager::RegisterInputChannel(
    /* [in] */ IInputChannel* _inputChannel)
{
    if (_inputChannel == NULL) {
        Slogger::E(TAG, "inputChannel must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (CheckInputManagerUnitialized()) {
        return E_RUNTIME_EXCEPTION;
    }

    NativeInputChannel* nativeInputChannel;
    _inputChannel->GetNativeInputChannel((Handle32*)&nativeInputChannel);
    android::sp<android::InputChannel> inputChannel =
            nativeInputChannel != NULL ? nativeInputChannel->getInputChannel() : NULL;
    if (inputChannel == NULL) {
        Slogger::E(TAG, "inputChannel is not initialized.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    android::status_t status = gNativeInputManager->registerInputChannel(
            inputChannel, _inputChannel, FALSE);
    if (status) {
        Slogger::E(TAG, "Failed to register input channel.  "
                "Check logs for details.");
        return E_RUNTIME_EXCEPTION;
    }

    if (nativeInputChannel == NULL) {
        Slogger::W(TAG, "Cannot set dispose callback because input "
                "channel object has not been initialized.");
    } else {
        nativeInputChannel->setDisposeCallback(
                InputManager_handleInputChannelDisposed, NULL);
    }
    return NOERROR;
}

/**
 * Unregisters an input channel.
 * @param inputChannel The input channel to unregister.
 */
ECode InputManager::UnregisterInputChannel(
    /* [in] */ IInputChannel* _inputChannel)
{
    if (_inputChannel == NULL) {
        Slogger::E(TAG, "inputChannel must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (CheckInputManagerUnitialized()) {
        return E_RUNTIME_EXCEPTION;
    }

    NativeInputChannel* nativeInputChannel;
    _inputChannel->GetNativeInputChannel((Handle32*)&nativeInputChannel);
    android::sp<android::InputChannel> inputChannel =
            nativeInputChannel != NULL ? nativeInputChannel->getInputChannel() : NULL;
    if (inputChannel == NULL) {
        Slogger::E(TAG, "inputChannel is not initialized.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (nativeInputChannel == NULL) {
        Slogger::W(TAG, "Cannot set dispose callback because input "
                "channel object has not been initialized.");
    } else {
        nativeInputChannel->setDisposeCallback(NULL, NULL);
    }

    android::status_t status = gNativeInputManager->unregisterInputChannel(inputChannel);
    if (status) {
        Slogger::E(TAG, "Failed to unregister input channel.  "
                "Check logs for details.");
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

/**
 * Injects an input event into the event system on behalf of an application.
 * The synchronization mode determines whether the method blocks while waiting for
 * input injection to proceed.
 *
 * {@link #INPUT_EVENT_INJECTION_SYNC_NONE} never blocks.  Injection is asynchronous and
 * is assumed always to be successful.
 *
 * {@link #INPUT_EVENT_INJECTION_SYNC_WAIT_FOR_RESULT} waits for previous events to be
 * dispatched so that the input dispatcher can determine whether input event injection will
 * be permitted based on the current input focus.  Does not wait for the input event to
 * finish processing.
 *
 * {@link #INPUT_EVENT_INJECTION_SYNC_WAIT_FOR_FINISH} waits for the input event to
 * be completely processed.
 *
 * @param event The event to inject.
 * @param injectorPid The pid of the injecting application.
 * @param injectorUid The uid of the injecting application.
 * @param syncMode The synchronization mode.
 * @param timeoutMillis The injection timeout in milliseconds.
 * @return One of the INPUT_EVENT_INJECTION_XXX constants.
 */
//public int injectInputEvent(InputEvent event, int injectorPid, int injectorUid,
//        int syncMode, int timeoutMillis) {
//    if (event == null) {
//        throw new IllegalArgumentException("event must not be null");
//    }
//    if (injectorPid < 0 || injectorUid < 0) {
//        throw new IllegalArgumentException("injectorPid and injectorUid must not be negative.");
//    }
//    if (timeoutMillis <= 0) {
//        throw new IllegalArgumentException("timeoutMillis must be positive");
//    }
//
//    return nativeInjectInputEvent(event, injectorPid, injectorUid, syncMode, timeoutMillis);
//}

ECode InputManager::SetInputWindows(
        /* [in] */ InputWindow** windows)
{
    if (CheckInputManagerUnitialized()) {
        return E_RUNTIME_EXCEPTION;
    }

    gNativeInputManager->setInputWindows(windows);
    return NOERROR;
}

ECode InputManager::SetFocusedApplication(
        /* [in] */ InputApplication* application)
{
    if (CheckInputManagerUnitialized()) {
        return E_RUNTIME_EXCEPTION;
    }

    gNativeInputManager->setFocusedApplication(application);
    return NOERROR;
}

ECode InputManager::SetInputDispatchMode(
    /* [in] */ Boolean enabled,
    /* [in] */ Boolean frozen)
{
    if (CheckInputManagerUnitialized()) {
        return E_RUNTIME_EXCEPTION;
    }

    gNativeInputManager->setInputDispatchMode(enabled, frozen);
    return NOERROR;
}
