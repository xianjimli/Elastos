
#include "server/NativeInputManager.h"
#include <Slogger.h>

using namespace Elastos::Utility::Logging;

#define UNUSED(x) ((void)x)

const char* NativeInputManager::TAG = "NativeInputManager";

NativeInputManager::NativeInputManager(
    /* [in] */ InputManagerCallbacks* callbacks) :
    mFilterTouchEvents(-1),
    mFilterJumpyTouchEvents(-1),
    mVirtualKeyQuietTime(-1),
    mMaxEventsPerSecond(-1),
    mDisplayWidth(-1),
    mDisplayHeight(-1),
    mDisplayOrientation(ROTATION_0)
{
    mCallbacks = callbacks;

    android::sp<android::EventHub> eventHub = new android::EventHub();
    mInputManager = new android::InputManager(eventHub, this, this);
}

NativeInputManager::~NativeInputManager()
{
}

void NativeInputManager::setDisplaySize(
    /* [in] */ int32_t displayId,
    /* [in] */ int32_t width,
    /* [in] */ int32_t height)
{
    if (displayId == 0) {
        android::AutoMutex _l(mDisplayLock);

        mDisplayWidth = width;
        mDisplayHeight = height;
    }
}

void NativeInputManager::setDisplayOrientation(
    /* [in] */ int32_t displayId,
    /* [in] */ int32_t orientation)
{
    if (displayId == 0) {
        android::AutoMutex _l(mDisplayLock);

        mDisplayOrientation = orientation;
    }
}

android::status_t NativeInputManager::registerInputChannel(
    /* [in] */ const android::sp<android::InputChannel>& inputChannel,
    /* [in] */ CInputChannel* inputChannelObj,
    /* [in] */ bool monitor)
{
    assert(inputChannelObj != NULL);

    android::status_t status;
    {
        android::AutoMutex _l(mInputChannelRegistryLock);

        ssize_t index = mInputChannelObjWeakTable.indexOfKey(inputChannel.get());
        if (index >= 0) {
            Slogger::E(TAG, StringBuffer("Input channel object ")
                    + inputChannel->getName().string()
                    + "has already been registered.");
            return android::INVALID_OPERATION;
        }

        mInputChannelObjWeakTable.add(inputChannel.get(), inputChannelObj);
    }

    status = mInputManager->getDispatcher()->registerInputChannel(inputChannel, monitor);
    if (! status) {
        // Success.
        return android::OK;
    }

    // Failed!
    {
        android::AutoMutex _l(mInputChannelRegistryLock);
        mInputChannelObjWeakTable.removeItem(inputChannel.get());
    }

    return status;
}

android::status_t NativeInputManager::unregisterInputChannel(
    /* [in] */ const android::sp<android::InputChannel>& inputChannel)
{
    CInputChannel* inputChannelObj;
    UNUSED(inputChannelObj);
    {
        android::AutoMutex _l(mInputChannelRegistryLock);

        ssize_t index = mInputChannelObjWeakTable.indexOfKey(inputChannel.get());
        if (index < 0) {
            Slogger::E(TAG, StringBuffer("Input channel object ")
                    + inputChannel->getName().string()
                    + "is not currently registered.");
            return android::INVALID_OPERATION;
        }

        mInputChannelObjWeakTable.removeItemsAt(index);
    }

    return mInputManager->getDispatcher()->unregisterInputChannel(inputChannel);
}

CInputChannel* NativeInputManager::getInputChannelObjLocal(
    /* [in] */ const android::sp<android::InputChannel>& inputChannel)
{
    android::InputChannel* inputChannelPtr = inputChannel.get();
    if (! inputChannelPtr) {
        return NULL;
    }

    {
        android::AutoMutex _l(mInputChannelRegistryLock);

        ssize_t index = mInputChannelObjWeakTable.indexOfKey(inputChannelPtr);
        if (index < 0) {
            return NULL;
        }

        return mInputChannelObjWeakTable.valueAt(index);
    }
}

bool NativeInputManager::getDisplayInfo(
    /* [in] */ int32_t displayId,
    /* [in] */ int32_t* width,
    /* [in] */ int32_t* height,
    /* [in] */ int32_t* orientation)
{
    //printf("("@@@@@@FILE: %s, LINE: %d@@@@@@\n", __FILE__, __LINE__);
    bool result = false;
    if (displayId == 0) {
        android::AutoMutex _l(mDisplayLock);

        if (mDisplayWidth > 0) {
            if (width) {
                *width = mDisplayWidth;
            }
            if (height) {
                *height = mDisplayHeight;
            }
            if (orientation) {
                *orientation = mDisplayOrientation;
            }
            result = true;
        }
    }
    return result;
}

