#ifndef __CCHARSETICU_H__
#define __CCHARSETICU_H__

#include "_CCharsetICU.h"
#include "Charset.h"

CarClass(CCharsetICU), public Charset
{
public:
    CARAPI constructor(
        /* [in] */ const String& canonicalName,
        /* [in] */ const String& icuCanonName,
        /* [in] */ const ArrayOf<String>& aliases);

    CARAPI NewDecoder(
        /* [out] */ ICharsetDecoder** charsetDecoder);

    CARAPI NewEncoder(
        /* [out] */ ICharsetEncoder** charsetEncoder);

    CARAPI Contains(
        /* [in] */ ICharset* charset,
        /* [out] */ Boolean* result);

    CARAPI Name(
        /* [out] */ String* name);

    CARAPI Aliases(
        /* [out, callee] */ IObjectContainer** aliases);

    CARAPI DisplayName(
        /* [out] */ String* name);

    CARAPI DisplayNameEx(
        /* [in] */ ILocale* l,
        /* [out] */ String* name);

    CARAPI IsRegistered(
        /* [out] */ Boolean* result);

    CARAPI CanEncode(
        /* [out] */ Boolean* result);

    CARAPI Encode(
        /* [in] */ ICharBuffer* buffer,
        /* [out] */ IByteBuffer** byteBuffer);

    CARAPI EncodeEx(
        /* [in] */ const String& s,
        /* [out] */ IByteBuffer** byteBuffer);

    CARAPI Decode(
        /* [in] */ IByteBuffer* buffer,
        /* [out] */ ICharBuffer** charBuffer);

    CARAPI CompareTo(
        /* [in] */ ICharset* charset,
        /* [out] */ Int32* result);

private:
	String mIcuCanonicalName;
};

#endif // __CCHARSETICU_H__
