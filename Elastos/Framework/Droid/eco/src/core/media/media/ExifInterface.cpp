
#include "media/ExifInterface.h"

String ExifInterface::TAG_GPS_ALTITUDE;
String ExifInterface::TAG_GPS_ALTITUDE_REF;

ExifInterface::ExifInterface()
{

}

/**
 * Reads Exif tags from the specified JPEG file.
 */
ExifInterface::ExifInterface(
    /* [in] */ String filename)
{
    mFilename = filename;
    LoadAttributes();
}

/**
 * Returns the value of the specified tag or {@code NULL} if there
 * is no such tag in the JPEG file.
 *
 * @param tag the name of the tag.
 */
String ExifInterface::GetAttribute(
    /* [in] */ String tag)
{
    //return mAttributes->Get(tag);

    return String("");
}

/**
 * Returns the integer value of the specified tag. If there is no such tag
 * in the JPEG file or the value cannot be parsed as integer, return
 * <var>defaultValue</var>.
 *
 * @param tag the name of the tag.
 * @param defaultValue the value to return if the tag is not available.
 */
Int32 ExifInterface::GetAttributeInt32(
    /* [in] */ String tag, 
    /* [in] */ Int32 defaultValue)
{
    /*String value = mAttributes.get(tag);
    if (value == NULL) return defaultValue;
    try {
        return Integer.valueOf(value);
    } catch (NumberFormatException ex) {
        return defaultValue;
    }*/

    return 0;
}

/**
 * Returns the Double value of the specified rational tag. If there is no
 * such tag in the JPEG file or the value cannot be parsed as Double, return
 * <var>defaultValue</var>.
 *
 * @param tag the name of the tag.
 * @param defaultValue the value to return if the tag is not available.
 */
Double ExifInterface::GetAttributeDouble(
    /* [in] */ String tag, 
    /* [in] */ Double defaultValue)
{
    /*String value = mAttributes.get(tag);
    if (value == NULL) return defaultValue;
    try {
        Int32 index = value.indexOf("/");
        if (index == -1) return defaultValue;
        Double denom = Double.parseDouble(value.substring(index + 1));
        if (denom == 0) return defaultValue;
        Double num = Double.parseDouble(value.substring(0, index));
        return num / denom;
    } catch (NumberFormatException ex) {
        return defaultValue;
    }*/

    return 0;
}

/**
 * Set the value of the specified tag.
 *
 * @param tag the name of the tag.
 * @param value the value of the tag.
 */
ECode ExifInterface::SetAttribute(
    /* [in] */ String tag, 
    /* [in] */ String value) 
{
    //mAttributes.Put(tag, value);

    return NOERROR;
}

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
void ExifInterface::LoadAttributes()
{
    // format of string passed from native C code:
    // "attrCnt attr1=valueLen value1attr2=value2Len value2..."
    // example:
    // "4 attrPtr ImageLength=4 1024Model=6 FooImageWidth=4 1280Make=3 FOO"
    //mAttributes = new HashMap<String, String>();

    //String attrStr;
    //synchronized (sLock) {
    //    attrStr = getAttributesNative(mFilename);
    //}

    //// get count
    //Int32 ptr = attrStr.indexOf(' ');
    //Int32 count = Integer.parseInt(attrStr.substring(0, ptr));
    //// skip past the space between item count and the rest of the attributes
    //++ptr;

    //for (Int32 i = 0; i < count; i++) {
    //    // extract the attribute name
    //    Int32 equalPos = attrStr.indexOf('=', ptr);
    //    String attrName = attrStr.substring(ptr, equalPos);
    //    ptr = equalPos + 1;     // skip past =

    //    // extract the attribute value length
    //    Int32 lenPos = attrStr.indexOf(' ', ptr);
    //    Int32 attrLen = Integer.parseInt(attrStr.substring(ptr, lenPos));
    //    ptr = lenPos + 1;       // skip pas the space

    //    // extract the attribute value
    //    String attrValue = attrStr.substring(ptr, ptr + attrLen);
    //    ptr += attrLen;

    //    if (attrName.equals("hasThumbnail")) {
    //        mHasThumbnail = attrValue.equalsIgnoreCase("true");
    //    } else {
    //        mAttributes.put(attrName, attrValue);
    //    }
    //}
}

/**
 * Save the tag data into the JPEG file. This is expensive because it involves
 * copying all the JPG data from one file to another and deleting the old file
 * and renaming the other. It's best to use {@link #setAttribute(String,String)}
 * to set all attributes to write and make a single call rather than multiple
 * calls for each attribute.
 */
ECode ExifInterface::SaveAttributes()
{
    // format of string passed to native C code:
    // "attrCnt attr1=valueLen value1attr2=value2Len value2..."
    // example:
    // "4 attrPtr ImageLength=4 1024Model=6 FooImageWidth=4 1280Make=3 FOO"
    //StringBuilder sb = new StringBuilder();
    //Int32 size = mAttributes.size();
    //if (mAttributes.containsKey("hasThumbnail")) {
    //    --size;
    //}
    //sb.append(size + " ");
    //for (Map.Entry<String, String> iter : mAttributes.entrySet()) {
    //    String key = iter.getKey();
    //    if (key.equals("hasThumbnail")) {
    //        // this is a fake attribute not saved as an exif tag
    //        continue;
    //    }
    //    String val = iter.getValue();
    //    sb.append(key + "=");
    //    sb.append(val.length() + " ");
    //    sb.append(val);
    //}
    //String s = sb.toString();
    //synchronized (sLock) {
    //    saveAttributesNative(mFilename, s);
    //    commitChangesNative(mFilename);
    //}

    return NOERROR;
}

