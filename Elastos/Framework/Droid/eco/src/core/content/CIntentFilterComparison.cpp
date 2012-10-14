
#include "ext/frameworkdef.h"
#include "content/CIntentFilterComparison.h"

/**
 * Return the Intent that this FilterComparison represents.
 * @return Returns the Intent held by the FilterComparison.  Do
 * not modify!
 */
ECode CIntentFilterComparison::GetIntent(
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent);
    *intent = mIntent;
    if (*intent != NULL) (*intent)->AddRef();
    return NOERROR;
}

ECode CIntentFilterComparison::Equals(
    /* [in] */ IIntentFilterComparison* other,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);
    if (other == NULL) {
        *isEqual = FALSE;
        return NOERROR;
    }
    AutoPtr<IIntent> otherIntent;
    other->GetIntent((IIntent**)&otherIntent);
    return mIntent->FilterEquals(otherIntent, isEqual);
}

ECode CIntentFilterComparison::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    *hashCode = mHashCode;
    return NOERROR;
}

ECode CIntentFilterComparison::GetDescription(
    /* [out] */ String* description)
{
    VALIDATE_NOT_NULL(description);
    return E_NOT_IMPLEMENTED;
}

ECode CIntentFilterComparison::constructor(
    /* [in] */ IIntent* intent)
{
    assert(intent != NULL);
    mIntent = intent;
    mIntent->GetFilterHashCode(&mHashCode);
    return NOERROR;
}
