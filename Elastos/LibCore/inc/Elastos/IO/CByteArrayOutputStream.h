//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#ifndef __CBYTEARRAYOUTPUTSTREAM_H__
#define __CBYTEARRAYOUTPUTSTREAM_H__

#include "_CByteArrayOutputStream.h"
/*!
	@brief This class implements an output stream in which the data is written into a byte buffer.
	The buffer automatically grows as data is written to it.
	
    Closing a ByteArrayOutputStream has no effect. The methods in this class can be called after the
	stream has been closed. 


*/
CarClass(CByteArrayOutputStream)
{
public:
/*!
	@brief delete the pointer of the byte buffer associted with this output stream. Closing 
	a CByteArrayOutputStream has no effect. The methods in this class can be called after the
	stream has been closed. 
	@return
    @since
    @sa
*/
    CARAPI Close();

/*!
	@brief Flushes this output stream and forces any buffered output bytes to be written out. 
	The general contract of flush is that calling it is an indication that, if any bytes 
	previously written have been buffered by the implementation of the output stream, such 
	bytes should immediately be written to their intended destination. 
	
	If the intended destination of this stream is an abstraction provided by the underlying 
	operating system, for example a file, then flushing the stream guarantees only that bytes 
	previously written to the stream are passed to the operating system for writing; it does 
	not guarantee that they are actually written to a physical device such as a disk drive. 
	The flush method of OutputStream does nothing. 
	@return
    @since
    @sa
*/	
    CARAPI Flush();

/*!
	@brief Writes one specified byte data to this byte array output stream. 
	@param byte <b>[in]</b> the data.
	@return
    @since
    @sa
*/	
    CARAPI Write(
        /* [in] */ Byte byte);

/*!
	@brief Writes all bytes from the specified byte buffer to this byte array output stream.
	@param buffer <b>[in]</b> a reference to the buffer input.
	@param pBytesWritten <b>[out]</b> a pointer pointed to the number of how many bytes has been written
	on the byte array output stream.
	@return
    @since
    @sa
*/
    CARAPI WriteBuffer(
        /* [in] */ const BufferOf<Byte> & buffer,
        /* [out] */ Int32 * pBytesWritten);

/*!
	@brief Writes @a length bytes from the specified byte buffer array starting at offset @a offset 
	to this byte array output stream.
	@param offset <b>[in]</b> the start offset in the data.
	@param length <b>[in]</b> the number of bytes to write.
	@param buffer <b>[in]</b> a reference to the buffer input.
	@param pBytesWritten <b>[out]</b> a pointer pointed to the number of how many bytes has been written
	on the byte array output stream.
	@return
    @since
    @sa
*/		
    CARAPI WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ const BufferOf<Byte> & buffer,
        /* [out] */ Int32 * pBytesWritten);

/*!
	@brief Creates a new byte array output stream, with the specified bytes buffer array.
	@param buffer <b>[in]</b> the data.
	@param pBytesWritten <b>[out]</b> a pointer pointed to the number of how many bytes has been written
	on the byte array output stream.
	@return
    @since
    @sa	 
*/		
    CARAPI constructor(
        /* [in] */ const BufferOf<Byte> & buffer);

/*!
	@brief Creates a new byte array output stream, with the specified bytes buffer array,using the
	data between buffer[offset] and buffer[offset+length-1].
	@param buffer <b>[in]</b> the data.
	@param offset <b>[in]</b> the start offset in the data.
	@param length <b>[in]</b> the number of bytes to write.
	@param pBytesWritten <b>[out]</b> a pointer pointed to the number of how many bytes has been written
	on the byte array output stream.
	@return
    @since
    @sa
*/		
    CARAPI constructor(
        /* [in] */ const BufferOf<Byte> & buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

private:
/*!
	@brief The buffer where data is stored. 
*/
    BufferOf<Byte>* m_pBuf;

/*!
	@brief The index of the next character to be written on the output stream buffer. 
*/
    Int32 m_pos;
/*!
	@brief The number of valid bytes in the buffer. 
*/
    Int32 m_tail;
};

#endif // __CBYTEARRAYOUTPUTSTREAM_H__
