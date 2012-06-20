//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#ifndef __CDATAINPUTSTREAM_H__
#define __CDATAINPUTSTREAM_H__
#include "_CDataInputStream.h"
/*!
  @brief A data input stream lets an application read primitive Elastos data types from an 
  underlying input stream in a machine-independent way. An application uses a data output 
  stream to write data that can later be read by a data input stream. 
*/
CarClass(CDataInputStream)
{
public:
/*!
	@brief This method is used to get an estimate number that can be read (or skipped over)
	from this data input stream.the number is stored in the argument @a pByte.
	@param pBytes[out] This argument is used to store the sum of the number of bytes remaining
	to be read in the buffer. 
	@return
    @since
    @sa
*/
    CARAPI Available(
        /* [out] */ Int32 * pBytes);
/*!
	@brief Closes this input stream and releases any system resources associated with the stream.
	Once the stream has been closed, further Read(),Mark(), Available(),IsMarkSupported(),ReadBuffer(),
	Reset(),ReadBufferEx() or Skip() invocations will throw an IOException. Closing a previously 
	closed stream has no effect. 
	@return
    @since
    @sa
*/
    CARAPI Close();
/*!
	@brief Marks the current position in this input stream. A subsequent call to the reset method 
	repositions this stream at the last marked position so that subsequent reads re-read the same 
	bytes. 
	
	The @a readlimit arguments tells this input stream to allow that many bytes to be read before 
	the mark position gets invalidated. 
	Marking a closed stream should not have any effect on the stream.
	The general contract of mark is that, if the method IsMarkSupported returns true, the stream 
	somehow remembers all the bytes read after the call to mark and stands ready to supply those 
	same bytes again if and whenever the method reset is called. However, the stream is not required
	to remember any data at all if more than readlimit bytes are read from the stream before reset 
	is called. 
	@param readLimit <b>[in]</b> the maximum limit of bytes that can be read before the mark position 
	becomes invalid.
	@return
    @since
    @sa
*/
    CARAPI Mark(
        /* [in] */ Int32 readLimit);
/*!
	@brief Set this input stream support the mark and reset methods. Whether or not mark and reset 
	are supported is an invariant property of a particular input stream instance. 
	@param pSupported <b>[out]</b> this argument will be true if this input stream supported the mark and 
	reset methods.false otherwise.
	@return
    @since
    @sa
*/
    CARAPI IsMarkSupported(
        /* [out] */ Boolean * pSupported);
/*!
	@brief Reads only one byte for one time from the input stream and store it into the place where the
	Byte pointer points to.The number of bytes actually read is returned as an integer. This method blocks
	until input data is unavailable, end of file is detected, or an exception is thrown. 
	@param pValue <b>[out]</b> a pointer pointed to the value store the data.
	@return
    @since
    @sa
*/
    CARAPI Read(
        /* [out] */ Byte * pValue);
/*!
  @brief Reads some number of bytes from the input stream and stores them into the buffer array @a pBuffer.
  The number of bytes actually read is returned as an integer. This method blocks until input data is
  unavailable, end of file is detected, or an exception is thrown. 
  @param pBuffer <b>[out]</b> a pointer pointed to the buffer into which the data is read.
  @return
  @since
  @sa
*/
    CARAPI ReadBuffer(
        /* [out] */ BufferOf<Byte> * pBuffer);
/*!
  @brief Reads up to @a length bytes of data from the input stream into the buffer array @a pBuffer.
  An attempt is made to read as many as @a length bytes, but a smaller number may be read. The number
  of bytes actually read is returned as an integer. 

  This method blocks until input data is unavailable, end of file is detected, or an exception is thrown. 
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
   @brief Repositions this stream to the position at the time the mark method was last called on this 
   input stream. 
   @return
   @since
   @sa
*/
    CARAPI Reset();
/*!
	@brief Skips over and discards @a length bytes of data from this input stream. The skip method may,
	for a variety of reasons, end up skipping over some smaller number of bytes, possibly 0. This may 
	result from any of a number of conditions; reaching end of file before @a length bytes have been 
	skipped is only one possibility. The actual number of bytes skipped is returned. If @a length is 
	negative, no bytes are skipped. 
	@param length <b>[in]</b> the number of bytes to be skipped.
	@return
    @since
    @sa
*/
    CARAPI Skip(
        /* [in] */ Int32 length);
/*!
	@brief Reads some data from this input stream out into an @a Int16 type variable @a pValue.
	@param pValue <b>[out]</b> a pointer pointed to the value store the data. 
	@return
    @since
    @sa
*/
    CARAPI ReadInt16(
        /* [out] */ Int16 * pValue);
/*!
	@brief Reads some data from this input stream out into an @a Int32 type variable @a pValue.
	@param pValue <b>[out]</b> a pointer pointed to the value store the data. 
	@return
    @since
    @sa
*/
    CARAPI ReadInt32(
        /* [out] */ Int32 * pValue);
/*!
	@brief Reads some data from this input stream out into an @a Int64 type variable @a pValue.
	@param pValue <b>[out]</b> a pointer pointed to the value store the data. 
	@return
    @since
    @sa
*/
    CARAPI ReadInt64(
        /* [out] */ Int64 * pValue);
/*!
	@brief Reads some data from this input stream out into an @a Float type variable @a pValue.
	@param pValue <b>[out]</b> a pointer pointed to the value store the data. 
	@return
    @since
    @sa
*/
    CARAPI ReadFloat(
        /* [out] */ Float * pValue);
/*!
	@brief Reads some data from this input stream out into an @a Double type variable @a pValue.
	@param pValue <b>[out]</b>  a pointer pointed to the value store the data. 
	@return
    @since
    @sa
*/
    CARAPI ReadDouble(
        /* [out] */ Double * pValue);
/*!
	@brief Reads some data from this input stream out into an @a Char8 type variable @a pValue.
	@param pValue <b>[out]</b> a pointer pointed to the value store the data. 
	@return
    @since
    @sa
*/
    CARAPI ReadChar8(
        /* [out] */ Char8 * pValue);
/*!
	@brief Reads some data from this input stream out into an @a Char16 type variable @a pValue.
	@param pValue <b>[out]</b> a pointer pointed to the value store the data. 
	@return
    @since
    @sa
*/
    CARAPI ReadChar16(
        /* [out] */ Char16 * pValue);
/*!
	@brief Reads some data from this input stream out into an @a Boolean type variable @a pValue.
	@param pValue <b>[out]</b>  a pointer pointed to the value store the data. 
	@return
    @since
    @sa
*/
    CARAPI ReadBoolean(
        /* [out] */ Boolean * pValue);
/*!
	@brief Reads some data from this input stream out into an @a EMuid type variable @a pValue.
	@param pValue <b>[out]</b>  a pointer pointed to the value store the data. 
	@return
    @since
    @sa
*/
    CARAPI ReadEMuid(
        /* [out] */ EMuid * pValue);
/*!
	@brief Reads some data from this input stream out into an @a EGuid type variable @a pValue.
	@param pValue <b>[out]</b>  a pointer pointed to the value store the data. 
	@return
    @since
    @sa
*/
    CARAPI ReadEGuid(
        /* [out] */ EGuid * pValue);
/*!
	@brief Reads some data from this input stream out into an @a ECode type variable @a pValue.
	@param pValue <b>[out]</b>  a pointer pointed to the value store the data. 
	@return
    @since
    @sa
*/
    CARAPI ReadECode(
        /* [out] */ ECode * pValue);
/*!
	@brief Creates a CDataInputStream and saves its argument, the input stream pStream, for later use.
	@param pStream <b>[in]</b>  the underlying input stream.
	@param pIInputStream <b>[out]</b>  a pointer points to the pointer of the interface of the underlying input 
	stream.
	@return
    @since
    @sa
*/
    CARAPI constructor(
        /* [in] */ IInputStream * pStream);
/*!
	@brief Destructor of CDataInputStream.
*/
    ~CDataInputStream();
/*!
	@brief Creates a null CDataInputStream that uses null @a m_pIis as its underlying input stream.
*/
    CDataInputStream() : m_pIis(NULL) {}

private:
/*!
	@brief the underlying input stream.
*/
    IInputStream * m_pIis;
};

#endif // __CDATAINPUTSTREAM_H__
