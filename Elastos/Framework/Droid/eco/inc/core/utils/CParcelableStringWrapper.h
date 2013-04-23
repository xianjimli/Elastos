
#ifndef __CPARCELABLESTRINGWRAPPER_H__
#define __CPARCELABLESTRINGWRAPPER_H__

#include "_CParcelableStringWrapper.h"

CarClass(CParcelableStringWrapper)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& str);

    CARAPI GetLength(
        /* [out] */ Int32* number);

    CARAPI GetCharAt(
        /* [in] */ Int32 index,
        /* [out] */ Char32* c);

    CARAPI SubSequence(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ICharSequence** csq);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

private:
    String mString;
};

#endif //__CPARCELABLESTRINGWRAPPER_H__
