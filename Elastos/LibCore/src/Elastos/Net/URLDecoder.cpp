
#include "URLDecoder.h"
#include "Elastos.IO.h"

ECode URLDecoder::Decode(
    /* [in] */ const String& s,
    /* [out] */ String* decodeS)
{
//    return DecodeEx2(s, Charset.defaultCharset());
    return E_NOT_IMPLEMENTED;
}

ECode URLDecoder::Decode(
    /* [in] */ const String& s,
    /* [in] */  const String& encoding,
    /* [out] */ String* decodedS)
{
    VALIDATE_NOT_NULL(decodedS);

    if (encoding.IsNull()) {
//        throw new NullPointerException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (encoding.IsEmpty()) {
//        throw new UnsupportedEncodingException(encoding);
        return E_UNSUPPORTED_ENCODING_EXCEPTION;
    }

    if (s.IndexOf('%') == -1) {
        if (s.IndexOf('+') == -1) {
            *decodedS = s;
            return NOERROR;
        }
        //Todo: wrong
        assert(0);
        StringBuf* str = StringBuf::Alloc(s.GetLength());
        str->Copy(s);
        for (Int32 i = 0; i < str->GetLength(); i++) {
            if ((*str)[i] == '+')
                (*str)[i] = ' ';
        }
        *decodedS = (String)*str;
        StringBuf::Free(str);
        return NOERROR;
    }

    // Charset charset = null;
    // try {
    //     charset = Charset.forName(encoding);
    // } catch (IllegalCharsetNameException e) {
    //     throw (UnsupportedEncodingException) (new UnsupportedEncodingException(
    //             encoding).initCause(e));
    // } catch (UnsupportedCharsetException e) {
    //     throw (UnsupportedEncodingException) (new UnsupportedEncodingException(
    //             encoding).initCause(e));
    // }

//    return DecodeEx2(s, charset);
    return E_NOT_IMPLEMENTED;
}

// ECode URLDecoder::DecodeEx2(
//     /* [in] */ const String& s,
//      [in]  Charset charset,
//     /* [out] */ String* decodeS)
// {
// }