/**
 * Returns true if the JPEG file has a thumbnail.
 */
Boolean ExifInterface::HasThumbnail() 
{
    return mHasThumbnail;
}

/**
 * Returns the thumbnail inside the JPEG file, or {@code NULL} if there is no thumbnail.
 * The returned data is in JPEG format and can be decoded using
 * {@link android.graphics.BitmapFactory#decodeByteArray(Byte[],Int32,Int32)}
 */
ArrayOf<Byte>* ExifInterface::GetThumbnail() 
{
    //synchronized (sLock) {
        return GetThumbnailNative(mFilename);
    //}
}

/**
 * Stores the latitude and longitude value in a Float array. The first element is
 * the latitude, and the second element is the longitude. Returns false if the
 * Exif tags are not available.
 */
Boolean ExifInterface::GetLatLong(
    /* [in] */ ArrayOf<Float>* output) 
{
    /*String latValue = mAttributes.Get(ExifInterface::TAG_GPS_LATITUDE);
    String latRef = mAttributes.Get(ExifInterface::TAG_GPS_LATITUDE_REF);
    String lngValue = mAttributes.Get(ExifInterface::TAG_GPS_LONGITUDE);
    String lngRef = mAttributes.Get(ExifInterface::TAG_GPS_LONGITUDE_REF);

    if (latValue != NULL && latRef != NULL && lngValue != NULL && lngRef != NULL) {
        output[0] = ConvertRationalLatLonToFloat(latValue, latRef);
        output[1] = ConvertRationalLatLonToFloat(lngValue, lngRef);
        return TRUE;
    } else {
        return FALSE;
    }*/

    return FALSE;
}

/**
 * Return the altitude in meters. If the exif tag does not exist, return
 * <var>defaultValue</var>.
 *
 * @param defaultValue the value to return if the tag is not available.
 */
Double ExifInterface::GetAltitude(
    /* [in] */ Double defaultValue)
{
    Double altitude = GetAttributeDouble(TAG_GPS_ALTITUDE, -1);
    Int32 ref = GetAttributeInt32(TAG_GPS_ALTITUDE_REF, -1);

    if (altitude >= 0 && ref >= 0) {
        return (Double) (altitude * ((ref == 1) ? -1 : 1));
    } else {
        return defaultValue;
    }
}

/**
 * Returns number of milliseconds since Jan. 1, 1970, midnight.
 * Returns -1 if the date time information if not available.
 * @hide
 */
Int64 ExifInterface::GetDateTime() 
{
    /*String dateTimeString = mAttributes.get(TAG_DATETIME);
    if (dateTimeString == NULL) return -1;

    ParsePosition pos = new ParsePosition(0);
    try {
        Date datetime = sFormatter.parse(dateTimeString, pos);
        if (datetime == NULL) return -1;
        return datetime.getTime();
    } catch (IllegalArgumentException ex) {*/
        return -1;
    //}
}

/**
 * Returns number of milliseconds since Jan. 1, 1970, midnight UTC.
 * Returns -1 if the date time information if not available.
 * @hide
 */
Int64 ExifInterface::GetGpsDateTime()
{
    /*String date = mAttributes.get(TAG_GPS_DATESTAMP);
    String time = mAttributes.get(TAG_GPS_TIMESTAMP);
    if (date == NULL || time == NULL) return -1;

    String dateTimeString = date + ' ' + time;
    if (dateTimeString == NULL) return -1;

    ParsePosition pos = new ParsePosition(0);
    try {
        Date datetime = sFormatter.parse(dateTimeString, pos);
        if (datetime == NULL) return -1;
        return datetime.getTime();
    } catch (IllegalArgumentException ex) {*/
        return -1;
    //}
}

Float ExifInterface::ConvertRationalLatLonToFloat(
    /* [in] */ String rationalString, 
    /* [in] */ String ref) 
{
    /*try {
        String [] parts = rationalString.split(",");

        String [] pair;
        pair = parts[0].split("/");
        Int32 degrees = (Int32) (Float.parseFloat(pair[0].trim())
                / Float.parseFloat(pair[1].trim()));

        pair = parts[1].split("/");
        Int32 minutes = (Int32) ((Float.parseFloat(pair[0].trim())
                / Float.parseFloat(pair[1].trim())));

        pair = parts[2].split("/");
        Double seconds = Double.parseDouble(pair[0].trim())
                / Double.parseDouble(pair[1].trim());

        Double result = degrees + (minutes / 60.0) + (seconds / 3600.0);
        if ((ref.equals("S") || ref.equals("W"))) {
            return (Float) -result;
        }
        return (Float) result;
    } catch (RuntimeException ex) {*/
        // if for whatever reason we can't parse the lat long then return
        // NULL
        return 0;
    //}
}

Boolean ExifInterface::AppendThumbnailNative(
    /* [in] */ String fileName,
    /* [in] */ String thumbnailFileName)
{
    return FALSE;
}

void ExifInterface::SaveAttributesNative(
    /* [in] */ String fileName,
    /* [in] */ String compressedAttributes)
{

}

String ExifInterface::GetAttributesNative(
    /* [in] */ String fileName)
{
    return String("");
}

void ExifInterface::CommitChangesNative(
    /* [in] */ String fileName)
{

}

ArrayOf<Byte>* ExifInterface::GetThumbnailNative(
    /* [in] */ String fileName)
{
    return NULL;
}

