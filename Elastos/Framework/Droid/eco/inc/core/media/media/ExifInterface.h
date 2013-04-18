
#ifndef __EXIFINTERFACE_H__
#define __EXIFINTERFACE_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <elastos/HashMap.h>

/**
 * This is a class for reading and writing Exif tags in a JPEG file.
 */
class ExifInterface
{
public:
    /*static {
        System.loadLibrary("exif");
        sFormatter = new SimpleDateFormat("yyyy:MM:dd HH:mm:ss");
        sFormatter.setTimeZone(TimeZone.getTimeZone("UTC"));
    }*/


    ExifInterface();

    /**
     * Reads Exif tags from the specified JPEG file.
     */
    ExifInterface(
        /* [in] */ String filename);

    /**
     * Returns the value of the specified tag or {@code NULL} if there
     * is no such tag in the JPEG file.
     *
     * @param tag the name of the tag.
     */
    virtual CARAPI_(String) GetAttribute(
        /* [in] */ String tag);

    /**
     * Returns the integer value of the specified tag. If there is no such tag
     * in the JPEG file or the value cannot be parsed as integer, return
     * <var>defaultValue</var>.
     *
     * @param tag the name of the tag.
     * @param defaultValue the value to return if the tag is not available.
     */
    virtual CARAPI_(Int32) GetAttributeInt32(
        /* [in] */ String tag, 
        /* [in] */ Int32 defaultValue);

    /**
     * Returns the Double value of the specified rational tag. If there is no
     * such tag in the JPEG file or the value cannot be parsed as Double, return
     * <var>defaultValue</var>.
     *
     * @param tag the name of the tag.
     * @param defaultValue the value to return if the tag is not available.
     */
    virtual CARAPI_(Double) GetAttributeDouble(
        /* [in] */ String tag, 
        /* [in] */ Double defaultValue);

    /**
     * Set the value of the specified tag.
     *
     * @param tag the name of the tag.
     * @param value the value of the tag.
     */
    virtual CARAPI SetAttribute(
        /* [in] */ String tag, 
        /* [in] */ String value);

    /**
     * Save the tag data into the JPEG file. This is expensive because it involves
     * copying all the JPG data from one file to another and deleting the old file
     * and renaming the other. It's best to use {@link #setAttribute(String,String)}
     * to set all attributes to write and make a single call rather than multiple
     * calls for each attribute.
     */
    virtual CARAPI SaveAttributes();

    /**
     * Returns TRUE if the JPEG file has a thumbnail.
     */
    virtual CARAPI_(Boolean) HasThumbnail();

    /**
     * Returns the thumbnail inside the JPEG file, or {@code NULL} if there is no thumbnail.
     * The returned data is in JPEG format and can be decoded using
     * {@link android.graphics.BitmapFactory#decodeByteArray(Byte[],Int32,Int32)}
     */
    virtual CARAPI_(ArrayOf<Byte>*) GetThumbnail();

    /**
     * Stores the latitude and longitude value in a Float array. The first element is
     * the latitude, and the second element is the longitude. Returns FALSE if the
     * Exif tags are not available.
     */
    virtual CARAPI_(Boolean) GetLatLong(
        /* [in] */ ArrayOf<Float>* output);

    /**
     * Return the altitude in meters. If the exif tag does not exist, return
     * <var>defaultValue</var>.
     *
     * @param defaultValue the value to return if the tag is not available.
     */
    virtual CARAPI_(Double) GetAltitude(
        /* [in] */ Double defaultValue);

    /**
     * Returns number of milliseconds since Jan. 1, 1970, midnight.
     * Returns -1 if the date time information if not available.
     * @hide
     */
    virtual CARAPI_(Int64) GetDateTime();

    /**
     * Returns number of milliseconds since Jan. 1, 1970, midnight UTC.
     * Returns -1 if the date time information if not available.
     * @hide
     */
    virtual CARAPI_(Int64) GetGpsDateTime();

private:
    /**
     * Initialize mAttributes with the attributes from the file mFilename.
     *
     * mAttributes is a HashMap which stores the Exif attributes of the file.
     * The key is the standard tag name and the value is the tag's value: e.g.
     * Model -> Nikon. Numeric values are stored as strings.
     *
     * This function also initialize mHasThumbnail to indicate whether the
     * file has a thumbnail inside.
     */
   CARAPI_(void) LoadAttributes();

