
#include "ext/frameworkdef.h"
#include "graphics/CBitmapRegionDecoderHelper.h"
#include "graphics/CBitmapRegionDecoder.h"


/**
* Create a BitmapRegionDecoder from the specified byte array.
* Currently only the JPEG and PNG formats are supported.
*
* @param data byte array of compressed image data.
* @param offset offset into data for where the decoder should begin
*               parsing.
* @param length the number of bytes, beginning at offset, to parse
* @param isShareable If this is true, then the BitmapRegionDecoder may keep a
*                    shallow reference to the input. If this is false,
*                    then the BitmapRegionDecoder will explicitly make a copy of the
*                    input data, and keep that. Even if sharing is allowed,
*                    the implementation may still decide to make a deep
*                    copy of the input data. If an image is progressively encoded,
*                    allowing sharing may degrade the decoding speed.
* @param ppIBrd, or null if the image data could not be decoded.
*
*/
ECode CBitmapRegionDecoderHelper::NewInstance(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder** decoder)
{
    VALIDATE_NOT_NULL(decoder);

    return CBitmapRegionDecoder::NewInstance(data, offset, length, isShareable, decoder);
}
/**
* Create a BitmapRegionDecoder from the file descriptor.
* The position within the descriptor will not be changed when
* this returns, so the descriptor can be used again as is.
* Currently only the JPEG and PNG formats are supported.
*
* @param fd The file descriptor containing the data to decode
* @param isShareable If this is true, then the BitmapRegionDecoder may keep a
*                    shallow reference to the input. If this is false,
*                    then the BitmapRegionDecoder will explicitly make a copy of the
*                    input data, and keep that. Even if sharing is allowed,
*                    the implementation may still decide to make a deep
*                    copy of the input data. If an image is progressively encoded,
*                    allowing sharing may degrade the decoding speed.
* @param ppIBrd, or null if the image data could not be decoded.
*/
ECode CBitmapRegionDecoderHelper::NewInstanceEx(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder** decoder)
{
    VALIDATE_NOT_NULL(decoder);

    return CBitmapRegionDecoder::NewInstance(fd, isShareable, decoder);
}

/**
* Create a BitmapRegionDecoder from an input stream.
* The stream's position will be where ever it was after the encoded data
* was read.
* Currently only the JPEG and PNG formats are supported.
*
* @param is The input stream that holds the raw data to be decoded into a
*           BitmapRegionDecoder.
* @param isShareable If this is true, then the BitmapRegionDecoder may keep a
*                    shallow reference to the input. If this is false,
*                    then the BitmapRegionDecoder will explicitly make a copy of the
*                    input data, and keep that. Even if sharing is allowed,
*                    the implementation may still decide to make a deep
*                    copy of the input data. If an image is progressively encoded,
*                    allowing sharing may degrade the decoding speed.
* @param ppIBrd, or null if the image data could not be decoded.
*/
ECode CBitmapRegionDecoderHelper::NewInstanceEx2(
    /* [in] */ IInputStream* is,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder** decoder)
{
    VALIDATE_NOT_NULL(decoder);

    return CBitmapRegionDecoder::NewInstance(is, isShareable, decoder);
}


/**
* Create a BitmapRegionDecoder from a file path.
* Currently only the JPEG and PNG formats are supported.
*
* @param pathName complete path name for the file to be decoded.
* @param isShareable If this is true, then the BitmapRegionDecoder may keep a
*                    shallow reference to the input. If this is false,
*                    then the BitmapRegionDecoder will explicitly make a copy of the
*                    input data, and keep that. Even if sharing is allowed,
*                    the implementation may still decide to make a deep
*                    copy of the input data. If an image is progressively encoded,
*                    allowing sharing may degrade the decoding speed.
* @param ppIBrd, or null if the image data could not be decoded.
*/
ECode CBitmapRegionDecoderHelper::NewInstanceEx3(
    /* [in] */ const String& pathName,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder** decoder)
{
    VALIDATE_NOT_NULL(decoder);

    return CBitmapRegionDecoder::NewInstance(pathName, isShareable, decoder);
}
