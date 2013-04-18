
#ifndef __METADATA_H__
#define __METADATA_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Set.h>
#include <elastos/HashMap.h>

/**
   Class to hold the media's metadata.  Metadata are used
   for human consumption and can be embedded in the media (e.g
   shoutcast) or available from an external source. The source can be
   local (e.g thumbnail stored in the DB) or remote (e.g caption
   server).

   Metadata is like a Bundle. It is sparse and each key can occur at
   most once. The key is an integer and the value is the actual metadata.

   The caller is expected to know the type of the metadata and call
   the right get* method to fetch its value.

   // FIXME: unhide.
   {@hide}
 */
class Metadata
{
public:
    Metadata();

private:
    /**
     * Go over all the records, collecting metadata keys and records'
     * type field offset in the Parcel. These are stored in
     * mKeyToPosMap for latter retrieval.
     * Format of a metadata record:
     <pre>
                         1                   2                   3
      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                     record size                               |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                     metadata key                              |  // TITLE
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                     metadata type                             |  // STRING_VAL
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                               |
      |                .... metadata payload ....                     |
      |                                                               |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     </pre>
     * @param parcel With the serialized records.
     * @param bytesLeft How many bytes in the parcel should be processed.
     * @return false if an error occurred during parsing.
     */
    CARAPI_(Boolean) ScanAllRecords(
        /* [in] */ IParcel* parcel, 
        /* [in] */ Int32 bytesLeft);

public:
    /**
     * Check a parcel containing metadata is well formed. The header
     * is checked as well as the individual records format. However, the
     * data inside the record is not checked because we do lazy access
     * (we check/unmarshall only data the user asks for.)
     *
     * Format of a metadata parcel:
     <pre>
                         1                   2                   3
      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                     metadata total size                       |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |     'M'       |     'E'       |     'T'       |     'A'       |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                               |
      |                .... metadata records ....                     |
      |                                                               |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     </pre>
     *
     * @param parcel With the serialized data. Metadata keeps a
     *               reference on it to access it later on. The caller
     *               should not modify the parcel after this call (and
     *               not call recycle on it.)
     * @return false if an error occurred.
     */
    virtual CARAPI_(Boolean) Parse(
        /* [in] */ IParcel* parcel);

    /**
     * @return The set of metadata ID found.
     */
    virtual CARAPI_(Set<Int32>*) KeySet();

    /**
     * @return true if a value is present for the given key.
     */
    virtual CARAPI_(Boolean) Has(
        /* [in] */ Int32 metadataId);

    // Accessors.
    // Caller must make sure the key is present using the {@code has}
    // method otherwise a RuntimeException will occur.

    virtual CARAPI_(String) GetString(
        /* [in] */ Int32 key);

    virtual CARAPI_(Int32) GetInt32(
        /* [in] */ Int32 key);

    virtual CARAPI_(Boolean) GetBoolean(
        /* [in] */ Int32 key);

    virtual CARAPI_(Int64) GetInt64(
        /* [in] */ Int32 key);

    virtual CARAPI_(Double) GetDouble(
        /* [in] */ Int32 key);

    virtual CARAPI_(ArrayOf<Byte>*) GetByteArray(
        /* [in] */ Int32 key);

    //Date getDate(final Int32 key);

    /**
     * Helper class to hold a triple (time, duration, text). Can be used to
     * implement caption.
     */
    class TimedText 
    {
    private:
        //Date mTime;
        Int32 mDuration;  // millisec
        String mText;

    public:
        //TimedText(
        //    /* [in] */ Date time, 
        //    /* [in] */ Int32 duration, 
        //    /* [in] */ String text);

        String ToString();
    };

    //virtual CARAPI_(TimedText*) GetTimedText(
    //    /* [in] */ Int32 key);

    // @return the last available system metadata id. Ids are
    // 1-indexed.
    static CARAPI_(Int32) LastSytemId();

    // @return the first available cutom metadata id.
    static CARAPI_(Int32) FirstCustomId();

    // @return the last value of known type. Types are 1-indexed.
    static CARAPI_(Int32) LastType();

private:
    // Check val is either a system id or a custom one.
    // @param val Metadata key to test.
    // @return true if it is in a valid range.
    CARAPI_(Boolean) CheckMetadataId(
        /* [in] */ Int32 val);

