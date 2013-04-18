#include "Charset.h"
#include "cmdef.h"
#include "CharBuffer.h"
#include <elastos/Mutex.h>
#include <elastos/AutoPtr.h>

using namespace Elastos::Core::Threading;

HashMap<String, Charset*>* Charset::CACHED_CHARSETS;

const Charset* Charset::DEFAULT_CHARSET
    = Charset::GetDefaultCharset();

static Mutex gCachedCharsetsLock;

Charset::Charset(
    /* [in] */ const String& canonicalName, 
    /* [in] */ const ArrayOf<String>& aliases)
    : mAliasesSet(new HashSet<String>())
{
    // check whether the given canonical name is legal
    ASSERT_SUCCEEDED(CheckCharsetName(canonicalName));
    mCanonicalName = canonicalName;
    // check each alias and put into a set
    if (mAliasesSet != NULL) {
        Int32 size = aliases.GetLength();
        for (Int32 i = 0; i < size; i++) {
            ASSERT_SUCCEEDED(CheckCharsetName(aliases[i]));
            mAliasesSet->Insert(aliases[i]);
        }
    }
}

// TODO:
// public static SortedMap<String, Charset> availableCharsets() {
//     // Start with a copy of the built-in charsets...
//     TreeMap<String, Charset> charsets = new TreeMap<String, Charset>(String.CASE_INSENSITIVE_ORDER);
//     for (String charsetName : NativeConverter.getAvailableCharsetNames()) {
//         Charset charset = NativeConverter.charsetForName(charsetName);
//         charsets.put(charset.name(), charset);
//     }

//     // Add all charsets provided by all charset providers...
//     for (CharsetProvider charsetProvider : ServiceLoader.load(CharsetProvider.class, null)) {
//         Iterator<Charset> it = charsetProvider.charsets();
//         while (it.hasNext()) {
//             Charset cs = it.next();
//             // A CharsetProvider can't override a built-in Charset.
//             if (!charsets.containsKey(cs.name())) {
//                 charsets.put(cs.name(), cs);
//             }
//         }
//     }

//     return Collections.unmodifiableSortedMap(charsets);
// }

ECode Charset::ForName(
    /* [in] */ const String& charsetName,
    /* [out] */ ICharset** charset)
{
    VALIDATE_NOT_NULL(charset);

    // Is this charset in our cache?
    Charset* cs;
    {
        Mutex::Autolock lock(gCachedCharsetsLock);
        if (CACHED_CHARSETS == NULL) {
            CACHED_CHARSETS = new HashMap<String, Charset*>();
        }

        HashMap<String, Charset*>::Iterator it = CACHED_CHARSETS->Find(charsetName);
        if (it != CACHED_CHARSETS->End()) {
            *charset = (ICharset *)it->mSecond;
            return NOERROR;
        }
    }

    // Is this a built-in charset supported by ICU?
    if (charsetName.IsNull()) {
        return E_ILLEGAL_CHARSET_NAME_EXCEPTION;
    }
    CheckCharsetName(charsetName);
    // TODO: 
    // cs = NativeConverter.charsetForName(charsetName);
    // if (cs != null) {
    //     return cacheCharset(charsetName, cs);
    // }

    // Does a configured CharsetProvider have this charset?
    // for (CharsetProvider charsetProvider : ServiceLoader.load(CharsetProvider.class, null)) {
    //     cs = charsetProvider.charsetForName(charsetName);
    //     if (cs != null) {
    //         return cacheCharset(charsetName, cs);
    //     }
    // }

	return E_UNSUPPORTED_CHARSET_EXCEPTION;
}

ECode Charset::ForNameUEE(
    /* [in] */ const String& charsetName,
    /* [out] */ ICharset** charset)
{
    VALIDATE_NOT_NULL(charset);
    return Charset::ForName(charsetName, charset);
}

ECode Charset::IsSupported(
    /* [in] */ const String& charsetName,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    ICharset* charset;
    ECode res = Charset::ForName(charsetName, &charset);
    *result = SUCCEEDED(res) ? TRUE : FALSE;
	return res;
}

ECode Charset::Name(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mCanonicalName;
	return NOERROR;
}

ECode Charset::Aliases(
    /* [out, callee] */ Set<String>** aliases)
{
    // TODO:
    // return Collections.unmodifiableSet(this.aliasesSet);
	return E_NOT_IMPLEMENTED;
}

ECode Charset::DisplayName(
    /**out**/ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mCanonicalName;
    return NOERROR;
}

ECode Charset::DisplayNameEx(
        /* [in] */ ILocale* l,
        /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mCanonicalName;
    return NOERROR;
}

ECode Charset::IsRegistered(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = !mCanonicalName.StartWith("x-") && !mCanonicalName.StartWith("X-");
    return NOERROR;
}

ECode Charset::CanEncode(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
	return NOERROR;
}

