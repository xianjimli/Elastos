
#ifndef __MEDIAFILE_H__
#define __MEDIAFILE_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <elastos/HashMap.h>

/**
 * MediaScanner helper class.
 *
 * {@hide}
 */
class MediaFile
{
public:    
    class MediaFileType
    {
    public:
        Int32 fileType;
        String mimeType;
        
        MediaFileType(
            /* [in] */ Int32 fileType, 
            /* [in] */ String mimeType);
    };
              
    static CARAPI_(void) AddFileType(
        /* [in] */ String extension, 
        /* [in] */ Int32 fileType, 
        /* [in] */ String mimeType);

private:
    static CARAPI_(Boolean) IsWMAEnabled();

    static CARAPI_(Boolean) IsWMVEnabled();

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

    //    if (isWMVEnabled()) {
    //        addFileType("WMV", FILE_TYPE_WMV, "video/x-ms-wmv");
    //        addFileType("ASF", FILE_TYPE_ASF, "video/x-ms-asf");
    //    }

    //    addFileType("JPG", FILE_TYPE_JPEG, "image/jpeg");
    //    addFileType("JPEG", FILE_TYPE_JPEG, "image/jpeg");
    //    addFileType("GIF", FILE_TYPE_GIF, "image/gif");
    //    addFileType("PNG", FILE_TYPE_PNG, "image/png");
    //    addFileType("BMP", FILE_TYPE_BMP, "image/x-ms-bmp");
    //    addFileType("WBMP", FILE_TYPE_WBMP, "image/vnd.wap.wbmp");
 
    //    addFileType("M3U", FILE_TYPE_M3U, "audio/x-mpegurl");
    //    addFileType("PLS", FILE_TYPE_PLS, "audio/x-scpls");
    //    addFileType("WPL", FILE_TYPE_WPL, "application/vnd.ms-wpl");

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

public:

    static CARAPI_(Boolean) IsAudioFileType(
        /* [in] */ Int32 fileType);
    
    static CARAPI_(Boolean) IsVideoFileType(
        /* [in] */ Int32 fileType);
    
    static CARAPI_(Boolean) IsImageFileType(
        /* [in] */ Int32 fileType);
    
    static CARAPI_(Boolean) IsPlayListFileType(
        /* [in] */ Int32 fileType);
    
    static CARAPI_(MediaFileType*) GetFileType(
        /* [in] */ String path);
    
    static CARAPI_(Int32) GetFileTypeForMimeType(
        /* [in] */ String mimeType);

public:
    // comma separated list of all file extensions supported by the media scanner
    static String sFileExtensions;

    // Audio file types
    static const Int32 FILE_TYPE_MP3     = 1;
    static const Int32 FILE_TYPE_M4A     = 2;
    static const Int32 FILE_TYPE_WAV     = 3;
    static const Int32 FILE_TYPE_AMR     = 4;
    static const Int32 FILE_TYPE_AWB     = 5;
    static const Int32 FILE_TYPE_WMA     = 6;
    static const Int32 FILE_TYPE_OGG     = 7;
    static const Int32 FILE_TYPE_AAC     = 8;
    static const Int32 FILE_TYPE_MKA     = 9;
    static const Int32 FIRST_AUDIO_FILE_TYPE = FILE_TYPE_MP3;
    static const Int32 LAST_AUDIO_FILE_TYPE = FILE_TYPE_MKA;

    // MIDI file types
    static const Int32 FILE_TYPE_MID     = 11;
    static const Int32 FILE_TYPE_SMF     = 12;
    static const Int32 FILE_TYPE_IMY     = 13;
    static const Int32 FIRST_MIDI_FILE_TYPE = FILE_TYPE_MID;
    static const Int32 LAST_MIDI_FILE_TYPE = FILE_TYPE_IMY;

    // Video file types
    static const Int32 FILE_TYPE_MP4     = 21;
    static const Int32 FILE_TYPE_M4V     = 22;
    static const Int32 FILE_TYPE_3GPP    = 23;
    static const Int32 FILE_TYPE_3GPP2   = 24;
    static const Int32 FILE_TYPE_WMV     = 25;
    static const Int32 FILE_TYPE_ASF     = 26;
    static const Int32 FILE_TYPE_MKV     = 27;
    static const Int32 FILE_TYPE_MP2TS   = 28;
    static const Int32 FIRST_VIDEO_FILE_TYPE = FILE_TYPE_MP4;
    static const Int32 LAST_VIDEO_FILE_TYPE = FILE_TYPE_MP2TS;

    // Image file types
    static const Int32 FILE_TYPE_JPEG    = 31;
    static const Int32 FILE_TYPE_GIF     = 32;
    static const Int32 FILE_TYPE_PNG     = 33;
    static const Int32 FILE_TYPE_BMP     = 34;
    static const Int32 FILE_TYPE_WBMP    = 35;
    static const Int32 FIRST_IMAGE_FILE_TYPE = FILE_TYPE_JPEG;
    static const Int32 LAST_IMAGE_FILE_TYPE = FILE_TYPE_WBMP;

    // Playlist file types
    static const Int32 FILE_TYPE_M3U     = 41;
    static const Int32 FILE_TYPE_PLS     = 42;
    static const Int32 FILE_TYPE_WPL     = 43;
    static const Int32 FIRST_PLAYLIST_FILE_TYPE = FILE_TYPE_M3U;
    static const Int32 LAST_PLAYLIST_FILE_TYPE = FILE_TYPE_WPL;

    static HashMap<String, MediaFileType*> sFileTypeMap;

    static HashMap<String, Int32> sMimeTypeMap; 
};

#endif
