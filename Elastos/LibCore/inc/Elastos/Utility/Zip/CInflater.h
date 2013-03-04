
#ifndef __CINFLATER_H__
#define __CINFLATER_H__

#include "_CInflater.h"
#include "Zip.h"

CarClass(CInflater)
{
public:
    CInflater();

    ~CInflater();

    /**
     * Release any resources associated with this {@code Inflater}. Any unused
     * input/output is discarded. This is also called by the finalize method.
     */
    // synchronized
    CARAPI End();

    /**
     * Indicates if the {@code Inflater} has inflated the entire deflated
     * stream. If deflated bytes remain and {@code needsInput()} returns {@code
     * true} this method will return {@code false}. This method should be
     * called after all deflated input is supplied to the {@code Inflater}.
     *
     * @return {@code true} if all input has been inflated, {@code false}
     *         otherwise.
     */
    // synchronized
    CARAPI Finished(
        /* [out] */ Boolean* finished);

    /**
     * Returns the <i>Adler32</i> checksum of either all bytes inflated, or the
     * checksum of the preset dictionary if one has been supplied.
     *
     * @return The <i>Adler32</i> checksum associated with this
     *         {@code Inflater}.
     */
    // synchronized
    CARAPI GetAdler(
        /* [out] */ Int32* checksum);

    /**
     * Returns the total number of bytes read by the {@code Inflater}. This
     * method performs the same as {@code getTotalIn()} except that it returns a
     * {@code long} value instead of an integer.
     *
     * @return the total number of bytes read.
     */
    // synchronized
    CARAPI GetBytesRead(
        /* [out] */ Int64* number);

    /**
     * Returns a the total number of bytes read by the {@code Inflater}. This
     * method performs the same as {@code getTotalOut} except it returns a
     * {@code long} value instead of an integer.
     *
     * @return the total bytes written to the output buffer.
     */
    // synchronized
    CARAPI GetBytesWritten(
        /* [out] */ Int64* number);

    /**
     * Returns the number of bytes of current input remaining to be read by the
     * inflater.
     *
     * @return the number of bytes of unread input.
     */
    // synchronized
    CARAPI GetRemaining(
        /* [out] */ Int32* number);

    /**
     * Returns total number of bytes of input read by the {@code Inflater}. The
     * result value is limited by {@code Integer.MAX_VALUE}.
     *
     * @return the total number of bytes read.
     */
    // synchronized
    CARAPI GetTotalIn(
        /* [out] */ Int32* number);

    /**
     * Returns total number of bytes written to the output buffer by the {@code
     * Inflater}. The result value is limited by {@code Integer.MAX_VALUE}.
     *
     * @return the total bytes of output data written.
     */
    // synchronized
    CARAPI GetTotalOut(
        /* [out] */ Int32* number);

    /**
     * Inflates bytes from current input and stores them in {@code buf}.
     *
     * @param buf
     *            the buffer where decompressed data bytes are written.
     * @return the number of bytes inflated.
     * @throws DataFormatException
     *             if the underlying stream is corrupted or was not compressed
     *             using a {@code Deflater}.
     */
    CARAPI Inflate(
        /* [out] */ ArrayOf<Byte>* buf,
        /* [out] */ Int32* number);

    /**
     * Inflates up to n bytes from the current input and stores them in {@code
     * buf} starting at {@code off}.
     *
     * @param buf
     *            the buffer to write inflated bytes to.
     * @param off
     *            the offset in buffer where to start writing decompressed data.
     * @param nbytes
     *            the number of inflated bytes to write to {@code buf}.
     * @throws DataFormatException
     *             if the underlying stream is corrupted or was not compressed
     *             using a {@code Deflater}.
     * @return the number of bytes inflated.
     */
    // synchronized
    CARAPI InflateEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 nbytes,
        /* [out] */ ArrayOf<Byte>* buf,
        /* [out] */ Int32* number);

    /**
     * Indicates whether the input bytes were compressed with a preset
     * dictionary. This method should be called prior to {@code inflate()} to
     * determine whether a dictionary is required. If so {@code setDictionary()}
     * should be called with the appropriate dictionary prior to calling {@code
     * inflate()}.
     *
     * @return {@code true} if a preset dictionary is required for inflation.
     * @see #setDictionary(byte[])
     * @see #setDictionary(byte[], int, int)
     */
    // synchronized
    CARAPI NeedsDictionary(
        /* [out] */ Boolean* result);

    /**
     * Indicates that input has to be passed to the inflater.
     *
     * @return {@code true} if {@code setInput} has to be called before
     *         inflation can proceed.
     * @see #setInput(byte[])
     */
    // synchronized
    CARAPI NeedsInput(
        /* [out] */ Boolean* result);

    /**
     * Resets the {@code Inflater}. Should be called prior to inflating a new
     * set of data.
     */
    // synchronized
    CARAPI Reset();

    /**
     * Sets the preset dictionary to be used for inflation to {@code buf}.
     * {@code needsDictionary()} can be called to determine whether the current
     * input was deflated using a preset dictionary.
     *
     * @param buf
     *            The buffer containing the dictionary bytes.
     * @see #needsDictionary
     */
    CARAPI SetDictionary(
        /* [in] */ const ArrayOf<Byte>& buf);

    /**
     * Like {@code setDictionary(byte[])}, allowing to define a specific region
     * inside {@code buf} to be used as a dictionary.
     * <p>
     * The dictionary should be set if the {@link #inflate(byte[])} returned
     * zero bytes inflated and {@link #needsDictionary()} returns
     * <code>true</code>.
     *
     * @param buf
     *            the buffer containing the dictionary data bytes.
     * @param off
     *            the offset of the data.
     * @param nbytes
     *            the length of the data.
     * @see #needsDictionary
     */
    // synchronized
    CARAPI SetDictionaryEx(
        /* [in] */ const ArrayOf<Byte>& buf,
        /* [in] */ Int32 off,
        /* [in] */ Int32 nbytes);

    /**
     * Sets the current input to to be decrompressed. This method should only be
     * called if {@code needsInput()} returns {@code true}.
     *
     * @param buf
     *            the input buffer.
     * @see #needsInput
     */
    CARAPI SetInput(
        /* [in] */ const ArrayOf<Byte>& buf);

    /**
     * Sets the current input to the region of the input buffer starting at
     * {@code off} and ending at {@code nbytes - 1} where data is written after
     * decompression. This method should only be called if {@code needsInput()}
     * returns {@code true}.
     *
     * @param buf
     *            the input buffer.
     * @param off
     *            the offset to read from the input buffer.
     * @param nbytes
     *            the number of bytes to read.
     * @see #needsInput
     */
    // synchronized
    CARAPI SetInputEx(
        /* [in] */ const ArrayOf<Byte>& buf,
        /* [in] */ Int32 off,
        /* [in] */ Int32 nbytes);

    /**
     * This constructor creates an inflater that expects a header from the input
     * stream. Use {@code Inflater(boolean)} if the input comes without a ZLIB
     * header.
     */
    CARAPI constructor();

    /**
     * This constructor allows to create an inflater that expects no header from
     * the input stream.
     *
     * @param noHeader
     *            {@code true} indicates that no ZLIB header comes with the
     *            input.
     */
    CARAPI constructor(
        /* [in] */ Boolean noHeader);

