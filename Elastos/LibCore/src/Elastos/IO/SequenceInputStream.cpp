#include "SequenceInputStream.h"
#include "_CSequenceInputStream.h"
#include "Elastos.Utility.h"

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
    CObjectContainer::New((IObjectContainer**)&mContainer);
    mContainer->Add(s2);
    mContainer->GetObjectEnumerator((IObjectEnumerator**)&mEnum);
    mIn = s1;
    return NOERROR;
}

ECode SequenceInputStream::Init(
    /* [in] */ IObjectEnumerator* e)
{
    mEnum = e;
    Boolean succeeded = FALSE;
    e->MoveNext(&succeeded);
    if (succeeded) {
        e->Current((IInterface**)(&mIn));
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
    if (mEnum != NULL && mIn != NULL) {
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
    mEnum = NULL;
    mContainer = NULL;
    return NOERROR;
}

ECode SequenceInputStream::NextStream()
{
    if (mIn != NULL) {
        mIn->Close();
    }
    Boolean succeeded = FALSE;
    mEnum->MoveNext(&succeeded);
    if (succeeded) {
        mEnum->Current((IInterface**)(&mIn));
        if (mIn == NULL) {
            return E_NULL_POINTER_EXCEPTION;
        }
    } else {
        mIn = NULL;
    }
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
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);
    if (mIn == NULL) {
        *number = -1;
        return NOERROR;
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
    if ((offset | count) < 0 || offset > buffer->GetLength() - count) {
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
    return NOERROR;
}
