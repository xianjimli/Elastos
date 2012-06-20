//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#ifndef __CFILEINPUTSTREAM_H__
#define __CFILEINPUTSTREAM_H__

#include "_CFileInputStream.h"
#include "FileStream.h"
/*!
  @brief A CFileInputStream obtains input bytes from a file in a file system. What files are available
  depends on the host environment. 
  
  CFileInputStream is meant for reading streams of raw bytes such as image data.
  @since
  @sa
*/
CarClass(CFileInputStream)
{
public:
/*!
	@brief This method is used to get an estimated number of bytes could read from this input stream.
	@param pBytes <b>[in]</b> a pointer points to the value of the number of bytes could read from this input
	stream.
	@return
    @since
    @sa
*/
    CARAPI Available(
        /* [out] */ Int32 * pBytes);
/*!
	@brief Closes this file input stream and releases any system resources associated with the stream.
	@return
    @since
    @sa	
*/
    CARAPI Close();
/*!
	@brief This method is used to assign the current position @a m_pos to @a m_mark.
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
        /* [out] */ Byte * pByte);
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
   @param offset <b>[in]</b>  the start offset in the buffer array @a pBuffer at which the data is read.
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
   @param length <b>[in]</b>  the number of bytes to be skipped.returns the actual number of bytes skipped.
   @return
   @since
   @sa
*/
    CARAPI Skip(
        /* [in] */ Int32 length);
/*!
	@brief the destructor of CFileInputStream.to release all the associated resources.
*/
    ~CFileInputStream();
/*!
    @brief Creates a CFileInputStream by opening a connection to an actual file, the file
	named by the path name @a fileName in the file system. 
	@param fileName <b>[in]</b>  the system-dependent file name. 
	@param pIFileInputStream <b>[out]</b>  a pointer points to the pointer of the interface of the 
	underlying input stream.
	@return
    @since
    @sa
*/
    CARAPI constructor(
        /* [in] */ String fileName);
/*!
	@brief Creates a null CDataInputStream that uses null @a m_pIab as its underlying input stream,
	and initializes both @a m_pos and @a m_mark to be zero.
*/
    CFileInputStream() : m_pIab(NULL), m_mark(0), m_pos(0) {}

private:
/*!
	@brief the underlying input stream.
*/
    FileStream * m_pIab;
/*!
	@brief The value of the pos field at the time the last Mark method was called. 
*/
    Int32 m_mark;
/*!
	@brief The current position in the buffer. This is the index of the next character to be read from 
	the buf array. 
*/
    Int32 m_pos;
};

#endif // __CFILEINPUTSTREAM_H__