bool NativeInputManager::filterTouchEvents()
{
    //printf("@@@@@@FILE: %s, LINE: %d@@@@@@\n", __FILE__, __LINE__);
    if (mFilterTouchEvents < 0) {
        Boolean result;
        if (FAILED(mCallbacks->FilterTouchEvents(&result))) {
            result = FALSE;
        }

        mFilterTouchEvents = result ? 1 : 0;
    }
    return mFilterTouchEvents;
}

bool NativeInputManager::filterJumpyTouchEvents()
{
    //printf("("@@@@@@FILE: %s, LINE: %d@@@@@@\n", __FILE__, __LINE__);
    if (mFilterJumpyTouchEvents < 0) {
        Boolean result;
        if (FAILED(mCallbacks->FilterJumpyTouchEvents(&result))) {
            result = FALSE;
        }

        mFilterJumpyTouchEvents = result ? 1 : 0;
    }
    return mFilterJumpyTouchEvents;
}

nsecs_t NativeInputManager::getVirtualKeyQuietTime()
{
    //printf("("@@@@@@FILE: %s, LINE: %d@@@@@@\n", __FILE__, __LINE__);
    if (mVirtualKeyQuietTime < 0) {
        Int32 result;
        if (FAILED(mCallbacks->GetVirtualKeyQuietTimeMillis(&result))) {
            result = 0;
        }
        if (result < 0) {
            result = 0;
        }

        mVirtualKeyQuietTime = milliseconds_to_nanoseconds(result);
    }
    return mVirtualKeyQuietTime;
}

void NativeInputManager::getVirtualKeyDefinitions(
    /* [in] */ const android::String8& deviceName,
    /* [in] */ android::Vector<android::VirtualKeyDefinition>& outVirtualKeyDefinitions)
{
    //printf("("@@@@@@FILE: %s, LINE: %d@@@@@@\n", __FILE__, __LINE__);
    outVirtualKeyDefinitions.clear();

    ArrayOf<VirtualKeyDefinition>* result;
    if (SUCCEEDED(mCallbacks->GetVirtualKeyDefinitions(String(deviceName.string()), &result))) {
        for (Int32 i = 0; i < result->GetLength(); i++) {
            VirtualKeyDefinition item = (*result)[i];

            outVirtualKeyDefinitions.add();
            outVirtualKeyDefinitions.editTop().scanCode = item.mScanCode;
            outVirtualKeyDefinitions.editTop().centerX = item.mCenterX;
            outVirtualKeyDefinitions.editTop().centerY = item.mCenterY;
            outVirtualKeyDefinitions.editTop().width = item.mWidth;
            outVirtualKeyDefinitions.editTop().height = item.mHeight;
        }

        //todo: release the result;
        ArrayOf<VirtualKeyDefinition>::Free(result);
    }
}

void NativeInputManager::getInputDeviceCalibration(
    /* [in] */ const android::String8& deviceName,
    /* [in] */ android::InputDeviceCalibration& outCalibration)
{
    //printf("("@@@@@@FILE: %s, LINE: %d@@@@@@\n", __FILE__, __LINE__);
    outCalibration.clear();

    InputDeviceCalibration* result;
    if (SUCCEEDED(mCallbacks->GetInputDeviceCalibration(String(deviceName.string()), &result))) {
        ArrayOf<String>* keys = result->mKeys;
        ArrayOf<String>* values = result->mValues;

        for (Int32 i = 0; i < keys->GetLength(); i++) {
            const char* keyChars = (*keys)[i];
            android::String8 key(keyChars);

            const char* valueChars = (*values)[i];
            android::String8 value(valueChars);

            outCalibration.addProperty(key, value);
        }

        //todo: release result
    }
}

void NativeInputManager::getExcludedDeviceNames(
    /* [in] */ android::Vector<android::String8>& outExcludedDeviceNames)
{
    //printf("("@@@@@@FILE: %s, LINE: %d@@@@@@\n", __FILE__, __LINE__);
    outExcludedDeviceNames.clear();

    ArrayOf<String>* result;
    if (SUCCEEDED(mCallbacks->GetExcludedDeviceNames(&result))) {
        for (Int32 i = 0; i < result->GetLength(); i++) {
            const char* deviceNameChars = (*result)[i];
            outExcludedDeviceNames.add(android::String8(deviceNameChars));
        }

        //todo: release result
    }
}

