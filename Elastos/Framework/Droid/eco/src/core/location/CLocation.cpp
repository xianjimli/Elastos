
#include "location/CLocation.h"
#include "os/CBundle.h"
#include <StringBuffer.h>
#include <elastos/Math.h>

CLocation::CLocation()
    : mTime(0)
    , mLatitude(0.0)
    , mLongitude(0.0)
    , mHasAltitude(FALSE)
    , mAltitude(0.0f)
    , mHasSpeed(FALSE)
    , mSpeed(0.0f)
    , mHasBearing(FALSE)
    , mBearing(0.0f)
    , mHasAccuracy(FALSE)
    , mAccuracy(0.0f)
    , mLat1(0.0f)
    , mLon1(0.0f)
    , mLat2(0.0f)
    , mLon2(0.0f)
    , mDistance(0.0f)
    , mInitialBearing(0.0f)
{
    mResults = ArrayOf<Float>::Alloc(2);
}

ECode CLocation::Set(
    /* [in] */ ILocation* l)
{
    AutoPtr<CLocation> location = (CLocation*)l;
    mProvider = location->mProvider;
    mTime = location->mTime;
    mLatitude = location->mLatitude;
    mLongitude = location->mLongitude;
    mHasAltitude = location->mHasAltitude;
    mAltitude = location->mAltitude;
    mHasSpeed = location->mHasSpeed;
    mSpeed = location->mSpeed;
    mHasBearing = location->mHasBearing;
    mBearing = location->mBearing;
    mHasAccuracy = location->mHasAccuracy;
    mAccuracy = location->mAccuracy;
    if (location->mExtras != NULL) {
        ASSERT_SUCCEEDED(CBundle::New(location->mExtras, (IBundle**)&mExtras));
    }
    else {
        mExtras = NULL;
    }

    return NOERROR;
}

ECode CLocation::Reset()
{
    mProvider = String(NULL);
    mTime = 0;
    mLatitude = 0;
    mLongitude = 0;
    mHasAltitude = FALSE;
    mAltitude = 0;
    mHasSpeed = FALSE;
    mSpeed = 0;
    mHasBearing = FALSE;
    mBearing = 0;
    mHasAccuracy = FALSE;
    mAccuracy = 0;
    mExtras = NULL;

    return NOERROR;
}

ECode CLocation::Convert(
    /* [in] */ Double coordinate,
    /* [in] */ Int32 outputType,
    /* [out] */ String* representation)
{
    VALIDATE_NOT_NULL(representation);

    if (coordinate < -180.0 || coordinate > 180.0/* || Double.isNaN(coordinate)*/) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("coordinate=" + coordinate);
    }
    if ((outputType != Location_FORMAT_DEGREES) &&
        (outputType != Location_FORMAT_MINUTES) &&
        (outputType != Location_FORMAT_SECONDS)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("outputType=" + outputType);
    }

    StringBuffer sb;// = new StringBuilder();

    // Handle negative values
    if (coordinate < 0) {
        sb += "-";
        coordinate = -coordinate;
    }

//    DecimalFormat df = new DecimalFormat("###.#####");
    if (outputType == Location_FORMAT_MINUTES || outputType == Location_FORMAT_SECONDS) {
        Int32 degrees = (Int32) Math::Floor(coordinate);
        sb += degrees + ":";
//        sb.Append(':');
        coordinate -= degrees;
        coordinate *= 60.0;
        if (outputType == Location_FORMAT_SECONDS) {
            Int32 minutes = (Int32) Math::Floor(coordinate);
            sb += minutes + ":";
            coordinate -= minutes;
            coordinate *= 60.0;
        }
    }
//    sb.Append(df.format(coordinate));
    *representation = (String)sb;

    return NOERROR;
}

