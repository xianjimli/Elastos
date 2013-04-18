
#ifndef __MINITHUMBFILE_H__
#define __MINITHUMBFILE_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <elastos/HashMap.h>

/**
 * This class handles the mini-thumb file. A mini-thumb file consists
 * of blocks, indexed by id. Each block has BYTES_PER_MINTHUMB bytes in the
 * following format:
 *
 * 1 Byte status (0 = empty, 1 = mini-thumb available)
 * 8 bytes magic (a magic number to match what's in the database)
 * 4 bytes data length (LEN)
 * LEN bytes jpeg data
 * (the remaining bytes are unused)
 *
 * @hide This file is shared between MediaStore and MediaProvider and should remained internal use
 *       only.
 */
class MiniThumbFile
{
public:

    MiniThumbFile();

    MiniThumbFile(
        /* [in] */ IUri* uri);

    virtual CARAPI Deactivate();

    // Get the magic number for the specified id in the mini-thumb file.
    // Returns 0 if the magic is not available.
    virtual CARAPI_(Int64) GetMagic(
        /* [in] */ Int64 id);

    virtual CARAPI SaveMiniThumbToFile(
        /* [in] */ ArrayOf<Byte>* data, 
        /* [in] */ Int64 id, 
        /* [in] */ Int64 magic);

    /**
     * Gallery app can use this method to retrieve mini-thumbnail. Full size
     * images share the same IDs with their corresponding thumbnails.
     *
     * @param id the ID of the image (same of full size image).
     * @param data the buffer to store mini-thumbnail.
     */
    virtual CARAPI_(ArrayOf<Byte>*) GetMiniThumbFromFile(
        /* [in] */ Int64 id, 
        /* [in] */ ArrayOf<Byte>* data);

    /**
     * We store different types of thumbnails in different files. To remain backward compatibility,
     * we should hashcode of content://media/external/images/media remains the same.
     */
    static CARAPI_(void) Reset();

    static CARAPI_(MiniThumbFile*) Instance(
        /* [in] */ IUri* uri);

private:
    CARAPI_(String) RandomAccessFilePath(
        /* [in] */ Int32 version);

    CARAPI_(void) RemoveOldFile();

    CARAPI_(AutoPtr<IRandomAccessFile>) MiniThumbDataFile();

public:
    static const Int32 BYTES_PER_MINTHUMB = 10000;

private:
    static String TAG;// = "MiniThumbFile";
    static const Int32 MINI_THUMB_DATA_FILE_VERSION = 3;
    
    static const Int32 HEADER_SIZE = 1 + 8 + 4;
    AutoPtr<IUri> mUri;
    AutoPtr<IRandomAccessFile> mMiniThumbFile;
    AutoPtr<IFileChannel> mChannel;
    AutoPtr<IByteBuffer> mBuffer;
    static HashMap<String, MiniThumbFile*>* sThumbFiles;// = new Hashtable<String, MiniThumbFile>();
};

#endif
