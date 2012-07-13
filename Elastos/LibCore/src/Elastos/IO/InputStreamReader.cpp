#include "cmdef.h"
#include "InputStreamReader.h"

InputStreamReader::InputStreamReader()
{
}

InputStreamReader::~InputStreamReader()
{
}

ECode InputStreamReader::Init(
    /* [in] */ IInputStream *in)
{
    return E_NOT_IMPLEMENTED;
}

ECode InputStreamReader::Init(
    /* [in] */ IInputStream *in,
    /* [in] */ const String &enc)
{
    return E_NOT_IMPLEMENTED;
}

ECode InputStreamReader::Close()
{
    return E_NOT_IMPLEMENTED;
}

ECode InputStreamReader::GetEncoding(
    /* [out] */ String* encoding)
{
    return E_NOT_IMPLEMENTED;
}

ECode InputStreamReader::Read(
    /* [out] */ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode InputStreamReader::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    return E_NOT_IMPLEMENTED;
}

ECode InputStreamReader::IsReady(
    /* [out] */ Boolean* ready)
{
    return E_NOT_IMPLEMENTED;
}

 