
#ifndef __CAUTHENTICATORDESCRIPTION_H__
#define __CAUTHENTICATORDESCRIPTION_H__

#include "_CAuthenticatorDescription.h"

CarClass(CAuthenticatorDescription)
{
public:
    CAuthenticatorDescription();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ const String& type,
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 labelId,
        /* [in] */ Int32 iconId,
        /* [in] */ Int32 smallIconId,
        /* [in] */ Int32 prefId,
        /* [in] */ Boolean customTokens);

    CARAPI constructor(
        /* [in] */ const String& type,
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 labelId,
        /* [in] */ Int32 iconId,
        /* [in] */ Int32 smallIconId,
        /* [in] */ Int32 prefId);

public:
    /** The string that uniquely identifies an authenticator */
    String mType;

    /** A resource id of a label for the authenticator that is suitable for displaying */
    Int32 mLabelId;

    /** A resource id of a icon for the authenticator */
    Int32 mIconId;

    /** A resource id of a smaller icon for the authenticator */
    Int32 mSmallIconId;

    /**
     * A resource id for a hierarchy of PreferenceScreen to be added to the settings page for the
     * account. See {@link AbstractAccountAuthenticator} for an example.
     */
    Int32 mAccountPreferencesId;

    /** The package name that can be used to lookup the resources from above. */
    String mCapsuleName;

    /** Authenticator handles its own token caching and permission screen
      * @hide
      */
    Boolean mCustomTokens;
};

#endif //__CAUTHENTICATORDESCRIPTION_H__
