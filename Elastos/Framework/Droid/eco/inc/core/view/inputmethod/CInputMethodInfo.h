
#ifndef  __CINPUTMETHODINFO_H__
#define  __CINPUTMETHODINFO_H__


#include "_CInputMethodInfo.h"
#include "content/CResolveInfo.h"
#include <elastos/AutoPtr.h>


CarClass(CInputMethodInfo)
{
public:
    CInputMethodInfo();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IResolveInfo* service);

    CARAPI constructor(
        /* [in] */ const String& packageName,
        /* [in] */ const String& className,
        /* [in] */ ICharSequence* label,
        /* [in] */ const String& settingsActivity);

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
        /* [in] */ ILocalCapsuleManager* pm,
        /* [out] */ ICharSequence** str);

    CARAPI LoadIcon(
        /* [in] */ ILocalCapsuleManager* pm,
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
    AutoPtr<CResolveInfo> mService;

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

#endif  //__CINPUTMETHODINFO_H__
