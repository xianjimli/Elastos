//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#ifndef __CBUFFEREDOUTPUTSTREAM_H__
#define __CBUFFEREDOUTPUTSTREAM_H__

#include "_CBufferedOutputStream.h"
/*!
 @brief The class implements a buffered output stream. By setting up such an output stream, 
 an application can write bytes to the underlying output stream without necessarily causing
 a call to the underlying system for each byte written. 
 @since
 @sa
*/
CarClass(CBufferedOutputStream)
{
public:
/*!
  @brief Close this output stream and releases any system resources associated with this stream.
  @return
  @since
  @sa
*/
    CARAPI Close();
	
/*!
  @brief Flushes this buffered output stream.This forces any buffered output bytes to be written 
  out to the underlying output stream.
  @return
  @since
  @sa
*/
    CARAPI Flush();
	
/*!
 @brief Writes the specified byte to this buffered output stream.
 @param byte <b>[in]</b> the data.
 @return
 @since
 @sa
*/
    CARAPI Write(
        /* [in] */ Byte byte);
		
/*!
 @brief Writes all the bytes from the specified byte buffer to this buffered output stream.
 @param buffer <b>[in]</b> the reference of the data to be written.
 @param pBytesWritten <b>[out]</b> the pointer points to the number of the data has been written.
 @return
 @since
 @sa
*/
    CARAPI WriteBuffer(
        /* [in] */ const BufferOf<Byte> & buffer,
        /* [out] */ Int32 * pBytesWritten);
		
/*!
 @brief Writes @a length bytes from the specified byte buffer starting at @a offset to this
 buffered output stream.
 @param offset <b>[in]</b> the start offset in the data.
 @param length <b>[in]</b> the number of bytes to write.
 @param buffer <b>[in]</b> the reference of the data to be written.
 @param pBytesWritten <b>[out]</b> the pointer points to the number of the data has been written.	
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
 @brief Creates one CBufferedOutputStream and saves its argument,the IOutputStream @a pIOutputStream,
 for later use.An internal buffer array is created and stored in @a m_pBuf.
 @param pStream <b>[in]</b> The underlying output stream.
 @param pIOutputStream <b>[out]</b> A pointer pointed to the pointer of the underlying Output stream.
 @return
 @since
 @sa
*/		
    CARAPI constructor(
        /* [in] */ IOutputStream * pStream);

/*!
 @brief Creates one CBufferedOutputStream and saves its argument,the IOutputStream @a pIOutputStream,
 for later use.An internal buffer array is created and stored in @a m_pBuf which is specified to the
 fixed buffer size @a bufferSize.
 @param pStream <b>[in]</b> The underlying output stream.
 @param bufferSize <b>[in]</b> The buffer size.
 @param pIOutputStream <b>[out]</b> A pointer pointed to the pointer of the underlying Output stream.
 @return
 @since
 @sa
*/		
    CARAPI constructor(
        /* [in] */ IOutputStream * pStream,
        /* [in] */ Int32 bufferSize);

/*!
 @brief Destructor.To close this input stream and release any system resources associated with the stream.
 */		
    ~CBufferedOutputStream();

/*!
 @brief Creates an empty buffered output stream by initializing both its underlying output stream and
 its buffer array to be null.
*/
    CBufferedOutputStream() : m_pIos(NULL), m_pBuf(NULL) {}

private:
/*!
	@brief Rewrite all the bytes in the buffer @a m_pBuf into the underlying output stream @a m_pIos,and 
	set the used buffer array data to be zero.
*/
    ECode flushBuffer();
/*!
	@brief The underlying output stream.
*/
    IOutputStream * m_pIos;
/*!
	@brief The internal buffer array where the data is stored.
*/
    BufferOf<Byte>* m_pBuf;
};

#endif // __CBUFFEREDOUTPUTSTREAM_H__
