//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#ifndef __CBUFFEREDINPUTSTREAM_H__
#define __CBUFFEREDINPUTSTREAM_H__

#include "_CBufferedInputStream.h"
/*!
 @brief A CBufferedInputStream adds functionality to another input stream-namely, the ability to 
 buffer the input and to support the mark and reset methods. 
 
 When the CBufferedInputStream is created, an internal buffer array is created. As bytes from the
 stream are read or skipped, the internal buffer is refilled as necessary from the contained input 
 stream, many bytes at a time.The mark operation remembers a point in the input stream and the reset
 operation causes all the bytes read since the most recent mark operation to be reread before new
 bytes are taken from the contained input stream.
 @since
 @sa
*/
CarClass(CBufferedInputStream)
{
public:
/*!
  @brief This method used to get an estimate of the number of bytes that can be read (or skipped
  over) from this input stream.
  
  @param pBytes <b>[out]</b> The pointer points to the value which stores the number of bytes that can be
  read (or skipped over) from this input stream.
  @return
  @since
  @sa
*/
    CARAPI Available(
        /* [out] */ Int32 * pBytes);

/*!
   @brief Closes this input stream and releases any system resources associated with the stream.
   
   Once the stream has been closed, further Read(), Available(), Reset(), or Skip() invocations 
   will be failed. Closing a previously closed stream has no effect. 
   @return
   @since
   @sa
*/		
    CARAPI Close();

/*!
  @brief Mark the current position in this input stream.A subsequent call to the @a reset 
  method repositions this stream at the last marked position so that the subsequent reads
  re-read the same bytes.
  
  This method also set the the maximum limit of bytes that can be read before the mark position 
  becomes invalid by setting the @a readLimit argument.
  The @a readLimit argument tells this input stream to allow how many bytes to be read before
  the mark position get invalidated.
  @param readLimit <b>[in]</b> The maximum limit of bytes that can be read before the mark position 
  becomes invalid.
  @return
  @since
  @sa
*/	
    CARAPI Mark(
        /* [in] */ Int32 readLimit);

/*!
 @brief Make this input stream support the @a mark and @a reset methods.After the invocation
 to this method,the underlying stream will be always support the @a mark and @a reset methods.
 @param pSupported <b>[out]</b> Point to the data,true if this stream instance supports the mark and 
 reset methods;false otherwise.
 @return
 @since
 @sa
*/
    CARAPI IsMarkSupported(
        /* [out] */ Boolean * pSupported);

/*!
 @brief Reads a byte from the input stream and stores it into the buffer array @a pByte.
 @param pByte <b>[out]</b> the data that is read from the input stream.
 @return
 @since
 @sa
*/
    CARAPI Read(
        /* [out] */ Byte * pByte);

/*!
 @brief Reads all the data from the input stream into the specified buffer array.
 @param pBuffer <b>[out]</b> destination buffer array.
 @return
 @since
 @sa
*/
    CARAPI ReadBuffer(
        /* [out] */ BufferOf<Byte> * pBuffer);

/*!
 @brief Reads up to @a length bytes of data from the input stream into the specified buffer
 array @a pBuffer, starting at the given offset @a offset.
 @param offset <b>[in]</b> offset at which to start storing bytes.
 @param length <b>[in]</b> maximum number of bytes to read.
 @param pBuffer <b>[out]</b> destination buffer.
 @return
 @since
 @sa
*/	
    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ BufferOf<Byte> * pBuffer);

/*!
 @brief Reposition this stream to the position at the time the @a mark method was last called 
 on this input stream.
  
 If m_markpos is less than zero (no m_mark has been set or the Mark has been invalidated),unable
 to call the Reset(). Otherwise, m_pos is set equal to m_markpos. 
 @return
 @since
 @sa
*/
    CARAPI Reset();

