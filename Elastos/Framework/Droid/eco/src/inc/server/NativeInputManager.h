
#ifndef __NATIVEINPUTMANAGER_H__
#define __NATIVEINPUTMANAGER_H__

#include <ui/InputReader.h>
#include <ui/InputDispatcher.h>
#include <ui/InputManager.h>
#include "server/InputManagerCallbacks.h"
#include "view/CInputChannel.h"

class NativeInputManager :
    public virtual android::RefBase,
    public virtual android::InputReaderPolicyInterface,
    public virtual android::InputDispatcherPolicyInterface
{
private:
    static const char* TAG;

private:
    class ApplicationToken : public android::InputApplicationHandle
    {
    public:
        ApplicationToken(
            /* [in] */ void* tokenObj) :
            mTokenObj(tokenObj) { }

        virtual ~ApplicationToken() {
        }

        inline void* GetTokenObj() { return mTokenObj; }

    private:
        void* mTokenObj;
    };

protected:
    virtual ~NativeInputManager();

public:
    NativeInputManager(
        /* [in] */ InputManagerCallbacks* callbacks);

    inline android::sp<android::InputManager> getInputManager() const
    { return mInputManager; }

    void setDisplaySize(
        /* [in] */ int32_t displayId,
        /* [in] */ int32_t width,
        /* [in] */ int32_t height);

    void setDisplayOrientation(
        /* [in] */ int32_t displayId,
        /* [in] */ int32_t orientation);

    android::status_t registerInputChannel(
        /* [in] */ const android::sp<android::InputChannel>& inputChannel,
        /* [in] */ CInputChannel* inputChannelObj,
        /* [in] */ bool monitor);

    android::status_t unregisterInputChannel(
        /* [in] */ const android::sp<android::InputChannel>& inputChannel);

    void setInputWindows(
        /* [in] */ InputWindow** windows);

    void setFocusedApplication(
        /* [in] */ InputApplication* application);

    void setInputDispatchMode(
        /* [in] */ Boolean enabled,
        /* [in] */ Boolean frozen);

    /* --- InputReaderPolicyInterface implementation --- */

    virtual bool getDisplayInfo(
        /* [in] */ int32_t displayId,
        /* [in] */ int32_t* width,
        /* [in] */ int32_t* height,
        /* [in] */ int32_t* orientation);

    virtual bool filterTouchEvents();

    virtual bool filterJumpyTouchEvents();

    virtual nsecs_t getVirtualKeyQuietTime();

    virtual void getVirtualKeyDefinitions(
        /* [in] */ const android::String8& deviceName,
        /* [in] */ android::Vector<android::VirtualKeyDefinition>& outVirtualKeyDefinitions);

    virtual void getInputDeviceCalibration(
        /* [in] */ const android::String8& deviceName,
        /* [in] */ android::InputDeviceCalibration& outCalibration);

    virtual void getExcludedDeviceNames(
        /* [in] */ android::Vector<android::String8>& outExcludedDeviceNames);

    /* --- InputDispatcherPolicyInterface implementation --- */
    virtual void notifySwitch(
        /* [in] */ nsecs_t when,
        /* [in] */ int32_t switchCode,
        /* [in] */ int32_t switchValue,
        /* [in] */ uint32_t policyFlags);

    virtual void notifyConfigurationChanged(
        /* [in] */ nsecs_t when);

    virtual nsecs_t notifyANR(
        /* [in] */ const android::sp<android::InputApplicationHandle>& inputApplicationHandle,
        /* [in] */ const android::sp<android::InputChannel>& inputChannel);

    virtual void notifyInputChannelBroken(
        /* [in] */ const android::sp<android::InputChannel>& inputChannel);

    virtual nsecs_t getKeyRepeatTimeout();

    virtual nsecs_t getKeyRepeatDelay();

    virtual int32_t getMaxEventsPerSecond();

    virtual void interceptKeyBeforeQueueing(
        /* [in] */ nsecs_t when,
        /* [in] */ int32_t deviceId,
        /* [in] */ int32_t action,
        /* [in] */ int32_t& flags,
        /* [in] */ int32_t keyCode,
        /* [in] */ int32_t scanCode,
        /* [in] */ uint32_t& policyFlags);

    virtual void interceptGenericBeforeQueueing(
        /* [in] */ nsecs_t when,
        /* [in] */ uint32_t& policyFlags);

    virtual bool interceptKeyBeforeDispatching(
        /* [in] */ const android::sp<android::InputChannel>& inputChannel,
        /* [in] */ const android::KeyEvent* keyEvent,
        /* [in] */ uint32_t policyFlags);

    virtual void pokeUserActivity(
        /* [in] */ nsecs_t eventTime,
        /* [in] */ int32_t eventType);

    virtual bool checkInjectEventsPermissionNonReentrant(
        /* [in] */ int32_t injectorPid,
        /* [in] */ int32_t injectorUid);

    // Power manager interactions.
    bool isScreenOn();
    bool isScreenBright();

    CInputChannel* getInputChannelObjLocal(
        /* [in] */ const android::sp<android::InputChannel>& inputChannel);

    static bool populateWindow(
        /* [in] */ InputWindow* windowObj,
        /* [in] */ android::InputWindow& outWindow);

private:
    android::sp<android::InputManager> mInputManager;

    InputManagerCallbacks* mCallbacks;

    // Cached filtering policies.
    Int32 mFilterTouchEvents;
    Int32 mFilterJumpyTouchEvents;
    nsecs_t mVirtualKeyQuietTime;

    // Cached throttling policy.
    Int32 mMaxEventsPerSecond;

    // Cached display state.  (lock mDisplayLock)
    android::Mutex mDisplayLock;
    Int32 mDisplayWidth, mDisplayHeight;
    Int32 mDisplayOrientation;

    // Weak references to all currently registered input channels by connection pointer.
    android::Mutex mInputChannelRegistryLock;
    android::KeyedVector<android::InputChannel*, CInputChannel*> mInputChannelObjWeakTable;
};

#endif //__NATIVEINPUTMANAGER_H__