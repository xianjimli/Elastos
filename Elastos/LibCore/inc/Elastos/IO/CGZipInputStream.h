//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#ifndef __CGZIPINPUTSTREAM_H__
#define __CGZIPINPUTSTREAM_H__

#include "_CGZipInputStream.h"
#include "zlib.h"
/*!
  @brief This class implements a stream filter for reading compressed data in the GZIP file format. 
  @since
  @sa
*/
CarClass(CGZipInputStream)
{
public:
/*!
	@brief Creates a null CGZipInputStream.
*/
    CGZipInputStream() :
        m_pIis(NULL),
        m_pBuf(NULL),
        m_isEof(FALSE),
        bytesRead(0),
        m_pDict(NULL) {}
/*!
	@brief the destructor of the CGZipInputStream.
*/
    ~CGZipInputStream();
/*!
	@brief Creates a new unzip file input stream which reads data from a specified 
	input stream.
	@param pStream <b>[in]</b> the underlying input stream.
	@param pIGZipInputStream <b>[out]</b> a pointer pointed to the pointer of the created 
	unzip file input stream's interface. 
   	@return
    @since
    @sa
*/
    CARAPI constructor(
        /* [in] */ IInputStream * pStream);
/*!
	@brief Creates a new unzip file input stream which reads data from a specified 
	input stream.
	@param pStream <b>[in]</b> the underlying input stream.
	@param windowBits <b>[in]</b>  Compression window's size(the size of history buffer) for
	2 logarithmic. Range of 8 to 15,in the case of loss of memory, the greater the 
	value the better the compression's effect, the default value is 15. When windowBits 
	take 8 to 15,means that the generated  zlib compressed data  contains the head and 
	tail.windowBits can take -15 to -8, that is-windowBIts, means that the generated 
	zlib compressed data  doesn't contain the head and tail or adler32-tested code.When 
	windowBits  adds 16, will add  the head and tail  gzip for the generated compressed 
	data .
	@param pIGZipInputStream <b>[out]</b> a pointer pointed to the pointer of the created 
	unzip file input stream's interface. 
   	@return
    @since
    @sa
*/
    CARAPI constructor(
        /* [in] */ IInputStream * pStream,
        /* [in] */ Int32 windowBits);
/*!
	@brief Creates a new unzip file input stream which reads data from a specified 
	input stream.
	@param  pStream <b>[in]</b> the underlying input stream.
	@param windowBits <b>[in]</b>  Compression window's size(the size of history buffer) for
	2 logarithmic. Range of 8 to 15,in the case of loss of memory, the greater the 
	value the better the compression's effect, the default value is 15. When windowBits 
	take 8 to 15,means that the generated  zlib compressed data  contains the head and 
	tail.windowBits can take -15 to -8, that is-windowBIts, means that the generated 
	zlib compressed data  doesn't contain the head and tail or adler32-tested code.When 
	windowBits  adds 16, will add  the head and tail  gzip for the generated compressed 
	data .
	@param dictionary <b>[in]</b> Set the dictionary, when compression and decompression,the 
	setting must be set the same.
	@param pIGZipInputStream <b>[out]</b> a pointer pointed to the pointer of the created 
	unzip file input stream's interface. 
   	@return
    @since
    @sa
*/
    CARAPI constructor(
        /* [in] */ IInputStream * pStream,
        /* [in] */ Int32 windowBits,
        /* [in] */ const BufferOf<Byte> & dictionary);
/*!
	@brief Gets an estimsted number of bytes can be read from the underlying input stream.
	@param pBytes <b>[out]</b> the number of bytes can be read from the underlying input stream.
   	@return
    @since
    @sa
*/
    CARAPI Available(
        /* [out] */ Int32 * pBytes);
/*!
	@brief Closes this input stream and releases any system resources associated with the
	stream. 
	@return
    @since
    @sa
*/
    CARAPI Close();
/*!
	@brief This method is used to verify the underlying stream is closed or not.
	@param readLimit <b>[in]</b> This argument has no effect.
   	@return
    @since
    @sa
*/
    CARAPI Mark(
        /* [in] */ Int32 readLimit);
/*!
	@brief This method is used to look up this input stream support the @a Mark method,
	that is to verify the underlying input stream is closed or not.if it is closed,returns 
	@a E_CLOSED_STREAM,if it is not closed,the argument @a pSupported will be set to true.
	@param pSupported <b>[in]</b> true if it support Mark;false otherwise.
   	@return
    @since
    @sa
*/
    CARAPI IsMarkSupported(
        /* [out] */ Boolean * pSupported);
/*!
	@brief Reads one byte of data from this input stream,and stores tha read data into the address which 
	@a pByte points to.
	@param pByte <b>[out]</b> used to store the data read out. 
	@return
    @since
    @sa
 */
    CARAPI Read(
        /* [out] */ Byte * pValue);
/*!
	@brief Reads all the bytes that can be read from this input stream into the buffer @a pBuffer.
	@param pBuffer <b>[out]</b> used to store the bytes read out from the underlying input stream.
   	@return
    @since
    @sa
*/
    CARAPI ReadBuffer(
        /* [out] */ BufferOf<Byte> * pBuffer);
/*!
   @brief Reads up to @a length bytes of data from this input stream into an buffer of bytes. 
   the total number of bytes read into the buffer, or -1 if there is no more data because the
   end of the file has been reached. 
   @param offset <b>[in]</b> the start offset in the buffer array @a pBuffer at which the data is read.
   @param length <b>[in]</b> the maximum number of bytes to read. 
   @param pBuffer <b>[out]</b> the buffer into which the data is read.
   @return
   @since
   @sa
*/
    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ BufferOf<Byte> * pBuffer);
/*!
	@brief Set the current position to the index of the last invokation to Mark method.
	@return
    @since
    @sa
*/
    CARAPI Reset();
/*!
	@brief Skips over and discards @a length bytes of data from the input stream.
	@param length <b>[in]</b> the number of bytes to be skipped.returns the actual number of bytes
	skipped.
   	@return
    @since
    @sa	
*/
    CARAPI Skip(
        /* [in] */ Int32 length);

private:
/*!
	@brief Reads the bytes from the underlying input stream ,and put the bytes into the
	specified stream @a z_stream.
	@return
    @since
    @sa
*/
    ECode fill();
/*!
	@brief 
	@param pBuffer
	@param bufferSize
	@param pCount
	@return
    @since
    @sa
*/
    ECode InflateStream(
        Byte * pBuffer,
        Int32 bufferSize,
        Int32 * pCount);

private:
/*!
	@brief the underlying input stream.
*/
    IInputStream * m_pIis;

/*!
	@brief A pointer of Byte buffer that was provided by the creator of the stream. Elements m_pBuf[0]
	through m_pBuf[m_tail-1] are the only bytes that can ever be read from the stream; element m_pBuf[m_pos]
	is the next byte to be read. 
*/
    BufferOf<Byte>* m_pBuf;
/*!
	@brief the compressed stream.
*/
    z_stream m_stream;
/*!
	@brief the variable used to test if the current position is the end of the stream.
*/
    Boolean m_isEof;
/*!
	@brief the number of the bytes that have been read.
*/
    Int32 bytesRead;
/*!
	@brief the dictionary set for the stream.
*/
    const BufferOf<Byte>* m_pDict;
};

#endif // __CGZIPINPUTSTREAM_H__
