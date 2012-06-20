//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#ifndef __CDATAOUTPUTSTREAM_H__
#define __CDATAOUTPUTSTREAM_H__
#include "_CDataOutputStream.h"
/*!
  @brief Defines the methods used to write primitive data types to output streams for marshalling
  custom value types. This interface is used by user written custom marshalling code for custom 
  value types. 
*/
CarClass(CDataOutputStream)
{
public:
/*!
	@brief Closes this output stream and releases any system resources associated with the stream.
	@return
    @since
    @sa
*/
    CARAPI Close();

/*!
	@brief Flushes this data output stream. This forces any data to be written out to the underlying
	output stream. 
	@return
    @since
    @sa
*/
    CARAPI Flush();

/*!
	@brief Writes the specified byte to this data output stream. 
	@param byte <b>[in]</b> the byte to be written. 
	@return
    @since
    @sa
*/
    CARAPI Write(
        /* [in] */ Byte byte);

/*!
	@brief Writes all bytes from the specified byte buffer to this data output stream.
	@param buffer <b>[in]</b> a reference to the buffer input.
	@param pBytesWritten  <b>[out]</b> a pointer pointed to the number of how many bytes has been written
	on the data output stream.
	@return
    @since
    @sa
*/
    CARAPI WriteBuffer(
        /* [in] */ const BufferOf<Byte> & buffer,
        /* [out] */ Int32 * pBytesWritten);

/*!
	@brief Writes @a length bytes from the specified byte buffer starting at @a offset to this output stream.
	The general contract for @a WriteBufferEx(offset,length,buffer,pBytesWritten) is that some of the bytes in 
	the buffer @a buffer are written to the output stream in order;element @a buffer[offset] is the first byte
	written and @a buffer[offset+length-1] is the last byte written by this operation.
	@param offset <b>[in]</b> the start offset in the data.
	@param length <b>[in]</b> the number of bytes to write.
	@param buffer <b>[in]</b> the data.
	@param pBytesWritten <b>[out]</b> the number of the bytes written to this output stream.
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
	@brief Writes the @a Int16 value to the output stream.
	@param value <b>[in]</b> The value to be written.
	@return
    @since
    @sa
*/
    CARAPI WriteInt16(
        /* [in] */ Int16 value);

/*!
	@brief Writes the @a Int32 value to the output stream.
	@param value <b>[in]</b> The value to be written.
	@return
    @since
    @sa
*/
    CARAPI WriteInt32(
        /* [in] */ Int32 value);

/*!
	@brief Writes the @a Int64 value to the output stream.
	@param value <b>[in]</b> The value to be written.
	@return
    @since
    @sa
*/
    CARAPI WriteInt64(
        /* [in] */ Int64 value);

/*!
	@brief Writes the @a Float value to the output stream.
	@param value <b>[in]</b> The value to be written.
	@return
    @since
    @sa
*/
    CARAPI WriteFloat(
        /* [in] */ Float value);

/*!
	@brief Writes the @a Double value to the output stream.
	@param value <b>[in]</b> The value to be written.
	@return
    @since
    @sa
*/
    CARAPI WriteDouble(
        /* [in] */ Double value);

/*!
	@brief Writes the @a Char8 value to the output stream.
	@param value <b>[in]</b> The value to be written.
	@return
    @since
    @sa
*/
    CARAPI WriteChar8(
        /* [in] */ Char8 value);

/*!
	@brief Writes the @a Char16 value to the output stream.
	@param value <b>[in]</b> The value to be written.
	@return
    @since
    @sa
*/
    CARAPI WriteChar16(
        /* [in] */ Char16 value);

/*!
	@brief Writes the @a Boolean value to the output stream.
	@param value <b>[in]</b> The value to be written.
	@return
    @since
    @sa
*/
    CARAPI WriteBoolean(
        /* [in] */ Boolean value);

/*!
	@brief Writes the @a Emuid value to the output stream.
	@param value <b>[in]</b> The value to be written.
	@return
    @since
    @sa
*/
    CARAPI WriteEMuid(
        /* [in] */ const EMuid& value);

/*!
	@brief Writes the @a EGuid value to the output stream.
	@param value <b>[in]</b> The value to be written.
	@return
    @since
    @sa
*/
    CARAPI WriteEGuid(
        /* [in] */ const EGuid& value);

/*!
	@brief Writes the @a ECode value to the output stream.
	@param value <b>[in]</b> The value to be written.
*/
    CARAPI WriteECode(
        /* [in] */ ECode value);

/*!
	@brief Creates a new data output stream to write data to the specified underlying output stream. 
	@param pStream <b>[in]</b> pointer of the underlying output stream.
	@param PPOutputStream <b>[out]</b> pointer pointes to the pointer of the creating data output stream's 
	interface.
	@return
    @since
    @sa
*/
    CARAPI constructor(
        /* [in] */ IOutputStream * pStream);

/*!
	@brief Destructor of CDataOutputStream.
*/
    ~CDataOutputStream();

/*!
	@brief Creates a null CDataInputStream that uses null @a m_pIis as its underlying input stream.
*/
    CDataOutputStream() : m_pIos(NULL) {}

private:
/*!
	@brief the underlying output stream.
*/
    IOutputStream * m_pIos;
};

#endif // __CDATAOUTPUTSTREAM_H__
