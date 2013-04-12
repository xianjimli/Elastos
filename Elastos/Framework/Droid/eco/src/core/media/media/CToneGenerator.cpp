
#include "ext/frameworkdef.h"
#include "media/CToneGenerator.h"
#include <media/AudioSystem.h>
#include <media/ToneGenerator.h>


CToneGenerator::~CToneGenerator()
{
    NativeFinalize();
}

ECode CToneGenerator::constructor(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 volume)
{
    return NativeSetup(streamType, volume);
}

ECode CToneGenerator::StartTone(
    /* [in] */ Int32 toneType,
    /* [out] */ Boolean* result)
{
    return StartToneEx(toneType, -1, result);
}

ECode CToneGenerator::StartToneEx(
    /* [in] */ Int32 toneType,
    /* [in] */ Int32 durationMs,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // LOGV("android_media_ToneGenerator_startTone: %x\n", (int)thiz);

    android::ToneGenerator *lpToneGen = (android::ToneGenerator *)mNativeContext;
    if (lpToneGen == NULL) {
        // jniThrowException(env, "java/lang/RuntimeException", "Method called after release()");
        *result = FALSE;
        return E_RUNTIME_EXCEPTION;
    }

    *result = lpToneGen->startTone(toneType, durationMs);
    return NOERROR;
}

/**
 * This method stops the tone currently playing playback.
 * @see #ToneGenerator(int, int)
 */
ECode CToneGenerator::StopTone()
{
    // LOGV("android_media_ToneGenerator_stopTone: %x\n", (int)thiz);

    android::ToneGenerator *lpToneGen = (android::ToneGenerator *)mNativeContext;

    // LOGV("ToneGenerator lpToneGen: %x\n", (unsigned int)lpToneGen);
    if (lpToneGen == NULL) {
        // jniThrowException(env, "java/lang/RuntimeException", "Method called after release()");
        return E_RUNTIME_EXCEPTION;
    }
    lpToneGen->stopTone();
    return NOERROR;
}

/**
 * Releases resources associated with this ToneGenerator object. It is good
 * practice to call this method when you're done using the ToneGenerator.
 */
ECode CToneGenerator::ReleaseResources()
{
    android::ToneGenerator *lpToneGen = (android::ToneGenerator *)mNativeContext;
    // LOGV("android_media_ToneGenerator_release lpToneGen: %x\n", (int)lpToneGen);

    mNativeContext = 0;

    if (lpToneGen) {
        delete lpToneGen;
    }
    return NOERROR;
}

ECode CToneGenerator::NativeSetup(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 volume)
{
    android::ToneGenerator *lpToneGen = new android::ToneGenerator(streamType,
            android::AudioSystem::linearToLog(volume), true);

    mNativeContext = 0;

    // LOGV("android_media_ToneGenerator_native_setup jobject: %x\n", (int)thiz);

    if (lpToneGen == NULL) {
        // LOGE("ToneGenerator creation failed \n");
        // jniThrowException(env, "java/lang/OutOfMemoryError", NULL);
        return E_OUT_OF_MEMORY_ERROR;
    }
    // LOGV("ToneGenerator lpToneGen: %x\n", (unsigned int)lpToneGen);

    if (!lpToneGen->isInited()) {
        // LOGE("ToneGenerator init failed \n");
        // jniThrowException(env, "java/lang/RuntimeException", "Init failed");
        return E_RUNTIME_EXCEPTION;
    }

    // Stow our new C++ ToneGenerator in an opaque field in the Java object.
    mNativeContext = (Int32)lpToneGen;

    // LOGV("ToneGenerator fields.context: %x\n", env->GetIntField(thiz, fields.context));
    return NOERROR;
}

void CToneGenerator::NativeFinalize()
{
    // LOGV("android_media_ToneGenerator_native_finalize jobject: %x\n", (int)thiz);

    android::ToneGenerator *lpToneGen = (android::ToneGenerator *)mNativeContext;

    if (lpToneGen) {
        // LOGV("delete lpToneGen: %x\n", (int)lpToneGen);
        delete lpToneGen;
    }
}