void NativeInputManager::notifySwitch(
        /* [in] */ nsecs_t when,
        /* [in] */ int32_t switchCode,
        /* [in] */ int32_t switchValue,
        /* [in] */ uint32_t policyFlags)
{
    //printf("("@@@@@@FILE: %s, LINE: %d@@@@@@\n", __FILE__, __LINE__);
//#if DEBUG_INPUT_DISPATCHER_POLICY
//    LOGD("notifySwitch - when=%lld, switchCode=%d, switchValue=%d, policyFlags=0x%x",
//            when, switchCode, switchValue, policyFlags);
//#endif

    switch (switchCode) {
    case SW_LID:
        if (FAILED(mCallbacks->NotifyLidSwitchChanged(when, switchValue == 0))) {
            //todo: log
        }
        break;
    default:
        assert(FALSE);
    }
}

void NativeInputManager::notifyConfigurationChanged(
    /* [in] */ nsecs_t when)
{
    //printf("("@@@@@@FILE: %s, LINE: %d@@@@@@\n", __FILE__, __LINE__);
//#if DEBUG_INPUT_DISPATCHER_POLICY
//    LOGD("notifyConfigurationChanged - when=%lld", when);
//#endif

    if (FAILED(mCallbacks->NotifyConfigurationChanged(when))) {
        //todo: log
    }
}

nsecs_t NativeInputManager::notifyANR(
    /* [in] */ const android::sp<android::InputApplicationHandle>& inputApplicationHandle,
    /* [in] */ const android::sp<android::InputChannel>& inputChannel)
{
    //printf("("@@@@@@FILE: %s, LINE: %d@@@@@@\n", __FILE__, __LINE__);
//#if DEBUG_INPUT_DISPATCHER_POLICY
//    LOGD("notifyANR");
//#endif

    void* tokenObjLocal;
    if (inputApplicationHandle.get()) {
        ApplicationToken* token = static_cast<ApplicationToken*>(inputApplicationHandle.get());
        tokenObjLocal = token->GetTokenObj();
    } else {
        tokenObjLocal = NULL;
    }

    Int64 newTimeout;
    CInputChannel* inputChannelObjLocal = getInputChannelObjLocal(inputChannel);
    if (SUCCEEDED(mCallbacks->NotifyANR(tokenObjLocal,
            inputChannelObjLocal, &newTimeout))) {
        assert(newTimeout >= 0);
    }
    else {
        newTimeout = 0; // abort dispatch
    }

    return newTimeout;
}

void NativeInputManager::notifyInputChannelBroken(
    /* [in] */ const android::sp<android::InputChannel>& inputChannel)
{
    //printf("("@@@@@@FILE: %s, LINE: %d@@@@@@\n", __FILE__, __LINE__);
//#if DEBUG_INPUT_DISPATCHER_POLICY
//    LOGD("notifyInputChannelBroken - inputChannel='%s'", inputChannel->getName().string());
//#endif

    CInputChannel* inputChannelObjLocal = getInputChannelObjLocal(inputChannel);
    if (inputChannelObjLocal) {
        mCallbacks->NotifyInputChannelBroken(inputChannelObjLocal);
    }
}

nsecs_t NativeInputManager::getKeyRepeatTimeout()
{
    //printf("("@@@@@@FILE: %s, LINE: %d@@@@@@\n", __FILE__, __LINE__);
    if (! isScreenOn()) {
        // Disable key repeat when the screen is off.
        return -1;
    } else {
        // TODO use ViewConfiguration.getLongPressTimeout()
        return milliseconds_to_nanoseconds(500);
    }
}

nsecs_t NativeInputManager::getKeyRepeatDelay()
{
    //printf("("@@@@@@FILE: %s, LINE: %d@@@@@@\n", __FILE__, __LINE__);
    return milliseconds_to_nanoseconds(50);
}

int32_t NativeInputManager::getMaxEventsPerSecond()
{
    //printf("("@@@@@@FILE: %s, LINE: %d@@@@@@\n", __FILE__, __LINE__);
    if (mMaxEventsPerSecond < 0) {
        Int32 result;

        if (FAILED(mCallbacks->GetMaxEventsPerSecond(&result))) {
            result = 60;
        }

        mMaxEventsPerSecond = result;
    }
    return mMaxEventsPerSecond;
}

void NativeInputManager::setInputWindows(
    /* [in] */ InputWindow** windowObjArray)
{
    android::Vector<android::InputWindow> windows;

    InputWindow* inputTargetObj;
    Int32 i = 0;
    while ((inputTargetObj = windowObjArray[i])!= NULL) {
        windows.push();
        android::InputWindow& window = windows.editTop();
        Boolean valid = populateWindow(inputTargetObj, window);
        if (!valid) {
            windows.pop();
        }
        i++;
    }

    mInputManager->getDispatcher()->setInputWindows(windows);
}

