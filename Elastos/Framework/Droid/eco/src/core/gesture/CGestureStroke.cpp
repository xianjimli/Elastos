#include <gesture/CGestureStroke.h>
#include <gesture/CGestureUtils.h>
#include <gesture/CGesturePoint.h>
#include <graphics/CRectF.h>
#include <graphics/CPath.h>
#include <elastos/AutoPtr.h>
#include <elastos/Math.h>

/**
 * A gesture stroke started on a touch down and ended on a touch up. A stroke
 * consists of a sequence of timed points. One or multiple strokes form a gesture.
 */
CGestureStroke::CGestureStroke()
{

}

CGestureStroke::~CGestureStroke()
{
    ArrayOf<Float>::Free(mPoints);
    ArrayOf<Int64>::Free(mTimestamps);
    mBoundingBox->Release();
    mCachedPath->Release();
}

 /**
 * A constructor that constructs a gesture stroke from a list of gesture points.
 *
 * @param points //IObjectContainer<IGesturePoint* >
 */
ECode CGestureStroke::constructor(
    /* [in] */ IObjectContainer* points)
{
    Int32 count = 0;
    points->GetObjectCount(&count);
    mPoints = ArrayOf<Float>::Alloc(count * 2);
    mTimestamps = ArrayOf<Int64>::Alloc(count);
    mBoundingBox = NULL;

    Float len = 0;

    //for (Int32 i = 0; i < count; i++) {
    if (count > 0) {
        AutoPtr<IObjectEnumerator>iter;
        points->GetObjectEnumerator((IObjectEnumerator**)&iter);
        Boolean hasNext = FALSE;
        Int32 i = 0;
        Int32 index = 0;
        iter->MoveNext(&hasNext);
        while(hasNext) {
            IGesturePoint* p=NULL;
            iter->Current((IInterface**)&p);
            //IGesturePoint* p = points->Get(i);
            Float x;
            Float y;
            Int64 timestamp;
            p->GetX(&x);
            p->GetY(&y);
            p->GetTimestamp(&timestamp);
            (*mPoints)[i * 2] = x;
            (*mPoints)[i * 2 + 1] = y;
            (*mTimestamps)[index] = timestamp;

            if (mBoundingBox == NULL) {
                CRectF::New(&mBoundingBox);
                mBoundingBox->Set(y, x ,x, y);
                len = 0;
            } else {
                len += Math::Sqrt(Math::Pow(x - (*mPoints)[(i - 1) * 2], 2)
                        + Math::Pow(y - (*mPoints)[(i -1 ) * 2 + 1], 2));
                mBoundingBox->UnionEx2(x, y);
            }
            index++;
            i++;
        }
    }
    mLength = len;
    return NOERROR;
}

/**
 * A faster constructor specially for cloning a stroke.
 */
ECode CGestureStroke::constructor(
    /* [in] */ IRectF* bbx,
    /* [in] */ Float len,
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ const ArrayOf<Int64>& times)
{
    CRectF::New(bbx, &mBoundingBox);
    mLength = len;
    mPoints = pts.Clone();
    mTimestamps = times.Clone();
    return NOERROR;
}
    
/* @Override */
ECode CGestureStroke::Clone(
    /* [out] */ IGestureStroke ** obj)
{
    return CGestureStroke::New(mBoundingBox, mLength, *mPoints, *mTimestamps, obj);
}
    
/**
 * Draws the stroke with a given canvas and paint.
 *
 * @param canvas
 */
ECode CGestureStroke::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(canvas);
    if (mCachedPath == NULL) {
        MakePath();
     }
    canvas->DrawPath(mCachedPath, paint);
    return NOERROR;
}

ECode CGestureStroke::GetPath(
    /* [out] */ IPath ** path)
{
    if (mCachedPath == NULL) {
        MakePath();
     }

    *path = mCachedPath;
    return NOERROR;
}

void CGestureStroke::MakePath()
{
    ArrayOf<Float>* localPoints = mPoints;//->Clone();
    Int32 count = localPoints->GetLength();

    IPath* path = NULL;
    Float mX = 0;
    Float mY = 0;

    for (Int32 i = 0; i < count; i += 2) {
        Float x = (*localPoints)[i];
        Float y = (*localPoints)[i + 1];
        if (path == NULL) {
            CPath::New(&path);
            path->MoveTo(x, y);
            mX = x;
            mY = y;
        } else {
            Float dx = Math::Abs(x - mX);
            Float dy = Math::Abs(y - mY);
            if (dx >= GestureStroke_TOUCH_TOLERANCE || dy >= GestureStroke_TOUCH_TOLERANCE) {
                path->QuadTo(mX, mY, (x + mX) / 2, (y + mY) / 2);
                mX = x;
                mY = y;
             }
         }
    }
    //ArrayOf<Float>::Free(localPoints);
    mCachedPath = path;

}

/**
 * Converts the stroke to a Path of a given number of points.
 *
 * @param width the width of the bounding box of the target path
 * @param height the height of the bounding box of the target path
 * @param numSample the number of points needed
 *
 * @return the path
 */
ECode CGestureStroke::ToPath(
    /* [in] */ Float width,
    /* [in] */ Float height,
    /* [in] */ Int32 numSample,
    /* [out] */ IPath ** path)

