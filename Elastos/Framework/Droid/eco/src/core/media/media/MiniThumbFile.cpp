
#include "media/MiniThumbFile.h"

HashMap<String, MiniThumbFile*>* MiniThumbFile::sThumbFiles;

/**
 * We store different types of thumbnails in different files. To remain backward compatibility,
 * we should hashcode of content://media/external/images/media remains the same.
 */
void MiniThumbFile::Reset() 
{
    /*for (MiniThumbFile file : sThumbFiles.values()) {
        file.deactivate();
    }*/
    sThumbFiles->Clear();
}

MiniThumbFile* MiniThumbFile::Instance(
    /* [in] */ IUri* uri)
{
    ArrayOf<String>* types;
    //uri->GetPathSegments(types);

    String type = (*types)[1];
    MiniThumbFile* file;// = sThumbFiles->Get(type);
    // Log.v(TAG, "get minithumbfile for type: "+type);
    /*if (file == NULL) {
        file = new MiniThumbFile(
                Uri.parse("content://media/external/" + type + "/media"));
        sThumbFiles.put(type, file);
    }*/

    return file;
}

String MiniThumbFile::RandomAccessFilePath(
    /* [in] */ Int32 version)
{
    /*String directoryName =
            Environment.getExternalStorageDirectory().toString()
            + "/DCIM/.thumbnails";
    return directoryName + "/.thumbdata" + version + "-" + mUri.hashCode();*/
}

void MiniThumbFile::RemoveOldFile() 
{
    String oldPath = RandomAccessFilePath(MINI_THUMB_DATA_FILE_VERSION - 1);
    AutoPtr<IFile> oldFile;
    //CFile::New((IFile**)&oldPath);
    
    Boolean exists;
    oldFile->Exists(&exists);
    if (exists) {
        //try {
            //oldFile->Delete();
        //} catch (SecurityException ex) {
        //    // ignore
        //}
    }
}

AutoPtr<IRandomAccessFile> MiniThumbFile::MiniThumbDataFile()
{
    //if (mMiniThumbFile == NULL) {
    //    RemoveOldFile();
    //    String path = RandomAccessFilePath(MINI_THUMB_DATA_FILE_VERSION);
    //    File directory = new File(path).getParentFile();
    //    if (!directory.isDirectory()) {
    //        if (!directory.mkdirs()) {
    //            Log.e(TAG, "Unable to create .thumbnails directory "
    //                    + directory.toString());
    //        }
    //    }
    //    File f = new File(path);
    //    try {
    //        mMiniThumbFile = new RandomAccessFile(f, "rw");
    //    } catch (IOException ex) {
    //        // Open as read-only so we can at least read the existing
    //        // thumbnails.
    //        try {
    //            mMiniThumbFile = new RandomAccessFile(f, "r");
    //        } catch (IOException ex2) {
    //            // ignore exception
    //        }
    //    }
    //    if (mMiniThumbFile != NULL) {
    //        mChannel = mMiniThumbFile.getChannel();
    //    }
    //}
    return mMiniThumbFile;
}

MiniThumbFile::MiniThumbFile()
{

}

MiniThumbFile::MiniThumbFile(
    /* [in] */ IUri* uri) 
{
    mUri = uri;
    //mBuffer = ByteBuffer.allocateDirect(BYTES_PER_MINTHUMB);
}

ECode MiniThumbFile::Deactivate()
{
    if (mMiniThumbFile != NULL) {
        //try {
            mMiniThumbFile->Close();
            mMiniThumbFile = NULL;
        //} catch (IOException ex) {
        //    // ignore exception
        //}
    }

    return NOERROR;
}