ECode CLocation::ConvertEx(
    /* [in] */ String coordinate,
    /* [out] */ Double* representation)
{
    VALIDATE_NOT_NULL(representation);

    // IllegalArgumentException if bad syntax
    if (coordinate == NULL) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException("coordinate");
    }

    Boolean negative = FALSE;
    if (coordinate.GetChar(0) == '-') {
        coordinate = coordinate.Substring(1);
        negative = TRUE;
    }

    StringTokenizer* st = new StringTokenizer(coordinate, ":");
    Int32 tokens = st->GetCount();
    if (tokens < 1) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("coordinate=" + coordinate);
    }
//    try {
    String degrees = st->NextToken();
    Double val;
    if (tokens == 1) {
        val = degrees.ToDouble();
        *representation = negative ? -val : val;
        return NOERROR;
    }

    String minutes = st->NextToken();
    Int32 deg = degrees.ToInt32();
    Double min;
    Double sec = 0.0;

    if (st->HasMoreTokens()) {
        min = minutes.ToInt32();
        String seconds = st->NextToken();
        sec = seconds.ToDouble();
    }
    else {
        min = minutes.ToDouble();
    }

    Boolean isNegative180 = negative && (deg == 180) &&
        (min == 0) && (sec == 0);

    // deg must be in [0, 179] except for the case of -180 degrees
    if ((deg < 0.0) || (deg > 179 && !isNegative180)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("coordinate=" + coordinate);
    }
    if (min < 0 || min > 59) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("coordinate=" +
//                coordinate);
    }
    if (sec < 0 || sec > 59) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("coordinate=" +
//                coordinate);
    }

    val = deg * 3600.0 + min * 60.0 + sec;
    val /= 3600.0;
    *representation = negative ? -val : val;

    return NOERROR;
//    } catch (NumberFormatException nfe) {
//        throw new IllegalArgumentException("coordinate=" + coordinate);
//    }
}

