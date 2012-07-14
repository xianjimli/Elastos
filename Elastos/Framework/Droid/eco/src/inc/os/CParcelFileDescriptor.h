
#ifndef __CPARCELFILEDESCRIPTOR_H__
#define __CPARCELFILEDESCRIPTOR_H__

#include "_CParcelFileDescriptor.h"

CarClass(CParcelFileDescriptor)
{
public:
    /**
     * Create a new ParcelFileDescriptor accessing a given file.
     *
     * @param file The file to be opened.
     * @param mode The desired access mode, must be one of
     * {@link #MODE_READ_ONLY}, {@link #MODE_WRITE_ONLY}, or
     * {@link #MODE_READ_WRITE}; may also be any combination of
     * {@link #MODE_CREATE}, {@link #MODE_TRUNCATE},
     * {@link #MODE_WORLD_READABLE}, and {@link #MODE_WORLD_WRITEABLE}.
     *
     * @return Returns a new ParcelFileDescriptor pointing to the given
     * file.
     *
     * @throws FileNotFoundException Throws FileNotFoundException if the given
     * file does not exist or can not be opened with the requested mode.
     */
    static CARAPI Open(
        /* [in] */ IFile* file,
        /* [in] */ Int32 mode,
        /* [out] */ IParcelFileDescriptor** descriptor);

    /**
     * Close the ParcelFileDescriptor. This implementation closes the underlying
     * OS resources allocated to represent this stream.
     *
     * @throws IOException
     *             If an error occurs attempting to close this ParcelFileDescriptor.
     */
    CARAPI Close();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ IParcelFileDescriptor* descriptor);

    CARAPI constructor(
        /* [in] */ IFileDescriptor* descriptor);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPARCELFILEDESCRIPTOR_H__
