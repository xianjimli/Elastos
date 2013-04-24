
#include "CNativeConverter.h"
#include "NativeConverter.h"
#include <elastos/AutoPtr.h>
#include <cmdef.h>
#include "CCodingErrorAction.h"


ECode CNativeConverter::Decode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ const ArrayOf<Byte>& input,
    /* [in] */ Int32 inEnd,
    /* [in] */ const ArrayOf<Char8>& output,
    /* [in] */ Int32 outEnd,
     /* [in, out] */ ArrayOf<Int32>* data,
    /* [in] */ Boolean flush,
    /* [out] */ Int32* errorCode)
{
    return NativeConverter::Decode(converterHandle, input, inEnd, output, outEnd, data, flush, errorCode);
}

ECode CNativeConverter::Encode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ const ArrayOf<Char8>& input,
    /* [in] */ Int32 inEnd,
    /* [in] */ const ArrayOf<Byte>& output,
    /* [in] */ Int32 outEnd,
    /* [in, out] */ ArrayOf<Int32>* data,
    /* [in] */ Boolean flush,
    /* [out] */ Int32* errorCode)
{
    return NativeConverter::Encode(converterHandle, input, inEnd, output, outEnd, data, flush, errorCode);
}

ECode CNativeConverter::FlushCharToByte(
    /* [in] */ Int64 converterHandle,
    /* [in] */ const ArrayOf<Byte>& output,
    /* [in] */ Int32 outEnd,
    /* [in, out] */ ArrayOf<Int32>* data,
    /* [out] */ Int32* errorCode)
{
    return CNativeConverter::FlushCharToByte(converterHandle, output, outEnd, data, errorCode);
}

ECode CNativeConverter::FlushByteToChar(
    /* [in] */ Int64 converterHandle,
    /* [in] */ const ArrayOf<Char8>& output,
    /* [in] */ Int32 outEnd,
    /* [in, out] */ ArrayOf<Int32>* data,
    /* [out] */ Int32* errorCode)
{
    return NativeConverter::FlushByteToChar(converterHandle, output, outEnd, data, errorCode);
}

ECode CNativeConverter::OpenConverter(
    /* [in] */ const String& encoding,
    /* [out] */ Int64* converterHandle)
{
    return NativeConverter::OpenConverter(encoding, converterHandle);
}

ECode CNativeConverter::CloseConverter(
    /* [in] */ Int64 converterHandle)
{
    return NativeConverter::CloseConverter(converterHandle);
}

ECode CNativeConverter::ResetByteToChar(
    /* [in] */ Int64 converterHandle)
{
    return NativeConverter::ResetByteToChar(converterHandle);
}

ECode CNativeConverter::ResetCharToByte(
    /* [in] */ Int64 converterHandle)
{
    return NativeConverter::ResetCharToByte(converterHandle);
}

ECode CNativeConverter::GetSubstitutionBytes(
    /* [in] */ Int64 converterHandle,
    /* [out, callee] */ ArrayOf<Byte>** bytes)
{
    return NativeConverter::GetSubstitutionBytes(converterHandle, bytes);
}

ECode CNativeConverter::GetMaxBytesPerChar(
    /* [in] */ Int64 converterHandle,
    /* [out] */ Int32* max)
{
    return NativeConverter::GetMaxBytesPerChar(converterHandle, max);
}

ECode CNativeConverter::GetMinBytesPerChar(
    /* [in] */ Int64 converterHandle,
    /* [out] */ Int32* min)
{
    return NativeConverter::GetMinBytesPerChar(converterHandle, min);
}

ECode CNativeConverter::GetAveBytesPerChar(
    /* [in] */ Int64 converterHandle,
    /* [out] */ Float* ave)
{
    return NativeConverter::GetAveBytesPerChar(converterHandle, ave);
}

ECode CNativeConverter::GetAveCharsPerByte(
    /* [in] */ Int64 converterHandle,
    /* [out] */ Float* ave)
{
    return NativeConverter::GetAveCharsPerByte(converterHandle, ave);
}

ECode CNativeConverter::Contains(
    /* [in] */ const String& converterName1,
    /* [in] */ const String& converterName2,
    /* [out] */ Boolean* isContains)
{
    return NativeConverter::Contains(converterName1, converterName2, isContains);
}

ECode CNativeConverter::CanEncode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ Int32 codeUnit,
    /* [out] */ Boolean* canEncode)
{
    return NativeConverter::CanEncode(converterHandle, codeUnit, canEncode);
}

ECode CNativeConverter::GetAvailableCharsetNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    return NativeConverter::GetAvailableCharsetNames(names);
}

ECode CNativeConverter::CharsetForName(
    /* [in] */ const String& charsetName,
    /* [out] */ ICharset** charset)
{
    return NativeConverter::CharsetForName(charsetName, charset);
}

ECode CNativeConverter::SetCallbackDecode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ ICharsetDecoder* decoder,
    /* [out] */ Int32* errorCode)
{
    VALIDATE_NOT_NULL(decoder);
    VALIDATE_NOT_NULL(errorCode);

    AutoPtr<ICodingErrorAction> mal;
    FAIL_RETURN(decoder->MalformedInputAction((ICodingErrorAction**)&mal));
    Int32 malCode;
    FAIL_RETURN(NativeConverter::TranslateCodingErrorAction(mal.Get(), &malCode));
    AutoPtr<ICodingErrorAction> unmap;
    FAIL_RETURN(decoder->UnmappableCharacterAction((ICodingErrorAction**)&unmap));
    Int32 unmapCode;
    FAIL_RETURN(NativeConverter::TranslateCodingErrorAction(unmap.Get(), &unmapCode));
    String rep;
    decoder->Replacement(&rep);
    const char* p = rep.string();

    return NativeConverter::SetCallbackDecode(converterHandle, malCode, unmapCode, p, rep.GetSize(), errorCode);
}

ECode CNativeConverter::SetCallbackEncode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ ICharsetEncoder* encoder,
    /* [out] */ Int32* errorCode)
{
    VALIDATE_NOT_NULL(encoder);
    VALIDATE_NOT_NULL(errorCode);

    AutoPtr<ICodingErrorAction> mal;
    FAIL_RETURN(encoder->MalformedInputAction((ICodingErrorAction**)&mal));
    Int32 malCode;
    FAIL_RETURN(NativeConverter::TranslateCodingErrorAction(mal.Get(), &malCode));
    AutoPtr<ICodingErrorAction> unmap;
    FAIL_RETURN(encoder->UnmappableCharacterAction((ICodingErrorAction**)&unmap));
    Int32 unmapCode;
    FAIL_RETURN(NativeConverter::TranslateCodingErrorAction(unmap.Get(), &unmapCode));
    ArrayOf<Byte>* rep;
    encoder->Replacement(&rep);
    const Byte* p = rep->GetPayload();

    return NativeConverter::SetCallbackEncode(converterHandle, malCode, unmapCode, p, rep->GetLength() * sizeof(Byte), errorCode);
}
