//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#ifndef __CFILEOUTPUTSTREAM_H__
#define __CFILEOUTPUTSTREAM_H__

#include "_CFileOutputStream.h"
#include "FileStream.h"
/*!
  @brief A CFileOutputStream is an output stream for writing data to a File or to a FileDescriptor.
  Whether or not a file is available or may be created depends upon the underlying platform. Some 
  platforms, in particular, allow a file to be opened for writing by only one CFileOutputStream (or 
  other file-writing object) at a time. In such situations the constructors in this class will fail 
  if the file involved is already open. 

  FileOutputStream is meant for writing streams of raw bytes such as image data. 
  @since
  @sa
*/
CarClass(CFileOutputStream)
{
public:
/*!
    @brief Closes this file output stream and releases any system resources associated 
    with this stream. This file output stream may no longer be used for writing bytes. 
   	@return
    @since
    @sa
*/
    CARAPI Close();
/*!
	@brief Flushes this output stream and forces any buffered output bytes to be written out.
	@return
    @since
    @sa
*/
    CARAPI Flush();
/*!
	@brief Writes the specified byte to this output stream.
	@param byte <b>[in]</b> the byte to be written.
	@return
    @since
    @sa
*/
    CARAPI Write(
        /* [in] */ Byte byte);
/*!
	@brief Writes all bytes that can be written from the specified byte buffer to this 
	file output stream. returns the bytes number has been written to the @a pBytesWritten.
	@param buffer <b>[in]</b> the data.
	@param pBytesWritten <b>[out]</b> the bytes number has been written.
	@return
    @since
    @sa
*/
    CARAPI WriteBuffer(
        /* [in] */ const BufferOf<Byte> & buffer,
        /* [out] */ Int32 * pBytesWritten);
/*!
	@brief Writes @a length bytes from the specified byte array starting at offset @a offset to this 
	file output stream. returns the bytes number has been written to the @a pBytesWritten.
	@param offset <b>[in]</b> the start offset in the data.
	@param length <b>[in]</b> the number of bytes to write. 
	@param buffer <b>[in]</b> the data.
	@param pBytesWritten <b>[out]</b> the bytes number has been written.
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
	@brief the destructor of CFileOutputStream.
*/
    ~CFileOutputStream();
/*!
	@brief Creates an CFileOutputStream which is used to write data on the specified file @a fileName.
	if the second argument @a append is true,then the bytes is written on starting from the end of the 
	file;if false,it is written on starting from the beginning of the file.
	@param fileName <b>[in]</b> the file name.
	@param append <b>[in]</b> Write starting from the file's end if true;or starting from the file's beginning
	if false.
	@param pIFileOutputStream <b>[out]</b> a pointer points to the pointer of the interface of the underlying 
	output stream.
	@return
    @since
    @sa
*/
    CARAPI constructor(
        /* [in] */ String fileName,
        /* [in] */ Boolean append);
/*!
	@brief Create an CFileOutputStream with an null FileStream,initialize zero as its buffer's current 
	position.
*/
    CFileOutputStream() : m_pIab(NULL), m_pos(0) {}

private:
/*!
	@brief the underlying output stream with an output of file.
*/
    FileStream * m_pIab;
/*!
	@brief the current position in the output stream's buffer.
*/
    Int32 m_pos;
};

#endif // __CFILEOUTPUTSTREAM_H__
