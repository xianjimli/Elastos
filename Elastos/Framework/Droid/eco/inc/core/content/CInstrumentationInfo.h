
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

    CARAPI LoadLabel(
        /* [in] */ ICapsuleManager* pm,
        /* [out] */ ICharSequence** label);

    CARAPI LoadIcon(
        /* [in] */ ICapsuleManager* pm,
        /* [out] */ IDrawable** icon);

    CARAPI LoadLogo(
        /* [in] */ ICapsuleManager* pm,
        /* [out] */ IDrawable** icon);

    CARAPI LoadXmlMetaData(
        /* [in] */ ICapsuleManager* pm,
        /* [in] */ const String& name,
        /* [out] */ IXmlResourceParser** resource);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI SetName(
        /* [in] */ const String& name);

    CARAPI GetCapsuleName(
        /* [out] */ String* name);

    CARAPI SetCapsuleName(
        /* [in] */ const String& name);

    CARAPI GetLabelRes(
        /* [out] */ Int32* labelRes);

    CARAPI SetLabelRes(
        /* [in] */ Int32 labelRes);

    CARAPI GetNonLocalizedLabel(
        /* [out] */ ICharSequence** label);

    CARAPI SetNonLocalizedLabel(
        /* [in] */ ICharSequence* label);

    CARAPI GetIcon(
        /* [out] */ Int32* icon);

    CARAPI SetIcon(
        /* [in] */ Int32 icon);

    CARAPI GetLogo(
        /* [out] */ Int32* logo);

    CARAPI SetLogo(
        /* [in] */ Int32 logo);

    CARAPI GetMetaData(
        /* [out] */ IBundle** metaData);

    CARAPI SetMetaData(
        /* [in] */ IBundle* metaData);

    CARAPI GetTargetCapsule(
        /* [out] */ String* capsule);

    CARAPI SetTargetCapsule(
        /* [in] */ const String& capsule);

    CARAPI GetSourceDir(
        /* [out] */ String* sourceDir);

    CARAPI SetSourceDir(
        /* [in] */ const String& sourceDir);

    CARAPI GetPublicSourceDir(
        /* [out] */ String* pubSrcDir);

    CARAPI SetPublicSourceDir(
        /* [in] */ const String& pubSrcDir);

    CARAPI GetDataDir(
        /* [out] */ String* dataDir);

    CARAPI SetDataDir(
        /* [in] */ const String& dataDir);

    CARAPI GetNativeLibraryDir(
        /* [out] */ String* libraryDir);

    CARAPI SetNativeLibraryDir(
        /* [in] */ const String& libraryDir);

    CARAPI IsHandleProfiling(
        /* [out] */ Boolean* handleProfiling);

    CARAPI SetHandleProfiling(
        /* [in] */ Boolean handleProfiling);

    CARAPI IsFunctionalTest(
        /* [out] */ Boolean* functionalTest);

    CARAPI SetFunctionalTest(
        /* [in] */ Boolean functionalTest);

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
