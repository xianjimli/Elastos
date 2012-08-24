
#ifndef __CINSTRUMENTATIONINFO_H__
#define __CINSTRUMENTATIONINFO_H__

#include "_CInstrumentationInfo.h"
#include "content/CapsuleItemInfo.h"

CarClass(CInstrumentationInfo), public CapsuleItemInfo
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IInstrumentationInfo* orig);

    CARAPI GetSourceDir(
        /* [out] */ String* sourceDir);

    CARAPI SetSourceDir(
        /* [in] */ const String& sourceDir);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

public:
    /**
     * The name of the application package being instrumented.  From the
     * "package" attribute.
     */
    String mTargetCapsule;

    /**
     * Full path to the location of this package.
     */
    String mSourceDir;

    /**
     * Full path to the location of the publicly available parts of this package (i.e. the resources
     * and manifest).  For non-forward-locked apps this will be the same as {@link #sourceDir).
     */
    String mPublicSourceDir;

    /**
     * Full path to a directory assigned to the package for its persistent
     * data.
     */
    String mDataDir;

    /**
     * Full path to the directory where the native JNI libraries are stored.
     *
     * {@hide}
     */
    String mNativeLibraryDir;

    /**
     * Specifies whether or not this instrumentation will handle profiling.
     */
    Boolean mHandleProfiling;

    /** Specifies whether or not to run this instrumentation as a functional test */
    Boolean mFunctionalTest;
};

#endif //__CINSTRUMENTATIONINFO_H__
