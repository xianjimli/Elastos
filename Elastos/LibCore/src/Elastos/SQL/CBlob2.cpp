
#include "cmdef.h"
#include "CBlob2.h"

ECode CBlob2::GetInputStream(
    /** [out] **/IInputStream** stream)
{
    return Blob2::GetInputStream(stream);
}

ECode CBlob2::GetOutputStream(
    /** [out] **/IOutputStream** stream)
{
    return Blob2::GetOutputStream(stream);
}

ECode CBlob2::Close()
{
    return Blob2::Close();
}

ECode CBlob2::Write(
    /** [in] **/Int32 off, 
    /** [in] **/Int32 pos, 
    /** [in] **/Int32 len,
    /** [in] **/const ArrayOf<Byte>& b, 
    /** [out] **/Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return Blob2::Write(off, pos, len, b, result);
}

ECode CBlob2::Read( 
    /** [in] **/Int32 off, 
    /** [in] **/Int32 pos, 
    /** [in] **/Int32 len,
    /** [in] **/ArrayOf<Byte>* b, 
    /** [out] **/Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return Blob2::Read(off, pos, len, b, result);
}

ECode CBlob2::Finalize()
{
    return Blob2::Finalize();
}

ECode CBlob2::constructor()
{
    return NOERROR;
}