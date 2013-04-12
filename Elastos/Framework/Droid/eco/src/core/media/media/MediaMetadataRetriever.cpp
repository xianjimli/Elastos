
#include "media/MediaMetadataRetriever.h"


MediaMetadataRetriever::MediaMetadataRetriever()
{
    native_setup();
}

/**
 * Sets the data source (file pathname) to use. Call this
 * method before the rest of the methods in this class. This method may be
 * time-consuming.
 * 
 * @param path The path of the input media file.
 * @throws IllegalArgumentException If the path is invalid.
 */
void MediaMetadataRetriever::SetDataSource(
    /* [in] */ String path)
{

}

/**
 * Sets the data source (FileDescriptor) to use.  It is the caller's
 * responsibility to close the file descriptor. It is safe to do so as soon
 * as this call returns. Call this method before the rest of the methods in
 * this class. This method may be time-consuming.
 * 
 * @param fd the FileDescriptor for the file you want to play
 * @param offset the offset into the file where the data to be played starts,
 * in bytes. It must be non-negative
 * @param length the length in bytes of the data to be played. It must be
 * non-negative.
 * @throws IllegalArgumentException if the arguments are invalid
 */
void MediaMetadataRetriever::SetDataSource(
    /* [in] */ IFileDescriptor* fd, 
    /* [in] */ Int64 offset, 
    /* [in] */ Int64 length)
{

}

/**
 * Sets the data source (FileDescriptor) to use. It is the caller's
 * responsibility to close the file descriptor. It is safe to do so as soon
 * as this call returns. Call this method before the rest of the methods in
 * this class. This method may be time-consuming.
 * 
 * @param fd the FileDescriptor for the file you want to play
 * @throws IllegalArgumentException if the FileDescriptor is invalid
 */
ECode MediaMetadataRetriever::SetDataSource(
    /* [in] */ IFileDescriptor* fd)
{
    // intentionally less than LONG_MAX
    SetDataSource(fd, 0, 0x7ffffffffffffffL);
}

/**
 * Sets the data source as a content Uri. Call this method before 
 * the rest of the methods in this class. This method may be time-consuming.
 * 
 * @param context the Context to use when resolving the Uri
 * @param uri the Content URI of the data you want to play
 * @throws IllegalArgumentException if the Uri is invalid
 * @throws SecurityException if the Uri cannot be used due to lack of
 * permission.
 */
ECode MediaMetadataRetriever::SetDataSource(
    /* [in] */ IContext* context, 
    /* [in] */ IUri* uri)
{
    if (uri == NULL) {
        //throw new IllegalArgumentException();
    }
    
    //String scheme;
    //uri->GetScheme(&scheme);
    //if(scheme == NULL || scheme.equals("file")) {
    //    setDataSource(uri.getPath());
    //    return;
    //}

    //AssetFileDescriptor fd = NULL;
    //try {
    //    ContentResolver resolver = context.getContentResolver();
    //    try {
    //        fd = resolver.openAssetFileDescriptor(uri, "r");
    //    } catch(FileNotFoundException e) {
    //        throw new IllegalArgumentException();
    //    }
    //    if (fd == NULL) {
    //        throw new IllegalArgumentException();
    //    }
    //    FileDescriptor descriptor = fd.getFileDescriptor();
    //    if (!descriptor.valid()) {
    //        throw new IllegalArgumentException();
    //    }
    //    // Note: using getDeclaredLength so that our behavior is the same
    //    // as previous versions when the content provider is returning
    //    // a full file.
    //    if (fd.getDeclaredLength() < 0) {
    //        setDataSource(descriptor);
    //    } else {
    //        setDataSource(descriptor, fd.getStartOffset(), fd.getDeclaredLength());
    //    }
    //    return;
    //} catch (SecurityException ex) {
    //} finally {
    //    try {
    //        if (fd != NULL) {
    //            fd.close();
    //        }
    //    } catch(IOException ioEx) {
    //    }
    //}
    //setDataSource(uri.toString());

    return NOERROR;
}

/**
 * Call this method after setDataSource(). This method retrieves the 
 * meta data value associated with the keyCode.
 * 
 * The keyCode currently supported is listed below as METADATA_XXX
 * constants. With any other value, it returns a NULL pointer.
 * 
 * @param keyCode One of the constants listed below at the end of the class.
 * @return The meta data value associate with the given keyCode on success; 
 * NULL on failure.
 */
String MediaMetadataRetriever::ExtractMetadata(
    /* [in] */ Int32 keyCode)
{
    return String("");
}

