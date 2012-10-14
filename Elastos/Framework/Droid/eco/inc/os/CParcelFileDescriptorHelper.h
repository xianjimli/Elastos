
#ifndef __CPARCELFILEDESCRIPTORHELPER_H__
#define __CPARCELFILEDESCRIPTORHELPER_H__

#include "_CParcelFileDescriptorHelper.h"

CarClass(CParcelFileDescriptorHelper)
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
    CARAPI Open(
        /* [in] */ IFile* file,
        /* [in] */ Int32 mode,
        /* [out] */ IParcelFileDescriptor** descriptor);
};

#endif //__CPARCELFILEDESCRIPTORHELPER_H__