void CLocation::ComputeDistanceAndBearing(
    /* [in] */ Double lat1,
    /* [in] */ Double lon1,
    /* [in] */ Double lat2,
    /* [in] */ Double lon2,
    /* [in] */ ArrayOf<Float>* results)
{
    // Based on http://www.ngs.noaa.gov/PUBS_LIB/inverse.pdf
    // using the "Inverse Formula" (section 4)

    Int32 MAXITERS = 20;
    // Convert lat/long to radians
    lat1 *= Math::DOUBLE_PI / 180.0;
    lat2 *= Math::DOUBLE_PI / 180.0;
    lon1 *= Math::DOUBLE_PI / 180.0;
    lon2 *= Math::DOUBLE_PI / 180.0;

    Double a = 6378137.0; // WGS84 major axis
    Double b = 6356752.3142; // WGS84 semi-major axis
    Double f = (a - b) / a;
    Double aSqMinusBSqOverBSq = (a * a - b * b) / (b * b);

    Double L = lon2 - lon1;
    Double A = 0.0;
    Double U1 = Math::Atan((1.0 - f) * Math::Tan(lat1));
    Double U2 = Math::Atan((1.0 - f) * Math::Tan(lat2));

    Double cosU1 = Math::Cos(U1);
    Double cosU2 = Math::Cos(U2);
    Double sinU1 = Math::Sin(U1);
    Double sinU2 = Math::Sin(U2);
    Double cosU1cosU2 = cosU1 * cosU2;
    Double sinU1sinU2 = sinU1 * sinU2;

    Double sigma = 0.0;
    Double deltaSigma = 0.0;
    Double cosSqAlpha = 0.0;
    Double cos2SM = 0.0;
    Double cosSigma = 0.0;
    Double sinSigma = 0.0;
    Double cosLambda = 0.0;
    Double sinLambda = 0.0;

    Double lambda = L; // initial guess
    for (Int32 iter = 0; iter < MAXITERS; iter++) {
        Double lambdaOrig = lambda;
        cosLambda = Math::Cos(lambda);
        sinLambda = Math::Sin(lambda);
        Double t1 = cosU2 * sinLambda;
        Double t2 = cosU1 * sinU2 - sinU1 * cosU2 * cosLambda;
        Double sinSqSigma = t1 * t1 + t2 * t2; // (14)
        sinSigma = Math::Sqrt(sinSqSigma);
        cosSigma = sinU1sinU2 + cosU1cosU2 * cosLambda; // (15)
        sigma = Math::Atan2(sinSigma, cosSigma); // (16)
        Double sinAlpha = (sinSigma == 0) ? 0.0 :
            cosU1cosU2 * sinLambda / sinSigma; // (17)
        cosSqAlpha = 1.0 - sinAlpha * sinAlpha;
        cos2SM = (cosSqAlpha == 0) ? 0.0 :
            cosSigma - 2.0 * sinU1sinU2 / cosSqAlpha; // (18)

        Double uSquared = cosSqAlpha * aSqMinusBSqOverBSq; // defn
        A = 1 + (uSquared / 16384.0) * // (3)
            (4096.0 + uSquared *
             (-768 + uSquared * (320.0 - 175.0 * uSquared)));
        Double B = (uSquared / 1024.0) * // (4)
            (256.0 + uSquared *
             (-128.0 + uSquared * (74.0 - 47.0 * uSquared)));
        Double C = (f / 16.0) *
            cosSqAlpha *
            (4.0 + f * (4.0 - 3.0 * cosSqAlpha)); // (10)
        Double cos2SMSq = cos2SM * cos2SM;
        deltaSigma = B * sinSigma * // (6)
            (cos2SM + (B / 4.0) *
             (cosSigma * (-1.0 + 2.0 * cos2SMSq) -
              (B / 6.0) * cos2SM *
              (-3.0 + 4.0 * sinSigma * sinSigma) *
              (-3.0 + 4.0 * cos2SMSq)));

        lambda = L +
            (1.0 - C) * f * sinAlpha *
            (sigma + C * sinSigma *
             (cos2SM + C * cosSigma *
              (-1.0 + 2.0 * cos2SM * cos2SM))); // (11)

        Double delta = (lambda - lambdaOrig) / lambda;
        if (Math::Abs(delta) < 1.0e-12) {
            break;
        }
    }

    Float distance = (Float) (b * A * (sigma - deltaSigma));
    (*results)[0] = distance;
    if (results->GetLength() > 1) {
        Float initialBearing = (Float) Math::Atan2(cosU2 * sinLambda,
            cosU1 * sinU2 - sinU1 * cosU2 * cosLambda);
        initialBearing *= 180.0 / Math::DOUBLE_PI;
        (*results)[1] = initialBearing;
        if (results->GetLength() > 2) {
            Float finalBearing = (Float) Math::Atan2(cosU1 * sinLambda,
                -sinU1 * cosU2 + cosU1 * sinU2 * cosLambda);
            finalBearing *= 180.0 / Math::DOUBLE_PI;
            (*results)[2] = finalBearing;
        }
    }
}

ECode CLocation::DistanceBetween(
    /* [in] */ Double startLatitude,
    /* [in] */ Double startLongitude,
    /* [in] */ Double endLatitude,
    /* [in] */ Double endLongitude,
    /* [in] */ ArrayOf<Float>* results)
{
    if (results == NULL || results->GetLength() < 1) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("results is null or has length < 1");
    }
    ComputeDistanceAndBearing(startLatitude, startLongitude,
        endLatitude, endLongitude, results);

    return NOERROR;
}

ECode CLocation::DistanceTo(
    /* [in] */ ILocation* dest,
    /* [out] */ Float* distance)
{
    VALIDATE_NOT_NULL(distance);

    // See if we already have the result
    Mutex::Autolock lock(mResultsLock);

    AutoPtr<CLocation> destLocation = (CLocation*)dest;
    if (mLatitude != mLat1 || mLongitude != mLon1 ||
        destLocation->mLatitude != mLat2 || destLocation->mLongitude != mLon2) {
        ComputeDistanceAndBearing(mLatitude, mLongitude,
            destLocation->mLatitude, destLocation->mLongitude, mResults);
        mLat1 = mLatitude;
        mLon1 = mLongitude;
        mLat2 = destLocation->mLatitude;
        mLon2 = destLocation->mLongitude;
        mDistance = (*mResults)[0];
        mInitialBearing = (*mResults)[1];
    }
    *distance = mDistance;

    return NOERROR;
}