void NativeInputManager::setFocusedApplication(
    /* [in] */ InputApplication* application)
{
}

void NativeInputManager::setInputDispatchMode(
    /* [in] */ Boolean enabled,
    /* [in] */ Boolean frozen)
{
}

bool NativeInputManager::populateWindow(
    /* [in] */ InputWindow* windowObj,
    /* [in] */ android::InputWindow& outWindow)
{
    bool valid = false;

    AutoPtr<CInputChannel> inputChannelObj = windowObj->mInputChannel;
    if (inputChannelObj) {
        NativeInputChannel* nativeInputChannel = inputChannelObj->mPtr;
        android::sp<android::InputChannel> inputChannel =
                nativeInputChannel != NULL ? nativeInputChannel->getInputChannel() : NULL;
        if (inputChannel != NULL) {
            outWindow.inputChannel = inputChannel;
            outWindow.name.setTo((const char*)windowObj->mName);
            outWindow.layoutParamsFlags = windowObj->mLayoutParamsFlags;
            outWindow.layoutParamsType = windowObj->mLayoutParamsType;
            outWindow.dispatchingTimeout = windowObj->mDispatchingTimeoutNanos;
            outWindow.frameLeft = windowObj->mFrameLeft;
            outWindow.frameTop = windowObj->mFrameTop;
            outWindow.frameRight = windowObj->mFrameRight;
            outWindow.frameBottom = windowObj->mFrameBottom;
            outWindow.visibleFrameLeft = windowObj->mVisibleFrameLeft;
            outWindow.visibleFrameTop = windowObj->mVisibleFrameTop;
            outWindow.visibleFrameRight = windowObj->mVisibleFrameRight;
            outWindow.visibleFrameBottom = windowObj->mVisibleFrameBottom;
            outWindow.touchableAreaLeft = windowObj->mTouchableAreaLeft;
            outWindow.touchableAreaTop = windowObj->mTouchableAreaTop;
            outWindow.touchableAreaRight = windowObj->mTouchableAreaRight;
            outWindow.touchableAreaBottom = windowObj->mTouchableAreaBottom;
            outWindow.visible = windowObj->mVisible;
            outWindow.canReceiveKeys = windowObj->mCanReceiveKeys;
            outWindow.hasFocus = windowObj->mHasFocus;
            outWindow.hasWallpaper = windowObj->mHasWallpaper;
            outWindow.paused = windowObj->mPaused;
            outWindow.layer = windowObj->mLayer;
            outWindow.ownerPid = windowObj->mOwnerPid;
            outWindow.ownerUid = windowObj->mOwnerUid;

            valid = true;
        } else {
//            LOGW("Dropping input target because its input channel is not initialized.");
        }
    } else {
//        LOGW("Dropping input target because the input channel object was null.");
    }
    return valid;
}

bool NativeInputManager::isScreenOn()
{
//    return android_server_PowerManagerService_isScreenOn();
    return true;
}

bool NativeInputManager::isScreenBright()
{
//    return android_server_PowerManagerService_isScreenBright();
    return true;
}

void NativeInputManager::interceptKeyBeforeQueueing(
    /* [in] */ nsecs_t when,
    /* [in] */ int32_t deviceId,
    /* [in] */ int32_t action,
    /* [in] */ int32_t& flags,
    /* [in] */ int32_t keyCode,
    /* [in] */ int32_t scanCode,
    /* [in] */ uint32_t& policyFlags)
{
    //printf("@@@@@@FILE: %s, LINE: %d@@@@@@\n", __FILE__, __LINE__);
//#if DEBUG_INPUT_DISPATCHER_POLICY
//    LOGD("interceptKeyBeforeQueueing - when=%lld, deviceId=%d, action=%d, flags=%d, "
//            "keyCode=%d, scanCode=%d, policyFlags=0x%x",
//            when, deviceId, action, flags, keyCode, scanCode, policyFlags);
//#endif

    if ((policyFlags & android::POLICY_FLAG_VIRTUAL) || (flags & AKEY_EVENT_FLAG_VIRTUAL_HARD_KEY)) {
        policyFlags |= android::POLICY_FLAG_VIRTUAL;
        flags |= AKEY_EVENT_FLAG_VIRTUAL_HARD_KEY;
    }

    // Policy:
    // - Ignore untrusted events and pass them along.
    // - Ask the window manager what to do with normal events and trusted injected events.
    // - For normal events wake and brighten the screen if currently off or dim.
    if ((policyFlags & android::POLICY_FLAG_TRUSTED)) {
        const int32_t WM_ACTION_PASS_TO_USER = 1;
        const int32_t WM_ACTION_POKE_USER_ACTIVITY = 2;
        const int32_t WM_ACTION_GO_TO_SLEEP = 4;

        bool isScreenOn = this->isScreenOn();
        bool isScreenBright = this->isScreenBright();

        Int32 wmActions;
        if (FAILED(mCallbacks->InterceptKeyBeforeQueueing(when, action, flags, keyCode,
            scanCode, policyFlags, isScreenOn, &wmActions))) {
            wmActions = 0;
        }

        if (!(flags & android::POLICY_FLAG_INJECTED)) {
            if (!isScreenOn) {
                policyFlags |= android::POLICY_FLAG_WOKE_HERE;
                flags |= AKEY_EVENT_FLAG_WOKE_HERE;
            }

            if (!isScreenBright) {
                policyFlags |= android::POLICY_FLAG_BRIGHT_HERE;
            }
        }

        if (wmActions & WM_ACTION_GO_TO_SLEEP) {
//            android_server_PowerManagerService_goToSleep(when);
        }

        if (wmActions & WM_ACTION_POKE_USER_ACTIVITY) {
//            android_server_PowerManagerService_userActivity(when, POWER_MANAGER_BUTTON_EVENT);
        }

        if (wmActions & WM_ACTION_PASS_TO_USER) {
            policyFlags |= android::POLICY_FLAG_PASS_TO_USER;
        }
    } else {
        policyFlags |= android::POLICY_FLAG_PASS_TO_USER;
    }
}

