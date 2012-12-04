
#include "FilterOutputStream.h"

FilterOutputStream::FilterOutputStream()
{
}

FilterOutputStream::~FilterOutputStream()
{
}

ECode FilterOutputStream::Init(
    /* [in] */ IOutputStream* outs)
{
    mOut = outs;
    return NOERROR;
}

ECode FilterOutputStream::Close()
{
    ECode ec = mOut->Flush();
    ECode ec2 = mOut->Close();
    if(ec != NOERROR || ec2 != NOERROR){
        return ec != NOERROR ? ec : ec2;
    }
    return NOERROR;
}

ECode FilterOutputStream::Flush()
{
    return mOut->Flush();
}

ECode FilterOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    return mOut->Write(oneByte);
}

ECode FilterOutputStream::WriteBuffer(
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    return WriteBufferEx(0, buffer.GetLength(), buffer);
}

ECode FilterOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    // Force null buffer check first!
    if (offset > buffer.GetLength() || offset < 0) {
//      throw new ArrayIndexOutOfBoundsException("Offset out of bounds: " + offset);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (count < 0 || count > buffer.GetLength() - offset) {
//      throw new ArrayIndexOutOfBoundsException("Length out of bounds: " + length);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    for (Int32 i = 0; i < count; i++) {
        // Call write() instead of out.write() since subclasses could
        // override the write() method.
        Write(buffer[offset + i]);
    }

    return NOERROR;
}
