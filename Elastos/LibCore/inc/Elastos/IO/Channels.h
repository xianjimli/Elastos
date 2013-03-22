#ifndef __CHANNELS_H__
#define __CHANNELS_H__

// #include <elastos.h>
// #include <Elastos.Core.h>
#include <Elastos.IO_server.h>
#include "AbstractInterruptibleChannel.h"

//using namespace Elastos;

class Channels
{
public:
	/*
	Channels();
	~Channels();
	*/
	/** Channels just provides only static functions, so use implicited
	 *  constructor
	 */
	/* 
	static CARAPI NewInputStream(
        IReadableByteChannel * pChannel,
        IInputStream ** ppInputStream);

    static CARAPI NewOutputStream(
        IWritableByteChannel * pChannel,
        IOutputStream ** ppOutputStream);
	*/
	/*
    static CARAPI NewChannel(
        IInputStream* inputStream,
        IReadableByteChannel** inputChannel);
	*/

	static IReadableByteChannel* NewChannel(IInputStream* inputStream);
	static IWritableByteChannel* NewChannel(IOutputStream* outputStream);
	
	/*
    static CARAPI NewChannelEx(
        IOutputStream* outputStream,
        IWritableByteChannel** outputChannel);
	*/
	/*
    static CARAPI NewReader(
        IReadableByteChannel * pChannel,
        ICharsetDecoder * pDecoder,
        Int32 minBufferCapacity,
        IReader ** ppReader);

    static CARAPI NewReaderEx(
        IReadableByteChannel * pChannel,
        const String& charsetName,
        IReader ** ppReader);

    static CARAPI NewWriter(
        IWritableByteChannel * pChannel,
        ICharsetEncoder * pEncoder,
        Int32 minBufferCapacity,
        IWriter ** ppWriter);

    static CARAPI NewWriterEx(
        IWritableByteChannel * pChannel,
        const String& charsetName,
        IWriter ** ppWriter);
	*/
private:
	/*
     * Wrapper class used for newChannel(InputStream inputStream)
     */
	///*
	class ReadableByteChannelImpl : public AbstractInterruptibleChannel 
	{
	public:
        ReadableByteChannelImpl(IInputStream* inputStreamPtr) : AbstractInterruptibleChannel()
		{
            mInputStreamPtr = inputStreamPtr;
        }

        int Read(IByteBuffer* targetPtr) {
            if (!IsOpen()) {
                return -1;
            }

            int bytesRemain;
			targetPtr->Remaining(&bytesRemain);
			ArrayOf<Byte>* buff = ArrayOf<Byte>::Alloc(bytesRemain);
            int readCount = 0;
            mInputStreamPtr->ReadBuffer(buff, &readCount);
            
            if (readCount > 0) {
				targetPtr->PutBytesEx(0, readCount, *buff);
            }
            return readCount;
        }

	protected:
        // @Override
        void ImplCloseChannel() {
			mInputStreamPtr->Close();
        }

	private:
		IInputStream*		mInputStreamPtr;
    };	// end of class ReadableByteChannelImpl
	//*/
	/*
     * Wrapper class used for newChannel(OutputStream outputStream)
     */
	///*
    class WritableByteChannelImpl : public AbstractInterruptibleChannel 
	{
	public:
        WritableByteChannelImpl(IOutputStream* outputStream) : AbstractInterruptibleChannel()
		{
            mOutputStream = outputStream;
        }
		
        int Write(IByteBuffer* source) {
            if (!IsOpen()) {
                return -1;
            }
            int bytesRemain;
			source->Remaining(&bytesRemain);

            if (bytesRemain == 0) {
                return 0;
            }
			
			ArrayOf<Byte>* buff = ArrayOf<Byte>::Alloc(bytesRemain);
            source->GetByte(buff->GetPayload());
            mOutputStream->WriteBufferEx(0, bytesRemain, *buff);

            return bytesRemain;
        }

		protected:
        void ImplCloseChannel() {
            mOutputStream->Close();
        }
		
	private:
		IOutputStream* 	mOutputStream;
    };
	//*/
    // TODO: Add your private member variables here.
};
#endif // __CHANNELS_H__



