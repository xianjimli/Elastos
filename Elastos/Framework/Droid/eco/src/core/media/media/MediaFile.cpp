
#include "media/MediaFile.h"
    
String MediaFile::sFileExtensions;

MediaFile::MediaFileType::MediaFileType(
    /* [in] */ Int32 fileType, 
    /* [in] */ String mimeType)
{
    this->fileType = fileType;
    this->mimeType = mimeType;
}
          
void MediaFile::AddFileType(
    /* [in] */ String extension, 
    /* [in] */ Int32 fileType, 
    /* [in] */ String mimeType)
{
    /*sFileTypeMap.put(extension, new MediaFileType(fileType, mimeType));
    sMimeTypeMap.put(mimeType, Integer.valueOf(fileType));*/
}

Boolean MediaFile::IsWMAEnabled()
{
    /*List<AudioDecoder> decoders = DecoderCapabilities.getAudioDecoders();
    for (AudioDecoder decoder: decoders) {
        if (decoder == AudioDecoder.AUDIO_DECODER_WMA) {
            return true;
        }
    }*/
    return FALSE;
}

Boolean MediaFile::IsWMVEnabled()
{
    /*List<VideoDecoder> decoders = DecoderCapabilities.getVideoDecoders();
    for (VideoDecoder decoder: decoders) {
        if (decoder == VideoDecoder.VIDEO_DECODER_WMV) {
            return true;
        }
    }*/
    return FALSE;
}

//static {
//    addFileType("MP3", FILE_TYPE_MP3, "audio/mpeg");
//    addFileType("M4A", FILE_TYPE_M4A, "audio/mp4");
//    addFileType("WAV", FILE_TYPE_WAV, "audio/x-wav");
//    addFileType("AMR", FILE_TYPE_AMR, "audio/amr");
//    addFileType("AWB", FILE_TYPE_AWB, "audio/amr-wb");
//    if (isWMAEnabled()) {
//        addFileType("WMA", FILE_TYPE_WMA, "audio/x-ms-wma");
//    }
//    addFileType("OGG", FILE_TYPE_OGG, "application/ogg");
//    addFileType("OGA", FILE_TYPE_OGG, "application/ogg");
//    addFileType("AAC", FILE_TYPE_AAC, "audio/aac");
//    addFileType("MKA", FILE_TYPE_MKA, "audio/x-matroska");
//
//    addFileType("MID", FILE_TYPE_MID, "audio/midi");
//    addFileType("MIDI", FILE_TYPE_MID, "audio/midi");
//    addFileType("XMF", FILE_TYPE_MID, "audio/midi");
//    addFileType("RTTTL", FILE_TYPE_MID, "audio/midi");
//    addFileType("SMF", FILE_TYPE_SMF, "audio/sp-midi");
//    addFileType("IMY", FILE_TYPE_IMY, "audio/imelody");
//    addFileType("RTX", FILE_TYPE_MID, "audio/midi");
//    addFileType("OTA", FILE_TYPE_MID, "audio/midi");
//    
//    addFileType("MPEG", FILE_TYPE_MP4, "video/mpeg");
//    addFileType("MP4", FILE_TYPE_MP4, "video/mp4");
//    addFileType("M4V", FILE_TYPE_M4V, "video/mp4");
//    addFileType("3GP", FILE_TYPE_3GPP, "video/3gpp");
//    addFileType("3GPP", FILE_TYPE_3GPP, "video/3gpp");
//    addFileType("3G2", FILE_TYPE_3GPP2, "video/3gpp2");
//    addFileType("3GPP2", FILE_TYPE_3GPP2, "video/3gpp2");
//    addFileType("MKV", FILE_TYPE_MKV, "video/x-matroska");
//    addFileType("WEBM", FILE_TYPE_MKV, "video/x-matroska");
//    addFileType("TS", FILE_TYPE_MP2TS, "video/mp2ts");
//
//    if (isWMVEnabled()) {
//        addFileType("WMV", FILE_TYPE_WMV, "video/x-ms-wmv");
//        addFileType("ASF", FILE_TYPE_ASF, "video/x-ms-asf");
//    }
//
//    addFileType("JPG", FILE_TYPE_JPEG, "image/jpeg");
//    addFileType("JPEG", FILE_TYPE_JPEG, "image/jpeg");
//    addFileType("GIF", FILE_TYPE_GIF, "image/gif");
//    addFileType("PNG", FILE_TYPE_PNG, "image/png");
//    addFileType("BMP", FILE_TYPE_BMP, "image/x-ms-bmp");
//    addFileType("WBMP", FILE_TYPE_WBMP, "image/vnd.wap.wbmp");
//
//    addFileType("M3U", FILE_TYPE_M3U, "audio/x-mpegurl");
//    addFileType("PLS", FILE_TYPE_PLS, "audio/x-scpls");
//    addFileType("WPL", FILE_TYPE_WPL, "application/vnd.ms-wpl");
//
//    // compute file extensions list for native Media Scanner
//    StringBuilder builder = new StringBuilder();
//    Iterator<String> iterator = sFileTypeMap.keySet().iterator();
//    
//    while (iterator.hasNext()) {
//        if (builder.length() > 0) {
//            builder.append(',');
//        }
//        builder.append(iterator.next());
//    } 
//    sFileExtensions = builder.toString();
//}

Boolean MediaFile::IsAudioFileType(
    /* [in] */ Int32 fileType)
{
    return ((fileType >= FIRST_AUDIO_FILE_TYPE &&
            fileType <= LAST_AUDIO_FILE_TYPE) ||
            (fileType >= FIRST_MIDI_FILE_TYPE &&
            fileType <= LAST_MIDI_FILE_TYPE));
}

Boolean MediaFile::IsVideoFileType(
    /* [in] */ Int32 fileType)
{
    return (fileType >= FIRST_VIDEO_FILE_TYPE &&
            fileType <= LAST_VIDEO_FILE_TYPE);
}

Boolean MediaFile::IsImageFileType(
    /* [in] */ Int32 fileType)
{
    return (fileType >= FIRST_IMAGE_FILE_TYPE &&
            fileType <= LAST_IMAGE_FILE_TYPE);
}

Boolean MediaFile::IsPlayListFileType(
    /* [in] */ Int32 fileType)
{
    return (fileType >= FIRST_PLAYLIST_FILE_TYPE &&
            fileType <= LAST_PLAYLIST_FILE_TYPE);
}

MediaFile::MediaFileType* MediaFile::GetFileType(
    /* [in] */ String path)
{
    /*Int32 lastDot = path.lastIndexOf(".");
    if (lastDot < 0)
        return null;
    return sFileTypeMap.get(path.substring(lastDot + 1).toUpperCase());*/

    return NULL;
}

Int32 MediaFile::GetFileTypeForMimeType(
    /* [in] */ String mimeType)
{
    /*Integer value = sMimeTypeMap.get(mimeType);
    return (value == null ? 0 : value.intValue());*/

    return 0;
}