   static CARAPI_(Float) ConvertRationalLatLonToFloat(
       /* [in] */ String rationalString, 
       /* [in] */ String ref);

   CARAPI_(Boolean) AppendThumbnailNative(
       /* [in] */ String fileName,
       /* [in] */ String thumbnailFileName);

   CARAPI_(void) SaveAttributesNative(
       /* [in] */ String fileName,
       /* [in] */ String compressedAttributes);

   CARAPI_(String) GetAttributesNative(
       /* [in] */ String fileName);

   CARAPI_(void) CommitChangesNative(
       /* [in] */ String fileName);

   CARAPI_(ArrayOf<Byte>*) GetThumbnailNative(
       /* [in] */ String fileName);

public:
     // The Exif tag names
    /** Type is Int32. */
    static String TAG_ORIENTATION;// = "Orientation";
    /** Type is String. */
    static String TAG_DATETIME;// = "DateTime";
    /** Type is String. */
    static String TAG_MAKE;// = "Make";
    /** Type is String. */
    static String TAG_MODEL;// = "Model";
    /** Type is Int32. */
    static String TAG_FLASH;// = "Flash";
    /** Type is Int32. */
    static String TAG_IMAGE_WIDTH;// = "ImageWidth";
    /** Type is Int32. */
    static String TAG_IMAGE_LENGTH;// = "ImageLength";
    /** String. Format is "num1/denom1,num2/denom2,num3/denom3". */
    static String TAG_GPS_LATITUDE;// = "GPSLatitude";
    /** String. Format is "num1/denom1,num2/denom2,num3/denom3". */
    static String TAG_GPS_LONGITUDE;// = "GPSLongitude";
    /** Type is String. */
    static String TAG_GPS_LATITUDE_REF;// = "GPSLatitudeRef";
    /** Type is String. */
    static String TAG_GPS_LONGITUDE_REF;// = "GPSLongitudeRef";

    /**
     * The altitude (in meters) based on the reference in TAG_GPS_ALTITUDE_REF.
     * Type is rational.
     */
    static String TAG_GPS_ALTITUDE;// = "GPSAltitude";

    /**
     * 0 if the altitude is above sea level. 1 if the altitude is below sea
     * level. Type is Int32.
     */
    static String TAG_GPS_ALTITUDE_REF;// = "GPSAltitudeRef";

    /** Type is String. */
    static String TAG_GPS_TIMESTAMP;// = "GPSTimeStamp";
    /** Type is String. */
    static String TAG_GPS_DATESTAMP;// = "GPSDateStamp";
    /** Type is Int32. */
    static String TAG_WHITE_BALANCE;// = "WhiteBalance";
    /** Type is rational. */
    static String TAG_FOCAL_LENGTH;// = "FocalLength";
    /** Type is String. Name of GPS processing method used for location finding. */
    static String TAG_GPS_PROCESSING_METHOD;// = "GPSProcessingMethod";

    // Constants used for the Orientation Exif tag.
    static const Int32 ORIENTATION_UNDEFINED = 0;
    static const Int32 ORIENTATION_NORMAL = 1;
    static const Int32 ORIENTATION_FLIP_HORIZONTAL = 2;  // left right reversed mirror
    static const Int32 ORIENTATION_ROTATE_180 = 3;
    static const Int32 ORIENTATION_FLIP_VERTICAL = 4;  // upside down mirror
    static const Int32 ORIENTATION_TRANSPOSE = 5;  // flipped about top-left <--> bottom-right axis
    static const Int32 ORIENTATION_ROTATE_90 = 6;  // rotate 90 cw to right it
    static const Int32 ORIENTATION_TRANSVERSE = 7;  // flipped about top-right <--> bottom-left axis
    static const Int32 ORIENTATION_ROTATE_270 = 8;  // rotate 270 to right it

    // Constants used for white balance
    static const Int32 WHITEBALANCE_AUTO = 0;
    static const Int32 WHITEBALANCE_MANUAL = 1;

private:
    //static SimpleDateFormat sFormatter;

    String mFilename;
    HashMap<String, String> mAttributes;
    Boolean mHasThumbnail;

    // Because the underlying implementation (jhead) uses static variables,
    // there can only be one user at a time for the native functions (and
    // they cannot keep state in the native code across function calls). We
    // use sLock to serialize the accesses.
    //static Object sLock = new Object();

};

#endif
