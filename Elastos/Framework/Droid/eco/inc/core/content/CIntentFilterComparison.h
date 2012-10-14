
#ifndef __CINTENTFILTERCOMPARISON_H__
#define __CINTENTFILTERCOMPARISON_H__

#include "_CIntentFilterComparison.h"
#include <elastos/AutoPtr.h>

CarClass(CIntentFilterComparison)
{
public:
    /**
     * Return the Intent that this FilterComparison represents.
     * @return Returns the Intent held by the FilterComparison.  Do
     * not modify!
     */
    CARAPI GetIntent(
        /* [out] */ IIntent** intent);

    CARAPI Equals(
        /* [in] */ IIntentFilterComparison* other,
        /* [out] */ Boolean* isEqual);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI GetDescription(
        /* [out] */ String* description);

    CARAPI constructor(
        /* [in] */ IIntent* intent);

private:
    AutoPtr<IIntent> mIntent;
    Int32 mHashCode;
};

#endif //__CINTENTFILTERCOMPARISON_H__
