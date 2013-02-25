
#ifndef __CINSTANCE_H__
#define __CINSTANCE_H__

#include "_CInstance.h"
#include "ext/frameworkext.h"
#include <elastos.h>

using namespace Elastos;
using namespace Elastos::Core;

/**
 * An instance represents a sample if the label is available or a query if the
 * label is null.
 */
CarClass(CInstance)
{
public:
	CInstance();
	virtual ~CInstance();

    //private Instance(long id, float[] sample, String sampleName)
    CARAPI constructor(
        /* [in] */ Int64 id,
        /* [in] */ const ArrayOf<Float> & sample,
        /* [in] */ const String& sampleName);

    /**
     * create a learning instance for a single stroke gesture
     * 
     * @param gesture
     * @param label
     * @return the instance
     */
    static CARAPI CreateInstance(
        /* [in] */ Int32 sequenceType,
        /* [in] */ Int32 orientationType,
        /* [in] */ IGesture* gesture,
        /* [in] */ const String& label,
        /* [out] */ IInstance ** instance);

    //the feature vector
    //final float[] vector;
    CARAPI GetVector(
        /* [out, callee] */ ArrayOf<Float> ** featureVector);

    //the label can be null
    CARAPI GetLabel(
        /* [out] */ String * label);

    //the id of the instance
    CARAPI GetId(
        /* [out] */ Int64 * id);

    CARAPI Normalize();

private:

    static const Int32 SEQUENCE_SAMPLE_SIZE; //= 16;
    static const Int32 PATCH_SAMPLE_SIZE; //= 16;
    //static const Int32 ORIENTATIONS_LENGTH; //=10;

    //private final static float[] ORIENTATIONS = {
    static const Float ORIENTATIONS[Instance_ORIENTATIONS_LENGTH];

    // the feature vector
    //final float[] vector;
    //Float *mVector;
    ArrayOf<Float>* mVector;

    // the label can be null
    String mLabel;

    // the id of the instance
    Int64 mId;

    //private static float[] spatialSampler(Gesture gesture) {
    static CARAPI_(ArrayOf<Float>* ) SpatialSampler(
            /* [in] */ IGesture* gesture);

    //private static float[] temporalSampler(int orientationType, Gesture gesture) {
    static CARAPI_(ArrayOf<Float>* ) TemporalSampler(
            /* [in] */ Int32 orientationType,
            /* [in] */ IGesture* gesture);

};

#endif //__CINSTANCE_H__