ECode CLocation::BearingTo(
    /* [in] */ ILocation* dest,
    /* [out] */ Float* bearing)
{
    VALIDATE_NOT_NULL(bearing);

    Mutex::Autolock lock(mResultsLock);

    // See if we already have the result
    AutoPtr<CLocation> destLocation = (CLocation*)dest;
    if (mLatitude != mLat1 || mLongitude != mLon1 ||
            destLocation->mLatitude != mLat2 || destLocation->mLongitude != mLon2) {
        ComputeDistanceAndBearing(mLatitude, mLongitude,
            destLocation->mLatitude, destLocation->mLongitude, mResults);
        mLat1 = mLatitude;
        mLon1 = mLongitude;
        mLat2 = destLocation->mLatitude;
        mLon2 = destLocation->mLongitude;
        mDistance = (*mResults)[0];
        mInitialBearing = (*mResults)[1];
    }
    *bearing = mInitialBearing;

    return NOERROR;
}

ECode CLocation::GetProvider(
    /* [out] */ String* provider)
{
    VALIDATE_NOT_NULL(provider);
    *provider = mProvider;

    return NOERROR;
}

ECode CLocation::SetProvider(
    /* [in] */ const String& provider)
{
    mProvider = provider;

    return NOERROR;
}

ECode CLocation::GetTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = mTime;

    return NOERROR;
}

ECode CLocation::SetTime(
    /* [in] */ Int64 time)
{
    mTime = time;

    return NOERROR;
}

ECode CLocation::GetLatitude(
    /* [out] */ Double* latitude)
{
    VALIDATE_NOT_NULL(latitude);
    *latitude = mLatitude;

    return NOERROR;
}

ECode CLocation::SetLatitude(
    /* [in] */ Double latitude)
{
    mLatitude = latitude;

    return NOERROR;
}

ECode CLocation::GetLongitude(
    /* [out] */ Double* longitude)
{
    VALIDATE_NOT_NULL(longitude);
    *longitude = mLongitude;

    return NOERROR;
}

ECode CLocation::SetLongitude(
    /* [in] */ Double longitude)
{
    mLongitude = longitude;

    return NOERROR;
}

ECode CLocation::HasAltitude(
    /* [out] */ Boolean* hasAltitude)
{
    VALIDATE_NOT_NULL(hasAltitude);
    *hasAltitude = mHasAltitude;

    return NOERROR;
}

ECode CLocation::GetAltitude(
    /* [out] */ Double* altitude)
{
    VALIDATE_NOT_NULL(altitude);
    *altitude = mAltitude;

    return NOERROR;
}

ECode CLocation::SetAltitude(
    /* [in] */ Double altitude)
{
    mAltitude = altitude;
    mHasAltitude = TRUE;

    return NOERROR;
}

ECode CLocation::RemoveAltitude()
{
    mAltitude = 0.0f;
    mHasAltitude = FALSE;

    return NOERROR;
}

ECode CLocation::HasSpeed(
    /* [out] */ Boolean* hasSpeed)
{
    VALIDATE_NOT_NULL(hasSpeed);
    *hasSpeed = mHasSpeed;

    return NOERROR;
}

ECode CLocation::GetSpeed(
    /* [out] */ Float* speed)
{
    VALIDATE_NOT_NULL(speed);
    *speed = mSpeed;

    return NOERROR;
}

ECode CLocation::SetSpeed(
    /* [in] */ Float speed)
{
    mSpeed = speed;
    mHasSpeed = TRUE;

    return NOERROR;
}

ECode CLocation::RemoveSpeed()
{
    mSpeed = 0.0f;
    mHasSpeed = FALSE;

    return NOERROR;
}

