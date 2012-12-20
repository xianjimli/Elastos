
#ifndef  _CINPUTMETHODINFO_H__
#define  _CINPUTMETHODINFO_H__

#include "ext/frameworkext.h"
#include "_CInputMethodInfo.h"
#include <elastos/AutoPtr.h>


CarClass(CInputMethodInfo)
{
public:
	CInputMethodInfo();

	~CInputMethodInfo();

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IResolveInfo* service);

	CARAPI constructor(
        /* [in] */ IParcel* source);

    CARAPI constructor(
        /* [in] */ String packageName,
        /* [in] */ String className,
        /* [in] */ ICharSequence* label,
        /* [in] */ String settingsActivity);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI GetId(
        /* [out] */ String* id);

    CARAPI GetCapsuleName(
        /* [out] */ String* name);

    CARAPI GetServiceName(
        /* [out] */ String* name);

    CARAPI GetServiceInfo(
        /* [out] */ IServiceInfo** info);

    CARAPI GetComponent(
        /* [out] */ IComponentName** name);

    CARAPI LoadLabel(
        /* [in] */ ICapsuleManager* pm,
        /* [out] */ ICharSequence** str);

    CARAPI LoadIcon(
        /* [in] */ ICapsuleManager* pm,
        /* [out] */ IDrawable** drawable);

    CARAPI GetSettingsActivity(
        /* [out] */ String* settings);

    CARAPI GetIsDefaultResourceId(
        /* [out] */ Int32* id);

private:
    static CString TAG;

    /**
     * The Service that implements this input method component.
     */
    AutoPtr<IResolveInfo> mService;

    /**
     * The unique string Id to identify the input method.  This is generated
     * from the input method component.
     */
    String mId;

    /**
     * The input method setting activity's name, used by the system settings to
     * launch the setting activity of this input method.
     */
    String mSettingsActivityName;

    /**
     * The resource in the input method's .apk that holds a boolean indicating
     * whether it should be considered the default input method for this
     * system.  This is a resource ID instead of the final value so that it
     * can change based on the configuration (in particular locale).
     */
    Int32 mIsDefaultResId;
};

#endif  //_CINPUTMETHODINFO_H__
