
#ifndef __CINSTANCELEARNER_H__
#define __CINSTANCELEARNER_H__

#include "_CInstanceLearner.h"
#include "_Elastos.Framework.Core.h"
#include <gesture/Learner.h>
#include "ext/frameworkdef.h"
#include <elastos/List.h>
#include <elastos/Map.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;
using namespace Elastos::Core;

/**
 * An implementation of an instance-based learner
 */
CarClass(CInstanceLearner)
, public Learner
{
public:
    CInstanceLearner();

    virtual ~CInstanceLearner();

    class ComparatorPrediction //: public IComparator
    {
    public:
        CARAPI_(Int32) Compare(
            /* [in] */ IPrediction *object1,
            /* [in] */ IPrediction *object2);

    };

    //??
     /*
    private static final Comparator<Prediction> sComparator = new Comparator<Prediction>() {
        public int compare(Prediction object1, Prediction object2) {
            double score1 = object1.score;
            double score2 = object2.score;
            if (score1 > score2) {
                return -1;
            } else if (score1 < score2) {
                return 1;
            } else {
                return 0;
            }
        }
    };
    */

    CARAPI AddInstance(
        /* [in] */ IInstance * instance);

    CARAPI GetInstances(
        /* [out] */ IObjectContainer ** ret);

    CARAPI RemoveInstance(
        /* [in] */ Int64 id);

    CARAPI RemoveInstances(
        /* [in] */ const String& name);

    //@Override
    //IObjectContainer<IPrediction*>* 
    CARAPI Classify(
        /* [in] */ Int32 sequenceType,
        /* [in] */ Int32 orientationType,
        /* [in] */ const ArrayOf<Float> & vectorParam,
        /* [out] */ IObjectContainer ** ret); 

private:
    CARAPI_(List<IPrediction *>*) Sort(
        /* [in] */ List<IPrediction *>* );

    CARAPI_(List<IInstance *>*) GetInstances();

};

#endif //__CINSTANCELEARNER_H__