    // Check the type of the data match what is expected.
    CARAPI_(void) CheckType(
        /* [in] */ Int32 key, 
        /* [in] */ Int32 expectedType);

public:
    // The metadata are keyed using integers rather than more heavy
    // weight strings. We considered using Bundle to ship the metadata
    // between the native layer and the java layer but dropped that
    // option since keeping in sync a native implementation of Bundle
    // and the java one would be too burdensome. Besides Bundle uses
    // String for its keys.
    // The key range [0 8192) is reserved for the system.
    //
    // We manually serialize the data in Parcels. For large memory
    // blob (bitmaps, raw pictures) we use MemoryFile which allow the
    // client to make the data purge-able once it is done with it.
    //

    static const Int32 ANY = 0;  // Never used for metadata returned, only for filtering.
                                      // Keep in sync with kAny in MediaPlayerService.cpp

    // TODO: Should we use numbers compatible with the metadata retriever?
    static const Int32 TITLE = 1;           // String
    static const Int32 COMMENT = 2;         // String
    static const Int32 COPYRIGHT = 3;       // String
    static const Int32 ALBUM = 4;           // String
    static const Int32 ARTIST = 5;          // String
    static const Int32 AUTHOR = 6;          // String
    static const Int32 COMPOSER = 7;        // String
    static const Int32 GENRE = 8;           // String
    static const Int32 DATE = 9;            // Date
    static const Int32 DURATION = 10;       // Integer(millisec)
    static const Int32 CD_TRACK_NUM = 11;   // Integer 1-based
    static const Int32 CD_TRACK_MAX = 12;   // Integer
    static const Int32 RATING = 13;         // String
    static const Int32 ALBUM_ART = 14;      // byte[]
    static const Int32 VIDEO_FRAME = 15;    // Bitmap
    static const Int32 CAPTION = 16;        // TimedText

    static const Int32 BIT_RATE = 17;       // Integer, Aggregate rate of
                                          // all the streams in bps.

    static const Int32 AUDIO_BIT_RATE = 18; // Integer, bps
    static const Int32 VIDEO_BIT_RATE = 19; // Integer, bps
    static const Int32 AUDIO_SAMPLE_RATE = 20; // Integer, Hz
    static const Int32 VIDEO_FRAME_RATE = 21;  // Integer, Hz

    // See RFC2046 and RFC4281.
    static const Int32 MIME_TYPE = 22;      // String
    static const Int32 AUDIO_CODEC = 23;    // String
    static const Int32 VIDEO_CODEC = 24;    // String

    static const Int32 VIDEO_HEIGHT = 25;   // Integer
    static const Int32 VIDEO_WIDTH = 26;    // Integer
    static const Int32 NUM_TRACKS = 27;     // Integer
    static const Int32 DRM_CRIPPLED = 28;   // Boolean

    // Playback capabilities.
    static const Int32 PAUSE_AVAILABLE = 29;         // Boolean
    static const Int32 SEEK_BACKWARD_AVAILABLE = 30; // Boolean
    static const Int32 SEEK_FORWARD_AVAILABLE = 31;  // Boolean
    static const Int32 SEEK_AVAILABLE = 32;          // Boolean

    static const Int32 LAST_SYSTEM = 32;
    static const Int32 FIRST_CUSTOM = 8192;

    // Shorthands to set the MediaPlayer's metadata filter.
    static const Set<Int32> MATCH_NONE;// = Collections.EMPTY_SET;
    static const Set<Int32> MATCH_ALL;// = Collections.singleton(ANY);

    static const Int32 STRING_VAL     = 1;
    static const Int32 INTEGER_VAL    = 2;
    static const Int32 BOOLEAN_VAL    = 3;
    static const Int32 LONG_VAL       = 4;
    static const Int32 DOUBLE_VAL     = 5;
    static const Int32 TIMED_TEXT_VAL = 6;
    static const Int32 DATE_VAL       = 7;
    static const Int32 BYTE_ARRAY_VAL = 8;
    // FIXME: misses a type for shared heap is missing (MemoryFile).
    // FIXME: misses a type for bitmaps.
    static const Int32 LAST_TYPE = 8;

    static const String TAG;// = "media.Metadata";
    static const Int32 kInt32Size = 4;
    static const Int32 kMetaHeaderSize = 2 * kInt32Size; //  size + marker
    static const Int32 kRecordHeaderSize = 3 * kInt32Size; // size + id + type

    static const Int32 kMetaMarker = 0x4d455441;  // 'M' 'E' 'T' 'A'

private:
    // After a successful parsing, set the parcel with the serialized metadata.
    AutoPtr<IParcel> mParcel;

    // Map to associate a Metadata key (e.g TITLE) with the offset of
    // the record's payload in the parcel.
    // Used to look up if a key was present too.
    // Key: Metadata ID
    // Value: Offset of the metadata type field in the record.
    HashMap<Int32, Int32> mKeyToPosMap;// =
            //new HashMap<Integer, Integer>();
};

#endif
