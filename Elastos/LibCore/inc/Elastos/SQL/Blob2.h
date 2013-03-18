#ifndef _BLOB2_H_
#define _BLOB2_H_

#include <elastos.h>
#include <Elastos.SQL_server.h>
#include "Elastos.IO.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;

class Blob2
{
private:
    /**
     * Internal handle for the SQLite3 blob.
     */
    Int64 mHandle;// = 0;

public:
    /**
     * Cached size of blob, setup right after blob
     * has been opened.
     */
    Int32 mSize;// = 0;

public:

    Blob2();

    /**
     * Return InputStream for this blob
     * @return InputStream
     */
    CARAPI GetInputStream(
        /** [out] **/IInputStream** stream);

    /**
     * Return OutputStream for this blob
     * @return OutputStream
     */
    CARAPI GetOutputStream(
        /** [out] **/IOutputStream** stream);

    /**
     * Close blob.
     */
    CARAPI Close();

    /**
     * Internal blob write method.
     * @param b byte array to be written
     * @param off offset into byte array
     * @param pos offset into blob
     * @param len length to be written
     * @return number of bytes written to blob
     */

    CARAPI Write(
        /** [in] **/Int32 off, 
        /** [in] **/Int32 pos, 
        /** [in] **/Int32 len,
        /** [in] **/const ArrayOf<Byte>& b, 
        /** [out] **/Int32* result);

    /**
     * Internal blob read method.
     * @param b byte array to be written
     * @param off offset into byte array
     * @param pos offset into blob
     * @param len length to be written
     * @return number of bytes written to blob
     */
    CARAPI Read(
        /** [in] **/Int32 off, 
        /** [in] **/Int32 pos, 
        /** [in] **/Int32 len,
        /** [out] **/ArrayOf<Byte>* b, 
        /** [out] **/Int32* result);

protected:
    /**
     * Destructor for object.
     */
    CARAPI Finalize();

private:
    /**
     * Internal native initializer.
     */
    CARAPI Internal_init();

};

class BlobR : public IInputStream
{
private:
    /**
     * Blob instance
     */
    Blob2 mBlob;

    /**
     * Read position, file pointer.
     */
    Int32 mPos;

public:
    /**
     * Contruct InputStream from blob instance.
     */
    BlobR(
        /** [in] **/Blob2 blob);

    //@Override
    CARAPI Available(
        /* [out] */ Int32* number);

    /**
     * Mark method; dummy to satisfy InputStream class.
     */

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    /**
     * Reset method; dummy to satisfy InputStream class.
     */

    CARAPI Reset();

    /**
     * Mark support; not for this class.
     * @return always false
     */
    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    /**
     * Close this blob InputStream.
     */
    CARAPI Close();

    /**
     * Skip over blob data.
     */
    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

    /**
     * Read single byte from blob.
     * @return byte read
     */
    CARAPI Read(
        /* [out] */ Int32* value);

    /**
     * Read byte array from blob.
     * @param b byte array to be filled
     * @return number of bytes read
     */
    CARAPI ReadBuffer(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    /**
     * Read slice of byte array from blob.
     * @param b byte array to be filled
     * @param off offset into byte array
     * @param len length to be read
     * @return number of bytes read
     */
    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);
};


/**
 * Internal class implementing java.io.OutputStream on
 * SQLite 3.4.0 incremental blob I/O interface.
 */

class BlobW : public IOutputStream
{
private:
    /**
     * Blob instance
     */
    Blob2 mBlob;

    /**
     * Read position, file pointer.
     */
    Int32 mPos;

public:
    /**
     * Contruct OutputStream from blob instance.
     */
    BlobW(Blob2 blob);

    /**
     * Flush blob; dummy to satisfy OutputStream class.
     */
    CARAPI Flush();

    /**
     * Close this blob OutputStream.
     */
    CARAPI Close();

    /**
     * Write blob data.
     * @param v byte to be written at current position.
     */
    CARAPI Write(
        /* [in] */ Int32 oneByte);

    /**
     * Write blob data.
     * @param b byte array to be written at current position.
     */
    CARAPI WriteBuffer(
        /* [in] */ const ArrayOf<Byte>& buffer);

    /**
     * Write blob data.
     * @param b byte array to be written.
     * @param off offset within byte array
     * @param len length of data to be written
     */
    CARAPI WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Byte>& buffer);
};

#endif// _BLOB2_H_