void NativeInputManager::interceptGenericBeforeQueueing(
    /* [in] */ nsecs_t when,
    /* [in] */ uint32_t& policyFlags)
{
    //printf("@@@@@@FILE: %s, LINE: %d@@@@@@\n", __FILE__, __LINE__);
//#if DEBUG_INPUT_DISPATCHER_POLICY
//    LOGD("interceptGenericBeforeQueueing - when=%lld, policyFlags=0x%x", when, policyFlags);
//#endif

    // Policy:
    // - Ignore untrusted events and pass them along.
    // - No special filtering for injected events required at this time.
    // - Filter normal events based on screen state.
    // - For normal events brighten (but do not wake) the screen if currently dim.
    if ((policyFlags & android::POLICY_FLAG_TRUSTED) && !(policyFlags & android::POLICY_FLAG_INJECTED)) {
        if (isScreenOn()) {
            policyFlags |= android::POLICY_FLAG_PASS_TO_USER;

            if (!isScreenBright()) {
                policyFlags |= android::POLICY_FLAG_BRIGHT_HERE;
            }
        }
    } else {
        policyFlags |= android::POLICY_FLAG_PASS_TO_USER;
    }
}

bool NativeInputManager::interceptKeyBeforeDispatching(
    /* [in] */ const android::sp<android::InputChannel>& inputChannel,
    /* [in] */ const android::KeyEvent* keyEvent,
    /* [in] */ uint32_t policyFlags)
{
    //printf("@@@@@@FILE: %s, LINE: %d@@@@@@\n", __FILE__, __LINE__);
    // Policy:
    // - Ignore untrusted events and pass them along.
    // - Filter normal events and trusted injected events through the window manager policy to
    //   handle the HOME key and the like.
    if (policyFlags & android::POLICY_FLAG_TRUSTED) {

        // Note: inputChannel may be null.
        Boolean consumed;
        CInputChannel* inputChannelObj = getInputChannelObjLocal(inputChannel);
        ECode ec = mCallbacks->InterceptKeyBeforeDispatching(inputChannelObj, keyEvent->getAction(),
                keyEvent->getFlags(), keyEvent->getKeyCode(), keyEvent->getScanCode(),
                keyEvent->getMetaState(), keyEvent->getRepeatCount(), policyFlags, &consumed);

        return (bool)(consumed && SUCCEEDED(ec));
    } else {
        return false;
    }
}

void NativeInputManager::pokeUserActivity(
    /* [in] */ nsecs_t eventTime,
    /* [in] */ int32_t eventType)
{
    //printf("("@@@@@@FILE: %s, LINE: %d@@@@@@\n", __FILE__, __LINE__);
//    android_server_PowerManagerService_userActivity(eventTime, eventType);
}

bool NativeInputManager::checkInjectEventsPermissionNonReentrant(
    /* [in] */ int32_t injectorPid,
    /* [in] */ int32_t injectorUid)
{
    //printf("("@@@@@@FILE: %s, LINE: %d@@@@@@\n", __FILE__, __LINE__);
    Boolean result;
    mCallbacks->CheckInjectEventsPermission(injectorPid, injectorUid, &result);
    return (bool)result;
}
