
#include "graphics/CreateOutputStreamAdaptor.h"

SkStream* CreateInputStreamAdaptor(
    /* [in] */ IInputStream* stream,
    /* [in] */ ArrayOf<Byte>* storage,
    /* [in] */ Int32 markSize)
{
    return NULL;
}

class SkOutputStream : public SkWStream {
public:
    SkOutputStream(
        /* [in] */ IOutputStream* stream,
        /* [in] */ ArrayOf<Byte>* storage)
        : mOutputStream(stream)
        , mByteArray(storage)
    {
        mCapacity = storage->GetLength();
    }

    virtual bool write(
        /* [in] */ const void* buffer,
        /* [in] */ size_t size)
    {
        ArrayOf<Byte>* storage = mByteArray;

        while (size > 0) {
            size_t requested = size;
            if (requested > (size_t)mCapacity) {
                requested = mCapacity;
            }

            storage->Replace(0, (const Byte*)buffer, requested);

            if (FAILED(mOutputStream->WriteBufferEx(
                0, (Int32)requested, *storage))) {
                return false;
            }

            buffer = (void*)((char*)buffer + requested);
            size -= requested;
        }
        return true;
    }

    virtual void flush() {
        mOutputStream->Flush();
    }

private:
    IOutputStream*  mOutputStream;  // the caller owns this object
    ArrayOf<Byte>*  mByteArray;     // the caller owns this object
    Int32           mCapacity;
};

SkWStream* CreateOutputStreamAdaptor(
    /* [in] */ IOutputStream* stream,
    /* [in] */ ArrayOf<Byte>* storage)
{
    return new SkOutputStream(stream, storage);
}

