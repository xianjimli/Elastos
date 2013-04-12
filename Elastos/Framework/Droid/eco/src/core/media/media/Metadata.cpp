
#include "media/Metadata.h"

/**
 * Helper class to hold a triple (time, duration, text). Can be used to
 * implement caption.
 */
//Metadata::TimedText::TimedText(
//    /* [in] */ Date time, 
//    /* [in] */ Int32 duration, 
//    /* [in] */ String text)
//{
//    mTime = time;
//    mDuration = duration;
//    mText = text;
//}

String Metadata::TimedText::ToString()
{
    /*StringBuilder res = new StringBuilder(80);
    res.append(mTime).append("-").append(mDuration)
            .append(":").append(mText);
    return res.toString();*/
}


Metadata::Metadata()
{ }

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
 * @return FALSE if an error occurred during parsing.
 */
Boolean Metadata::ScanAllRecords(
    /* [in] */ IParcel* parcel, 
    /* [in] */ Int32 bytesLeft) 
{
    Int32 recCount = 0;
    Boolean error = FALSE;

    //mKeyToPosMap.Clear();
    while (bytesLeft > kRecordHeaderSize) {
        Int32 start;
        //parcel->DataPosition(&start);
        // Check the size.
        Int32 size;
        parcel->ReadInt32(&size);

        if (size <= kRecordHeaderSize) {  // at least 1 byte should be present.
            //Log.e(TAG, "Record is too short");
            error = TRUE;
            break;
        }

        // Check the metadata key.
        Int32 metadataId;
        parcel->ReadInt32(&metadataId);
        if (!CheckMetadataId(metadataId)) 
        {
            error = TRUE;
            break;
        }

        // Store the record offset which points to the type
        // field so we can later on read/unmarshall the record
        // payload.
        //if (mKeyToPosMap.ContainsKey(metadataId)) {
        //    //Log.e(TAG, "Duplicate metadata ID found");
        //    error = TRUE;
        //    break;
        //}

        Int32 pos;
        //parcel->DataPosition(&pos);
        //mKeyToPosMap.Put(metadataId, pos);

        // Check the metadata type.
        Int32 metadataType;
        parcel->ReadInt32(&metadataType);
        if (metadataType <= 0 || metadataType > LAST_TYPE) {
            //Log.e(TAG, "Invalid metadata type " + metadataType);
            error = TRUE;
            break;
        }

        // Skip to the next one.
        //parcel->SetDataPosition(start + size);
        bytesLeft -= size;
        ++recCount;
    }

    if (0 != bytesLeft || error) {
        //Log.e(TAG, "Ran out of data or error on record " + recCount);
        //mKeyToPosMap.Clear();
        return FALSE;
    } else {
        return TRUE;
    }
}

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
 * @return FALSE if an error occurred.
 */
Boolean Metadata::Parse(
    /* [in] */ IParcel* parcel) 
{
    Int32 avail;
    //parcel->DataAvail(&avail);
    if (avail < kMetaHeaderSize) {
        //Log.e(TAG, "Not enough data " + parcel.dataAvail());
        return FALSE;
    }

    Int32 pin;
    //parcel->DataPosition(&pin);  // to roll back in case of errors.
    Int32 size;
    parcel->ReadInt32(&size);

    // The extra kInt32Size below is to account for the int32 'size' just read.
    if (avail + kInt32Size < size || size < kMetaHeaderSize) {
        //Log.e(TAG, "Bad size " + size + " avail " + parcel.dataAvail() + " position " + pin);
        //parcel->SetDataPosition(pin);
        return FALSE;
    }

    // Checks if the 'M' 'E' 'T' 'A' marker is present.
    Int32 kShouldBeMetaMarker;
    parcel->ReadInt32(&kShouldBeMetaMarker);
    if (kShouldBeMetaMarker != kMetaMarker ) {
        //Log.e(TAG, "Marker missing " + Integer.toHexString(kShouldBeMetaMarker));
        //parcel->SetDataPosition(pin);
        return FALSE;
    }

    // Scan the records to collect metadata ids and offsets.
    if (!ScanAllRecords(parcel, size - kMetaHeaderSize)) {
        //parcel->SetDataPosition(pin);
        return FALSE;
    }
    mParcel = parcel;
    return TRUE;
}

