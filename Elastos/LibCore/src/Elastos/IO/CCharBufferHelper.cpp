
#include "cmdef.h"
#include "CCharBufferHelper.h"
#include "BufferFactory.h"
#include "CharBuffer.h"

ECode CCharBufferHelper::constructor()
{}

ECode CCharBufferHelper::Allocate(
    /* [in]  */ Int32         capacity,
    /* [out] */ ICharBuffer** charBuffer)
{
    if ((capacity < 0) || (NULL == charBuffer)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    BufferFactory::NewCharBufferEx2(capacity, charBuffer);

    return NOERROR;
}

ECode CCharBufferHelper::WrapArray(
    /* [in]  */ ArrayOf<Char32>* array,
    /* [out] */ ICharBuffer**    charBuffer)
{
    VALIDATE_NOT_NULL(array);
    VALIDATE_NOT_NULL(charBuffer);

    Int32 len;
    len = array->GetLength();

    WrapArrayEx(array, 0, len, charBuffer);

    return NOERROR;
}

ECode CCharBufferHelper::WrapArrayEx(
    /*[in]  */  ArrayOf<Char32>* array,
    /*[in]  */  Int32            start,
    /*[in]  */  Int32            len,
    /*[out] */  ICharBuffer**    charBuffer)
{
    VALIDATE_NOT_NULL(array);
    VALIDATE_NOT_NULL(charBuffer);

    Int32 arrayLen;
    arrayLen = array->GetLength();
    if ((start < 0) ||
        (len < 0)   ||
        ((Int64)start + (Int64)len) > arrayLen) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    BufferFactory::NewCharBuffer(array, charBuffer);
    CharBuffer* obj = (CharBuffer*)(*charBuffer)->Probe(EIID_CharBuffer);
    obj->mPosition = start;
    obj->mLimit = start + len;

    return NOERROR;
}

ECode CCharBufferHelper::WrapSequence(
    /* [in]  */  ICharSequence* charSequence,
    /* [out] */  ICharBuffer**  charBuffer)
{
    VALIDATE_NOT_NULL(charSequence);
    VALIDATE_NOT_NULL(charBuffer);

    return BufferFactory::NewCharBufferEx(charSequence,
                                          charBuffer);
}

ECode CCharBufferHelper::WrapSequenceEx(
    /* [in]  */ ICharSequence* charSequence,
    /* [in]  */ Int32          start,
    /* [in]  */ Int32          end,
    /* [out] */ ICharBuffer**  charBuffer)
{
    VALIDATE_NOT_NULL(charSequence);
    VALIDATE_NOT_NULL(charBuffer);

    int SequenceLen;
    charSequence->GetLength(&SequenceLen);

    if (start < 0 || end < start || end > SequenceLen) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    BufferFactory::NewCharBufferEx(charSequence,
                                   charBuffer);
    CharBuffer* obj = (CharBuffer*)(*charBuffer)->Probe(
                                                        EIID_CharBuffer);
    obj->mPosition  = start;
    obj->mLimit     = end;

    return NOERROR;
}