protected:
    // android-only
    /**
     * Sets the current input to the region within a file starting at {@code
     * off} and ending at {@code nbytes - 1}. This method should only be called
     * if {@code needsInput()} returns {@code true}.
     *
     * @param fd
     *            the input file.
     * @param off
     *            the offset to read from in buffer.
     * @param nbytes
     *            the number of bytes to read.
     * @see #needsInput
     */
    // synchronized
    /* package */ CARAPI_(Int32) SetFileInput(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int64 off,
        /* [in] */ Int32 nbytes);

private:
    CARAPI CreateStream(
        /* [in] */ Boolean noHeader);

    CARAPI_(void) EndImplLocked(
        /* [in] */ NativeZipStream* stream);

    CARAPI_(Int32) GetAdlerImplLocked(
        /* [in] */ NativeZipStream* stream);

    CARAPI_(Int64) GetTotalInImplLocked(
        /* [in] */ NativeZipStream* stream);

    CARAPI_(Int64) GetTotalOutImplLocked(
        /* [in] */ NativeZipStream* stream);

    CARAPI_(Int64) InflateImplLocked(
        /* [in] */ Int32 off,
        /* [in] */ Int32 nbytes,
        /* [out] */ ArrayOf<Byte>* buf,
        /* [in] */ NativeZipStream* stream);

    CARAPI ResetImplLocked(
        /* [in] */ NativeZipStream* stream);

    CARAPI_(void) SetDictionaryImplLocked(
        /* [in] */ const ArrayOf<Byte>& buf,
        /* [in] */ Int32 off,
        /* [in] */ Int32 nbytes,
        /* [in] */ NativeZipStream* stream);

    CARAPI_(void) SetInputImplLocked(
        /* [in] */ const ArrayOf<Byte>& buf,
        /* [in] */ Int32 off,
        /* [in] */ Int32 nbytes,
        /* [in] */ NativeZipStream* stream);

    // BEGIN android-only
    CARAPI_(Int32) SetFileInputImplLocked(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int64 off,
        /* [in] */ Int32 nbytes,
        /* [in] */ NativeZipStream* stream);

    CARAPI NeedsInputImplLocked(
        /* [out] */ Boolean* result);

private:
    Boolean mFinished; // Set by the inflateImpl native

    /* package */ Int32 mInLength;

    /* package */ Int32 mInRead;

    Boolean mNeedsDictionary; // Set by the inflateImpl native

    NativeZipStream* mStreamHandle;
};

#endif //__CINFLATER_H__