ECode CLocation::HasBearing(
    /* [out] */ Boolean* hasBearing)
{
    VALIDATE_NOT_NULL(hasBearing);
    *hasBearing = mHasBearing;

    return NOERROR;
}

ECode CLocation::GetBearing(
    /* [out] */ Float* bearing)
{
    VALIDATE_NOT_NULL(bearing);
    *bearing = mBearing;

    return NOERROR;
}

ECode CLocation::SetBearing(
    /* [in] */ Float bearing)
{
    while (bearing < 0.0f) {
        bearing += 360.0f;
    }
    while (bearing >= 360.0f) {
        bearing -= 360.0f;
    }
    mBearing = bearing;
    mHasBearing = TRUE;

    return NOERROR;
}

ECode CLocation::RemoveBearing()
{
    mBearing = 0.0f;
    mHasBearing = FALSE;

    return NOERROR;
}

ECode CLocation::HasAccuracy(
    /* [out] */ Boolean* hasAccuracy)
{
    VALIDATE_NOT_NULL(hasAccuracy);
    *hasAccuracy = mHasAccuracy;

    return NOERROR;
}

ECode CLocation::GetAccuracy(
    /* [out] */ Float* accuracy)
{
    VALIDATE_NOT_NULL(accuracy);
    *accuracy = mAccuracy;

    return NOERROR;
}

ECode CLocation::SetAccuracy(
    /* [in] */ Float accuracy)
{
    mAccuracy = accuracy;
    mHasAccuracy = TRUE;

    return NOERROR;
}

ECode CLocation::RemoveAccuracy()
{
    mAccuracy = 0.0f;
    mHasAccuracy = FALSE;

    return NOERROR;
}

ECode CLocation::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras);
    *extras = mExtras;

    return NOERROR;
}

ECode CLocation::SetExtras(
    /* [in] */ IBundle* extras)
{
    if (extras != NULL) {
        ASSERT_SUCCEEDED(CBundle::New(extras, (IBundle**)&mExtras));
    }
    else {
        mExtras = NULL;
    }

    return NOERROR;
}

ECode CLocation::GetDescription(
    /* [out] */ String* str)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CLocation::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadString(&mProvider);
    source->ReadInt64(&mTime);
    source->ReadDouble(&mLatitude);
    source->ReadDouble(&mLongitude);
    source->ReadBoolean(&mHasAltitude);
    source->ReadDouble(&mAltitude);
    source->ReadBoolean(&mHasSpeed);
    source->ReadFloat(&mSpeed);
    source->ReadBoolean(&mHasBearing);
    source->ReadFloat(&mBearing);
    source->ReadBoolean(&mHasAccuracy);
    source->ReadFloat(&mAccuracy);
    AutoPtr<IInterface> obj;
    source->ReadInterfacePtrPtr((Handle32*)&obj);
    mExtras = obj != NULL ? IBundle::Probe(obj) : NULL;
    return NOERROR;
}

ECode CLocation::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteString(mProvider);
    dest->WriteInt64(mTime);
    dest->WriteDouble(mLatitude);
    dest->WriteDouble(mLongitude);
    dest->WriteBoolean(mHasAltitude);
    dest->WriteDouble(mAltitude);
    dest->WriteBoolean(mHasSpeed);
    dest->WriteFloat(mSpeed);
    dest->WriteBoolean(mHasBearing);
    dest->WriteFloat(mBearing);
    dest->WriteBoolean(mHasAccuracy);
    dest->WriteFloat(mAccuracy);
    dest->WriteInterfacePtr((IBundle*)mExtras);
    return NOERROR;
}

ECode CLocation::constructor()
{
    return NOERROR;
}

ECode CLocation::constructor(
    /* [in] */ const String& provider)
{
    mProvider = provider;

    return NOERROR;
}

ECode CLocation::constructor(
    /* [in] */ ILocation* l)
{
    return Set(l);
}