/*!
 @brief Skips over and discards @a length bytes of data from this input stream. 
	
 The Skip method may, for a variety of reasons, end up skipping over some smaller number of bytes, 
 possibly 0. This may result from any of a number of conditions; reaching end of file before
 @a length bytes have been skipped is only one possibility.If @a length is negative, no bytes are 
 skipped. The skip method of this class creates a byte array and then repeatedly reads into it 
 until @a lengthbytes have been read or the end of the stream has been reached.
 @param length <b>[in]</b> The number of bytes to be skipped.
 @return
 @since
 @sa
*/
    CARAPI Skip(
        /* [in] */ Int32 length);

/*!
  @brief Creates one CBufferedInputStream and saves its argument,the IInputStream @a pIInputStream,
  for later use.An internal buffer array is created and stored in @a m_pBuf.
  @param pStream <b>[in]</b> The underlying input stream.
  @param pIInputStream <b>[out]</b> A pointer pointed to the pointer of the underlying input stream.
  @return
  @since
  @sa
*/
    CARAPI constructor(
        /* [in] */ IInputStream * pStream);

/*!
  @brief Creates one CBufferedInputStream with the specified buffer size,and save its argument,
  the IInputStream pointer @a pStream,for later use.An internal buffer array of length @a buff
  -erSize is created and stored in @a m_pBuf.
  @param pStream <b>[in]</b> The underlying input stream.
  @param bufferSize <b>[in]</b> The buffer size.
  @param pIInputStream <b>[out]</b> A pointer pointed to the pointer of the underlying input stream.
  @return
  @since
  @sa
*/
    CARAPI constructor(
        /* [in] */ IInputStream * pStream,
        /* [in] */ Int32 defaultBufferSize);

/*!
	@brief Destructor.To close this output stream and release any system resources 
	associated with the stream.
*/	
    ~CBufferedInputStream();

/*!
  @brief Creates an empty buffered input stream by initializing both its underlying input stream and
  its buffer array to be null.
  @since
  @sa
*/
    CBufferedInputStream() : m_pIis(NULL), m_pBuf(NULL) {}

private:
/*!
  @brief Reads @a len bytes from this buffered input stream,and store data in the specified
  byte buffer @a pBuf starting at @a offset and stores the number if red bytes in @a pRead.
  @param pBuf <b>[out]</b> destination buffer.
  @param offset <b>[in]</b> offset at which to start storing bytes.
  @param len <b>[in]</b> maximum number of bytes to read.
  @param pRead <b>[out]</b> the number has been read.
  @return
  @since
  @sa
*/
    ECode read(BufferOf<Byte> *pBuf, Int32 offset, Int32 len, Int32 *pRead);
/*!
  @brief 
  @return
  @since
  @sa
*/
    ECode fill();

private:
/*!
  @brief The underlying input stream.
*/
    IInputStream * m_pIis;
	
/*!
  @brief The internal buffer array where the data is stored.
*/
    BufferOf<Byte>* m_pBuf;// we need a automatic updater to update the m_buf,
	
/*!
	@brief The current position in the buffer. This is the index of the next 
	character to be read from the buf array. 
*/

    Int32 m_pos;
	
/*!
   @brief The value of the @a m_pos field at the time the last @a mark method was called.
   
   This value is always in the range -1 through @a m_pos. If there is no marked position in the input 
   stream, this field is -1. If there is a marked position in the input stream, then @a buf[m_markpos] 
   is the first byte to be supplied as input after a reset operation. If @a m_markpos is not -1, then 
   all bytes from positions @a buf[m_markpos] through @a buf[m_pos-1] must remain in the  buffer array 
   (though they may be moved to another place in the buffer array; they may not be discarded unless and 
   until the difference between @a m_pos and @a m_markpos exceeds @a m_markLimit. 
*/
    Int32 m_markpos;
	
/*!
	@brief The maximum read ahead allowed after a call to the @a mark method before subsequent
	calls to the @a reset method fail. Whenever the difference between @a m_pos and @a m_markpos 
	exceeds @a m_markLimit, then the @a mark may be dropped by setting @a markpos to -1. 
*/
    Int32 m_markLimit;
};
#endif // __CBUFFEREDINPUTSTREAM_H__
