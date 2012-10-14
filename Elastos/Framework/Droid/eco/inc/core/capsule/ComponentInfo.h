
#ifndef __COMPONENTINFO_H__
#define __COMPONENTINFO_H__

#include "content/CApplicationInfo.h"
#include <elastos/AutoPtr.h>

class ComponentInfo : public CapsuleItemInfo
{
public:
    ComponentInfo();

    virtual ~ComponentInfo();

    CARAPI constructor(
        /* [in] */ ComponentInfo* orig);

    //@override
    CARAPI LoadLabel(
        /* [in] */ ICapsuleManager* pm,
        /* [out] */ ICharSequence** label);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

public:
    /**
     * Global information about the application/package this component is a
     * part of.
     */
    AutoPtr<CApplicationInfo> mApplicationInfo;

    /**
     * The name of the process this component should run in.
     * From the "android:process" attribute or, if not set, the same
     * as <var>applicationInfo.processName</var>.
     */
    String mProcessName;

    /**
     * A string resource identifier (in the package's resources) containing
     * a user-readable description of the component.  From the "description"
     * attribute or, if not set, 0.
     */
    Int32 mDescriptionRes;

    /**
     * Indicates whether or not this component may be instantiated.  Note that this value can be
     * overriden by the one in its parent {@link ApplicationInfo}.
     */
    Boolean mEnabled;

    /**
     * Set to true if this component is available for use by other applications.
     * Comes from {@link android.R.attr#exported android:exported} of the
     * &lt;activity&gt;, &lt;receiver&gt;, &lt;service&gt;, or
     * &lt;provider&gt; tag.
     */
    Boolean mExported;
};

#endif //__COMPONENTINFO_H__
