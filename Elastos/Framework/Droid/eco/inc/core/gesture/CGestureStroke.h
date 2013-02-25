
#ifndef __CGESTURESTROKE_H__
#define __CGESTURESTROKE_H__

#include "_CGestureStroke.h"
#include "ext/frameworkext.h"
#include <elastos.h>
#include <elastos/List.h>

using namespace Elastos;
using namespace Elastos::Core;

/**
 * A gesture stroke started on a touch down and ended on a touch up. A stroke
 * consists of a sequence of timed points. One or multiple strokes form a gesture.
 */
CarClass(CGestureStroke)
{
public:

    CGestureStroke();

    virtual ~CGestureStroke();

    //public final RectF boundingBox;
    CARAPI GetBoundingBox(
        /* [out] */ IRectF** boundingBox);

    //public final float length;
    CARAPI GetLength(
        /* [out] */ Float* length);

    //public final float[] points;
    CARAPI GetPoints(
        /* [out, callee] */ ArrayOf<Float> ** points);

    /**
     * A constructor that constructs a gesture stroke from a list of gesture points.
     * 
     * @param points
     */
    CARAPI constructor(
        /* [in] */ IObjectContainer* points); //IObjectContainer<IGesturePoint* >*

    /**
     * A faster constructor specially for cloning a stroke.
     */
    //private GestureStroke(RectF bbx, float len, float[] pts, long[] times) {
    CARAPI constructor(
        /* [in] */ IRectF* bbx,
        /* [in] */ Float len,
        /* [in] */ const ArrayOf<Float>& pts,
        /* [in] */ const ArrayOf<Int64>& times);
    
    /* @Override */
    CARAPI Clone(
        /* [out] */ IGestureStroke ** obj);

    /**
     * Draws the stroke with a given canvas and paint.
     * 
     * @param canvas
     */
    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IPaint* paint);

    CARAPI GetPath(
        /* [out] */ IPath ** path);

    /**
     * Converts the stroke to a Path of a given number of points.
     * 
     * @param width the width of the bounding box of the target path
     * @param height the height of the bounding box of the target path
     * @param numSample the number of points needed
     * 
     * @return the path
     */
    CARAPI ToPath(
        /* [in] */ Float width,
        /* [in] */ Float height,
        /* [in] */ Int32 numSample,
        /* [out] */ IPath ** path);

    CARAPI Serialize(
        /* [in] */ IDataOutputStream* outStream);

    static CARAPI Deserialize(
        /* [in] */ IDataInputStream* inStream,
        /* [out] */ IGestureStroke ** gestureStroke);

    static CARAPI DeserializeEx(
        /* [in] */ IParcel* inStream,
        /* [out] */ IGestureStroke ** gestureStroke);
    /**
     * Invalidates the cached path that is used to render the stroke.
     */
    CARAPI ClearPath();
    
    /**
     * Computes an oriented bounding box of the stroke.
     * 
     * @return OrientedBoundingBox
     */
    CARAPI ComputeOrientedBoundingBox(
        /* [out] */ IOrientedBoundingBox ** orientedBoundingBox);

private:
    IRectF* mBoundingBox;
    Float mLength;
    ArrayOf<Float>* mPoints;
    ArrayOf<Int64>* mTimestamps;
    IPath* mCachedPath;

    void MakePath();

};

#endif //__CGESTURESTROKE_H__
