
#include <gesture/CInstanceLearner.h>
#include <gesture/CPrediction.h>
#include <gesture/CGestureUtils.h>
#include <elastos.h>
#include <elastos/Map.h>
#include <elastos/Math.h>
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;
using namespace Elastos::Core;


/**
 * An implementation of an instance-based learner
 */
CInstanceLearner::CInstanceLearner()
{

}

CInstanceLearner::~CInstanceLearner()
{

}

Int32 CInstanceLearner::ComparatorPrediction::Compare(
    /* [in] */ IPrediction * object1,
    /* [in] */ IPrediction * object2)
{
    Double score1 = ((CPrediction *)object1)->mScore;//GetScore(); //??
    Double score2 = ((CPrediction *)object2)->mScore;//GetScore(); //??
    if (score1 > score2) {
        return -1;
    } else if (score1 < score2) {
        return 1;
    } else {
        return 0;
    }
}

//@Override
//IObjectContainer<IPrediction*>*
ECode CInstanceLearner::Classify(
    /* [in] */ Int32 sequenceType,
    /* [in] */ Int32 orientationType,
    /* [in] */ const ArrayOf<Float> & vectorParam,
    /* [out] */ IObjectContainer ** ret)
{

    List<IPrediction *>* predictions = new List<IPrediction*>();
    List<IInstance *>* instances = GetInstances();
    Int32 count = instances->GetSize();
    Map<String, Double>* label2score = new Map<String, Double>();
    for (Int32 i = 0; i < count; i++) {
        IInstance *sample = (*instances)[i];//.get(i);
        ArrayOf<Float> * featureVector = NULL;
        sample->GetVector(&featureVector);
        Int32 length1=featureVector->GetLength();

        if (length1 != vectorParam.GetLength()) {
            ArrayOf<Float>::Free(featureVector);
            continue;
        }
        Double distance;
        if (sequenceType == GestureStore_SEQUENCE_SENSITIVE) {
            distance = CGestureUtils::MinimumCosineDistance(*featureVector, vectorParam, orientationType);
        } else {
            distance = CGestureUtils::SquaredEuclideanDistance(*featureVector, vectorParam);
        }
        ArrayOf<Float>::Free(featureVector);
        Double weight;
        if (distance == 0) {
            weight = Math::DOUBLE_MAX_VALUE;
        } else {
            weight = 1 / distance;
        }
        Double score ;
        String label;
        sample->GetLabel(&label);
        Map<String, Double>::Iterator iter= label2score->Find(label);
        if (iter!=label2score->End()) {
            score=iter->mSecond;
        } else {
            if (weight > score) {
                (*label2score)[label]=weight;
             }
         }
    }

//        double sum = 0;
/*
    for (String name : label2score.keySet()) {
        Double score = label2score.get(name);
//            sum += score;
        predictions->Add(new Prediction(name, score));
    }
*/
    Map<String, Double>::Iterator iteri;
    for (iteri=label2score->Begin();iteri!=label2score->End();iteri++) {
        String name=iteri->mFirst;
        Double score=iteri->mSecond;
        IPrediction *prediction;
        CPrediction::New(name, score,&prediction);
        predictions->PushBack(prediction);
    }

        // normalize
//        for (Prediction prediction : predictions) {
//            prediction.score /= sum;
//        }

//    Collections.sort(predictions, sComparator);
    IObjectContainer* retobj = NULL;
    List<IPrediction *>* predictionsSort = NULL;
    if (predictions->GetSize() > 0) {
        predictionsSort = Sort(predictions);
        CObjectContainer::New(&retobj);
        for (UInt32 i=0;i<predictions->GetSize();i++) {
            retobj->Add((IInterface *)((*predictions)[i]));
        }
    }

    *ret =retobj;
    if (predictionsSort != NULL) {
        delete predictionsSort;
    }
    if (predictions != NULL) {
        delete predictions;
    }
    if (label2score != NULL) {
        delete label2score;
    }
    return NOERROR;

}

ECode CInstanceLearner::AddInstance(
    /* [in] */ IInstance * instance)
{
    return Learner::AddInstance(instance);
}

ECode CInstanceLearner::GetInstances(
    /* [out] */ IObjectContainer ** ret)
{
    return Learner::GetInstances(ret);
}

ECode CInstanceLearner::RemoveInstance(
    /* [in] */ Int64 id)
{
    return Learner::RemoveInstance(id);
}

ECode CInstanceLearner::RemoveInstances(
    /* [in] */ const String& name)
{
    return Learner::RemoveInstances(name);
}

List<IInstance *>* CInstanceLearner::GetInstances()
{
    List<IInstance *>* retList=new List<IInstance *>();
    IObjectContainer *ret = NULL;
    GetInstances(&ret);
    Int32 count = 0;
    ret->GetObjectCount(&count);

    if (count > 0) {
        AutoPtr<IObjectEnumerator>iter;
        ret->GetObjectEnumerator((IObjectEnumerator**)&iter);
        Boolean hasNext = FALSE;
        iter->MoveNext(&hasNext);
        while(hasNext) {
            IInstance* p=NULL;
            iter->Current((IInterface**)&p);
            retList->PushBack(p);
        }
    }
    ret->Release();
    return retList;
}

List<IPrediction *>* CInstanceLearner::Sort(
    /* [in] */ List<IPrediction *>* predictions)
{
    List<IPrediction *>* predictionsSort=new List<IPrediction *>();

    //??
    CInstanceLearner::ComparatorPrediction c;
    Int32 count = predictions->GetSize();
    for (Int32 index=0; index<count; index++) {
        predictionsSort->PushBack((*predictions)[index]);
    }

    for (Int32 i=0; i<count;i++) {
        for (Int32 j=i+1;j<i;j++) {
            Int32 comp=c.Compare((*predictionsSort)[i],(*predictionsSort)[j]);
            if (comp > 0) {
                IPrediction * tmp = (*predictionsSort)[i];
                (*predictionsSort)[i] = (*predictionsSort)[j];
                (*predictionsSort)[j] = tmp;
            }
        }
    }
    return predictionsSort;
}

