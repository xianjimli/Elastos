
#include "cmdef.h"
#include "BufferFactory.h"
#include "ReadWriteHeapByteBuffer.h"
#include "ReadWriteCharArrayBuffer.h"
#include "CharSequenceAdapter.h"
#include "ReadWriteDirectByteBuffer.h"
#include "ReadWriteDoubleArrayBuffer.h"
#include "ReadWriteFloatArrayBuffer.h"
#include "ReadWriteInt32ArrayBuffer.h"
#include "ReadWriteInt64ArrayBuffer.h"
#include "ReadWriteInt16ArrayBuffer.h"

ECode BufferFactory::NewByteBuffer(
    /* [in] */ ArrayOf<Byte>* array,
    /* [out] */ IByteBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    ReadWriteHeapByteBuffer* buffer = new ReadWriteHeapByteBuffer(array);
    if (buffer->Probe(EIID_IByteBuffer) != NULL) {
        *buf = (IByteBuffer*)buffer->Probe(EIID_IByteBuffer);
        (*buf)->AddRef();
    }
    else {
        *buf = NULL;
    }

    return NOERROR;
}

ECode BufferFactory::NewByteBufferEx(
    /* [in] */ Int32 capacity,
    /* [out] */ IByteBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    ReadWriteHeapByteBuffer* buffer = new ReadWriteHeapByteBuffer(capacity);
    if (buffer->Probe(EIID_IByteBuffer) != NULL) {
        *buf = (IByteBuffer*)buffer->Probe(EIID_IByteBuffer);
        (*buf)->AddRef();
    }
    else {
        *buf = NULL;
    }

    return NOERROR;
}

ECode BufferFactory::NewCharBuffer(
    /* [in] */ ArrayOf<Char32>* array,
    /* [out] */ ICharBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    ReadWriteCharArrayBuffer* buffer = new ReadWriteCharArrayBuffer(array);
    if (buffer->Probe(EIID_ICharBuffer) != NULL) {
        *buf = (ICharBuffer*)buffer->Probe(EIID_ICharBuffer);
        (*buf)->AddRef();
    }
    else {
        *buf = NULL;
    }

    return NOERROR;
}

ECode BufferFactory::NewCharBufferEx(
    /* [in] */ ICharSequence* chseq,
    /* [out] */ ICharBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    Int32 len;
    chseq->GetLength(&len);
    CharSequenceAdapter* buffer = new CharSequenceAdapter(len, chseq);
    if (buffer->Probe(EIID_ICharBuffer) != NULL) {
        *buf = (ICharBuffer*)buffer->Probe(EIID_ICharBuffer);
        (*buf)->AddRef();
    }
    else {
        *buf = NULL;
    }

    return NOERROR;
}

ECode BufferFactory::NewCharBufferEx2(
    /* [in] */ Int32 capacity,
    /* [out] */ ICharBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    ReadWriteCharArrayBuffer* buffer = new ReadWriteCharArrayBuffer(capacity);
    if (buffer->Probe(EIID_ICharBuffer) != NULL) {
        *buf = (ICharBuffer*)buffer->Probe(EIID_ICharBuffer);
        (*buf)->AddRef();
    }
    else {
        *buf = NULL;
    }

    return NOERROR;
}

ECode BufferFactory::NewDirectByteBuffer(
    /* [in] */ Int32 capacity,
    /* [out] */ IByteBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    ReadWriteDirectByteBuffer* buffer = new ReadWriteDirectByteBuffer(capacity);
    if (buffer->Probe(EIID_IByteBuffer) != NULL) {
        *buf = (IByteBuffer*)buffer->Probe(EIID_IByteBuffer);
        (*buf)->AddRef();
    }
    else {
        *buf = NULL;
    }

    return NOERROR;
}

ECode BufferFactory::NewDoubleBuffer(
    /* [in] */ ArrayOf<Double>* array,
    /* [out] */ IDoubleBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    ReadWriteDoubleArrayBuffer* buffer = new ReadWriteDoubleArrayBuffer(array);
    if (buffer->Probe(EIID_IDoubleBuffer) != NULL) {
        *buf = (IDoubleBuffer*)buffer->Probe(EIID_IDoubleBuffer);
    (*buf)->AddRef();
    }
    else {
        *buf = NULL;
    }

    return NOERROR;
}

ECode BufferFactory::NewDoubleBufferEx(
    /* [in] */ Int32 capacity,
    /* [out] */ IDoubleBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    ReadWriteDoubleArrayBuffer* buffer = new ReadWriteDoubleArrayBuffer(capacity);
    if (buffer->Probe(EIID_IDoubleBuffer) != NULL) {
        *buf = (IDoubleBuffer*)buffer->Probe(EIID_IDoubleBuffer);
        (*buf)->AddRef();
    }
    else {
        *buf = NULL;
    }

    return NOERROR;
}