// Get the magic number for the specified id in the mini-thumb file.
// Returns 0 if the magic is not available.
Int64 MiniThumbFile::GetMagic(
    /* [in] */ Int64 id) 
{
    // check the mini thumb file for the right data.  Right is
    // defined as having the right magic number at the offset
    // reserved for this "id".
    AutoPtr<IRandomAccessFile> r = MiniThumbDataFile();
    if (r != NULL) {
        Int64 pos = id * BYTES_PER_MINTHUMB;
        AutoPtr<IFileLock> lock = NULL;
        //try {
            mBuffer->Clear();
            //mBuffer->Limit(1 + 8);

            //mChannel->Lock(pos, 1 + 8, TRUE, &lock);
            // check that we can read the following 9 bytes
            // (1 for the "status" and 8 for the Int64)
            /*if (mChannel->Read(mBuffer, pos) == 9) {
                mBuffer->Position(0);
                if (mBuffer->Get() == 1) {
                    return mBuffer->GetInt64();
                }
            }*/
        //} catch (IOException ex) {
        //    Log.v(TAG, "Got exception checking file magic: ", ex);
        //} catch (RuntimeException ex) {
        //    // Other NIO related exception like disk full, read only channel..etc
        //    Log.e(TAG, "Got exception when reading magic, id = " + id +
        //            ", disk full or mount read-only? " + ex.getClass());
        //} finally {
        //    try {
                if (lock != NULL) lock->Release();
            //}
            //catch (IOException ex) {
            //    // ignore it.
            //}
        //}
    }
    return 0;
}

ECode MiniThumbFile::SaveMiniThumbToFile(
    /* [in] */ ArrayOf<Byte>* data, 
    /* [in] */ Int64 id, 
    /* [in] */ Int64 magic)
{
    AutoPtr<IRandomAccessFile> r = MiniThumbDataFile();
    if (r == NULL) return NOERROR;

    Int64 pos = id * BYTES_PER_MINTHUMB;
    AutoPtr<IFileLock> lock = NULL;
    //try {
        if (data != NULL) {
            if (data->GetLength() > BYTES_PER_MINTHUMB - HEADER_SIZE) {
                // not enough space to store it.
                return NOERROR;
            }
            mBuffer->Clear();
            //mBuffer->Put((Byte) 1);
            //mBuffer=>PutInt64(magic);
            mBuffer->PutInt32(data->GetLength());
            //mBuffer->Put(data);
            mBuffer->Flip();

            //lock = mChannel->Lock(pos, BYTES_PER_MINTHUMB, FALSE);
            //mChannel->Write(mBuffer, pos);
        }
    //} catch (IOException ex) {
    //    Log.e(TAG, "couldn't save mini thumbnail data for "
    //            + id + "; ", ex);
    //    throw ex;
    //} catch (RuntimeException ex) {
    //    // Other NIO related exception like disk full, read only channel..etc
    //    Log.e(TAG, "couldn't save mini thumbnail data for "
    //            + id + "; disk full or mount read-only? " + ex.getClass());
    //} finally {
        //try {
            if (lock != NULL) lock->Release();
        //}
        //catch (IOException ex) {
        //    // ignore it.
        //}
    //}
}

/**
 * Gallery app can use this method to retrieve mini-thumbnail. Full size
 * images share the same IDs with their corresponding thumbnails.
 *
 * @param id the ID of the image (same of full size image).
 * @param data the buffer to store mini-thumbnail.
 */
ArrayOf<Byte>* MiniThumbFile::GetMiniThumbFromFile(
    /* [in] */ Int64 id, 
    /* [in] */ ArrayOf<Byte>* data)
{
    AutoPtr<IRandomAccessFile> r = MiniThumbDataFile();
    if (r == NULL) return NULL;

    Int64 pos = id * BYTES_PER_MINTHUMB;
    AutoPtr<IFileLock> lock = NULL;
    //try {
        mBuffer->Clear();
        //mChannel->Lock(pos, BYTES_PER_MINTHUMB, TRUE, (IFileLock**)&lock);
        Int32 size;
        //mChannel->Read(mBuffer, pos, &size);
        if (size > 1 + 8 + 4) { // flag, magic, length
            //mBuffer->Position(0);
            Byte flag;
            //mBuffer->Get(&flag);
            Int64 magic;
            mBuffer->GetInt64(&magic);
            Int32 length;
            mBuffer->GetInt32(&length);

            if (size >= 1 + 8 + 4 + length && data->GetLength() >= length) {
                //mBuffer->Get(data, 0, length);
                return data;
            }
        }
    //} catch (IOException ex) {
    //    Log.w(TAG, "got exception when reading thumbnail id=" + id + ", exception: " + ex);
    //} catch (RuntimeException ex) {
    //    // Other NIO related exception like disk full, read only channel..etc
    //    Log.e(TAG, "Got exception when reading thumbnail, id = " + id +
    //            ", disk full or mount read-only? " + ex.getClass());
    //} finally {
    //    try {
            if (lock != NULL) lock->Release();
    //    }
    //    catch (IOException ex) {
    //        // ignore it.
    //    }
    //}
    return NULL;
}