ECode Charset::Encode(
    /* [in] */ ICharBuffer* buffer,
    /* [out] */ IByteBuffer** byteBuffer)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(byteBuffer);

    AutoPtr<ICharsetEncoder> encoder;
    FAIL_RETURN(NewEncoder((ICharsetEncoder **)&encoder));

    AutoPtr<ICodingErrorAction> action;
    CCodingErrorAction::New((ICodingErrorAction** )&action);
    assert(action != NULL);
    AutoPtr<ICodingErrorAction> replace;
    FAIL_RETURN(action->GetREPLACE((ICodingErrorAction **)&replace));

    AutoPtr<ICharsetEncoder> malformEncoder;
    FAIL_RETURN(encoder->OnMalformedInput(replace,
        (ICharsetEncoder **)&malformEncoder));

    AutoPtr<ICharsetEncoder> unMappableEncoder;
    FAIL_RETURN(malformEncoder->OnUnmappableCharacter(replace,
        (ICharsetEncoder **)&unMappableEncoder));

    FAIL_RETURN(unMappableEncoder->Encode(buffer, byteBuffer));

	return NOERROR;
}

ECode Charset::EncodeEx(
        /* [in] */ const String& s,
        /* [out] */ IByteBuffer** byteBuffer)
{
    VALIDATE_NOT_NULL(byteBuffer);

    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(s, (ICharSequence**)&cs);
    ICharBuffer* charBuffer;
    CharBuffer::WrapEx2(cs.Get(), &charBuffer);

    return Encode(charBuffer, byteBuffer);
}

ECode Charset::Decode(
    /* [in] */ IByteBuffer* buffer,
    /* [out] */ ICharBuffer** charBuffer)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(charBuffer);

    AutoPtr<ICharsetDecoder> decoder;
    FAIL_RETURN(NewDecoder((ICharsetDecoder **)&decoder));

    AutoPtr<ICodingErrorAction> action;
    CCodingErrorAction::New((ICodingErrorAction** )&action);
    assert(action != NULL);
    AutoPtr<ICodingErrorAction> replace;
    FAIL_RETURN(action->GetREPLACE((ICodingErrorAction **)&replace));

    AutoPtr<ICharsetDecoder> malformDecoder;
    FAIL_RETURN(decoder->OnMalformedInput(replace,
        (ICharsetDecoder **)&malformDecoder));

    AutoPtr<ICharsetDecoder> unMappableDecoder;
    FAIL_RETURN(malformDecoder->OnUnmappableCharacter(replace,
        (ICharsetDecoder **)&unMappableDecoder));

    FAIL_RETURN(unMappableDecoder->Decode(buffer, charBuffer));

	return NOERROR;
}

ECode Charset::CompareTo(
    /* [in] */ ICharset* charset,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(charset);
    VALIDATE_NOT_NULL(result);
    return mCanonicalName.Compare(((Charset *)charset)->mCanonicalName, StringCase_Insensitive);
}

ECode Charset::DefaultCharset(
    /* [out] */ ICharset** charset)
{
    VALIDATE_NOT_NULL(charset);
    *charset = (ICharset *)DEFAULT_CHARSET;
    return NOERROR;
}

ECode Charset::CheckCharsetName(
    /* [in] */ const String& name)
{
    if (name.IsNull()) {
        return E_ILLEGAL_CHARSET_NAME_EXCEPTION;
    }

    Int32 length = name.GetSize();
    Boolean result = false; 
    for (Int32 i = 0; i < length; ++i) {
        IsValidCharsetNameCharacter(name[i], &result);
        if (!result) {
            return E_ILLEGAL_CHARSET_NAME_EXCEPTION;
        }
    }

    return NOERROR;
}

ECode Charset::IsValidCharsetNameCharacter(
    /* [in] */ const Char8& c,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')
        || c == '-' || c == '.' || c == ':' || c == '_';
    return NOERROR;
}

ECode Charset::CacheCharset(
    /* [in] */ const String& charsetName,
    /* [in] */ Charset* cs,
    /* [out] */ Charset** charset)
{
    VALIDATE_NOT_NULL(cs);
    VALIDATE_NOT_NULL(charset);

    Mutex::Autolock lock(gCachedCharsetsLock);

    // Get the canonical name for this charset, and the canonical instance from the table.
    String canonicalName;
    cs->Name(&canonicalName);
    Charset* canonicalCharset;

    if (CACHED_CHARSETS == NULL) {
        CACHED_CHARSETS = new HashMap<String, Charset*>();
    }

    HashMap<String, Charset*>::Iterator it = CACHED_CHARSETS->Find(canonicalName);
    if (it == CACHED_CHARSETS->End()) {
        canonicalCharset = cs;
    }

    // Cache the charset by its canonical name...
    (*CACHED_CHARSETS)[canonicalName] = canonicalCharset;

    // And the name the user used... (Section 1.4 of http://unicode.org/reports/tr22/ means
    // that many non-alias, non-canonical names are valid. For example, "utf8" isn't an
    // alias of the canonical name "UTF-8", but we shouldn't penalize consistent users of
    // such names unduly.)
    (*CACHED_CHARSETS)[charsetName] = canonicalCharset;

    // And all its aliases...
    HashSet<String>::Iterator itr;
    for (itr = cs->mAliasesSet->Begin(); itr != cs->mAliasesSet->End(); itr++) {
        String alias = *itr;
        (*CACHED_CHARSETS)[alias] = canonicalCharset;
    }

    *charset = canonicalCharset;

    return NOERROR;
}

Charset* Charset::GetDefaultCharset()
{
    // TODO:
    // String encoding = AccessController.doPrivileged(new PrivilegedAction<String>() {
    //     public String run() {
    //         return System.getProperty("file.encoding", "UTF-8");
    //     }
    // });
    // try {
    //     return Charset.forName(encoding);
    // } catch (UnsupportedCharsetException e) {
    //     return Charset.forName("UTF-8");
    // }
    return NULL;
}