ECode BufferFactory::NewFloatBuffer(
    /* [in] */ ArrayOf<Float>* array,
    /* [out] */ IFloatBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    ReadWriteFloatArrayBuffer* buffer = new ReadWriteFloatArrayBuffer(array);
    if (buffer->Probe(EIID_IFloatBuffer) != NULL) {
        *buf = (IFloatBuffer*)buffer->Probe(EIID_IFloatBuffer);
        (*buf)->AddRef();
    }
    else {
        *buf = NULL;
    }

    return NOERROR;
}

ECode BufferFactory::NewFloatBufferEx(
    /* [in] */ Int32 capacity,
    /* [out] */ IFloatBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    ReadWriteFloatArrayBuffer* buffer = new ReadWriteFloatArrayBuffer(capacity);
    if (buffer->Probe(EIID_IFloatBuffer) != NULL) {
        *buf = (IFloatBuffer*)buffer->Probe(EIID_IFloatBuffer);
        (*buf)->AddRef();
    }
    else {
        *buf = NULL;
    }

    return NOERROR;
}

ECode BufferFactory::NewInt32Buffer(
    /* [in] */ Int32 capacity,
    /* [out] */ IInt32Buffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    ReadWriteInt32ArrayBuffer* buffer = new ReadWriteInt32ArrayBuffer(capacity);
    if (buffer->Probe(EIID_IInt32Buffer) != NULL) {
        *buf = (IInt32Buffer*)buffer->Probe(EIID_IInt32Buffer);
        (*buf)->AddRef();
    }
    else {
        *buf = NULL;
    }

    return NOERROR;
}

ECode BufferFactory::NewInt32BufferEx(
    /* [in] */ ArrayOf<Int32>* array,
    /* [out] */ IInt32Buffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    ReadWriteInt32ArrayBuffer* buffer = new ReadWriteInt32ArrayBuffer(array);
    if (buffer->Probe(EIID_IInt32Buffer) != NULL) {
        *buf = (IInt32Buffer*)buffer->Probe(EIID_IInt32Buffer);
        (*buf)->AddRef();
    }
    else {
        *buf = NULL;
    }

    return NOERROR;
}

ECode BufferFactory::NewInt64Buffer(
    /* [in] */ Int32 capacity,
    /* [out] */ IInt64Buffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    ReadWriteInt64ArrayBuffer* buffer = new ReadWriteInt64ArrayBuffer(capacity);
    if (buffer->Probe(EIID_IInt64Buffer) != NULL) {
        *buf = (IInt64Buffer*)buffer->Probe(EIID_IInt64Buffer);
        (*buf)->AddRef();
    }
    else {
        *buf = NULL;
    }

    return NOERROR;
}

ECode BufferFactory::NewInt64BufferEx(
    /* [in] */ ArrayOf<Int64>* array,
    /* [out] */ IInt64Buffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    ReadWriteInt64ArrayBuffer* buffer = new ReadWriteInt64ArrayBuffer(array);
    if (buffer->Probe(EIID_IInt64Buffer) != NULL) {
        *buf = (IInt64Buffer*)buffer->Probe(EIID_IInt64Buffer);
        (*buf)->AddRef();
    }
    else {
        *buf = NULL;
    }

    return NOERROR;
}

ECode BufferFactory::NewInt16Buffer(
    /* [in] */ Int32 capacity,
    /* [out] */ IInt16Buffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    ReadWriteInt16ArrayBuffer* buffer = new ReadWriteInt16ArrayBuffer(capacity);
    if (buffer->Probe(EIID_IInt16Buffer) != NULL) {
        *buf = (IInt16Buffer*)buffer->Probe(EIID_IInt16Buffer);
        (*buf)->AddRef();
    }
    else {
        *buf = NULL;
    }

    return NOERROR;
}

ECode BufferFactory::NewInt16BufferEx(
    /* [in] */ ArrayOf<Int16>* array,
    /* [out] */ IInt16Buffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    ReadWriteInt16ArrayBuffer* buffer = new ReadWriteInt16ArrayBuffer(array);
    if (buffer->Probe(EIID_IInt16Buffer) != NULL) {
        *buf = (IInt16Buffer*)buffer->Probe(EIID_IInt16Buffer);
        (*buf)->AddRef();
    }
    else {
        *buf = NULL;
    }

    return NOERROR;
}
