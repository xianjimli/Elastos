//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#ifndef __CBYTEARRAYINPUTSTREAM_H__
#define __CBYTEARRAYINPUTSTREAM_H__

#include "_CByteArrayInputStream.h"
/*!
   @brief A CByteArrayInputStream contains an internal buffer that contains bytes that may be 
   read from the stream. An internal counter keeps track of the next byte to be supplied by 
   the read method. 

   Closing a ByteArrayInputStream has no effect. The methods in this class can be called after
   the stream has been closed. 
   
*/
CarClass(CByteArrayInputStream)
{
public:
/*!
	@brief This method used to get an estimate number of bytes that can be read (or skipped
	over) from this input byte array stream.
	@param pBytes <b>[out]</b> an Int32 pointer that point to the value that represents the number can 
	be read (or skipped over).
	@return
    @since
    @sa
*/
    CARAPI Available(
        /* [out] */ Int32 * pBytes);

/*!
  @brief Closing a ByteArrayInputStream has no effect. The methods in this class can be called
  after the stream has been closed without generating an IOException. 
  @return
  @since
  @sa
*/
    CARAPI Close();

/*!
	@brief This method used to tell the user who call it that the CByteArrayInputStream don't 
	support to mark the current position for the internal buffer. 
	@param readLimit <b>[in]</b> the maximun number allowed to be read this CByteArrayInputStream.
	@return
    @since
    @sa
*/	
    CARAPI Mark(
        /* [in] */ Int32 readLimit);

/*!
  @brief This method is used to get @a true or @a false boolean value that represents Yes or No
  to support the @a Mark method.
  @param pSupported <b>[out]</b> @a true if this input stream supports @a Mark method; @a False otherwise.
  @return
  @since
  @sa
*/
    CARAPI IsMarkSupported(
        /* [out] */ Boolean * pSupported);

/*!
	@brief Reads only one byte for a time from the input stream and store the read byte data into @a pByte.
	@param pByte <b>[out]</b> the pointer of the buffer into which the data is read.
	@return
    @since
    @sa
*/
    CARAPI Read(
        /* [out] */ Byte * pByte);

/*!
	@brief Reads all bytes of data into an buffer of bytes from this input stream.
	@param pBuffer <b>[out]</b> the pointer of the buffer into which the data is read.
	@return
    @since
    @sa
*/
    CARAPI ReadBuffer(
        /* [out] */ BufferOf<Byte> * pBuffer);

/*!
	@brief Reads up to @a length bytes of data into the buffer of bytes from this input stream.
	
	If @a m_pos equals of larger than @a m_tail, then @a E_OUT_OF_STREAM is returned to indicate
	end of file. Otherwise, the number k of bytes read is equal to the smaller of @a length and 
	@a m_tail-m_pos . If k is positive, then bytes @a m_pBuf[m_pos] through @a m_pBuf[m_pos+k-1]
	are copied into @a pBuffer[offset] through @a pBuffer[offset+k-1] in the manner performed by 
	BufferOf.Replace. The value k is added into @a m_pos. 
	@param offset <b>[in]</b> the start offset in the destination array pointer @a pBuffer.
	@param length <b>[in]</b> the maximum number of bytes read. 
	@param pBuffer <b>[out]</b> the pointer of the buffer into which the data is read.
	@return
    @since
    @sa
*/
    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ BufferOf<Byte> * pBuffer);

/*!
	@brief Resets the buffer to the marked position. The marked position is 0 unless another 
	position was marked or an offset was specified in the constructor. 
	@return
    @since
    @sa
*/
    CARAPI Reset();

/*!
	@brief Skips @a length bytes of input from this input stream. Fewer bytes might be skipped if
	the end of the input stream is reached. The actual number k of bytes to be skipped is equal to
	the smaller of @a length and @a m_tail-m_pos. The value k is added into @a m_pos. 
	@param length <b>[in]</b> the number of bytes to be skipped.
	@return
    @since
    @sa
*/
    CARAPI Skip(
        /* [in] */ Int32 length);

/*!
	@brief Creates CByteArrayInputStream so that it uses @a buffer as its buffer array. The buffer
	array is not copied. The initial value of @a m_pos is 0 and the initial value of @a m_tail is
	the length of @a buffer. 
	@param buffer <b>[in]</b> a reference of the input buffer.
	@param pIInputStream <b>[out]</b> a pointer points to the pointer of the interface of the underlying
	input stream.
	@return
    @since
    @sa
*/
    CARAPI constructor(
        /* [in] */ const BufferOf<Byte> & buf);

/*!
	@brief Creates CByteArrayInputStream that uses @a m_pBuf as its buffer array. The initial value 
	of @a m_pos is @a offset and the initial value of @a m_tail is the minimum of @a offset+length 
	and @a m_pBuf.length. The buffer array is not copied. The buffer's mark is set to the specified
	offset. 
	@param buffer <b>[in]</b> a reference of the input buffer.
	@param offset <b>[in]</b> the offset in the buffer of the first byte to read.
	@param length <b>[in]</b> the maximum number of bytes to read from the buffer.
	@param pIInputStream <b>[out]</b> a pointer points to the pointer of the interface of the underlying
	input stream.
	@return
    @since
    @sa
*/
    CARAPI constructor(
        /* [in] */ const BufferOf<Byte> & buf,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 len);

/*!
	@brief Creates a null CByteArrayInputStream that uses null @a m_pBuf as its buffer array.The initial 
	value of @a m_pos is 0 , the initial value of @a m_tail is 0 and the initial value of @a m_mark is 0.
    @since
    @sa
*/
    CByteArrayInputStream() : m_pBuf(NULL), m_pos(0), m_mark(0), m_tail(0) {}

private:
 /*!
	@brief An pointer of byte buffer that was provided by the creator of the stream. Elements m_pBuf[0]
	through m_pBuf[m_tail-1] are the only bytes that can ever be read from the stream; element m_pBuf[m_pos]
	is the next byte to be read. 
 */
    BufferOf<Byte>* m_pBuf;

/*!
	@brief The index of the next character to read from the input stream buffer. This value should always be
	nonnegative and not larger than the value of @a m_tail. The next byte to be read from the input stream
	buffer will be @a m_pBuf[m_pos]. 
*/
    Int32 m_pos;
	
/*!
	@brief The currently marked position in the stream. ByteArrayInputStream objects are marked at position zero
	by default when constructed. They may be marked at another position within the buffer by the @a Mark() method. 
	The current buffer position is set to this point by the Reset() method. 
*/
    Int32 m_mark;
	
/*!
	@brief The index one greater than the last valid character in the input stream buffer. This value should always
	be nonnegative and not larger than the length of @a m_pBuf. It is one greater than the position of the last byte
	within @a m_pBuf that can ever be read from the input stream buffer. 
*/
    Int32 m_tail;
};

#endif // __CBYTEARRAYINPUTSTREAM_H__