{
    IGestureStroke* obj = NULL;
    Clone(&obj);
    ArrayOf<Float>* pts = NULL;
    CGestureUtils::TemporalSampling((IGestureStroke*)obj, numSample, &pts);
    IRectF* rect = mBoundingBox;

    CGestureUtils::Translate(pts, -((CRectF *)rect)->mLeft, -((CRectF *)rect)->mTop);

    Float rectWidth;
    rect->Width(&rectWidth);
    Float rectHeight;
    rect->Height(&rectHeight);

    Float sx = width / rectWidth;
    Float sy = height / rectHeight;
    Float scale = sx > sy ? sy : sx;
    CGestureUtils::Scale(pts, scale, scale);

    Float mX = 0;
    Float mY = 0;

    IPath* pathret = NULL;

    Int32 count = pts->GetLength();

    for (Int32 i = 0; i < count; i += 2) {
        Float x = (*pts)[i];
        Float y = (*pts)[i + 1];
        if (pathret == NULL) {
            CPath::New(&pathret);
            pathret->MoveTo(x, y);
            mX = x;
            mY = y;
        } else {
            Float dx = Math::Abs(x - mX);
            Float dy = Math::Abs(y - mY);
            if (dx >= GestureStroke_TOUCH_TOLERANCE || dy >= GestureStroke_TOUCH_TOLERANCE) {
                pathret->QuadTo(mX, mY, (x + mX) / 2, (y + mY) / 2);
                mX = x;
                mY = y;
             }
         }
    }
    obj->Release();
    ArrayOf<Float>::Free(pts);
    *path = pathret;
    return NOERROR;
}

ECode CGestureStroke::Serialize(
    /* [in] */ IDataOutputStream* out)
{
    VALIDATE_NOT_NULL(out);
    ArrayOf<Float>* pts = mPoints;
    ArrayOf<Int64>* times = mTimestamps;
    Int32 count = mPoints->GetLength();

    IDataOutput* dout = IDataOutput::Probe(out);

    // Write number of points
    dout->WriteInt32(count / 2);

    for (Int32 i = 0; i < count; i += 2) {
        // Write X
        dout->WriteFloat((*pts)[i]);
        // Write Y
        dout->WriteFloat((*pts)[i + 1]);
        // Write timestamp
        dout->WriteInt64((*times)[i / 2]);
    }

    return NOERROR;
}

ECode CGestureStroke::Deserialize(
    /* [in] */ IDataInputStream* in,
    /* [out] */ IGestureStroke** gestureStroke )
{
    VALIDATE_NOT_NULL(in);
    VALIDATE_NOT_NULL(gestureStroke);
    // Number of points

    IDataInput* din = IDataInput::Probe(in);
    Int32 count;
    din->ReadInt32(&count);

    IObjectContainer* points = NULL;
    CObjectContainer::New(&points);

    for (Int32 i = 0; i < count; i++) {
        IGesturePoint * gesturePoint = NULL;
        CGesturePoint::Deserialize(in, &gesturePoint);
        points->Add((IInterface *)gesturePoint);
    }

    CGestureStroke::New(points, gestureStroke);
    points->Release();
    return NOERROR;
}

ECode CGestureStroke::DeserializeEx(
    /* [in] */ IParcel* in,
    /* [out] */ IGestureStroke** gestureStroke )
{
    VALIDATE_NOT_NULL(in);
    VALIDATE_NOT_NULL(gestureStroke);
    // Number of points
    Int32 count;
    in->ReadInt32(&count);

    IObjectContainer* points = NULL;
    CObjectContainer::New(&points);

    for (Int32 i = 0; i < count; i++) {
        IGesturePoint * gesturePoint = NULL;
        CGesturePoint::DeserializeEx(in, &gesturePoint);
        points->Add((IInterface *)gesturePoint);
    }

    CGestureStroke::New(points, gestureStroke);
    points->Release();
    return NOERROR;
}

/**
 * Invalidates the cached path that is used to render the stroke.
 */
ECode CGestureStroke::ClearPath()
{
    if (mCachedPath != NULL) {
        mCachedPath->Rewind();
    }
    return NOERROR;
}
    
/**
 * Computes an oriented bounding box of the stroke.
 *
 * @return OrientedBoundingBox
 */
ECode CGestureStroke::ComputeOrientedBoundingBox(
    /* [out] */ IOrientedBoundingBox ** orientedBoundingBox)
{
    *orientedBoundingBox = CGestureUtils::ComputeOrientedBoundingBox(*mPoints);
    return NOERROR;
}

//public final RectF boundingBox;
ECode CGestureStroke::GetBoundingBox(
    /* [out] */ IRectF** boundingBox)
{
    return CRectF::New(mBoundingBox,boundingBox);
}

//public final float length;
ECode CGestureStroke::GetLength(
    /* [out] */ Float* length)
{
    *length = mLength;
    return NOERROR;
}

//public final float[] points;
ECode CGestureStroke::GetPoints(
    /* [out, callee] */ ArrayOf<Float> ** points)
{
    *points = mPoints->Clone();
    return NOERROR;
}