/**
 * @return The set of metadata ID found.
 */
Set<Int32>* Metadata::KeySet()
{
    return NULL;//mKeyToPosMap.KeySet();
}

/**
 * @return TRUE if a value is present for the given key.
 */
Boolean Metadata::Has(
    /* [in] */ Int32 metadataId)
{
    if (!CheckMetadataId(metadataId)) {
        //throw new IllegalArgumentException("Invalid key: " + metadataId);
    }
    return FALSE;//mKeyToPosMap.ContainsKey(metadataId);
}

// Accessors.
// Caller must make sure the key is present using the {@code has}
// method otherwise a RuntimeException will occur.

String Metadata::GetString(
    /* [in] */ Int32 key)
{
    CheckType(key, STRING_VAL); 
    
    String str;
    mParcel->ReadString(&str);
    return str;
}

Int32 Metadata::GetInt32(
    /* [in] */ Int32 key)
{
    CheckType(key, INTEGER_VAL);
    Int32 res;
    mParcel->ReadInt32(&res);
    return res;
}

Boolean Metadata::GetBoolean(
    /* [in] */ Int32 key)
{
    CheckType(key, BOOLEAN_VAL);

    Int32 res;
    mParcel->ReadInt32(&res);
    return res == 1;
}

Int64 Metadata::GetInt64(
    /* [in] */ Int32 key) 
{
    CheckType(key, LONG_VAL);
    
    Int64 res;
    mParcel->ReadInt64(&res);
    return res;
}

Double Metadata::GetDouble(
    /* [in] */ Int32 key)
{
    CheckType(key, DOUBLE_VAL);
    
    Double res;
    mParcel->ReadDouble(&res);
    return res;
}

ArrayOf<Byte>* Metadata::GetByteArray(
    /* [in] */ Int32 key)
{
    CheckType(key, BYTE_ARRAY_VAL);

    /*ArrayOf<Byte>* arr;
    mParcel->CreateByteArray();*/
    return NULL;
}

//public Date getDate(final Int32 key) {
//    checkType(key, DATE_VAL);
//    final long timeSinceEpoch = mParcel.readLong();
//    final String timeZone = mParcel.readString();
//
//    if (timeZone.length() == 0) {
//        return new Date(timeSinceEpoch);
//    } else {
//        TimeZone tz = TimeZone.getTimeZone(timeZone);
//        Calendar cal = Calendar.getInstance(tz);
//
//        cal.setTimeInMillis(timeSinceEpoch);
//        return cal.getTime();
//    }
//}

//public TimedText getTimedText(final Int32 key) {
//    checkType(key, TIMED_TEXT_VAL);
//    final Date startTime = new Date(mParcel.readLong());  // epoch
//    final Int32 duration = mParcel.readInt();  // millisec
//
//    return new TimedText(startTime,
//                         duration,
//                         mParcel.readString());
//}

// @return the last available system metadata id. Ids are
// 1-indexed.
Int32 Metadata::LastSytemId() 
{ 
    return LAST_SYSTEM; 
}

// @return the first available cutom metadata id.
Int32 Metadata::FirstCustomId() 
{ 
    return FIRST_CUSTOM; 
}

// @return the last value of known type. Types are 1-indexed.
Int32 Metadata::LastType() 
{ 
    return LAST_TYPE; 
}

// Check val is either a system id or a custom one.
// @param val Metadata key to test.
// @return TRUE if it is in a valid range.
Boolean Metadata::CheckMetadataId(
    /* [in] */ Int32 val) 
{
    if (val <= ANY || (LAST_SYSTEM < val && val < FIRST_CUSTOM)) {
        //Log.e(TAG, "Invalid metadata ID " + val);
        return FALSE;
    }
    return TRUE;
}

// Check the type of the data match what is expected.
void Metadata::CheckType(
    /* [in] */ Int32 key, 
    /* [in] */ Int32 expectedType)
{
    Int32 pos;// = mKeyToPosMap.Get(key);

    //mParcel->SetDataPosition(pos);

    Int32 type;
    mParcel->ReadInt32(&type);
    if (type != expectedType) {
        //throw new IllegalStateException("Wrong type " + expectedType + " but got " + type);
    }
}
