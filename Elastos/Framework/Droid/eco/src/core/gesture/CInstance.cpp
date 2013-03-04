
#include <gesture/CInstance.h>
#include <gesture/CGestureUtils.h>
#include <gesture/CGestureStroke.h>
#include <gesture/CGestureStore.h>
#include <elastos.h>
#include <elastos/Math.h>
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;
using namespace Elastos::Core;

const Int32 CInstance::SEQUENCE_SAMPLE_SIZE = 16;
const Int32 CInstance::PATCH_SAMPLE_SIZE = 16;
//const Int32 CInstance::ORIENTATIONS_LENGTH =10;
//?? ->change to declare in the car file

//#define Instance_ORIENTATIONS_LENGTH 10

const Float CInstance::ORIENTATIONS[Instance_ORIENTATIONS_LENGTH]={
    0, (Float) (Math::DOUBLE_PI / 4), (Float) (Math::DOUBLE_PI  / 2), (Float) (Math::DOUBLE_PI  * 3 / 4),
    (Float) Math::DOUBLE_PI , -0, (Float) (-Math::DOUBLE_PI  / 4), (Float) (-Math::DOUBLE_PI  / 2),
    (Float) (-Math::DOUBLE_PI  * 3 / 4), (Float) -Math::DOUBLE_PI
};

/**
 * An instance represents a sample if the label is available or a query if the
 * label is null.
 */


CInstance::CInstance()
{

}

CInstance::~CInstance()
{
    ArrayOf<Float>::Free(mVector);
}

ECode CInstance::constructor(
    /* [in] */ Int64 id,
    /* [in] */ const ArrayOf<Float> & sample,
    /* [in] */ const String& sampleName)
{
    mId = id;
    mVector = (ArrayOf<Float>*) sample.Clone();
    mLabel = sampleName;
    return NOERROR;
}

ECode CInstance::Normalize()
{
    //ArrayOf<Float>* sample = mVector;
    Float sum = 0;

    Int32 size = mVector->GetLength();
    for (Int32 i = 0; i < size; i++) {
    	Float element = (Float) ((*mVector)[i]);
        sum +=(element * element);
    }

    Float magnitude = (Float)Math::Sqrt((Double)sum);
    for (Int32 i = 0; i < size; i++) {
    	Float element = (Float) ((*mVector)[i]);
    	element /= magnitude;
    	(*mVector)[i]=element;
    }
    return NOERROR;
}

 /**
  * create a learning instance for a single stroke gesture
  *
  * @param gesture
  * @param label
  * @return the instance
  */

ECode CInstance::CreateInstance(
    /* [in] */ Int32 sequenceType,
    /* [in] */ Int32 orientationType,
    /* [in] */ IGesture* gesture,
    /* [in] */ const String& label,
    /* [out] */ IInstance ** instance)
{
    VALIDATE_NOT_NULL(gesture);
    VALIDATE_NOT_NULL(instance);
    Int64 id;
    gesture->GetID(&id);
    ArrayOf<Float>* pts =NULL;
    if (sequenceType == GestureStore_SEQUENCE_SENSITIVE) {
        pts = TemporalSampler(orientationType, gesture);
        CInstance::New(id, *pts, label, instance);
        //(*instance)->Normalize(); //?? need to unmarked it, due to currently car file does not declare it, so the interface does not with this method
    } else {
        pts = SpatialSampler(gesture);
        CInstance::New(id, *pts, label, instance);
    }
    ArrayOf<Float>::Free(pts);
    return NOERROR;

}

ArrayOf<Float>* CInstance::SpatialSampler(
    /* [in] */ IGesture* gesture)
{
    assert(gesture != NULL);
    ArrayOf<Float> * ret = NULL;
    CGestureUtils::SpatialSampling(gesture, PATCH_SAMPLE_SIZE, FALSE, &ret);
    return ret;
}

ArrayOf<Float>* CInstance::TemporalSampler(
   /* [in] */ Int32 orientationType,
   /* [in] */ IGesture* gesture)
{
    assert(gesture != NULL);
    ArrayOf<Float>* pts = NULL;
    IObjectContainer *gestureStrokes = NULL;
    gesture->GetStrokes(&gestureStrokes); //need to be Released
    AutoPtr<IObjectEnumerator> iter;
    gestureStrokes->GetObjectEnumerator((IObjectEnumerator**)&iter);
    Int32 strokeCount = 0;
    gestureStrokes->GetObjectCount(&strokeCount);
    if (strokeCount > 0) {
        IGestureStroke *gestureStroke = NULL;
        Boolean hasNext = FALSE;
        iter->MoveNext(&hasNext);
        if (hasNext) {
            iter->Current((IInterface **)&gestureStroke);

            CGestureUtils::TemporalSampling(gestureStroke,SEQUENCE_SAMPLE_SIZE, &pts);
            ArrayOf<Float>* center = NULL;
            CGestureUtils::ComputeCentroid(*pts, &center);
            Float orientation = (Float)Math::Atan2(pts[1] - center[1], pts[0] - center[0]);

            Float adjustment = -orientation;
            if (orientationType != GestureStore_ORIENTATION_INVARIANT) {
                Int32 count = Instance_ORIENTATIONS_LENGTH;
                for (Int32 i = 0; i < count; i++) {
                    Float delta = ORIENTATIONS[i] - orientation;
                    if (Math::Abs(delta) < Math::Abs(adjustment)) {
                        adjustment = delta;
                    }//end if (Math::Abs(delta) < Math::Abs(adjustment))
                }// end for (Int32 i = 0; i < count; i++)
            }// end if (orientationType != CGestureStore::ORIENTATION_INVARIANT)

            CGestureUtils::Translate(pts, (Float)-(*center)[0], (Float)-(*center)[1]);
            CGestureUtils::Rotate(pts, adjustment);

            ArrayOf<Float>::Free(center);
        } //end if (hasNext)
    }//end if (strokeCount > 0)
    gestureStrokes->Release();
    return pts;
}

//the feature vector
//final float[] vector;
ECode CInstance::GetVector(
    /* [out, callee] */ ArrayOf<Float> ** featureVector)
{
    VALIDATE_NOT_NULL(featureVector);
    *featureVector = mVector->Clone();
    return NOERROR;
}

//the label can be null
ECode CInstance::GetLabel(
    /* [out] */ String * label)
{
    VALIDATE_NOT_NULL(label);
    *label = mLabel;
    return NOERROR;
}

//the id of the instance
ECode CInstance::GetId(
    /* [out] */ Int64 * id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;
    return NOERROR;
}
