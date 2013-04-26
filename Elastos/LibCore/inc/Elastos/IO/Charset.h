#ifndef __CHAARSET_H__
#define __CHAARSET_H__

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include <elastos.h>
#include <Elastos.IO.h>
#include <elastos/HashMap.h>
#include <elastos/HashSet.h>
#include <elastos/Set.h>

using namespace Elastos;

class Charset
{
public:
    Charset();

    CARAPI Init(
        /* [in] */ const String& canonicalName, 
        /* [in] */ const ArrayOf<String>& aliases);

    // TODO:
    // static CARAPI availableCharsets(
    //	/* [out] */ SortedMap<String, Charset>** charsets);

    static CARAPI ForName(
        /* [in] */ const String& charsetName,
        /* [out] */ ICharset** charset);

    static CARAPI ForNameUEE(
        /* [in] */ const String& charsetName,
        /* [out] */ ICharset** charset);

    static CARAPI IsSupported(
        /* [in] */ const String& charsetName,
        /* [out] */ Boolean* result);

    virtual CARAPI Contains(
        /* [in] */ ICharset* charset,
        /* [out] */ Boolean* result) = 0;

    virtual CARAPI NewEncoder(
        /* [out] */ ICharsetEncoder** charsetEncoder) = 0;

    virtual CARAPI NewDecoder(
        /* [out] */ ICharsetDecoder** charsetDecoder) = 0;

    CARAPI Name(
        /* [out] */ String* name);

    CARAPI Aliases(
        /* [out, callee] */ Set<String>** aliases);

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

    static CARAPI DefaultCharset(
        /* [out] */ ICharset** charset);

private:
    static CARAPI CheckCharsetName(
        /* [in] */ const String& name);

    static CARAPI IsValidCharsetNameCharacter(
        /* [in] */ const Char8& c,
        /* [out] */ Boolean* result);

    static CARAPI CacheCharset(
        /* [in] */ const String& charsetName,
        /* [in] */ ICharset* s,
        /* [out] */ ICharset** charset);

    static ICharset* GetDefaultCharset();

public:
    String mCanonicalName;

private:
    static HashMap<String, ICharset*>* CACHED_CHARSETS;
        // = new HashMap<String, Charset>();

    static const ICharset* DEFAULT_CHARSET;
        // = GetDefaultCharset();

    HashSet<String>* const mAliasesSet;
};

#endif //__CHAARSET_H__