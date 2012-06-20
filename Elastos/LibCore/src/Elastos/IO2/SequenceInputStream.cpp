#include "SequenceInputStream.h"
#include "_CSequenceInputStream.h"

using namespace Elastos;

SequenceInputStream::SequenceInputStream()
{

}
SequenceInputStream::~SequenceInputStream()
{
}

ECode SequenceInputStream::Init(
    /* [in] */ IInputStream* s1,
    /* [in] */ IInputStream* s2)
{
    if (s1 == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
//    vector<InputStream> inVector = new vector<InputStream>(1);
//    inVector.addElement(s2);
//    e = inVector.elements();
    mIn = s1;
    return NOERROR;
}

ECode SequenceInputStream::Init(
    /* [in] */ IObjectEnumerator* e)
{
    this->e = e;
    Boolean pSucceeded = FALSE;
    e->MoveNext(&pSucceeded);
    if (pSucceeded) {
        e->Current((PInterface*)(&mIn));
        if (mIn == NULL) {
            return E_NULL_POINTER_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode SequenceInputStream::Available(
    /* [out] */ Int32* number)
{
    assert(number != NULL);
    if (e != NULL && mIn != NULL) {
        return mIn->Available(number);
    }
    *number = 0;

    return NOERROR;
}

ECode SequenceInputStream::Close()
{
    while (mIn != NULL) {
        NextStream();
    }
    e = NULL;
    return NOERROR;
}

ECode SequenceInputStream::Read(
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    while (mIn != NULL) {
        mIn->Read(value);
        if (value >= 0) {
            return NOERROR;
        }
        NextStream();
    }
    return -1;
}

ECode SequenceInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ BufferOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);
    if (mIn == NULL) {
        *number = -1;
        return -1;
    }
    // BEGIN android-changed
    if (buffer == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    // avoid int overflow
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // used (offset | count) < 0 instead of (offset < 0) || (count < 0)
    // to safe one operation
    if ((offset | count) < 0 || offset > buffer->GetCapacity() - count) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed
    while (mIn != NULL) {
        mIn->ReadBufferEx(offset, count, buffer, number);
        if (number >= 0) {
            return NOERROR;
        }
        NextStream();
    }
    *number = -1;
    
    return -1;
}

ECode SequenceInputStream::NextStream()
{
    if (mIn != NULL) {
        mIn->Close();
    }
    Boolean pSucceeded = FALSE;
    e->MoveNext(&pSucceeded);
    if (pSucceeded) {
        e->Current((PInterface*)(&mIn));
        if (mIn == NULL) {
            return E_NULL_POINTER_EXCEPTION;
        }
    } else {
        mIn = NULL;
    }
    return NOERROR;
}

ECode SequenceInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return mIn->Mark(readLimit);
}

ECode SequenceInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    assert(supported != NULL);
    return mIn->IsMarkSupported(supported);
}

ECode SequenceInputStream::ReadBuffer(
    /* [out] */ BufferOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    return ReadBufferEx(0, buffer->GetCapacity(), buffer, number);
}

ECode SequenceInputStream::Reset()
{
    return mIn->Reset();
}

ECode SequenceInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    return mIn->Skip(count, number);
}