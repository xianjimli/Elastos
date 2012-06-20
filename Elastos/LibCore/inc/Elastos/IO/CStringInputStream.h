//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#ifndef __CSTRINGINPUTSTREAM_H__
#define __CSTRINGINPUTSTREAM_H__

#include "_CStringInputStream.h"
/*!
  @brief This class allows an application to create an input stream in which the bytes
  read are supplied by the contents of a string. 
*/
CarClass(CStringInputStream)
{
public:
/*!
	@brief Gets the number of bytes that can be read from the input stream without block.
	@param pBytes <b>[out]</b> the value of @a m_tail-m_pos,which is the number of bytes remaining
	to be read from the input buffer.
	@return
    @since
    @sa
*/
    CARAPI Available(
        /* [out] */ Int32 * pBytes);
/*!
	@brief Closing a CStringInputStream has no effect. The methods in this class can be called
	after the stream has been closed. 
	@return
    @since
    @sa
*/
    CARAPI Close();
/*!
    @brief Set the current marked position in the stream. CStringInputStream objects are marked 
	at position zero by default when constructed. They may be marked at another position within the 
	buffer by this method. 
    If no  m_mark has been set, then the value of the m_mark is the offset passed to the constructor 
	(or 0 if the offset was not supplied). 
    Note: The readLimit for this class has no meaning. 
    @param readLimit <b>[in]</b> The readLimit for this class has no meaning.
	@return
    @since
    @sa
*/
    CARAPI Mark(
        /* [in] */ Int32 readLimit);
/*!
	@brief Tests if this InputStream supports Mark/Reset. The IsMarkSupported method of CStringInputStream
	always returns false to the value of @a pSupported pointed to.
	
	Note:The CStringInputStream doesn't support Reset and Mark methods.
	@param pSupported <b>[out]</b> a pointer points to the value of true if this stream instance supports the Mark and 
	Reset methods; false otherwise.
	@return
    @since
    @sa
*/
    CARAPI IsMarkSupported(
        /* [out] */ Boolean * pSupported);
/*!
   @brief Reads a single character into a Byte variable @a pByte.
   @param pByte <b>[out]</b> The character read, or -1 if the end of the stream has been reached.
   @return
   @since
   @sa
*/
    CARAPI Read(
        /* [out] */ Byte * pByte);
/*!
	@brief Reads characters into a Byte buffer @a pBuffer.
	@param pBuffer <b>[out]</b> Destination buffer.It represents the characters read.
	@return
    @since
    @sa
*/
    CARAPI ReadBuffer(
        /* [out] */ BufferOf<Byte> * pBuffer);
/*!
	@brief Reads characters into a portion of an byte buffer @a pBuffer.
	@param offset <b>[in]</b> offset at which to start writing characters.
	@param length <b>[in]</b> Maximum number of characters to read.
	@param pBuffer <b>[out]</b> Destination buffer.It represents the characters read.
	@return
    @since
    @sa
*/
    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ BufferOf<Byte> * pBuffer);
/*!
	@brief Resets the stream to the position zero.
	@return
    @since
    @sa
*/
    CARAPI Reset();
/*!
	@brief Skips the specified number of characters in the stream.
	
	The @a length parameter may be negative, even though the skip method of the Reader 
	superclass throws an exception in this case. Negative values of @a length cause the
	stream to skip backwards. Negative return values indicate a skip backwards. It is not 
	possible to skip backwards past the beginning of the string.
	If the entire string has been read or skipped, then this method has no effect and always
	returns 0.
	@param length <b>[in]</b> The number of characters to skip.
	@return
    @since
    @sa
*/
    CARAPI Skip(
        /* [in] */ Int32 number);
/*!
	@brief Reads a character from the input stream,and returns the value as the Char8 type to the
	@a pCharacter.
	@param pCharacter <b>[out]</b> the data read.
	@return
    @since
    @sa
*/
    CARAPI ReadChar8(
        /* [out] */ Char8 * pCharacter);
/*!
	@brief Reads characters from the input stream,and returns the value as the Char8 buffer type to the
	@a pcharArray.
	@param pcharArray <b>[out]</b> Destination buffer.It represents the characters read.
	@return
    @since
    @sa
*/
    CARAPI ReadChar8Array(
        /* [out] */ BufferOf<Char8> * pcharArray);
/*!
	@brief Reads characters into a portion of an Char8 buffer @a pBuffer from the input stream,and returns
	the value as the Char8 buffer type to the @a pcharArray.
	@param offset <b>[in]</b> offset at which to start writing characters.
	@param length <b>[in]</b> Maximum number of characters to read.
	@param pcharArray <b>[out]</b> Destination buffer.It represents the characters read.
	@return
    @since
    @sa
*/
    CARAPI ReadChar8ArrayEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ BufferOf<Char8> * pcharArray);
/*!
	@brief Reads characters from the input stream,and returns the value as the StringBuf type to the
	@a pString.
	@param pString <b>[out]</b> Destination buffer.It represents the characters read.
	@return
    @since
    @sa
*/
    CARAPI ReadString(
        /* [out] */ StringBuf * pString);
/*!
	@brief Creates a string input stream to read data from the specified string.
	@param string <b>[in]</b> the underlying input buffer.
	@return
    @since
    @sa
*/
    CARAPI constructor(
        /* [in] */ String string);
/*!
	@brief Creates a null CByteArrayInputStream that uses null @a m_pBuf as its buffer array.The initial 
	value of @a m_pos is 0 , the initial value of @a m_tail is 0 and the initial value of @a m_mark is 0.

*/
    CStringInputStream() : m_pBuf(NULL), m_pos(0), m_tail(0) {}

private:
/*!
	@brief A pointer of char8 buffer that was provided by the creator of the stream. Elements m_pBuf[0]
	through m_pBuf[m_tail-1] are the only characters that can ever be read from the stream; element m_pBuf[m_pos]
	is the next character to be read. 
 */
    Char8 * m_pBuf;
/*!
	@brief The index of the next character to read from the input stream buffer. This value should always be
	nonnegative and not larger than the value of @a m_tail. The next byte to be read from the input stream
	buffer will be @a m_pBuf[m_pos]. 
*/	
    Int32 m_pos;
/*!
	@brief The index one greater than the last valid character in the input stream buffer. This value should always
	be nonnegative and not larger than the length of @a m_pBuf. It is one greater than the position of the last byte
	within @a m_pBuf that can ever be read from the input stream buffer. 
*/	
    Int32 m_tail;
};

#endif // __CSTRINGINPUTSTREAM_H__
