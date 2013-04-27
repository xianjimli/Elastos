
#include "CSocketOutputStream.h"

ECode CSocketOutputStream::Close()
{
    // TODO: Add your code here
    return mSocket->Close();
}

ECode CSocketOutputStream::Flush()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSocketOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    // TODO: Add your code here
    ArrayOf<Byte> *buffer = ArrayOf<Byte>::Alloc(1);
    ECode ec = NOERROR;
    (*buffer)[0] = (Byte) (oneByte & 0xFF);

    Int32 number;
    ec = mSocket->Write(*buffer, 0, 1, &number);
    ArrayOf<Byte>::Free(buffer);
    return ec;
}

ECode CSocketOutputStream::WriteBuffer(
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    // TODO: Add your code here
    Int32 number;
    return mSocket->Write(buffer, 0, buffer.GetLength(), &number);
}

ECode CSocketOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    // TODO: Add your code here
    Int32 number;
    if (buffer.GetLength() == 0) {
        return E_INVALID_ARGUMENT;
    }
    if (0 <= offset && offset <= buffer.GetLength() && 0 <= count && count <= buffer.GetLength() - offset) {
        mSocket->Write(buffer, offset, count, &number);
    } else {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
}

ECode CSocketOutputStream::constructor(
    /* [in] */ ISocketImpl * pImpl)
{
    // TODO: Add your code here
    mSocket = (PlainSocketImpl*) pImpl;
    return NOERROR;
}