/**
 * Call this method after setDataSource(). This method finds a
 * representative frame close to the given time position by considering
 * the given option if possible, and returns it as a bitmap. This is
 * useful for generating a thumbnail for an input data source or just
 * obtain and display a frame at the given time position.
 *
 * @param timeUs The time position where the frame will be retrieved.
 * When retrieving the frame at the given time position, there is no
 * guarantee that the data source has a frame located at the position.
 * When this happens, a frame nearby will be returned. If timeUs is
 * negative, time position and option will ignored, and any frame
 * that the implementation considers as representative may be returned.
 *
 * @param option a hint on how the frame is found. Use
 * {@link #OPTION_PREVIOUS_SYNC} if one wants to retrieve a sync frame
 * that has a timestamp earlier than or the same as timeUs. Use
 * {@link #OPTION_NEXT_SYNC} if one wants to retrieve a sync frame
 * that has a timestamp later than or the same as timeUs. Use
 * {@link #OPTION_CLOSEST_SYNC} if one wants to retrieve a sync frame
 * that has a timestamp closest to or the same as timeUs. Use
 * {@link #OPTION_CLOSEST} if one wants to retrieve a frame that may
 * or may not be a sync frame but is closest to or the same as timeUs.
 * {@link #OPTION_CLOSEST} often has larger performance overhead compared
 * to the other options if there is no sync frame located at timeUs.
 *
 * @return A Bitmap containing a representative video frame, which 
 *         can be NULL, if such a frame cannot be retrieved.
 */
AutoPtr<IBitmap> MediaMetadataRetriever::GetFrameAtTime(
    /* [in] */ Int64 timeUs, 
    /* [in] */ Int32 option)
{
    if (option < OPTION_PREVIOUS_SYNC ||
        option > OPTION_CLOSEST) {
        //throw new IllegalArgumentException("Unsupported option: " + option);
    }

    return _getFrameAtTime(timeUs, option);
}

/**
 * Call this method after setDataSource(). This method finds a
 * representative frame close to the given time position if possible,
 * and returns it as a bitmap. This is useful for generating a thumbnail
 * for an input data source. Call this method if one does not care
 * how the frame is found as Int64 as it is close to the given time;
 * otherwise, please call {@link #getFrameAtTime(Int64, Int32)}.
 *
 * @param timeUs The time position where the frame will be retrieved.
 * When retrieving the frame at the given time position, there is no
 * guarentee that the data source has a frame located at the position.
 * When this happens, a frame nearby will be returned. If timeUs is
 * negative, time position and option will ignored, and any frame
 * that the implementation considers as representative may be returned.
 *
 * @return A Bitmap containing a representative video frame, which
 *         can be NULL, if such a frame cannot be retrieved.
 *
 * @see #getFrameAtTime(Int64, Int32)
 */
AutoPtr<IBitmap> MediaMetadataRetriever::GetFrameAtTime(
    /* [in] */ Int64 timeUs) 
{
    return GetFrameAtTime(timeUs, OPTION_CLOSEST_SYNC);
}

/**
 * Call this method after setDataSource(). This method finds a
 * representative frame at any time position if possible,
 * and returns it as a bitmap. This is useful for generating a thumbnail
 * for an input data source. Call this method if one does not
 * care about where the frame is located; otherwise, please call
 * {@link #getFrameAtTime(Int64)} or {@link #getFrameAtTime(Int64, Int32)}
 *
 * @return A Bitmap containing a representative video frame, which
 *         can be NULL, if such a frame cannot be retrieved.
 *
 * @see #getFrameAtTime(Int64)
 * @see #getFrameAtTime(Int64, Int32)
 */
AutoPtr<IBitmap> MediaMetadataRetriever::GetFrameAtTime()
{
    return GetFrameAtTime(-1, OPTION_CLOSEST_SYNC);
}

AutoPtr<IBitmap> MediaMetadataRetriever::_getFrameAtTime(
    /* [in] */ Int64 timeUs, 
    /* [in] */ Int32 option)
{
    return NULL;
}


/**
 * Call this method after setDataSource(). This method finds the optional
 * graphic or album art associated associated with the data source. If
 * there are more than one pictures, (any) one of them is returned.
 * 
 * @return NULL if no such graphic is found.
 */
ArrayOf<Byte>* MediaMetadataRetriever::GetEmbeddedPicture()
{
    return GetEmbeddedPicture(EMBEDDED_PICTURE_TYPE_ANY);
}

ArrayOf<Byte>* MediaMetadataRetriever::GetEmbeddedPicture(
    /* [in] */ Int32 pictureType)
{
    return NULL;
}

/**
 * Call it when one is done with the object. This method releases the memory
 * allocated internally.
 */
void MediaMetadataRetriever::ReleaseEx()
{

}

void MediaMetadataRetriever::native_setup()
{

}

void MediaMetadataRetriever::native_init()
{

}

void MediaMetadataRetriever::native_finalize()
{

}

void MediaMetadataRetriever::Finalize()
{
    //try {
        native_finalize();
    /*} finally {
        super.finalize();
    }*/
}

   